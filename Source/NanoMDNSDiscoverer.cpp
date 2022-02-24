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

#include "NanoMDNSDiscoverer.h"


/**
* Constructor of the NanoMDNSDiscoverer class
* @param portNumber	The port to listen on for incoming data
*/
NanoMDNSDiscoverer::NanoMDNSDiscoverer(int portNumber) : Thread("RTTrPM_Connection_Server")
{
	m_socket = std::make_unique<DatagramSocket>();
	m_listeningPort = portNumber;
}

/**
* Destructor of the NanoMDNSDiscoverer class
*/
NanoMDNSDiscoverer::~NanoMDNSDiscoverer()
{
}

/**
* Method to start the listener thread
* @return True on success, false on failure
*/
bool NanoMDNSDiscoverer::start()
{
	return BeginWaitingForSocket(m_listeningPort);
}

/**
* Method to terminate the listener thread
* @return Currently always true
*/
bool NanoMDNSDiscoverer::stop()
{
	signalThreadShouldExit();

	if (m_socket != nullptr)
		m_socket->shutdown();

	stopThread(4000);
	m_socket.reset();

	return true;
}

/**
 * Method to add a Listener to internal list.
 * @param listenerToAdd	The listener object to add.
 */
void NanoMDNSDiscoverer::addListener(NanoMDNSDiscoverer::DiscoveryListener* listenerToAdd)
{
	m_listeners.add(listenerToAdd);
}

/**
 * Method to add a Listener to internal list of realtime listeners.
 * @param listenerToAdd	The realtime listener object to add.
 */
void NanoMDNSDiscoverer::addListener(NanoMDNSDiscoverer::RealtimeDiscoveryListener* listenerToAdd)
{
	m_realtimeListeners.add(listenerToAdd);
}

/**
 * Method to remove a Listener from internal list.
 * @param listenerToRemove	The listener object to remove.
 */
void NanoMDNSDiscoverer::removeListener(NanoMDNSDiscoverer::DiscoveryListener* listenerToRemove)
{
	m_listeners.remove(listenerToRemove);
}

/**
 * Method to remove a Listener from internal list of realtime listeners.
 * @param listenerToRemove	The realtime listener object to remove.
 */
void NanoMDNSDiscoverer::removeListener(NanoMDNSDiscoverer::RealtimeDiscoveryListener* listenerToRemove)
{
	m_realtimeListeners.remove(listenerToRemove);
}

/**
* Reads all packet modules and sorts them within the CCentroidMod class, saves all modules into packetModules vector
* @param	dataBuffer		: An array which keeps the caught data information.
* @param	bytesRead		: Keeps the number of read bytes
* @param	packetModules	: Module vector that is filled with the modules read from the buffer
*
* @return	Returns the count of packet modules read into given target module content vector
*/
int NanoMDNSDiscoverer::HandleBuffer(unsigned char* dataBuffer, size_t bytesRead, ServiceDiscoverMessage& decodedMessage)
{
	std::vector<unsigned char> data(dataBuffer, dataBuffer + bytesRead);
	int readPos = 0;

	//auto& header = decodedMessage.header;
	//auto& packetModules = decodedMessage.modules;
	//
	//header = RTTrPMHeader(data, readPos);					
	//
	//if(readPos == 0)
	//	return 0;
	//
	//auto packetModuleCount = header.GetNumberOfModules();
	//for (int i = 0; i < packetModuleCount; i++)
	//{
	//	packetModules.push_back(std::make_unique<PacketModuleTrackable>(data, readPos));
	//
	//	auto trackableSubModuleCount = dynamic_cast<PacketModuleTrackable*>(packetModules.back().get())->GetNumberOfSubModules();
	//	for (int j = 0; j < trackableSubModuleCount; j++)
	//	{
	//		auto metaInfoReadPos = readPos;
	//		PacketModule packetModuleMetaInfo(data, metaInfoReadPos);
	//		
	//		switch(packetModuleMetaInfo.GetModuleType())				
	//		{
	//			case PacketModule::CentroidPosition:
	//				packetModules.push_back(std::make_unique<CentroidPositionModule>(data, readPos));
	//				break;
	//			case PacketModule::CentroidAccelerationAndVelocity:
	//				packetModules.push_back(std::make_unique<CentroidAccelAndVeloModule>(data, readPos));
	//				break;
	//			case PacketModule::TrackedPointPosition:
	//				packetModules.push_back(std::make_unique<TrackedPointPositionModule>(data, readPos));
	//				break;
	//			case PacketModule::TrackedPointAccelerationandVelocity:
	//				packetModules.push_back(std::make_unique<TrackedPointAccelAndVeloModule>(data, readPos));
	//				break;
	//			case PacketModule::OrientationQuaternion:
	//				packetModules.push_back(std::make_unique<OrientationQuaternionModule>(data, readPos));
	//				break;
	//			case PacketModule::OrientationEuler:
	//				packetModules.push_back(std::make_unique<OrientationEulerModule>(data, readPos));
	//				break;
	//			case PacketModule::ZoneCollisionDetection:
	//				packetModules.push_back(std::make_unique<ZoneCollisionDetectionModule>(data, readPos));
	//				break;
	//			default:
	//				break;
	//		}
	//	}
	//}
	//
	//return static_cast<int>(packetModules.size());
	
	return 0;
}

