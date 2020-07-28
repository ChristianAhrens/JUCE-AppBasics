/*
  ==============================================================================

    DemoOverlayComponent.cpp
    Created: 11 Jul 2020 10:23:17am
    Author:  musah

  ==============================================================================
*/

#include "DemoOverlayComponent.h"

DemoOverlayComponent::DemoOverlayComponent()
    : OverlayToggleComponentBase()
{
    m_stateLabel = std::make_unique<Label>();
    addAndMakeVisible(m_stateLabel.get());
    m_stateLabel->setText(getCurrentStateAsText(), dontSendNotification);
}

DemoOverlayComponent::~DemoOverlayComponent()
{

}

void DemoOverlayComponent::resized()
{
    OverlayToggleComponentBase::resized();

    FlexBox fb;
    fb.flexDirection = FlexBox::Direction::row;
    fb.alignItems = FlexBox::AlignItems::center;
    fb.items.addArray({
        FlexItem(*m_stateLabel.get()).withFlex(1)
        });
    
    fb.performLayout(getOverlayBounds().toFloat());
}

void DemoOverlayComponent::changeOverlayState()
{
    OverlayToggleComponentBase::changeOverlayState();

    m_stateLabel->setText(getCurrentStateAsText(), dontSendNotification);
}

String DemoOverlayComponent::getCurrentStateAsText()
{
    String retVal;

    switch (getCurrentOverlayState())
    {
    case minimized:
        retVal = "Minimized state.";
        break;
    case maximized:
        retVal = "Maximized state.";
        break;
    case invalid:
    default:
        retVal = "Invalid state.";
        break;
    }

    return retVal;
}
