/*
  ==============================================================================

    ZeroconfDiscoverComponent
    Created: 20 Sept 2020 20:20:00pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../submodules/mdns/mdns.h"

namespace JUCEAppBasics
{

class ZeroconfDiscoverComponent :
    public Component,
    public Button::Listener,
	public Thread,
	public Timer
{
public:
    enum ZeroconfServiceType
    {
        ZST_Unkown,
        ZST_OSC,
        ZST_OCA,
        ZST_Max
    };

	struct ServiceInfo
	{
		String name;
		String host;
		String ip;
		int port;
	};

	class ZeroconfSearcher
	{
	public:
		ZeroconfSearcher(StringRef name, StringRef serviceName, unsigned short announcementPort = 0);
		~ZeroconfSearcher();

		String                  m_name;
		String                  m_serviceName;
		OwnedArray<ServiceInfo> m_services;
        int                     m_socketIdx;

        static int recvCallback(int sock, const struct sockaddr* from, size_t addrlen, mdns_entry_type_t entry,
                uint16_t query_id, uint16_t rtype, uint16_t rclass, uint32_t ttl, const void* data,
                size_t size, size_t name_offset, size_t name_length, size_t record_offset,
            size_t record_length, void* user_data)
        {
            char buffer[512];
            mdns_string_t fromaddrstr;

            if (from->sa_family == AF_INET6)
            {
                auto addr = (const struct sockaddr_in*)from;

                char host[NI_MAXHOST] = { 0 };
                char service[NI_MAXSERV] = { 0 };
                int ret = getnameinfo((const struct sockaddr*)from, (socklen_t)addrlen, host, NI_MAXHOST,
                    service, NI_MAXSERV, NI_NUMERICSERV | NI_NUMERICHOST);
                int len = 0;
                if (ret == 0) {
                    if (addr->sin_port != 0)
                        len = snprintf(buffer, sizeof(buffer), "%s:%s", host, service);
                    else
                        len = snprintf(buffer, sizeof(buffer), "%s", host);
                }
                if (len >= (int)sizeof(buffer))
                    len = (int)sizeof(buffer) - 1;
                mdns_string_t str;
                str.str = buffer;
                str.length = len;
                fromaddrstr = str;
            }
            else
            {
                auto addr = (const struct sockaddr_in6*)from;

                char host[NI_MAXHOST] = { 0 };
                char service[NI_MAXSERV] = { 0 };
                int ret = getnameinfo((const struct sockaddr*)from, (socklen_t)addrlen, host, NI_MAXHOST,
                    service, NI_MAXSERV, NI_NUMERICSERV | NI_NUMERICHOST);
                int len = 0;
                if (ret == 0) {
                    if (addr->sin6_port != 0)
                        len = snprintf(buffer, sizeof(buffer), "[%s]:%s", host, service);
                    else
                        len = snprintf(buffer, sizeof(buffer), "%s", host);
                }
                if (len >= (int)sizeof(buffer))
                    len = (int)sizeof(buffer) - 1;
                mdns_string_t str;
                str.str = buffer;
                str.length = len;
                fromaddrstr = str;
            }

            DBG(String(__FUNCTION__) + String(" received answer from ") + String(fromaddrstr.str));

            return 0;
        };

		bool search();
		String getIPForHostAndPort(String host, int port);

		ServiceInfo * getService(StringRef name, StringRef host, int port);
		void addService(StringRef name, StringRef host, StringRef ip, int port);
		void removeService(ServiceInfo * service);
		void updateService(ServiceInfo * service, StringRef host, StringRef ip, int port);
	};
    
public:
    ZeroconfDiscoverComponent(bool useSeparateServiceSearchers, bool showServiceNameLabels = true);
    ~ZeroconfDiscoverComponent();
    
    void clearServices();
    void addDiscoverService(ZeroconfServiceType serviceType, unsigned short announcementPort = 0);
    void removeDiscoverService(ZeroconfServiceType serviceType);
    void setShowServiceNameLabels(bool show);
    
    //==============================================================================
    void timerCallback() override;
    
    //==============================================================================
    void run() override;
    
    //==============================================================================
    void buttonClicked(Button*) override;
	
    //==============================================================================
    void resized() override;

    //==============================================================================
    void lookAndFeelChanged() override;
    
    //==============================================================================
    static String getServiceName(ZeroconfServiceType type);
    static ZeroconfServiceType getServiceType(String name);
    static String getServiceDescriptor(ZeroconfServiceType type);
    
    //==============================================================================
    std::function<void(ZeroconfServiceType, ServiceInfo*)> onServiceSelected;

private:
    ZeroconfSearcher * getSearcher(StringRef name);

    void showMenuAndGetService(const juce::String& serviceName);
    
    void search();

    void addSearcher(StringRef name, StringRef serviceName, unsigned short announcementPort = 0);
    void removeSearcher(StringRef name);

    bool                                                m_useSeparateServiceSearchers;
    OwnedArray<ZeroconfSearcher, CriticalSection>       m_searchers;
    
    std::map<String, std::unique_ptr<DrawableButton>>   m_discoveryButtons;
    std::map<String, std::unique_ptr<Label>>            m_serviceNameLabels;

    std::vector<ServiceInfo*>                           m_currentServiceBrowsingList;   /**< Flat list of services currently available for selection. Only valid while popup menu is shown!. */
    PopupMenu                                           m_currentServiceBrowsingPopup;
    
    bool m_showServiceNameLabels { false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZeroconfDiscoverComponent)
};

};
