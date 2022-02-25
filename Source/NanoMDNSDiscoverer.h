/* Copyright (c) 2020-2022, Christian Ahrens
 *
 * This file is part of JUCE-AppBasics <https://github.com/ChristianAhrens/JUCE-AppBasics>
 *
 * This application/library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This application/library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#pragma once

#include <JuceHeader.h>

namespace JUCEAppBasics
{

// **************************************************************
// class NanoMDNSDiscoverer
// **************************************************************
/**
*
* Creates an uninitialised server object, that will wait
* for client sockets to connect to a port on this host.
*/
class NanoMDNSDiscoverer : private Thread,
	private MessageListener
{
public:
	//==============================================================================
	/**
	 * Helper type that combines information on the discovered service
	 */
	struct ServiceDiscoverInfo
	{
	public:
		ServiceDiscoverInfo(const String& serviceIPAddress, const int& servicePort)
		{
			m_IPAddress = serviceIPAddress;
			m_Port = servicePort;
		}

		String	GetIPAddress() { return m_IPAddress; };
		int		GetPort() { return m_Port; };
		String	GetName() { return m_Name; };
		String	GetHost() { return m_Host; };

		void SetIPAddress(String	iPAddress) { m_IPAddress = iPAddress; };
		void SetPort(int port) { m_Port = port; };
		void SetName(String name) { m_Name = name; };
		void SetHost(String host) { m_Host = host; };

	private:
		String	m_IPAddress;
		int		m_Port;
		String	m_Name;
		String	m_Host;
	};

	//==============================================================================
	/**
	 * Implementation of a ServiceDiscovered message. This is designed similar to JUCEs' OSCReceiver::pimpl
	 */
	struct CallbackMessage : public Message
	{
		/**
		* Constructor with default initialization of sender ip and port.
		*
		* @param rttrpmModule	The rttrpm module data to use in this message.
		*/
		CallbackMessage(ServiceDiscoverInfo& service) : service(std::move(service)), senderIPAddress(String()), senderPort(0) {}

		/**
		* Constructor with default initialization of sender ip and port.
		*
		* @param rttrpmModule	The rttrpm module data to use in this message.
		* @param sndIP	The sender ip of this message.
		* @param sndPort The port this message was received on.
		*/
		CallbackMessage(ServiceDiscoverInfo& service, String sndIP, int sndPort) : service(std::move(service)), senderIPAddress(sndIP), senderPort(sndPort) {}

		ServiceDiscoverInfo	service;			/**< The payload of the message. */
		String				senderIPAddress;	/**< The sender ip address from whom the message was received. */
		int					senderPort;			/**< The sender port from where the message was received. */
	};
	
	//==============================================================================
	/** 
	 * A class for receiving service info data from NanoMDNSDiscoverer.
	 */
	class DiscoveryListener
	{
	public:
		/** Destructor. */
		virtual ~DiscoveryListener() = default;

		/** Called when the NanoMDNSDiscoverer receives new service infos. */
		virtual void ServiceDiscovered(const ServiceDiscoverInfo& service) = 0;
	};

	//==============================================================================
	/**
	 * A class for receiving RTTrPM data in realtime (not through message queue, but direct callback) from NanoMDNSDiscoverer.
	 */
	class RealtimeDiscoveryListener
	{
	public:
		/** Destructor. */
		virtual ~RealtimeDiscoveryListener() = default;

		/** Called when the NanoMDNSDiscoverer receives new service infos. */
		virtual void ServiceDiscovered(const ServiceDiscoverInfo& service) = 0;
	};

public:
	NanoMDNSDiscoverer(const String& mDNSServiceTag);
	~NanoMDNSDiscoverer();

	//==============================================================================
	bool start();
	bool stop();

	//==============================================================================
	void addListener(NanoMDNSDiscoverer::DiscoveryListener* listenerToAdd);
	void addListener(NanoMDNSDiscoverer::RealtimeDiscoveryListener* listenerToAdd);
	void removeListener(NanoMDNSDiscoverer::DiscoveryListener* listenerToRemove);
	void removeListener(NanoMDNSDiscoverer::RealtimeDiscoveryListener* listenerToRemove);

private:
	//==============================================================================
	void run() override;

	//==============================================================================
	bool HandleBuffer(unsigned char* dataBuffer, size_t bytesRead, ServiceDiscoverInfo& decodedMessage);
	bool ProcessRFC6762Header(std::vector<unsigned char>& data, int& readPos, std::uint16_t& transactionID, std::uint16_t& flags, std::uint16_t& questions, std::uint16_t& answers, std::uint16_t& authorities, std::uint16_t& additionals);
	bool ProcessRFC6762Question(std::vector<unsigned char>& data, int& readPos, std::string& questionData);
	bool ProcessRFC6762Answer(std::vector<unsigned char>& data, int& readPos, std::string& answerData);
	bool ProcessRFC6762Authority(std::vector<unsigned char>& data, int& readPos);
	bool ProcessRFC6762Additional(std::vector<unsigned char>& data, int& readPos);

	bool sendQuery();

	//==============================================================================
	void handleMessage(const Message& msg) override;
	void callListeners(const ServiceDiscoverInfo& content/*, const String& senderIPAddress, const int& senderPort*/);
	void callRealtimeListeners(const ServiceDiscoverInfo& content/*, const String& senderIPAddress, const int& senderPort*/);

	//==============================================================================
	std::unique_ptr<DatagramSocket>	m_socket;

	//==============================================================================
	ListenerList<NanoMDNSDiscoverer::DiscoveryListener>			m_listeners;
	ListenerList<NanoMDNSDiscoverer::RealtimeDiscoveryListener>	m_realtimeListeners;

	String m_mDNSServiceTag;

	static constexpr char LOCALHOST_IP[] = "127.0.0.1";
	static constexpr char RFC6762_MULTICAST_IP[] = "224.0.0.251";	// https://tools.ietf.org/html/rfc6762
	static constexpr int RFC6762_MULTICAST_PORT = 5353;				// https://tools.ietf.org/html/rfc6762

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NanoMDNSDiscoverer)
};

};