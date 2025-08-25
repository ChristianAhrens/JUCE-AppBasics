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
    m_broadcastPort(MDNS_PORT),
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

void mDNSServiceAdvertiser::addTxtRecord(const std::string& key, const std::string& value)
{
    m_txtRecords[key] = value;

    buildService();
}

void mDNSServiceAdvertiser::addTxtRecords(const std::map<std::string, std::string>& txtRecords)
{
    for(auto const& recordKV : txtRecords)
        m_txtRecords[recordKV.first] = recordKV.second;

    buildService();
}

void mDNSServiceAdvertiser::removeTxtRecord(const std::string& key)
{
    m_txtRecords.erase(key);

    buildService();
}

void mDNSServiceAdvertiser::buildService()
{
    auto hostDescription = juce::JUCEApplication::getInstance()->getApplicationName()
        + "@" + juce::SystemStats::getComputerName();

    mdns_string_t service_string = { m_serviceTypeUID.toRawUTF8(), size_t(m_serviceTypeUID.length()) };
    mdns_string_t hostname_string = { hostDescription.toRawUTF8(), size_t(hostDescription.length()) };

    // Build the service instance "<hostname>.<_service-name>._tcp.local." string
    auto serviceInstanceString = hostDescription + "." + m_serviceTypeUID + "._tcp.local.";
    mdns_string_t service_instance_string = { serviceInstanceString.toRawUTF8(), size_t(serviceInstanceString.length()) };

    // Build the "<hostname>.local." string
    auto hostnameQualifiedString = hostDescription + ".local.";
    mdns_string_t hostname_qualified_string = { hostnameQualifiedString.toRawUTF8(), size_t(hostnameQualifiedString.length()) };

    service_t service = { 0 };
    service.service = service_string;
    service.hostname = hostname_string;
    service.service_instance = service_instance_string;
    service.hostname_qualified = hostname_qualified_string;
    service.address_ipv4 = makeSockAddrIn(juce::IPAddress::getLocalAddress(), m_connectionPort);
    service.address_ipv6 = makeSockAddrIn6(juce::IPAddress::getLocalAddress(), m_connectionPort);
    service.port = m_broadcastPort;

    // Setup our mDNS records

    // PTR record reverse mapping "<_service-name>._tcp.local." to
    // "<hostname>.<_service-name>._tcp.local."
    service.record_ptr.name = service.service;
    service.record_ptr.type = MDNS_RECORDTYPE_PTR;
    service.record_ptr.data.ptr.name = service.service_instance;
    service.record_ptr.rclass = 0;
    service.record_ptr.ttl = 0;

    // SRV record mapping "<hostname>.<_service-name>._tcp.local." to
    // "<hostname>.local." with port. Set weight & priority to 0.
    service.record_srv.name = service.service_instance;
    service.record_srv.type = MDNS_RECORDTYPE_SRV;
    service.record_srv.data.srv.name = service.hostname_qualified;
    service.record_srv.data.srv.port = (uint16_t)service.port;
    service.record_srv.data.srv.priority = 0;
    service.record_srv.data.srv.weight = 0;
    service.record_srv.rclass = 0;
    service.record_srv.ttl = 0;

    // A/AAAA records mapping "<hostname>.local." to IPv4/IPv6 addresses
    service.record_a.name = service.hostname_qualified;
    service.record_a.type = MDNS_RECORDTYPE_A;
    service.record_a.data.a.addr = service.address_ipv4;
    service.record_a.rclass = 0;
    service.record_a.ttl = 0;

    service.record_aaaa.name = service.hostname_qualified;
    service.record_aaaa.type = MDNS_RECORDTYPE_AAAA;
    service.record_aaaa.data.aaaa.addr = service.address_ipv6;
    service.record_aaaa.rclass = 0;
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
            service.txt_record[i].rclass = 0;
            service.txt_record[i].ttl = 0;
            i++;
        }
        service.txt_record_cnt = m_txtRecords.size();
    }

    m_service = service;
}

/**
 * @brief Gets the broadcast address for an interface with the given assigned IP
 */
juce::IPAddress mDNSServiceAdvertiser::getInterfaceBroadcastAddress(const juce::IPAddress& address)
{
    if (address.isIPv6)
        // TODO
        return {};

    juce::String broadcastAddress = address.toString().upToLastOccurrenceOf(".", true, false) + "255";
    return juce::IPAddress(broadcastAddress);
}

/*
 * @brief Starts the thread and triggers broadcasting
 */
void mDNSServiceAdvertiser::run()
{
    if (!m_socket.bindToPort(0))
    {
        jassertfalse;
        return;
    }

    while (!threadShouldExit())
    {
        sendBroadcast();
        wait((int)m_minInterval.inMilliseconds());
    }
}

#include <cstring> // Für memcpy

