/*
  ==============================================================================

    SplitButtonComponent.cpp
    Created: 12 Jun 2020 13:57:00pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace JUCEAppBasics
{

//==============================================================================
/*
*/
class SplitButtonComponent :    public juce::Component,
                                public juce::TextButton::Listener
{
public:
    class Listener
    {
    public:
        virtual ~Listener(){};

        virtual void buttonClicked(JUCEAppBasics::SplitButtonComponent* button, std::uint64_t buttonId) = 0;
    };
    
public:
    SplitButtonComponent();
	~SplitButtonComponent();

    void addListener(Listener *l);

    std::uint64_t addButton(const juce::String& buttonText, const std::float_t buttonFlex = -1);
    void addButtons(const std::vector<juce::String>& buttonTexts, const std::vector<std::float_t>& buttonFlexes = {});

    void setButtonDown(const std::uint64_t buttonId);
    void setButtonDown(const juce::String& buttonText);
    const std::uint64_t getButtonDown();
    const juce::String getButtonDownText();

    void setButtonEnabled(const std::uint64_t buttonId, bool enabled);
    void setButtonEnabled(const juce::String& buttonText, bool enabled);
    bool getButtonEnabled(const std::uint64_t buttonId) const;
    bool getButtonEnabled(const juce::String& buttonText) const;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void buttonClicked(juce::Button* button) override;

private:
    Listener* m_listener{ nullptr };

    std::map<std::uint64_t, std::unique_ptr<juce::TextButton>>  m_buttons;
    std::map<std::uint64_t, std::float_t>                       m_buttonFlexes;
    std::vector<std::uint_least64_t>                            m_buttonIds;
    std::uint64_t m_firstButtonID{ 1 };
    std::uint64_t m_currentButtonID{ 0 };
    std::uint64_t getNextButtonID() { m_buttonIds.push_back(++m_currentButtonID); return m_currentButtonID; };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SplitButtonComponent)
};

}
