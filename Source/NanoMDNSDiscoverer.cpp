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


namespace JUCEAppBasics
{

/**
* Constructor of the NanoMDNSDiscoverer class
* @param portNumber	The port to listen on for incoming data
*/
NanoMDNSDiscoverer::NanoMDNSDiscoverer(const String& mDNSServiceTag) : Thread("NanoMDNS_Discovery_Server")
{
	m_mDNSServiceTag = mDNSServiceTag;
	m_socket = std::make_unique<DatagramSocket>();
}

/**
* Destructor of the NanoMDNSDiscoverer class
*/
NanoMDNSDiscoverer::~NanoMDNSDiscoverer()
{
}

bool NanoMDNSDiscoverer::sendQuery()
{
	std::vector<unsigned char> queryData;

	// 2 bytes transaction ID
	queryData.push_back(0x00);
	queryData.push_back(0x00);

	// 2 bytes flags
	// 0x	0    0    0    0
	//		0... .... .... .... = Response: Message is a query
	//		.000 0... .... .... = Opcode: Standard query (0)
	//		.... ..0. .... .... = Truncated: Message is not truncated
	//		.... ...0 .... .... = Recursion desired: Don't do query recursively
	//		.... .... .0.. .... = Z: reserved (0)
	//		.... .... ...0 .... = Non-authenticated data: Unacceptable
	queryData.push_back(0x00);
	queryData.push_back(0x00);

	// 2 bytes questions count
	queryData.push_back(0x00);
	queryData.push_back(0x01); // one question

	// 2 bytes answer RRs
	queryData.push_back(0x00);
	queryData.push_back(0x00); // no answer

	// 2 bytes authorityRRs
	queryData.push_back(0x00);
	queryData.push_back(0x00);

	// 2 bytes additionalRRs
	queryData.push_back(0x00);
	queryData.push_back(0x00);

	// single actual query
	StringArray serviceTagElements;
	serviceTagElements.addTokens(m_mDNSServiceTag, ". ", "");
	for (auto const& element : serviceTagElements)
	{
		queryData.push_back(0x04);

		for (auto iter = element.begin(); iter != element.end(); iter++)
			queryData.push_back(*iter);
	}
	queryData.push_back(0x05); // ??
	queryData.push_back(0x6c); // l
	queryData.push_back(0x6f); // a
	queryData.push_back(0x63); // b
	queryData.push_back(0x61); // e
	queryData.push_back(0x6c); // l
	queryData.push_back(0x00); // ??
	
	// 2 bytes request type
	queryData.push_back(0x00);
	queryData.push_back(0xff); // all records the server/cache has available

	// 1 byte class
	queryData.push_back(0x80); // question: true

	// 1 byte type
	queryData.push_back(0x01); // IN

	if (!m_socket->waitUntilReady(false, 1000))
	{
		DBG(String(__FUNCTION__) + String(" failed to prepare writing query on ") + RFC6762_MULTICAST_IP + ":" + String(RFC6762_MULTICAST_PORT));
		return false;
	}
	else if (-1 == m_socket->write(RFC6762_MULTICAST_IP, RFC6762_MULTICAST_PORT, queryData.data(), queryData.size()))
	{
		DBG(String(__FUNCTION__) + String(" failed to write mDNS service tag to ") + RFC6762_MULTICAST_IP + ":" + String(RFC6762_MULTICAST_PORT));
		return false;
	}
	else
	{
		return true;
	}
}

/**
* Method to start the listener thread
* @return True on success, false on failure
*/
bool NanoMDNSDiscoverer::start()
{
	stop();

	m_socket = std::make_unique<DatagramSocket>();	//  deletes the old object that it was previously pointing to if there was one. 

	if (m_socket->bindToPort(RFC6762_MULTICAST_PORT))
	{
		if (!m_socket->joinMulticast(RFC6762_MULTICAST_IP))
			DBG(String(__FUNCTION__) + String(" failed to join multicast on ") + RFC6762_MULTICAST_IP + ":" + String(RFC6762_MULTICAST_PORT));

		startThread();

		if (!sendQuery())
			DBG(String(__FUNCTION__) + String(" failed to write query on ") + RFC6762_MULTICAST_IP + ":" + String(RFC6762_MULTICAST_PORT));
		else
			return true;
	}
	else
		DBG(String(__FUNCTION__) + String(" failed to bind to ") + RFC6762_MULTICAST_IP + ":" + String(RFC6762_MULTICAST_PORT));

	m_socket->leaveMulticast(RFC6762_MULTICAST_IP);
	m_socket.reset();
	return false;
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

	m_socket->leaveMulticast(RFC6762_MULTICAST_IP);
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
bool NanoMDNSDiscoverer::HandleBuffer(unsigned char* dataBuffer, size_t bytesRead, ServiceDiscoverInfo& decodedMessage)
{
	std::vector<unsigned char> data(dataBuffer, dataBuffer + bytesRead);

	DBG(String(__FUNCTION__) + String(" processing ") + String(data.size()) + String(" bytes"));

	auto readPos = 0;
	std::uint16_t transactionID, flags, questions, answers, authorities, additionals;
	if (!ProcessRFC6762Header(data, readPos, transactionID, flags, questions, answers, authorities, additionals))
		return false;
	
	for (int i = 0; i < questions; i++)
	{
		std::string questionData;
		if (!ProcessRFC6762Question(data, readPos, questionData))
			return false;

		DBG(String(__FUNCTION__) + String(" processed question ") + String(questionData));
	}
	for (int i = 0; i < answers; i++)
	{
		std::string answerData;
		if (!ProcessRFC6762Answer(data, readPos, answerData))
			return false;

		DBG(String(__FUNCTION__) + String(" processed answer ") + String(answerData));
	}
	for (int i = 0; i < authorities; i++)
	{
		if (!ProcessRFC6762Authority(data, readPos))
			return false;
	}
	for (int i = 0; i < additionals; i++)
	{
		if (!ProcessRFC6762Additional(data, readPos))
			return false;
	}
	
	return true;
}

bool NanoMDNSDiscoverer::ProcessRFC6762Header(std::vector<unsigned char>& data, int& readPos, std::uint16_t& transactionID, std::uint16_t& flags, std::uint16_t& questions, std::uint16_t& answers, std::uint16_t& authorities, std::uint16_t& additionals)
{
	if (data.size() + readPos < 12)
		return false;

	transactionID = std::uint16_t(0);
	transactionID += (data.at(readPos) << 4) & 0xf0;
	readPos++;
	transactionID += (data.at(readPos) << 0) & 0x0f;
	readPos++;

	flags = std::uint16_t(0);
	flags += (data.at(readPos) << 4) & 0xf0;
	readPos++;
	flags += (data.at(readPos) << 0) & 0x0f;
	readPos++;

	questions = std::uint16_t(0);
	questions += (data.at(readPos) << 4) & 0xf0;
	readPos++;
	questions += (data.at(readPos) << 0) & 0x0f;
	readPos++;

	answers = std::uint16_t(0);
	answers += (data.at(readPos) << 4) & 0xf0;
	readPos++;
	answers += (data.at(readPos) << 0) & 0x0f;
	readPos++;

	authorities = std::uint16_t(0);
	authorities += (data.at(readPos) << 4) & 0xf0;
	readPos++;
	authorities += (data.at(readPos) << 0) & 0x0f;
	readPos++;

	additionals = std::uint16_t(0);
	additionals += (data.at(readPos) << 4) & 0xf0;
	readPos++;
	additionals += (data.at(readPos) << 0) & 0x0f;
	readPos++;

	DBG(String(__FUNCTION__) + String(" processed ")
		+ String("transactionID:") + String(transactionID) + String(" ")
		+ String("flags:") + String(flags) + String(" ")
		+ String("questions:") + String(questions) + String(" ")
		+ String("answers:") + String(answers) + String(" ")
		+ String("authorities:") + String(authorities) + String(" ")
		+ String("additionals:") + String(additionals) + String(" "));
	
	return true;
}

bool NanoMDNSDiscoverer::ProcessRFC6762Question(std::vector<unsigned char>& data, int& readPos, std::string& questionData)
{
	auto questionLength = 0;
	while (data.size() > (readPos + questionLength) && data.at((readPos + questionLength)) != 0x05)
	{
		questionLength++;
	}
	questionData = std::string(reinterpret_cast<char*>(data.data() + readPos), questionLength);

	readPos += questionLength;

	if (data.at(readPos) == 0x05)
	{
		auto questionDomain = String(reinterpret_cast<char*>(data.data()) + readPos, 7);
		auto expectedQuestionDomain = String("\x5local\0");
		if (questionDomain == expectedQuestionDomain)
			return true;
	}

	return false;
}

bool NanoMDNSDiscoverer::ProcessRFC6762Answer(std::vector<unsigned char>& data, int& readPos, std::string& answerData)
{ 
	return false;
}

bool NanoMDNSDiscoverer::ProcessRFC6762Authority(std::vector<unsigned char>& data, int& readPos)
{ 
	return false;
}

bool NanoMDNSDiscoverer::ProcessRFC6762Additional(std::vector<unsigned char>& data, int& readPos)
{
	return false;
}

/**
* It waits until a client sends any message to the host.
* If a message arrives it will call up the HandleBuffer method which will sort the module information.
*/
void NanoMDNSDiscoverer::run()
{
	int bufferSize = 512;
	HeapBlock<unsigned char> serviceInfoBuffer(bufferSize);
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
		int bytesRead = m_socket->read(serviceInfoBuffer.getData(), bufferSize, false, senderIPAddress, senderPortNumber);

		if(bytesRead >= 0)
		{
			ServiceDiscoverInfo receivedServiceMessage(senderIPAddress, senderPortNumber);
			int moduleCount = HandleBuffer(serviceInfoBuffer.getData(), bytesRead, receivedServiceMessage);
			if (moduleCount > 0)
			{
				if (!m_realtimeListeners.isEmpty())
					callRealtimeListeners(receivedServiceMessage);
				
				if (!m_listeners.isEmpty())
					postMessage(std::make_unique<CallbackMessage>(receivedServiceMessage).release());
			}
		}
	}
}

/**
 * Reimplemented from MessageListener to handle messages posted to queue.
 * @param msg	The incoming message to handle
 */
void NanoMDNSDiscoverer::handleMessage(const Message& msg)
{
	if (auto* callbackMessage = dynamic_cast<const CallbackMessage*> (&msg))
	{
		callListeners(callbackMessage->service/*, callbackMessage->senderIPAddress, callbackMessage->senderPort*/);
	}
}

/**
 * Helper method that handles distributing given message data to all registered datamessage listeners.
 * @param content			The message to distribute.
 * @param senderIPAddress	The ip address the data message was received from.
 * @param senderPort		The port the data message was received from.
 */
void NanoMDNSDiscoverer::callListeners(const ServiceDiscoverInfo& content/*, const String& senderIPAddress, const int& senderPort*/)
{
	m_listeners.call([&](NanoMDNSDiscoverer::DiscoveryListener& l) { l.ServiceDiscovered(content); });
}

/**
 * Helper method that handles distributing given message data to all registered realtime datamessage listeners.
 * @param content			The service info to distribute.
 * @param senderIPAddress	The ip address the data message was received from.
 * @param senderPort		The port the data message was received from.
 */
void NanoMDNSDiscoverer::callRealtimeListeners(const ServiceDiscoverInfo& content/*, const String& senderIPAddress, const int& senderPort*/)
{
	m_realtimeListeners.call([&](NanoMDNSDiscoverer::RealtimeDiscoveryListener& l) { l.ServiceDiscovered(content); });
}

};