// Hilfsfunktionen aus mdns.h (angepasst für C++)
inline void* mdns_htons(void* data, uint16_t val) {
    val = htons(val);
    std::memcpy(data, &val, sizeof(uint16_t));
    return static_cast<uint8_t*>(data) + sizeof(uint16_t);
}
inline void* mdns_htonl(void* data, uint32_t val) {
    val = htonl(val);
    std::memcpy(data, &val, sizeof(uint32_t));
    return static_cast<uint8_t*>(data) + sizeof(uint32_t);
}

// String-Serialisierung im DNS-Label-Format
inline void* mdns_string_make(void* buffer, size_t capacity, void* data, const char* name, size_t length) {
    size_t last_pos = 0;
    size_t remain = capacity - (static_cast<uint8_t*>(data) - static_cast<uint8_t*>(buffer));
    if (length > 0 && name[length - 1] == '.') --length;
    while (last_pos < length) {
        size_t pos = std::string_view(name, length).find('.', last_pos);
        if (pos == std::string_view::npos) pos = length;
        size_t sub_length = pos - last_pos;
        if (remain <= (sub_length + 1)) return nullptr;
        *(unsigned char*)data = (unsigned char)sub_length;
        std::memcpy(static_cast<uint8_t*>(data) + 1, name + last_pos, sub_length);
        data = static_cast<uint8_t*>(data) + sub_length + 1;
        last_pos = (pos < length) ? pos + 1 : length;
        remain = capacity - (static_cast<uint8_t*>(data) - static_cast<uint8_t*>(buffer));
    }
    if (!remain) return nullptr;
    *(unsigned char*)data = 0;
    return static_cast<uint8_t*>(data) + 1;
}

// Serialisiert einen einzelnen Resource Record (PTR, SRV, A, AAAA, TXT)
void* mdns_add_record(void* buffer, size_t capacity, void* data, const JUCEAppBasics::mDNSServiceAdvertiser::mdns_record_t& record) {
    // Name
    data = mdns_string_make(buffer, capacity, data, record.name.str, record.name.length);
    if (!data) return nullptr;
    size_t remain = capacity - (static_cast<uint8_t*>(data) - static_cast<uint8_t*>(buffer));
    if (remain < 10) return nullptr;

    // Typ, Klasse, TTL, Datenlänge (Platzhalter)
    data = mdns_htons(data, (uint16_t)record.type);
    data = mdns_htons(data, record.rclass);
    data = mdns_htonl(data, record.ttl);
    void* length_ptr = data;
    data = mdns_htons(data, 0); // Platzhalter für Datenlänge

    void* record_data = data;
    remain = capacity - (static_cast<uint8_t*>(data) - static_cast<uint8_t*>(buffer));
    switch (record.type) {
    case JUCEAppBasics::mDNSServiceAdvertiser::MDNS_RECORDTYPE_PTR:
        data = mdns_string_make(buffer, capacity, data, record.data.ptr.name.str, record.data.ptr.name.length);
        break;
    case JUCEAppBasics::mDNSServiceAdvertiser::MDNS_RECORDTYPE_SRV:
        if (remain < 6) return nullptr;
        data = mdns_htons(data, record.data.srv.priority);
        data = mdns_htons(data, record.data.srv.weight);
        data = mdns_htons(data, record.data.srv.port);
        data = mdns_string_make(buffer, capacity, data, record.data.srv.name.str, record.data.srv.name.length);
        break;
    case JUCEAppBasics::mDNSServiceAdvertiser::MDNS_RECORDTYPE_A:
        if (remain < 4) return nullptr;
        std::memcpy(data, &record.data.a.addr.sin_addr.s_addr, 4);
        data = static_cast<uint8_t*>(data) + 4;
        break;
    case JUCEAppBasics::mDNSServiceAdvertiser::MDNS_RECORDTYPE_AAAA:
        if (remain < 16) return nullptr;
        std::memcpy(data, &record.data.aaaa.addr.sin6_addr, 16);
        data = static_cast<uint8_t*>(data) + 16;
        break;
    case JUCEAppBasics::mDNSServiceAdvertiser::MDNS_RECORDTYPE_TXT:
        // TXT wird separat behandelt (siehe unten)
        break;
    default:
        break;
    }
    // Datenlänge eintragen
    mdns_htons(length_ptr, (uint16_t)(static_cast<uint8_t*>(data) - static_cast<uint8_t*>(record_data)));
    return data;
}

