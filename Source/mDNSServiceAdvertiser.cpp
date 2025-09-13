/* Copyright (c) 2025, Christian Ahrens
 *
 * This file is part of JUCEAppBasics <https://github.com/ChristianAhrens/JUCE-AppBasics>
 *
 * This module is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This module is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this tool; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "mDNSServiceAdvertiser.h"


namespace JUCEAppBasics
{


//==============================================================================
/**
 * @brief Constructor. Thread base class is initialized but not started here.
 */
mDNSServiceAdvertiser::mDNSServiceAdvertiser(const std::string& serviceTypeUID, std::uint16_t servicePort, juce::RelativeTime minTimeBetweenBroadcasts)
    : juce::Thread(juce::JUCEApplication::getInstance()->getApplicationName() + ": mDNS_Discovery_broadcast"),
    m_serviceTypeUID(serviceTypeUID),
    m_connectionPort(servicePort),
    m_minInterval(minTimeBetweenBroadcasts)
{
    buildService();

    startThread(juce::Thread::Priority::background);
}

/**
 * @brief Destructor. Base class thread is stopped.
 */
mDNSServiceAdvertiser::~mDNSServiceAdvertiser()
{
    stopThread(2000);
    m_socket.shutdown();
}

/**
 * @brief   Add a string key/value to the list of txtRecords
 */
void mDNSServiceAdvertiser::addTxtRecord(const std::string& key, const std::string& value)
{
    m_txtRecords[key] = value;

    buildService();
}

/**
 * @brief   Add a map of string key/value to the list of txtRecords
 */
void mDNSServiceAdvertiser::addTxtRecords(const std::map<std::string, std::string>& txtRecords)
{
    for(auto const& recordKV : txtRecords)
        m_txtRecords[recordKV.first] = recordKV.second;

    buildService();
}

/**
 * @brief   Remove a given string key/value from the list of txtRecords
 */
void mDNSServiceAdvertiser::removeTxtRecord(const std::string& key)
{
    m_txtRecords.erase(key);

    buildService();
}

/**
 * @brief   Build the internal service_t member with currently known data
 */
