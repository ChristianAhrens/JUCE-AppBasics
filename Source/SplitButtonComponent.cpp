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
	: juce::Component()
{

}

SplitButtonComponent::~SplitButtonComponent()
{

}

void SplitButtonComponent::addListener(Listener *l)
{
    m_listener = l;
}


std::uint64_t SplitButtonComponent::addButton(const juce::String& buttonText, const std::float_t buttonFlex)
{
    std::uint64_t buttonID = getNextButtonID();

    m_buttons[buttonID] = std::make_unique<TextButton>(buttonText);
    addAndMakeVisible(m_buttons[buttonID].get(), dontSendNotification);
    m_buttons[buttonID]->setToggleState(false, dontSendNotification);
    m_buttons[buttonID]->addListener(this);

    if (buttonFlex != -1)
        m_buttonFlexes[buttonID] = buttonFlex;

    if (m_buttonIds.size() == 1 && m_buttons.size() == 1 && m_buttons.at(m_buttonIds.at(0)))
    {
        // a single button has no connected edges
        m_buttons.at(m_buttonIds.at(0))->setConnectedEdges(0);
    }
    else if (m_buttons.size() == 2 && m_buttons.at(m_buttonIds.at(0)) && m_buttons.at(m_buttonIds.at(1)))
    {
        // dual buttons have connected edges only at the inner one
        m_buttons.at(m_buttonIds.at(0))->setConnectedEdges(juce::Button::ConnectedEdgeFlags::ConnectedOnRight);
        m_buttons.at(m_buttonIds.at(1))->setConnectedEdges(juce::Button::ConnectedEdgeFlags::ConnectedOnLeft);
    }
    else
    {
        // all other cases require that the button that is added gets only the left edge
        // set to flat and the previously rightmost button is modified to have both left and right edges now flat
        auto prevButtonId = m_buttonIds.at(m_buttonIds.size() - 2);
        auto lastButtonId = m_buttonIds.at(m_buttonIds.size() - 1);
        if (m_buttons.at(prevButtonId) && m_buttons.at(lastButtonId))
        {
            m_buttons.at(prevButtonId)->setConnectedEdges(juce::Button::ConnectedEdgeFlags::ConnectedOnRight | juce::Button::ConnectedEdgeFlags::ConnectedOnLeft);
            m_buttons.at(lastButtonId)->setConnectedEdges(juce::Button::ConnectedEdgeFlags::ConnectedOnLeft);
        }
    }

    return buttonID;
}

void SplitButtonComponent::addButtons(const std::vector<juce::String>& buttonTexts, const std::vector<std::float_t>& buttonFlexes)
{
    if (buttonTexts.size() != buttonFlexes.size())
    {
        for (auto const& buttonText : buttonTexts)
            addButton(buttonText);
    }
    else
    {
        for (int i = 0; i < buttonTexts.size() && i < buttonFlexes.size(); i++)
            addButton(buttonTexts.at(i), buttonFlexes.at(i));
    }
}

void SplitButtonComponent::setButtonDown(const std::uint64_t buttonId)
{
    for (auto const& p : m_buttons)
    {
        p.second->setToggleState(p.first == buttonId, dontSendNotification);
    }
}

void SplitButtonComponent::setButtonDown(const juce::String& buttonText)
{
    for (auto const& p : m_buttons)
    {
        p.second->setToggleState(p.second->getButtonText() == buttonText, dontSendNotification);
    }
}

const std::uint64_t SplitButtonComponent::getButtonDown()
{
    for (auto const& p : m_buttons)
    {
        if (p.second->getToggleState())
            return p.first;
    }

    return m_firstButtonID;
}

const juce::String SplitButtonComponent::getButtonDownText()
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

void SplitButtonComponent::setButtonEnabled(const juce::String& buttonText, bool enabled)
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

bool SplitButtonComponent::getButtonEnabled(const juce::String& buttonText) const
{
    for (auto const& p : m_buttons)
    {
        if (p.second && p.second->getButtonText() == buttonText)
            return p.second->isEnabled();
    }

    return false;
}

void SplitButtonComponent::paint (juce::Graphics& g)
{
    juce::Component::paint(g);
}

void SplitButtonComponent::resized()
{
    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::row;
    for (auto const& buttonId : m_buttonIds)
    {
        auto margin = juce::FlexItem::Margin(0, 0, 0, 0);
        auto isFirstButton = (buttonId == m_buttonIds.at(0));
        auto isFirstOfMultiButton = (isFirstButton && (m_buttonIds.size() > 1));
        auto isLastButton = (buttonId == m_buttonIds.at(m_buttonIds.size() - 1));
        auto isLastOfMultiButton = (isLastButton && (m_buttonIds.size() > 1));
        auto isInbetweenButton = !isFirstButton && !isLastButton;
        if (isFirstOfMultiButton)
            margin = juce::FlexItem::Margin(0, -1, 0, 0);
        else if (isLastOfMultiButton)
            margin = juce::FlexItem::Margin(0, 0, 0, -1);
        else if (isInbetweenButton)
            margin = juce::FlexItem::Margin(0, -1, 0, -1);

        if (0 != m_buttons.count(buttonId) && m_buttons.at(buttonId))
        {
            auto& button = m_buttons.at(buttonId);
            auto hasFlexInfo = (1 == m_buttonFlexes.count(buttonId));
            if (hasFlexInfo)
                fb.items.add(juce::FlexItem(*button.get()).withFlex(m_buttonFlexes.at(buttonId)).withMargin(margin));
            else
                fb.items.add(juce::FlexItem(*button.get()).withFlex(1).withMargin(margin));
        }
    }
    fb.performLayout(getLocalBounds().toFloat());
}

void SplitButtonComponent::buttonClicked(juce::Button* button)
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
