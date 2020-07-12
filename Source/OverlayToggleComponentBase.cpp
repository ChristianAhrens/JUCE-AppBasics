/*
  ==============================================================================

    OverlayToggleComponentBase.cpp
    Created: 11 Jul 2020 10:10:30am
    Author:  musah

  ==============================================================================
*/

#include "OverlayToggleComponentBase.h"

#include "Image_utils.hpp"

namespace JUCEAppBasics
{

OverlayToggleComponentBase::OverlayToggleComponentBase()
{
    m_toggleOverlayButton = std::make_unique<DrawableButton>(String(), DrawableButton::ButtonStyle::ImageFitted);

    std::unique_ptr<juce::Drawable> NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage;
    JUCEAppBasics::Image_utils::getDrawableButtonImages(BinaryData::close_fullscreen24px_svg, NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage);
    JUCEAppBasics::Image_utils::getDrawableButtonImages(BinaryData::open_in_full24px_svg, NormalImage, OverImage, DownImage, DisabledImage);
    // set the images to button
    m_toggleOverlayButton->setImages(NormalImage.get(), OverImage.get(), DownImage.get(), DisabledImage.get(), NormalOnImage.get(), OverOnImage.get(), DownOnImage.get(), DisabledOnImage.get());
    m_toggleOverlayButton->setClickingTogglesState(true);
    m_toggleOverlayButton->setAlwaysOnTop(true);
    m_toggleOverlayButton->addListener(this);
    addAndMakeVisible(m_toggleOverlayButton.get());

    setMinimized();
}

OverlayToggleComponentBase::~OverlayToggleComponentBase()
{
    m_overlayParent = nullptr;
}

void OverlayToggleComponentBase::addOverlayParent(OverlayParent *p)
{
    m_overlayParent = p;
}

Rectangle<int>  OverlayToggleComponentBase::getOverlayBounds() const
{
    if (getCurrentOverlayState() == maximized)
        return getLocalBounds().reduced(10);
    else if (getCurrentOverlayState() == minimized)
        return getLocalBounds();
    else
        return getBounds();
}

void OverlayToggleComponentBase::paint(Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());

    g.setColour(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    g.fillRect(getOverlayBounds().toFloat());
}

void OverlayToggleComponentBase::resized()
{
    auto buttonSize = Point<int>(25, 25);
    auto topRight = getOverlayBounds().getTopRight();
    auto buttonOrig = topRight - Point<int>(buttonSize.getX(), 0);

    m_toggleOverlayButton->setBounds(Rectangle<int>(buttonOrig, buttonOrig + buttonSize));
}

void OverlayToggleComponentBase::buttonClicked(Button* button)
{
    if (button == m_toggleOverlayButton.get())
    {
        changeOverlayState();

        if (m_overlayParent)
        {
            if ((getCurrentOverlayState() == minimized) && m_overlayParent->isOverlayActive())
            {
                m_overlayParent->activateOverlayComponent(nullptr);
                setAlwaysOnTop(false);
            }
            else if((getCurrentOverlayState() == maximized) && !m_overlayParent->isOverlayActive())
            {
                m_overlayParent->activateOverlayComponent(this);
                setAlwaysOnTop(true);
            }

            parentResize();
        }
    }
}

OverlayToggleComponentBase::OverlayState OverlayToggleComponentBase::getCurrentOverlayState() const
{
    return m_overlayState;
}

void OverlayToggleComponentBase::changeOverlayState()
{
    if (getCurrentOverlayState() == OverlayState::minimized)
        setMaximized();
    else
        setMinimized();
}

void OverlayToggleComponentBase::setMinimized()
{
    if (m_toggleOverlayButton->getToggleState())
        m_toggleOverlayButton->setToggleState(false, dontSendNotification);
    m_overlayState = minimized;
}

void OverlayToggleComponentBase::setMaximized()
{
    if (!m_toggleOverlayButton->getToggleState())
        m_toggleOverlayButton->setToggleState(true, dontSendNotification);
    m_overlayState = maximized;
}

}; // namespace JUCEAppBasics