/*
  ==============================================================================

    ZeroconfDiscoverComponent
    Created: 20 Sept 2020 20:20:00pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../submodules/ZeroconfSearcher/Source/ZeroconfSearcher.h"

namespace JUCEAppBasics
{

class ZeroconfDiscoverComponent :
    public Component,
    public Button::Listener,
    public ZeroconfSearcher::ZeroconfSearcher::ZeroconfSearcherListener,
    public MessageListener
{
public:
    enum ZeroconfServiceType
    {
        ZST_Unkown,
        ZST_OSC,
        ZST_OCA,
        ZST_Max
    };

    class ServiceChangedMessage : public Message
    {
    public:
        ServiceChangedMessage(String serviceName) { m_serviceName = serviceName; };

        String GetServiceName() const { return m_serviceName; };

    private:
        String m_serviceName;
    };

public:
    ZeroconfDiscoverComponent(bool useSeparateServiceSearchers, bool showServiceNameLabels = true);
    ~ZeroconfDiscoverComponent();
    
    void clearServices();
    void addDiscoverService(ZeroconfServiceType serviceType);
    void removeDiscoverService(ZeroconfServiceType serviceType);
    void setShowServiceNameLabels(bool show);
    void handlePopupResult(int result);
    
    //==============================================================================
    void buttonClicked(Button*) override;
	
    //==============================================================================
    void resized() override;

    //==============================================================================
    void lookAndFeelChanged() override;

    //==============================================================================
    void handleServicesChanged(std::string serviceName) override;

    //==============================================================================
    void handleMessage(const Message& message) override;
    
    //==============================================================================
    static String getServiceName(ZeroconfServiceType type);
    static ZeroconfServiceType getServiceType(String name);
    static String getServiceDescriptor(ZeroconfServiceType type);
    
    //==============================================================================
    std::function<void(ZeroconfServiceType, ZeroconfSearcher::ZeroconfSearcher::ServiceInfo*)> onServiceSelected;

private:
    ZeroconfSearcher::ZeroconfSearcher * getSearcherByName(const juce::String& name);
    ZeroconfSearcher::ZeroconfSearcher* getSearcherByServiceName(const juce::String& serviceName);

    void showMenuAndGetService(const juce::String& serviceName);

    void addSearcher(String name, String serviceName);
    void removeSearcher(String name);
    
    bool                                                                                    m_useSeparateServiceSearchers;
    std::vector<std::unique_ptr<ZeroconfSearcher::ZeroconfSearcher>>                        m_searchers;
    
    std::map<String, std::unique_ptr<DrawableButton>>                                       m_discoveryButtons;
    std::map<String, std::unique_ptr<Label>>                                                m_serviceNameLabels;

    std::vector<std::tuple<std::string, ZeroconfSearcher::ZeroconfSearcher::ServiceInfo*>>  m_currentServiceBrowsingList;
    PopupMenu                                                                               m_currentServiceBrowsingPopup;

    bool isListeningForPopupResults() { return m_listeningForPopupResults; };
    void setListeningForPopupResults(bool listening) { m_listeningForPopupResults = listening; };
    bool m_listeningForPopupResults { false };
    int m_ignorePopupResultCount{ 0 };
    
    bool m_showServiceNameLabels { false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZeroconfDiscoverComponent)
};

};
