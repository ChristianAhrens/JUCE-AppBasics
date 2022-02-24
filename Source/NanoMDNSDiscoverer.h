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
	struct ServiceDiscoverMessage
	{
		String	m_serviceIPAddress;
		int		m_servicePort;
		String	m_serviceName;
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
		CallbackMessage(ServiceDiscoverMessage& service) : service(std::move(service)), senderIPAddress(String()), senderPort(0) {}

		/**
		* Constructor with default initialization of sender ip and port.
		*
		* @param rttrpmModule	The rttrpm module data to use in this message.
		* @param sndIP	The sender ip of this message.
		* @param sndPort The port this message was received on.
		*/
		CallbackMessage(ServiceDiscoverMessage& service, String sndIP, int sndPort) : service(std::move(service)), senderIPAddress(sndIP), senderPort(sndPort) {}

		ServiceDiscoverMessage	service;			/**< The payload of the message. */
		String					senderIPAddress;	/**< The sender ip address from whom the message was received. */
		int						senderPort;			/**< The sender port from where the message was received. */
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

		/** Called when the NanoMDNSDiscoverer receives new RTTrPM module(s). */
		virtual void ServiceDiscovered(const ServiceDiscoverMessage& service) = 0;
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

		/** Called when the NanoMDNSDiscoverer receives new RTTrPM module(s). */
		virtual void ServiceDiscovered(const ServiceDiscoverMessage& service) = 0;
	};

public:
	NanoMDNSDiscoverer(int portNumber);
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
	bool BeginWaitingForSocket(const int portNumber, const String &bindAddress = String());

	void run() override;
	int HandleBuffer(unsigned char* dataBuffer, size_t bytesRead, ServiceDiscoverMessage& decodedMessage);

	//==============================================================================
	void handleMessage(const Message& msg) override;
	void callListeners(const ServiceDiscoverMessage& content, const String& senderIPAddress, const int& senderPort);
	void callRealtimeListeners(const ServiceDiscoverMessage& content, const String& senderIPAddress, const int& senderPort);

	//==============================================================================
	std::unique_ptr<DatagramSocket>	m_socket;

	//==============================================================================
	int													m_listeningPort{ 0 };
	ListenerList<NanoMDNSDiscoverer::DiscoveryListener>			m_listeners;
	ListenerList<NanoMDNSDiscoverer::RealtimeDiscoveryListener>	m_realtimeListeners;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NanoMDNSDiscoverer)
};