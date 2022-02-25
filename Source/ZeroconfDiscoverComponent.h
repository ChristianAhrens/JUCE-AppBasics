/*
  ==============================================================================

    ZeroconfDiscoverComponent
    Created: 20 Sept 2020 20:20:00pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "NanoMDNSDiscoverer.h"

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

	class ZeroconfSearcher : public NanoMDNSDiscoverer::DiscoveryListener
	{
	public:
		ZeroconfSearcher(StringRef name, StringRef serviceName);
		~ZeroconfSearcher();

		String m_name;
		String m_serviceName;
        NanoMDNSDiscoverer m_mDNSDiscoverer;
		OwnedArray<NanoMDNSDiscoverer::ServiceDiscoverInfo> m_services;

        void ServiceDiscovered(const NanoMDNSDiscoverer::ServiceDiscoverInfo& service) override;

		bool processDiscovery();
		String getIPForHostAndPort(String host, int port);

        NanoMDNSDiscoverer::ServiceDiscoverInfo* getService(StringRef name, StringRef host, StringRef ip, int port);
		void addService(StringRef name, StringRef host, StringRef ip, int port);
		void removeService(NanoMDNSDiscoverer::ServiceDiscoverInfo* service);
		void updateService(NanoMDNSDiscoverer::ServiceDiscoverInfo* service, StringRef name, StringRef host, StringRef ip, int port);
	};
    
public:
    ZeroconfDiscoverComponent(bool useSeparateServiceSearchers, bool showServiceNameLabels = true);
    ~ZeroconfDiscoverComponent();
    
    void clearServices();
    void addDiscoverService(ZeroconfServiceType serviceType);
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
    std::function<void(ZeroconfServiceType, NanoMDNSDiscoverer::ServiceDiscoverInfo*)> onServiceSelected;

private:
    ZeroconfSearcher * getSearcher(StringRef name);

    void showMenuAndGetService(const juce::String& serviceName);
    
    void search();

    void addSearcher(StringRef name, StringRef serviceName);
    void removeSearcher(StringRef name);
    
    bool                                                    m_useSeparateServiceSearchers;
    OwnedArray<ZeroconfSearcher, CriticalSection>           m_searchers;
    
    std::map<String, std::unique_ptr<DrawableButton>>       m_discoveryButtons;
    std::map<String, std::unique_ptr<Label>>                m_serviceNameLabels;

    std::vector<NanoMDNSDiscoverer::ServiceDiscoverInfo*>   m_currentServiceBrowsingList;   /**< Flat list of services currently available for selection. Only valid while popup menu is shown!. */
    PopupMenu                                               m_currentServiceBrowsingPopup;
    
    bool m_showServiceNameLabels { false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZeroconfDiscoverComponent)
};

};
