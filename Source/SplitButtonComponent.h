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
class SplitButtonComponent : public Component,
                            public TextButton::Listener
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

    std::uint64_t addButton(const String& buttonText);
    void addButtons(const StringArray& buttonTexts);

    void setButtonDown(const std::uint64_t buttonId);
    void setButtonDown(const String& buttonText);
    const std::uint64_t getButtonDown();
    const String getButtonDownText();

    void setButtonEnabled(const std::uint64_t buttonId, bool enabled);
    void setButtonEnabled(const String& buttonText, bool enabled);
    bool getButtonEnabled(const std::uint64_t buttonId) const;
    bool getButtonEnabled(const String& buttonText) const;

    void paint (Graphics&) override;
    void resized() override;
    
    void buttonClicked(Button* button) override;

private:
    Listener* m_listener{ nullptr };

    std::map<std::uint64_t, std::unique_ptr<TextButton>>   m_buttons;
    std::uint64_t m_firstButtonID{ 1 };
    std::uint64_t m_currentButtonID{ 0 };
    std::uint64_t getNextButtonID() { return ++m_currentButtonID; };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SplitButtonComponent)
};

}
