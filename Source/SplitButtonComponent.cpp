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
    std::uint64_t buttonID = getNextButtonID();

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
    for (auto const& p : m_buttons)
    {
        p.second->setToggleState(p.first == buttonId, dontSendNotification);
    }
}

void SplitButtonComponent::setButtonDown(const String& buttonText)
{
    for (auto const& p : m_buttons)
    {
        p.second->setToggleState(p.second->getButtonText() == buttonText, dontSendNotification);
    }
}

const uint64 SplitButtonComponent::getButtonDown()
{
    for (auto const& p : m_buttons)
    {
        if (p.second->getToggleState())
            return p.first;
    }

    return m_firstButtonID;
}

const String SplitButtonComponent::getButtonDownText()
{
    for (auto const& p : m_buttons)
    {
        if (p.second->getToggleState())
            return p.second->getButtonText();
    }

    return "";
}

void SplitButtonComponent::setButtonEnabled(const std::uint64_t buttonId, bool enabled)
{
    if (m_buttons.count(buttonId) > 0)
        m_buttons[buttonId]->setEnabled(enabled);
}

void SplitButtonComponent::setButtonEnabled(const String& buttonText, bool enabled)
{
    for (auto const& p : m_buttons)
    {
        if (p.second->getButtonText() == buttonText)
            p.second->setEnabled(enabled);
    }
}

bool SplitButtonComponent::getButtonEnabled(std::uint64_t buttonId) const
{
    if (m_buttons.count(buttonId) > 0 && m_buttons.at(buttonId))
        return m_buttons.at(buttonId)->isEnabled();
    else
        return false;
}

bool SplitButtonComponent::getButtonEnabled(const String& buttonText) const
{
    for (auto const& p : m_buttons)
    {
        if (p.second && p.second->getButtonText() == buttonText)
            return p.second->isEnabled();
    }

    return false;
}

void SplitButtonComponent::paint (Graphics& g)
{
	Component::paint(g);
}

void SplitButtonComponent::resized()
{
    FlexBox fb;
    fb.flexDirection = FlexBox::Direction::row;
    for (auto const& p : m_buttons)
    {
        fb.items.add(FlexItem(*p.second.get()).withFlex(1));
    }
    fb.performLayout(getLocalBounds().toFloat());
}

void SplitButtonComponent::buttonClicked(Button* button)
{
    if (button)
    {
        std::uint64_t buttonID = m_firstButtonID;
        for (auto const& p : m_buttons)
        {
            if (p.second.get() == button)
                buttonID = p.first;
        }

        setButtonDown(buttonID);

        if(m_listener)
        {
            m_listener->buttonClicked(this, buttonID);
        }
    }
}

}