/**
* It waits until a client sends any message to the host.
* If a message arrives it will call up the HandleBuffer method which will sort the module information.
*/
void NanoMDNSDiscoverer::run()
{
	int bufferSize = 512;
	HeapBlock<unsigned char> rttrpmBuffer(bufferSize);
	String senderIPAddress;
	int senderPortNumber;

	while (!threadShouldExit())
	{
		jassert(m_socket != nullptr);
		auto ready = m_socket->waitUntilReady(true, 100);

		if (ready < 0 || threadShouldExit())
			return;

		if (ready == 0)
			continue;

		//	bytesRead returns the number of read bytes, or -1 if there was an error.
		int bytesRead = m_socket->read(rttrpmBuffer.getData(), bufferSize, false, senderIPAddress, senderPortNumber);

		if(bytesRead >= 4)
		{
			ServiceDiscoverMessage receivedMessage;
			int moduleCount = HandleBuffer(rttrpmBuffer.getData(), bytesRead, receivedMessage);
			if (moduleCount > 0)
			{
				if (!m_realtimeListeners.isEmpty())
					callRealtimeListeners(receivedMessage, senderIPAddress, senderPortNumber);
				
				if (!m_listeners.isEmpty())
					postMessage(std::make_unique<CallbackMessage>(receivedMessage, senderIPAddress, senderPortNumber).release());
			}
		}
	}
}

/**
* Method for binding the socket to the specified local port and local address.
*
* @param	portNumber		: The port on which the server will receive connections
* @param	bindAddress		: The address on which the server will listen for connections.
*							  An empty string indicates that it should listen on all addresses assigned to this machine.
* @return	bool			: if true, thread is running. If false, it clears the pointer
*/
bool NanoMDNSDiscoverer::BeginWaitingForSocket(const int portNumber, const String &bindAddress)
{
	stop();

	m_socket = std::make_unique<DatagramSocket>();	//  deletes the old object that it was previously pointing to if there was one. 

	if(m_socket->bindToPort(portNumber, bindAddress))
	{
		startThread();
		return true;
	}

	m_socket.reset();
	return false;
}

/**
 * Reimplemented from MessageListener to handle messages posted to queue.
 * @param msg	The incoming message to handle
 */
void NanoMDNSDiscoverer::handleMessage(const Message& msg)
{
	if (auto* callbackMessage = dynamic_cast<const CallbackMessage*> (&msg))
	{
		callListeners(callbackMessage->service, callbackMessage->senderIPAddress, callbackMessage->senderPort);
	}
}

/**
 * Helper method that handles distributing given message data to all registered datamessage listeners.
 * @param contentMessage	The message to distribute.
 * @param senderIPAddress	The ip address the data message was received from.
 * @param senderPort		The port the data message was received from.
 */
void NanoMDNSDiscoverer::callListeners(const ServiceDiscoverMessage& contentMessage, const String& senderIPAddress, const int& senderPort)
{
	m_listeners.call([&](NanoMDNSDiscoverer::DiscoveryListener& l) { l.ServiceDiscovered(contentMessage); });
}

/**
 * Helper method that handles distributing given message data to all registered realtime datamessage listeners.
 * @param contentMessage	The message to distribute.
 * @param senderIPAddress	The ip address the data message was received from.
 * @param senderPort		The port the data message was received from.
 */
void NanoMDNSDiscoverer::callRealtimeListeners(const ServiceDiscoverMessage& contentMessage, const String& senderIPAddress, const int& senderPort)
{
	m_realtimeListeners.call([&](NanoMDNSDiscoverer::RealtimeDiscoveryListener& l) { l.ServiceDiscovered(contentMessage); });
}