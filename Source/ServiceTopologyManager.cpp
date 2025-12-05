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

#include "ServiceTopologyManager.h"

#include <ServiceTopologyTreeView.h>

namespace JUCEAppBasics
{


//==============================================================================
ServiceTopologyManager::ServiceDiscovery::ServiceDiscovery(const juce::String& serviceTypeBase, int broadcastPort)
    : juce::Thread(juce::JUCEApplication::getInstance()->getApplicationName() + ": Discovery_listen"), m_serviceTypeUIDBase(serviceTypeBase)
{
#if JUCE_ANDROID
    acquireMulticastLock();
#endif

    auto success = true;
    success = success && m_socket.setEnablePortReuse(true);
    jassert(success);
    success = success && m_socket.bindToPort(broadcastPort);
    jassert(success);
    startThread(juce::Thread::Priority::background);
}

ServiceTopologyManager::ServiceDiscovery::~ServiceDiscovery()
{
    m_socket.shutdown();
    stopThread(2000);

#if JUCE_ANDROID
    releaseMulticastLock();
#endif
}

void ServiceTopologyManager::ServiceDiscovery::run()
{
    while (!threadShouldExit())
    {
        if (m_socket.waitUntilReady(true, 200) == 1)
        {
            char buffer[1024];
            auto bytesRead = m_socket.read(buffer, sizeof(buffer) - 1, false);

            if (bytesRead > 10)
                if (auto xml = parseXML(juce::String(juce::CharPointer_UTF8(buffer),
                    juce::CharPointer_UTF8(buffer + bytesRead))))
                    if (xml->getTagName().startsWith(m_serviceTypeUIDBase))
                        handleMessage(*xml);
        }

        removeTimedOutServices();
    }
}

std::vector<SessionMasterAwareService> ServiceTopologyManager::ServiceDiscovery::getServices() const
{
    const juce::ScopedLock sl(m_listLock);
    auto listCopy = m_services;
    return listCopy;
}

juce::String ServiceTopologyManager::ServiceDiscovery::knownServicesToString()
{
    auto services = getServices();
    if (services.empty())
        return " <EMPTY>";
    auto strL = juce::String("\n");
    for (auto const& service : services)
        strL += "  " + service.type + " (on " + service.sessionMasterDescription + ")\n";
    return strL;
}

void ServiceTopologyManager::ServiceDiscovery::handleAsyncUpdate()
{
    juce::NullCheckedInvocation::invoke(onChange);
}

void ServiceTopologyManager::ServiceDiscovery::handleMessage(const juce::XmlElement& xml)
{
    SessionMasterAwareService service;
    service.type = xml.getTagName();
    service.instanceID = xml.getStringAttribute("id");

    if (service.instanceID.trim().isNotEmpty())
    {
        service.description = xml.getStringAttribute("name");
        service.sessionMasterDescription = xml.getStringAttribute("master_name");
        service.address = juce::IPAddress(xml.getStringAttribute("address"));
        service.port = xml.getIntAttribute("port");
        service.lastSeen = juce::Time::getCurrentTime();

        handleMessage(service);
    }
}

static void sortServiceList(std::vector<SessionMasterAwareService>& services)
{
    auto compareServices = [](const SessionMasterAwareService& s1,
        const SessionMasterAwareService& s2)
        {
            return s1.instanceID < s2.instanceID;
        };

    std::sort(services.begin(), services.end(), compareServices);
}

void ServiceTopologyManager::ServiceDiscovery::handleMessage(const SessionMasterAwareService& service)
{
    const juce::ScopedLock sl(m_listLock);

    for (auto& refService : m_services)
    {
        if (refService.instanceID == service.instanceID)
        {
            if (refService != service)
            {
                refService = service;
                triggerAsyncUpdate();
            }

            refService.lastSeen = service.lastSeen;
            return;
        }
    }

    m_services.push_back(service);
    sortServiceList(m_services);
    triggerAsyncUpdate();
}

void ServiceTopologyManager::ServiceDiscovery::removeTimedOutServices()
{
    const double timeoutSeconds = 5.0;
    auto oldestAllowedTime = juce::Time::getCurrentTime() - juce::RelativeTime::seconds(timeoutSeconds);

    const juce::ScopedLock sl(m_listLock);

    auto oldEnd = std::end(m_services);
    auto newEnd = std::remove_if(std::begin(m_services), oldEnd,
        [=](const SessionMasterAwareService& s) { return s.lastSeen < oldestAllowedTime; });

    if (newEnd != oldEnd)
    {
        m_services.erase(newEnd, oldEnd);
        triggerAsyncUpdate();
    }
}


//==============================================================================
ServiceTopologyManager::ServiceTopologyManager(const juce::String& serviceTypeUIDBase, const juce::String& serviceTypeUID, const juce::String& serviceDescription, const juce::String& sessionMasterServiceDescription, int broadcastPort, int connectionPort, juce::RelativeTime minTimeBetweenBroadcasts)
    : juce::Thread(juce::JUCEApplication::getInstance()->getApplicationName() + ": Discovery_broadcast"), m_message(serviceTypeUID), m_broadcastPort(broadcastPort), m_minInterval(minTimeBetweenBroadcasts)
{
    m_message.setAttribute("id", juce::Uuid().toString());
    m_message.setAttribute("name", serviceDescription);
    m_message.setAttribute("master_name", sessionMasterServiceDescription);
    m_message.setAttribute("address", juce::String());
    m_message.setAttribute("port", connectionPort);

    startThread(juce::Thread::Priority::background);

    m_serviceDiscovery = std::make_unique<ServiceDiscovery>(serviceTypeUIDBase, broadcastPort);
    m_serviceDiscovery->onChange = [=]() {
        updateKnownTopology();
        if (onDiscoveredTopologyChanged)
            onDiscoveredTopologyChanged();
    };
}

ServiceTopologyManager::~ServiceTopologyManager()
{
    stopThread(2000);
    m_socket.shutdown();
}

void ServiceTopologyManager::showServiceTopologyMenu(const SessionServiceTopology& topology, std::function<void(const SessionMasterAwareService&)> onServiceSelected)
{
    bool allowSelection = onServiceSelected != nullptr;
    
    class CustomPopupTopologyComponent : public juce::PopupMenu::CustomComponent
    {
    public:
        CustomPopupTopologyComponent(const SessionServiceTopology& topology, bool allowSelection) : juce::PopupMenu::CustomComponent()
        {
            m_topologyTree = std::make_unique<ServiceTopologyTreeView>(allowSelection);
            m_topologyTree->setServiceTopology(topology);
            m_topologyTree->setDefaultOpenness(true);
            addAndMakeVisible(m_topologyTree.get());
        };
        ~CustomPopupTopologyComponent() override {};

        void resized() override
        {
            if (m_topologyTree)
                m_topologyTree->setBounds(getLocalBounds());
        };

        void getIdealSize(int& idealWidth, int& idealHeight)
        {
            auto idealSize = m_topologyTree->getIdealSize();
            idealWidth = idealSize.first;
            idealHeight = idealSize.second;
        };

    private:
        std::unique_ptr<ServiceTopologyTreeView>    m_topologyTree;
    };

    juce::PopupMenu topologyMenu;
    topologyMenu.addCustomItem(1, std::make_unique<CustomPopupTopologyComponent>(topology, allowSelection), nullptr, "Detected service topology tree");
    topologyMenu.showMenuAsync(juce::PopupMenu::Options());
}

void ServiceTopologyManager::setSessionMasterServiceDescription(const juce::String& sessionMasterServiceDescription)
{
    std::lock_guard<std::mutex> l(m_messagelock);
    m_message.setAttribute("master_name", sessionMasterServiceDescription);
}

const SessionServiceTopology& ServiceTopologyManager::getDiscoveredServiceTopology()
{
    return m_serviceTopology;
}

const juce::String ServiceTopologyManager::getServiceDescription()
{
    return juce::JUCEApplication::getInstance()->getApplicationName()
+ "." + juce::JUCEApplication::getInstance()->getApplicationVersion()
+ "@" + juce::SystemStats::getComputerName();
}

juce::IPAddress ServiceTopologyManager::getInterfaceBroadcastAddress(const juce::IPAddress& address)
{
    if (address.isIPv6)
        // TODO
        return {};

    juce::String broadcastAddress = address.toString().upToLastOccurrenceOf(".", true, false) + "255";
    return juce::IPAddress(broadcastAddress);
}

void ServiceTopologyManager::run()
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

void ServiceTopologyManager::sendBroadcast()
{
    static juce::IPAddress local = juce::IPAddress::local();

    for (auto& address : juce::IPAddress::getAllAddresses())
    {
        if (address == local)
            continue;

        auto broadcastAddress = getInterfaceBroadcastAddress(address);
        auto data = juce::String();
        {
            std::lock_guard<std::mutex> l(m_messagelock);
            m_message.setAttribute("address", address.toString());
            data = m_message.toString(juce::XmlElement::TextFormat().singleLine().withoutHeader());
            //DBG(juce::String(__FUNCTION__) + " " + data);
        }

        auto retVal = m_socket.write(broadcastAddress.toString(), m_broadcastPort, data.toRawUTF8(), (int)data.getNumBytesAsUTF8());
        if(-1 != retVal)
        {
            DBG(juce::String(__FUNCTION__) + " unable to write to " + broadcastAddress.toString());
        }
    }
}

void ServiceTopologyManager::updateKnownTopology()
{
    if (!m_serviceDiscovery)
        return;

    auto currentServices = m_serviceDiscovery->getServices();

    // derive currently present session masters
    auto currentSessionMasters = std::vector<SessionMasterAwareService>();
    // create 'No session' default sessionMaster dummy item
    auto sessionLessPlaceHolder = SessionMasterAwareService();
    sessionLessPlaceHolder.description = "No session";
    currentSessionMasters.push_back(sessionLessPlaceHolder);
    m_serviceTopology[sessionLessPlaceHolder] = {};
    // walk the services and derive currently present sessionMasters
    for (auto const& service : currentServices)
    {
        if (service.sessionMasterDescription == service.description)
        {
            currentSessionMasters.push_back(service);
            m_serviceTopology[service] = {};
        }
    }
    // fill in the session participants
    for (auto const& service : currentServices)
    {
        if (service.sessionMasterDescription != service.description && service.sessionMasterDescription.isNotEmpty())
        {
            bool foundAndProcessed = false;
            for (auto& session : m_serviceTopology)
            {
                if (session.first.description == service.sessionMasterDescription)
                {
                    if (std::find(session.second.begin(), session.second.end(), service) == session.second.end())
                    {
                        session.second.push_back(service);
                        foundAndProcessed = true;
                        break;
                    }
                }
            }
        }
        else if (service.sessionMasterDescription.isEmpty())
            m_serviceTopology[sessionLessPlaceHolder].push_back(service);
    }
    // delete any stale sessionMasters
    auto staleSessions = std::vector<SessionMasterAwareService>();
    for (auto const& sessionMaster : m_serviceTopology)
    {
        if (std::find(currentSessionMasters.begin(), currentSessionMasters.end(), sessionMaster.first) == currentSessionMasters.end())
            staleSessions.push_back(sessionMaster.first);
    }
    for (auto const& staleSession : staleSessions)
        m_serviceTopology.erase(staleSession);

    DBG(juce::String(__FUNCTION__) + " " + m_serviceDiscovery->knownServicesToString());
}


} // namespace JUCEAppBasics
