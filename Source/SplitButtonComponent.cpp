/*
  ==============================================================================

    SplitButtonComponent.cpp
    Created: 12 Jun 2020 13:57:00pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "SplitButtonComponent.h"


namespace JUCEAppBasics
{

//==============================================================================
SplitButtonComponent::SplitButtonComponent()
	: Component()
{

}

SplitButtonComponent::~SplitButtonComponent()
{

}

void SplitButtonComponent::addListener(Listener *l)
{
    m_listener = l;
}


uint64 SplitButtonComponent::addButton(const String& buttonText)
{
    uint64 buttonID = getNextButtonID();

    m_buttons[buttonID] = std::make_unique<TextButton>(buttonText);
    addAndMakeVisible(m_buttons[buttonID].get(), dontSendNotification);
    m_buttons[buttonID]->setToggleState(false, dontSendNotification);
    m_buttons[buttonID]->addListener(this);

    return buttonID;
}

void SplitButtonComponent::addButtons(const StringArray& buttonTexts)
{
    for (auto buttonText : buttonTexts)
        addButton(buttonText);
}

void SplitButtonComponent::setButtonDown(const uint64 buttonId)
{
    for (const std::pair<const uint64, std::unique_ptr<TextButton>>& p : m_buttons)
    {
        p.second->setToggleState(p.first == buttonId, dontSendNotification);
    }
}

void SplitButtonComponent::setButtonDown(const String& buttonText)
{
    for (const std::pair<const uint64, std::unique_ptr<TextButton>>& p : m_buttons)
    {
        p.second->setToggleState(p.second->getButtonText() == buttonText, dontSendNotification);
    }
}

const uint64 SplitButtonComponent::getButtonDown()
{
    for (const std::pair<const uint64, std::unique_ptr<TextButton>>& p : m_buttons)
    {
        if (p.second->getToggleState())
            return p.first;
    }

    return m_firstButtonID;
}

const String SplitButtonComponent::getButtonDownText()
{
    for (const std::pair<const uint64, std::unique_ptr<TextButton>>& p : m_buttons)
    {
        if (p.second->getToggleState())
            return p.second->getButtonText();
    }

    return "";
}

void SplitButtonComponent::paint (Graphics& g)
{
	Component::paint(g);
}

void SplitButtonComponent::resized()
{
    FlexBox fb;
    fb.flexDirection = FlexBox::Direction::row;
    for (const std::pair<const uint64, std::unique_ptr<TextButton>>& p : m_buttons)
    {
        fb.items.add(FlexItem(*p.second.get()).withFlex(1));
    }
    fb.performLayout(getLocalBounds().toFloat());
}

void SplitButtonComponent::buttonClicked(Button* button)
{
    if (button)
    {
        uint64 buttonID = m_firstButtonID;
        for (auto const& buttonKV : m_buttons)
        {
            if (buttonKV.second.get() == button)
                buttonID = buttonKV.first;
        }

        setButtonDown(buttonID);

        if(m_listener)
        {
            m_listener->buttonClicked(buttonID);
        }
    }
}

}
