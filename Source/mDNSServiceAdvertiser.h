/* Copyright (c) 2024, Christian Ahrens
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
#pragma once

#include <JuceHeader.h>

#ifdef _WIN32
#include <winsock2.h>
//#include <iphlpapi.h>
#include <WS2tcpip.h>
//#define sleep(x) Sleep(x * 1000)
#else
//#include <netdb.h>
//#include <ifaddrs.h>
//#include <net/if.h>
//#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

namespace JUCEAppBasics
{


/** 
 * @brief   Copy of juce::NetworkServiceDiscovery::Advertiser, tweaked to do a brute-force mDNS broadcast 
 *          with a custom data structure instead of the original juce::NetworkServiceDiscovery::Advertiser::message XmlELement.
 */
class mDNSServiceAdvertiser : private juce::Thread
{
public:
	//==============================================================================
	#define MDNS_PORT 5353
	#define MDNS_UNICAST_RESPONSE 0x8000U
	#define MDNS_CACHE_FLUSH 0x8000U
	#define MDNS_MAX_SUBSTRINGS 64
	#define MDNS_MAX_TXTRECORDS 16

	#define MDNS_UNSOLICITED_QUERY_ID 0
	#define MDNS_AUTHORITATIVE_RESPONSE_FLAG 0x8400U

	enum mdns_record_type {
		MDNS_RECORDTYPE_IGNORE = 0,
		// Address
		MDNS_RECORDTYPE_A = 1,
		// Domain Name pointer
		MDNS_RECORDTYPE_PTR = 12,
		// Arbitrary text string
		MDNS_RECORDTYPE_TXT = 16,
		// IP6 Address [Thomson]
		MDNS_RECORDTYPE_AAAA = 28,
		// Server Selection [RFC2782]
		MDNS_RECORDTYPE_SRV = 33,
		// Any available records
		MDNS_RECORDTYPE_ANY = 255
	};

	enum mdns_entry_type {
		MDNS_ENTRYTYPE_QUESTION = 0,
		MDNS_ENTRYTYPE_ANSWER = 1,
		MDNS_ENTRYTYPE_AUTHORITY = 2,
		MDNS_ENTRYTYPE_ADDITIONAL = 3
	};

	enum mdns_class { MDNS_CLASS_IN = 1, MDNS_CLASS_ANY = 255 };

	typedef enum mdns_record_type mdns_record_type_t;
	typedef enum mdns_entry_type mdns_entry_type_t;
	typedef enum mdns_class mdns_class_t;

#ifdef _WIN32
	typedef int mdns_size_t;
	typedef int mdns_ssize_t;
#else
	typedef size_t mdns_size_t;
	typedef ssize_t mdns_ssize_t;
#endif

	struct mdns_string_t {
		const char* str = nullptr;
		size_t length = 0;
	};

	struct mdns_string_pair_t {
		size_t offset;
		size_t length;
		int ref;
	};

	struct mdns_string_table_t {
		size_t offset[16];
		size_t count;
		size_t next;
	};

	struct mdns_record_srv_t {
		std::uint16_t priority;
		std::uint16_t weight;
		std::uint16_t port;
		mdns_string_t name;
	};

	struct mdns_record_ptr_t {
		mdns_string_t name;
	};

	struct mdns_record_a_t {
		struct sockaddr_in addr;
	};

	struct mdns_record_aaaa_t {
		struct sockaddr_in6 addr;
	};

	struct mdns_record_txt_t {
		mdns_string_t key;
		mdns_string_t value;
	};

	struct mdns_record_data_t {
		mdns_record_ptr_t ptr;
		mdns_record_srv_t srv;
		mdns_record_a_t a;
		mdns_record_aaaa_t aaaa;
		mdns_record_txt_t txt;
	};

	struct mdns_record_t {
		mdns_string_t name;
		mdns_record_type_t type;
		mdns_record_data_t data;
		std::uint16_t rclass;
		std::uint32_t ttl;
	};

	struct mdns_header_t {
		std::uint16_t query_id;
		std::uint16_t flags;
		std::uint16_t questions;
		std::uint16_t answer_rrs;
		std::uint16_t authority_rrs;
		std::uint16_t additional_rrs;
	};

	struct mdns_query_t {
		mdns_record_type_t type;
		const char* name;
		size_t length;
	};

    // Data for our service including the mDNS records
    struct service_t {
		mdns_string_t service;
        mdns_string_t hostname;
        mdns_string_t service_instance;
        mdns_string_t hostname_qualified;
        struct sockaddr_in address_ipv4;
        struct sockaddr_in6 address_ipv6;
        int port;
        mdns_record_t record_ptr;
        mdns_record_t record_srv;
        mdns_record_t record_a;
        mdns_record_t record_aaaa;
        mdns_record_t txt_record[MDNS_MAX_TXTRECORDS];
		size_t txt_record_cnt;
    };

	static void append(const std::vector<uint8_t>& dataIn, std::vector<uint8_t>& bufferOut)
	{
		bufferOut.insert(bufferOut.end(), dataIn.begin(), dataIn.end());
	};
	static void append16(std::uint16_t val, std::vector<uint8_t>& dataOut)
	{
		// append data in network byteorder
		dataOut.push_back(static_cast<std::uint8_t>(val >> 8));
		dataOut.push_back(static_cast<std::uint8_t>(val & 0xFF));
	};
	static void append32(std::uint32_t val, std::vector<uint8_t>& dataOut)
	{
		// append data in network byteorder
		dataOut.push_back(static_cast<std::uint8_t>(val >> 24));
		dataOut.push_back(static_cast<std::uint8_t>((val >> 16) & 0xFF));
		dataOut.push_back(static_cast<std::uint8_t>((val >> 8) & 0xFF));
		dataOut.push_back(static_cast<std::uint8_t>(val & 0xFF));
	};

public:
    //==============================================================================
    mDNSServiceAdvertiser(const juce::String& serviceTypeUID, std::uint16_t servicePort, juce::RelativeTime minTimeBetweenBroadcasts = juce::RelativeTime::seconds(1.5));
    ~mDNSServiceAdvertiser() override;

	void addTxtRecord(const std::string& key, const std::string& value);
	void addTxtRecords(const std::map<std::string, std::string>& txtRecords);
	void removeTxtRecord(const std::string& key);

private:
    //==============================================================================
    void run() override;

    //==============================================================================
    juce::String getMulticastDNSAddress();
	int getMulticastDNSPort();
    void buildService();
    void sendMulticast();

	//==============================================================================
	std::vector<uint8_t> makeDnsLabel(std::string_view name);
	std::vector<uint8_t> serializeMDNSRecord(const mDNSServiceAdvertiser::mdns_record_t& record);
	std::vector<std::uint8_t> serializeMDNSTxtRecords(const mDNSServiceAdvertiser::mdns_record_t* records, size_t record_count);
	sockaddr_in makeSockAddrIn(const juce::IPAddress& ip, std::uint16_t port);
	sockaddr_in6 makeSockAddrIn6(const juce::IPAddress& ip, std::uint16_t port);
	void mallocMDNSString(const juce::String& inputString, mdns_string_t& outputString);
	void freeMDNSString(mdns_string_t& string);

    //==============================================================================
	juce::String m_serviceTypeUID;
    std::uint16_t m_connectionPort;
	std::map<std::string, std::string>	m_txtRecords;
    const juce::RelativeTime m_minInterval;
    juce::DatagramSocket m_socket{ true };

	std::mutex m_serviceMutex;
	service_t m_service{ 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(mDNSServiceAdvertiser);
};


} // namespace JUCEAppBasics
