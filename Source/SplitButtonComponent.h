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

        virtual void buttonClicked(uint64 buttonId) = 0;
    };
    
public:
    SplitButtonComponent();
	~SplitButtonComponent();

    void addListener(Listener *l);

    uint64 addButton(const String& buttonText);
    void addButtons(const StringArray& buttonTexts);

    void setButtonDown(const uint64 buttonId);
    void setButtonDown(const String& buttonText);
    const uint64 getButtonDown();
    const String getButtonDownText();

    void paint (Graphics&) override;
    void resized() override;
    
    void buttonClicked(Button* button) override;

private:
    Listener* m_listener{ nullptr };

    std::map<const uint64, std::unique_ptr<TextButton>>   m_buttons;
    uint64 m_firstButtonID{ 1 };
    uint64 m_currentButtonID{ 0 };
    uint64 getNextButtonID() { return ++m_currentButtonID; };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SplitButtonComponent)
};

}