void mDNSServiceAdvertiser::buildService()
{
    std::lock_guard<std::mutex> l(m_serviceMutex);

    m_service.service = m_serviceTypeUID + ".local.\0";
    m_service.hostname = juce::SystemStats::getComputerName().toStdString() + "\0";
    m_service.services_name = "_services._dns-sd._udp.local.\0";
    m_service.service_instance = juce::JUCEApplication::getInstance()->getApplicationName().removeCharacters(".").toStdString() + "." + m_serviceTypeUID + ".local.\0";
    m_service.hostname_qualified = juce::SystemStats::getComputerName().toStdString() + ".local.\0";
    m_service.address_ipv4 = makeSockAddrIn(juce::IPAddress::getLocalAddress(), m_connectionPort);
    m_service.address_ipv6 = makeSockAddrIn6(juce::IPAddress::getLocalAddress(), m_connectionPort);
    m_service.port = m_connectionPort;

    // Setup our mDNS records
    constexpr std::uint16_t DNS_CLASS_IN = 0x0001;
    constexpr std::uint16_t MDNS_CLASS_CACHE_FLUSH = 0x8000;

    // PTR record reverse mapping "<_service-name>._tcp.local." to
    // "<hostname>.<_service-name>._tcp.local."
    m_service.record_ptr.name = m_service.service;
    m_service.record_ptr.type = MDNS_RECORDTYPE_PTR;
    m_service.record_ptr.data.ptr.name = m_service.service_instance;
    m_service.record_ptr.rclass = DNS_CLASS_IN;
    m_service.record_ptr.ttl = 0;

    // PTR record for _services._dns-sd._udp.local
    m_service.record_ptr_service.name = m_service.services_name;
    m_service.record_ptr_service.type = MDNS_RECORDTYPE_PTR;
    m_service.record_ptr_service.data.ptr.name = m_service.service;
    m_service.record_ptr_service.rclass = DNS_CLASS_IN;
    m_service.record_ptr_service.ttl = 4500;

    // SRV record mapping "<hostname>.<_service-name>._tcp.local." to
    // "<hostname>.local." with port. Set weight & priority to 0.
    m_service.record_srv.name = m_service.service_instance;
    m_service.record_srv.type = MDNS_RECORDTYPE_SRV;
    m_service.record_srv.data.srv.name = m_service.hostname_qualified;
    m_service.record_srv.data.srv.port = (std::uint16_t)m_service.port;
    m_service.record_srv.data.srv.priority = 0;
    m_service.record_srv.data.srv.weight = 0;
    m_service.record_srv.rclass = DNS_CLASS_IN | MDNS_CLASS_CACHE_FLUSH;
    m_service.record_srv.ttl = 0;

    // A/AAAA records mapping "<hostname>.local." to IPv4/IPv6 addresses
    m_service.record_a.name = m_service.hostname_qualified;
    m_service.record_a.type = MDNS_RECORDTYPE_A;
    m_service.record_a.data.a.addr = m_service.address_ipv4;
    m_service.record_a.rclass = DNS_CLASS_IN | MDNS_CLASS_CACHE_FLUSH;
    m_service.record_a.ttl = 0;

    m_service.record_aaaa.name = m_service.hostname_qualified;
    m_service.record_aaaa.type = MDNS_RECORDTYPE_AAAA;
    m_service.record_aaaa.data.aaaa.addr = m_service.address_ipv6;
    m_service.record_aaaa.rclass = DNS_CLASS_IN | MDNS_CLASS_CACHE_FLUSH;
    m_service.record_aaaa.ttl = 0;

    jassert(MDNS_MAX_TXTRECORDS >= m_txtRecords.size());
    if (MDNS_MAX_TXTRECORDS >= m_txtRecords.size())
    {
        int i = 0;
        for (auto const& record : m_txtRecords)
        {
            m_service.txt_record[i].name = m_service.service_instance;
            m_service.txt_record[i].type = MDNS_RECORDTYPE_TXT;
            m_service.txt_record[i].data.txt.key = { record.first.c_str(), record.first.size() };
            m_service.txt_record[i].data.txt.value = { record.second.c_str(), record.second.size() };
            m_service.txt_record[i].rclass = DNS_CLASS_IN | MDNS_CLASS_CACHE_FLUSH;
            m_service.txt_record[i].ttl = 0;
            i++;
        }
        m_service.txt_record_cnt = m_txtRecords.size();
    }

    DBG(juce::String(__FUNCTION__) + " " + juce::String(m_service.record_ptr.name));
}

/**
 * @brief Gets the multicast address relevant for mDNS
 */
std::string mDNSServiceAdvertiser::getMulticastDNSAddress()
{
    return "224.0.0.251";
}

/**
 * @brief Gets the multicast port relevant for mDNS
 */
int mDNSServiceAdvertiser::getMulticastDNSPort()
{
    return MDNS_PORT;
}

/**
 * @brief Sets the datagram sockets' options to use a mDNS compliant TTL value of 255
 */
void mDNSServiceAdvertiser::setMulticastTTL(int ttl)
{
    int fd = m_socket.getRawSocketHandle();
    if (fd > 0)
    {
        // IP_MULTICAST_TTL erwartet einen unsigned char auf Windows, int auf Linux
#ifdef _WIN32
        DWORD ttlValue = static_cast<DWORD>(ttl);
        setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)&ttlValue, sizeof(ttlValue));
#else
        int ttlValue = ttl;
        setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttlValue, sizeof(ttlValue));
#endif
    }
}

/*
 * @brief Starts the thread and triggers broadcasting
 */
void mDNSServiceAdvertiser::run()
{
    if (m_socket.bindToPort(getMulticastDNSPort()) && m_socket.joinMulticast(getMulticastDNSAddress()))
    {
        setMulticastTTL();

        while (!threadShouldExit())
        {
            sendMulticast();
            juce::Thread::wait((int)m_minInterval.inMilliseconds());
        }

        m_socket.leaveMulticast(getMulticastDNSAddress());
    }
    else
    {
        jassertfalse;
        return;
    }
}

/**
 * @brief   Serialize a given string to mDNS label compliant byte string vector
 */
