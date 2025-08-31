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
mDNSServiceAdvertiser::mDNSServiceAdvertiser(const juce::String& serviceTypeUID, std::uint16_t servicePort, juce::RelativeTime minTimeBetweenBroadcasts)
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

    //freeMDNSString(m_service.record_ptr.name);
    //freeMDNSString(m_service.record_srv.name);
    //freeMDNSString(m_service.record_a.name);
    //freeMDNSString(m_service.record_aaaa.name);
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
    auto hostDescription = juce::SystemStats::getComputerName() + "\0";
    auto serviceType = m_serviceTypeUID + ".local.\0";
    auto serviceInstanceString = juce::JUCEApplication::getInstance()->getApplicationName().removeCharacters(".") + "." + serviceType + "\0";
    auto hostnameQualifiedString = hostDescription + ".local.\0";

    service_t service = { 0 };
    mallocMDNSString(serviceType, service.service);
    mallocMDNSString(hostDescription, service.hostname);
    mallocMDNSString(serviceInstanceString, service.service_instance);
    mallocMDNSString(hostnameQualifiedString, service.hostname_qualified);
    service.address_ipv4 = makeSockAddrIn(juce::IPAddress::getLocalAddress(), m_connectionPort);
    service.address_ipv6 = makeSockAddrIn6(juce::IPAddress::getLocalAddress(), m_connectionPort);
    service.port = m_connectionPort;

    // Setup our mDNS records
    constexpr std::uint16_t DNS_CLASS_IN = 0x0001;
    constexpr std::uint16_t MDNS_CLASS_CACHE_FLUSH = 0x8000;

    // PTR record reverse mapping "<_service-name>._tcp.local." to
    // "<hostname>.<_service-name>._tcp.local."
    service.record_ptr.name = service.service;
    service.record_ptr.type = MDNS_RECORDTYPE_PTR;
    service.record_ptr.data.ptr.name = service.service_instance;
    service.record_ptr.rclass = DNS_CLASS_IN;
    service.record_ptr.ttl = 0;

    // SRV record mapping "<hostname>.<_service-name>._tcp.local." to
    // "<hostname>.local." with port. Set weight & priority to 0.
    service.record_srv.name = service.service_instance;
    service.record_srv.type = MDNS_RECORDTYPE_SRV;
    service.record_srv.data.srv.name = service.hostname_qualified;
    service.record_srv.data.srv.port = (std::uint16_t)service.port;
    service.record_srv.data.srv.priority = 0;
    service.record_srv.data.srv.weight = 0;
    service.record_srv.rclass = DNS_CLASS_IN | MDNS_CLASS_CACHE_FLUSH;
    service.record_srv.ttl = 0;

    // A/AAAA records mapping "<hostname>.local." to IPv4/IPv6 addresses
    service.record_a.name = service.hostname_qualified;
    service.record_a.type = MDNS_RECORDTYPE_A;
    service.record_a.data.a.addr = service.address_ipv4;
    service.record_a.rclass = DNS_CLASS_IN | MDNS_CLASS_CACHE_FLUSH;
    service.record_a.ttl = 0;

    service.record_aaaa.name = service.hostname_qualified;
    service.record_aaaa.type = MDNS_RECORDTYPE_AAAA;
    service.record_aaaa.data.aaaa.addr = service.address_ipv6;
    service.record_aaaa.rclass = DNS_CLASS_IN | MDNS_CLASS_CACHE_FLUSH;
    service.record_aaaa.ttl = 0;

    jassert(MDNS_MAX_TXTRECORDS >= m_txtRecords.size());
    if (MDNS_MAX_TXTRECORDS >= m_txtRecords.size())
    {
        int i = 0;
        for (auto const& record : m_txtRecords)
        {
            service.txt_record[i].name = service.service_instance;
            service.txt_record[i].type = MDNS_RECORDTYPE_TXT;
            service.txt_record[i].data.txt.key = { record.first.c_str(), record.first.size() };
            service.txt_record[i].data.txt.value = { record.second.c_str(), record.second.size() };
            service.txt_record[i].rclass = DNS_CLASS_IN | MDNS_CLASS_CACHE_FLUSH;
            service.txt_record[i].ttl = 0;
            i++;
        }
        service.txt_record_cnt = m_txtRecords.size();
    }

    std::lock_guard<std::mutex> l(m_serviceMutex);
    m_service = service;
    DBG(juce::String(__FUNCTION__) + " " + juce::String(m_service.record_ptr.name.str, m_service.record_ptr.name.length));
}