// TXT-Records werden zu einem Eintrag zusammengefasst
void* mdns_add_txt_records(void* buffer, size_t capacity, void* data, const JUCEAppBasics::mDNSServiceAdvertiser::mdns_record_t* records, size_t record_count) {
    void* length_ptr = nullptr;
    void* record_data = nullptr;
    size_t remain = 0;
    for (size_t i = 0; i < record_count; ++i) {
        if (records[i].type != JUCEAppBasics::mDNSServiceAdvertiser::MDNS_RECORDTYPE_TXT)
            continue;
        if (!record_data) {
            // Header für TXT-Record
            data = mdns_string_make(buffer, capacity, data, records[i].name.str, records[i].name.length);
            if (!data) return nullptr;
            remain = capacity - (static_cast<uint8_t*>(data) - static_cast<uint8_t*>(buffer));
            if (remain < 10) return nullptr;
            data = mdns_htons(data, (uint16_t)records[i].type);
            data = mdns_htons(data, records[i].rclass);
            data = mdns_htonl(data, records[i].ttl);
            length_ptr = data;
            data = mdns_htons(data, 0); // Platzhalter
            record_data = data;
        }
        // TXT-String: <len>key=value
        juce::String key = juce::String(records[i].data.txt.key.str, (int)records[i].data.txt.key.length);
        juce::String value = juce::String(records[i].data.txt.value.str, (int)records[i].data.txt.value.length);
        juce::String txt = key + "=" + value;
        size_t txtlen = txt.getNumBytesAsUTF8();
        remain = capacity - (static_cast<uint8_t*>(data) - static_cast<uint8_t*>(buffer));
        if (remain <= txtlen + 1) return nullptr;
        *(unsigned char*)data = (unsigned char)txtlen;
        std::memcpy(static_cast<uint8_t*>(data) + 1, txt.toRawUTF8(), txtlen);
        data = static_cast<uint8_t*>(data) + txtlen + 1;
    }
    if (record_data)
        mdns_htons(length_ptr, (uint16_t)(static_cast<uint8_t*>(data) - static_cast<uint8_t*>(record_data)));
    return data;
}

sockaddr_in mDNSServiceAdvertiser::makeSockAddrIn(const juce::IPAddress& ip, uint16_t port)
{
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    // Umwandlung von String zu binär
    ::inet_pton(AF_INET, ip.toString().toRawUTF8(), &addr.sin_addr);
    return addr;
}

sockaddr_in6 mDNSServiceAdvertiser::makeSockAddrIn6(const juce::IPAddress& ip, uint16_t port)
{
    sockaddr_in6 addr = {};
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(port);
    // Umwandlung von String zu binär
    ::inet_pton(AF_INET6, ip.toString().toRawUTF8(), &addr.sin6_addr);
    return addr;
}

/*
 * @brief Sends a single broadcast to all available addresses
 */
void mDNSServiceAdvertiser::sendBroadcast()
{
    static juce::IPAddress local = juce::IPAddress::local();

    uint8_t buffer[2048];
    for (auto& address : juce::IPAddress::getAllAddresses())
    {
        if (address == local)
            continue;

        auto broadcastAddress = getInterfaceBroadcastAddress(address);

        // mDNS Header
        struct mdns_header_t {
            uint16_t query_id;
            uint16_t flags;
            uint16_t questions;
            uint16_t answer_rrs;
            uint16_t authority_rrs;
            uint16_t additional_rrs;
        };
        mdns_header_t* header = reinterpret_cast<mdns_header_t*>(buffer);
        header->query_id = MDNS_UNSOLICITED_QUERY_ID;
        header->flags = htons(MDNS_AUTHORITATIVE_RESPONSE_FLAG);
        header->questions = MDNS_QUESTIONS_COUNT_NONE;
        header->answer_rrs = htons(MDNS_ANSWER_RESOURCE_RECORD_COUNT_ONE);
        header->authority_rrs = MDNS_AUTHORITY_RESOURCE_RECORD_COUNT_NONE;
        header->additional_rrs = htons(MDNS_ADDITIONAL_RESOURCE_RECORD_COUNT_WOTXT + m_service.txt_record_cnt);

        void* data = buffer + sizeof(mdns_header_t);

        // Answer: PTR-Record
        data = mdns_add_record(buffer, sizeof(buffer), data, m_service.record_ptr);

        // Additional Records: SRV, A, AAAA, TXT[0], TXT[1]
        data = mdns_add_record(buffer, sizeof(buffer), data, m_service.record_srv);
        if (m_service.address_ipv4.sin_family == AF_INET)
            data = mdns_add_record(buffer, sizeof(buffer), data, m_service.record_a);
        if (m_service.address_ipv6.sin6_family == AF_INET6)
            data = mdns_add_record(buffer, sizeof(buffer), data, m_service.record_aaaa);

        // TXT-Records (werden zu einem Eintrag zusammengefasst)
        data = mdns_add_txt_records(buffer, sizeof(buffer), data, m_service.txt_record, m_service.txt_record_cnt);

        // Paketlänge berechnen
        size_t datalen = static_cast<uint8_t*>(data) - buffer;

        m_socket.write(broadcastAddress.toString(), m_broadcastPort, (const char*)buffer, (int)datalen);
    }
}


} // namespace JUCEAppBasics