inline std::vector<uint8_t> mDNSServiceAdvertiser::makeDnsLabel(std::string name)
{
    std::vector<uint8_t> result;
    size_t start = 0;

    // Eliminate trailing dot
    if (!name.empty() && name.back() == '.')
        name.pop_back();

    while (start < name.size()) {
        size_t dot = name.find('.', start);
        if (dot == std::string::npos)
            dot = name.size();
        size_t labelLen = dot - start;
        if (labelLen > 63)
        {
            jassertfalse;
            DBG("DNS label too long");
        }
        result.push_back(static_cast<uint8_t>(labelLen));
        result.insert(result.end(), name.begin() + start, name.begin() + dot);
        start = dot + 1;
    }
    result.push_back(0); // Mandatory zero termination
    return result;
}

/**
 * @brief   Serialize a single Resource record (PTR, SRV, A, AAAA, TXT)
 */
std::vector<uint8_t> mDNSServiceAdvertiser::serializeMDNSRecord(const mDNSServiceAdvertiser::mdns_record_t& record)
{
    std::vector<uint8_t> data;

    // Name in mDNS label format
    auto nameLabel = makeDnsLabel(record.name);
    data.insert(data.end(), nameLabel.begin(), nameLabel.end());

    append16(static_cast<uint16_t>(record.type), data);
    append16(record.rclass, data);
    append32(record.ttl, data);

    // RDATA prep
    std::vector<std::uint8_t> rdata;

    switch (record.type)
    {
        case mDNSServiceAdvertiser::MDNS_RECORDTYPE_PTR:
            {
                auto ptrLabel = makeDnsLabel(record.data.ptr.name);
                rdata.insert(rdata.end(), ptrLabel.begin(), ptrLabel.end());
            }
            break;
        case mDNSServiceAdvertiser::MDNS_RECORDTYPE_SRV:
            {
                std::uint16_t priority = htons(record.data.srv.priority);
                std::uint16_t weight = htons(record.data.srv.weight);
                std::uint16_t port = htons(record.data.srv.port);
                rdata.insert(rdata.end(), reinterpret_cast<std::uint8_t*>(&priority), reinterpret_cast<std::uint8_t*>(&priority) + 2);
                rdata.insert(rdata.end(), reinterpret_cast<std::uint8_t*>(&weight), reinterpret_cast<std::uint8_t*>(&weight) + 2);
                rdata.insert(rdata.end(), reinterpret_cast<std::uint8_t*>(&port), reinterpret_cast<std::uint8_t*>(&port) + 2);
                auto srvLabel = makeDnsLabel(record.data.srv.name);
                rdata.insert(rdata.end(), srvLabel.begin(), srvLabel.end());
            }
            break;
        case mDNSServiceAdvertiser::MDNS_RECORDTYPE_A:
            {
                const std::uint8_t* addr = reinterpret_cast<const std::uint8_t*>(&record.data.a.addr.sin_addr.s_addr);
                rdata.insert(rdata.end(), addr, addr + 4);
            }
            break;
        case mDNSServiceAdvertiser::MDNS_RECORDTYPE_AAAA:
            {
                const std::uint8_t* addr = reinterpret_cast<const std::uint8_t*>(&record.data.aaaa.addr.sin6_addr);
                rdata.insert(rdata.end(), addr, addr + 16);
            }
            break;
        default:
            break;
    }

    // RDATA length insertion in front of RDATA itelsef
    append16(static_cast<std::uint16_t>(rdata.size()), data);
    data.insert(data.end(), rdata.begin(), rdata.end());

    return data;
}

/**
 * @brief   Serialize all TXT records to a single entry
 */
std::vector<std::uint8_t> mDNSServiceAdvertiser::serializeMDNSTxtRecords(const mDNSServiceAdvertiser::mdns_record_t* records, size_t record_count)
{
    std::vector<std::uint8_t> data;
    bool headerWritten = false;
    size_t lengthPos = 0;

    for (size_t i = 0; i < record_count; ++i)
    {
        if (records[i].type != mDNSServiceAdvertiser::MDNS_RECORDTYPE_TXT)
            continue;

        if (!headerWritten)
        {
            // Name in mDNS label format
            auto nameLabel = makeDnsLabel(records[i].name);
            data.insert(data.end(), nameLabel.begin(), nameLabel.end());

            // Type, Classe, TTL
            append16(static_cast<std::uint16_t>(records[i].type), data);
            append16(records[i].rclass, data);
            append32(records[i].ttl, data);

            // reserve place for data length field
            lengthPos = data.size();
            data.push_back(0);
            data.push_back(0); // placeholder
            headerWritten = true;
        }

        // TXT-string: <len>key=value
        const std::string& key = records[i].data.txt.key;
        const std::string& value = records[i].data.txt.value;
        std::string txt = key + "=" + value;
        if (txt.size() > 255)
        {
            jassertfalse;
            DBG("TXT record too long");
        }
        data.push_back(static_cast<std::uint8_t>(txt.size()));
        data.insert(data.end(), txt.begin(), txt.end());
    }

    // insert length into data length placeholder
    if (headerWritten)
    {
        std::uint16_t txtLen = static_cast<std::uint16_t>(data.size() - lengthPos - 2);
        data[lengthPos] = static_cast<std::uint8_t>(txtLen >> 8);
        data[lengthPos + 1] = static_cast<std::uint8_t>(txtLen & 0xFF);
    }

    return data;
}