/**
 * @brief Gets the multicast address relevant for mDNS
 */
juce::String mDNSServiceAdvertiser::getMulticastDNSAddress()
{
    return juce::String("224.0.0.251");
}

/**
 * @brief Gets the multicast port relevant for mDNS
 */
int mDNSServiceAdvertiser::getMulticastDNSPort()
{
    return MDNS_PORT;
}

/*
 * @brief Starts the thread and triggers broadcasting
 */
void mDNSServiceAdvertiser::run()
{
    if (m_socket.bindToPort(getMulticastDNSPort()) && m_socket.joinMulticast(getMulticastDNSAddress()))
    {
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
inline std::vector<uint8_t> mDNSServiceAdvertiser::makeDnsLabel(std::string_view name)
{
    std::vector<uint8_t> result;
    size_t start = 0;

    // Eliminate trailing dot
    if (!name.empty() && name.back() == '.')
        name.remove_suffix(1);

    while (start < name.size()) {
        size_t dot = name.find('.', start);
        if (dot == std::string_view::npos)
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
    auto nameLabel = makeDnsLabel(std::string_view(record.name.str, record.name.length));
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
                auto ptrLabel = makeDnsLabel(std::string_view(record.data.ptr.name.str, record.data.ptr.name.length));
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
                auto srvLabel = makeDnsLabel(std::string_view(record.data.srv.name.str, record.data.srv.name.length));
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
            auto nameLabel = makeDnsLabel(std::string_view(records[i].name.str, records[i].name.length));
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
        std::string key(records[i].data.txt.key.str, records[i].data.txt.key.length);
        std::string value(records[i].data.txt.value.str, records[i].data.txt.value.length);
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
 * @brief   juce::String to mdns_string_t conversion.
 *          Includes allocation of new memory and deallocation of previously used memory.
 */
void mDNSServiceAdvertiser::mallocMDNSString(const juce::String& inputString, mDNSServiceAdvertiser::mdns_string_t& outputString)
{
    freeMDNSString(outputString);

    auto str = inputString.toRawUTF8();
    auto strl = inputString.getNumBytesAsUTF8();

    outputString.str = (const char*)std::malloc(strl);
    std::memcpy((void*)outputString.str, str, strl);
    outputString.length = strl;
}

/**
 * @brief   Cleanup mdns_string_t data
 */
void mDNSServiceAdvertiser::freeMDNSString(mDNSServiceAdvertiser::mdns_string_t& string)
{
    if (string.length >= 0)
    {
        std::free((void*)string.str);
        string.length = 0;
    }
}

/**
 * @brief Sends a single multicast to multicast address and mDNS port
 */
void mDNSServiceAdvertiser::sendMulticast()
{
    static juce::IPAddress local = juce::IPAddress::local();

    service_t serviceDataLocalCopy{ 0 };
    {
        std::lock_guard<std::mutex> l(m_serviceMutex);
        DBG(juce::String(__FUNCTION__) + " " + juce::String(m_service.record_ptr.name.str, m_service.record_ptr.name.length));
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
    header->answer_rrs = htons(1/*PTR*/ + 1/*SRV*/ + (hasTxtRecs ? 1 : 0)/*TXT*/ + (hasIPv4 ? 1 : 0)/*A*/ + (hasIPv6 ? 1 : 0)/*AAAA*/);
    header->authority_rrs = htons(0);
    header->additional_rrs = htons(0);

    append(serializeMDNSRecord(serviceDataLocalCopy.record_ptr), buffer);
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
