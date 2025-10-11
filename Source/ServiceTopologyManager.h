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
#pragma once

#include <JuceHeader.h>


namespace JUCEAppBasics
{


/**
 * Derived from juce::NetworkServiceDiscovery::Service,
 * to extend the xml structure with a string attribute
 * to hold the session master descrioption.
 * 
 * @see juce::NetworkServiceDiscovery::Service
*/
struct SessionMasterAwareService
{
    juce::String instanceID;                /**< A UUID that identifies the particular instance of the Advertiser class.  */
    juce::String description;               /**< The service description as sent by the Advertiser */
    juce::String type;                      /**< The service type as sent by the Advertiser */
    juce::String sessionMasterDescription;  /**< The sessions master service description, derived from the services master advertiser */
    juce::IPAddress address;                /**< The IP address of the advertiser */
    int port;                               /**< The port number of the advertiser */
    juce::Time lastSeen;                    /**< The time of the last ping received from the advertiser */
};


/**
  * ServiceTopologyManager is a class derived from juce::NetworkServiceDiscovery::Advertiser 
  * to enable support for windows (not contained in JUCE) and extend the class 
  * with service participation topology tracking on top.
  */
class ServiceTopologyManager : private juce::Thread
{
public:
    /**
     * JUCEs' AvailableServiceList has the used listening socket in non-reuse mode, therefor onyl one of our MemaClients can listen on a single machine at a time
     * (applies for JUCE 8.0.8 and is only a problem on UNIX systems)
     * To overcome this, the ServiceDiscovery is a code clone of JUCEs' AvailableServiceList, with only a single adapted line of code - adding a call
     * to socket.setEnablePortReuse(true) in the constructor. As soon as the JUCE implemenation is changed, we should roll back to the original implementation if possible.
     * See e.g.: https://forum.juce.com/t/networkservicediscovery-multiple-listeners-on-same-machine/66306
     */
    struct ServiceDiscovery : private juce::Thread,
        private juce::AsyncUpdater
    {
        /** Creates an AvailableServiceList that will bind to the given port number and watch
            the network for Advertisers broadcasting the given service type.

            This will only detect broadcasts from an Advertiser object with a matching
            serviceTypeUID value, and where the broadcastPort matches.
        */
        ServiceDiscovery(const String& serviceTypeUIDBase, int broadcastPort);

        /** Destructor */
        ~ServiceDiscovery() override;

        /** A lambda that can be set to receive a callback when the list changes */
        std::function<void()> onChange;

        /** Returns a list of the currently known services. */
        std::vector<SessionMasterAwareService> getServices() const;

        juce::String knownServicesToString();

    private:
        juce::DatagramSocket m_socket{ true };
        juce::String m_serviceTypeUIDBase;
        juce::CriticalSection m_listLock;
        std::vector<SessionMasterAwareService> m_services;

        void run() override;
        void handleAsyncUpdate() override;
        void handleMessage(const juce::XmlElement&);
        void handleMessage(const SessionMasterAwareService&);
        void removeTimedOutServices();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ServiceDiscovery)
    };

public:
	ServiceTopologyManager(const juce::String& serviceTypeUIDBase, const juce::String& serviceTypeUID, const juce::String& serviceDescription, const juce::String& sessionMasterServiceDescription,
        int broadcastPort, int connectionPort, juce::RelativeTime minTimeBetweenBroadcasts = RelativeTime::seconds(1.5));
	~ServiceTopologyManager() override;

    void setSessionMasterServiceDescription(const juce::String& sessionMasterServiceDescription);

protected:
	IPAddress getInterfaceBroadcastAddress(const IPAddress& address);
	void run() override;
	void sendBroadcast();

    void updateKnownTopology();

private:
    std::mutex m_messagelock;
	juce::XmlElement m_message;
	const int m_broadcastPort;
	const juce::RelativeTime m_minInterval;
	juce::DatagramSocket m_socket{ true };

    std::unique_ptr<ServiceDiscovery> m_serviceDiscovery;

};

} // namespace JUCEAppBasics