/**
 * @brief IP to sockaddr (IPv4) conversion
 */
sockaddr_in mDNSServiceAdvertiser::makeSockAddrIn(const juce::IPAddress& ip, std::uint16_t port)
{
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    // Conversion from string to binary
    ::inet_pton(AF_INET, ip.toString().toRawUTF8(), &addr.sin_addr);
    return addr;
}

/**
 * @brief IP to sockaddr (IPv6) conversion
 */
sockaddr_in6 mDNSServiceAdvertiser::makeSockAddrIn6(const juce::IPAddress& ip, std::uint16_t port)
{
    sockaddr_in6 addr = {};
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(port);
    // Conversion from string to binary
    ::inet_pton(AF_INET6, ip.toString().toRawUTF8(), &addr.sin6_addr);
    return addr;
}

/**
 * @brief Sends a single multicast to multicast address and mDNS port
 */
void mDNSServiceAdvertiser::sendMulticast()
{
    static juce::IPAddress local = juce::IPAddress::local();

    service_t serviceDataLocalCopy;
    {
        std::lock_guard<std::mutex> l(m_serviceMutex);
        DBG(juce::String(__FUNCTION__) + " " + juce::String(m_service.record_ptr.name));
        serviceDataLocalCopy = m_service;
    }

    auto hasIPv4 = serviceDataLocalCopy.address_ipv4.sin_family == AF_INET;
    auto hasIPv6 = serviceDataLocalCopy.address_ipv6.sin6_family == AF_INET6;
    auto hasTxtRecs = serviceDataLocalCopy.txt_record_cnt > 0;

    std::vector<uint8_t> buffer(sizeof(mdns_header_t));

    auto header = reinterpret_cast<mdns_header_t*>(buffer.data());
    header->query_id = MDNS_UNSOLICITED_QUERY_ID;
    header->flags = htons(MDNS_AUTHORITATIVE_RESPONSE_FLAG);

    header->questions = htons(0);
    header->answer_rrs = htons(1/*PTR*/ + 1/*PTR services*/ + 1/*SRV*/ + (hasTxtRecs ? 1 : 0)/*TXT*/ + (hasIPv4 ? 1 : 0)/*A*/ + (hasIPv6 ? 1 : 0)/*AAAA*/);
    header->authority_rrs = htons(0);
    header->additional_rrs = htons(0);

    append(serializeMDNSRecord(serviceDataLocalCopy.record_ptr), buffer);
    append(serializeMDNSRecord(serviceDataLocalCopy.record_ptr_service), buffer);
    append(serializeMDNSRecord(serviceDataLocalCopy.record_srv), buffer);
    if (hasTxtRecs)
        append(serializeMDNSTxtRecords(serviceDataLocalCopy.txt_record, serviceDataLocalCopy.txt_record_cnt), buffer);
    if (hasIPv4)
        append(serializeMDNSRecord(serviceDataLocalCopy.record_a), buffer);
    if (hasIPv6)
        append(serializeMDNSRecord(serviceDataLocalCopy.record_aaaa), buffer);

    int datalen = static_cast<int>(buffer.size());

    auto numBytesWritten = m_socket.write(getMulticastDNSAddress(), getMulticastDNSPort(), reinterpret_cast<const char*>(buffer.data()), datalen);
    if (numBytesWritten < 0)
    {
        jassertfalse;
        DBG(juce::String(__FUNCTION__) + " unable to write to socket!");
    }
}


} // namespace JUCEAppBasics
