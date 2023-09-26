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
    public juce::DrawableButton,
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

    enum ZeroconfServiceCategoryType
    {
        ZSCT_Unknown,
        ZSCT_MetaInfo,       /**> The category is characterized by a meta info field string "NAME", "HOST", "IP" or "PORT". */
        ZSCT_TxtRecord,      /**> The category is characterized by a txtRecord entry name. */
        ZSCT_Max
    };

    enum ZeroconfServiceCategoryMatch
    {
        ZSCM_Unknown,
        ZSCM_Match,     /**> The expected categorization value has to exactly match what was found in a serviceinfo. */
        ZSCM_Contain,   /**> The expected categorization value has to be contained what was found in a serviceinfo. */
        ZSCM_Max
    };

    class ServiceChangedMessage : public Message
    {
    public:
        ServiceChangedMessage(String serviceName) { m_serviceName = serviceName; };

        juce::String GetServiceName() const { return m_serviceName; };

    private:
        juce::String m_serviceName;
    };

public:
    ZeroconfDiscoverComponent(const juce::String& name);
    ~ZeroconfDiscoverComponent();
    
    void clearServices();
    void addDiscoverService(ZeroconfServiceType serviceType);
    void removeDiscoverService(ZeroconfServiceType serviceType);
    bool addPopupCategory(const juce::String& categoryName, const std::pair<std::pair<ZeroconfServiceCategoryType, juce::String>, std::pair<ZeroconfServiceCategoryMatch, juce::String>>& categoryProcessingData);
    void removePopupCategory(const juce::String& categoryName);
    void handlePopupResult(int result);

    //==============================================================================
    juce::Rectangle<float> getImageBounds() const override;

    //==============================================================================
    void lookAndFeelChanged() override;

    //==============================================================================
    void mouseDown(const juce::MouseEvent& e) override;

    //==============================================================================
    void handleServicesChanged(std::string serviceName) override;

    //==============================================================================
    void handleMessage(const Message& message) override;
    
    //==============================================================================
    static juce::String getServiceName(ZeroconfServiceType type);
    static ZeroconfServiceType getServiceType(juce::String name);
    static juce::String getServiceDescriptor(ZeroconfServiceType type);
    
    //==============================================================================
    std::function<void(ZeroconfServiceType, ZeroconfSearcher::ZeroconfSearcher::ServiceInfo*)> onServiceSelected;

private:
    //==============================================================================
    class CustomLabelItemComponent : public juce::PopupMenu::CustomComponent
    {
    public:
        CustomLabelItemComponent(const juce::String& componentName, const juce::String& labelText)
            : juce::PopupMenu::CustomComponent(true) {
            m_label = std::make_unique<juce::Label>(componentName, labelText);
            
            addAndMakeVisible(m_label.get());
        };
        void getIdealSize(int& idealWidth, int& idealHeight) override {
            auto margin = 8;
            idealWidth = m_label->getFont().getStringWidth(m_label->getText()) + margin;
            idealHeight = static_cast<int>(m_label->getFont().getHeight()) + margin;
        };
        void resized() override {
            m_label->setBounds(getLocalBounds());
        };
        void paint(Graphics& g) override {
            if (isItemHighlighted())
                m_label->setColour(juce::Label::ColourIds::backgroundColourId, getLookAndFeel().findColour(juce::PopupMenu::ColourIds::highlightedBackgroundColourId));
            else
                m_label->setColour(juce::Label::ColourIds::backgroundColourId, getLookAndFeel().findColour(juce::Label::ColourIds::backgroundColourId));
            juce::PopupMenu::CustomComponent::paint(g);
        };

    private:
        std::unique_ptr<juce::Label>    m_label;
    };

    //==============================================================================
    ZeroconfSearcher::ZeroconfSearcher * getSearcherByName(const juce::String& name);
    ZeroconfSearcher::ZeroconfSearcher* getSearcherByServiceName(const juce::String& serviceName);

    void showMenuAndGetService(const juce::String& serviceName);

    void addSearcher(const juce::String& name, const juce::String& serviceName);
    void removeSearcher(const juce::String& name);

    bool isSearchingActive();
    void startupSearching();
    void shutdownSearching();
    
    bool isPopupActive();
    void setPopupActive(bool active);
    
    bool                                                                                                                                            m_useSeparateServiceSearchers;
    std::vector<std::unique_ptr<ZeroconfSearcher::ZeroconfSearcher>>                                                                                m_searchers;

    std::map<juce::String, std::pair<std::pair<ZeroconfServiceCategoryType, juce::String>, std::pair<ZeroconfServiceCategoryMatch, juce::String>>>  m_currentPopupCategories;

    std::vector<std::tuple<juce::String, ZeroconfSearcher::ZeroconfSearcher::ServiceInfo>>                                                          m_currentServiceBrowsingList;
    juce::PopupMenu                                                                                                                                 m_currentServiceBrowsingPopup;
    
    bool m_popupActive{ false };
    bool m_searchingActive{ false };
    int m_ignoreBlankPopupResultCount{ 0 };
    
    bool m_showServiceNameLabels { false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZeroconfDiscoverComponent)
};

};
