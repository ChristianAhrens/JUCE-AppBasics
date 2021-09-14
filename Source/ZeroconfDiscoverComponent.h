/*
  ==============================================================================

    ZeroconfDiscoverComponent
    Created: 20 Sept 2020 20:20:00pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include <servus/servus.h>

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

		String m_name;
		String m_serviceName;
		servus::Servus m_servus;
		OwnedArray<ServiceInfo> m_services;

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
