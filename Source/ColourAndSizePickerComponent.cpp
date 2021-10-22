/*
  ==============================================================================

    ColourAndSizePickerComponent.h
    Created: 22 Oct 2021 12:07:32pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "ColourAndSizePickerComponent.h"

#include "Image_utils.h"

namespace JUCEAppBasics
{

//==============================================================================
DrawableButtonWithColourIndicator::DrawableButtonWithColourIndicator(const juce::String buttonName, const juce::DrawableButton::ButtonStyle buttonStyle)
    : DrawableButton(buttonName, buttonStyle)
{
}

DrawableButtonWithColourIndicator::~DrawableButtonWithColourIndicator()
{
}

void DrawableButtonWithColourIndicator::setIndicatorColour(const juce::Colour& indicatorColour)
{
    m_indicatorColour = indicatorColour;
}

void DrawableButtonWithColourIndicator::paint(Graphics& g)
{
    DrawableButton::paint(g);

    auto imageBounds = DrawableButton::getImageBounds();
    g.setColour(m_indicatorColour);
    g.fillRect(imageBounds.removeFromBottom(0.2f * imageBounds.getHeight()));
}

Rectangle<float> DrawableButtonWithColourIndicator::getImageBounds() const
{
    auto imageBounds = DrawableButton::getImageBounds();

    return imageBounds.removeFromTop(0.8f * imageBounds.getHeight());
}


//==============================================================================
ColourAndSizeCustomPopupContent::ColourAndSizeCustomPopupContent()
    : CustomComponent(false)
{
    m_colourSelector = std::make_unique<ColourSelector>();
    addAndMakeVisible(m_colourSelector.get());
    m_colourSelectorLabel = std::make_unique<Label>("colour", "Colour:");
    addAndMakeVisible(m_colourSelectorLabel.get());

    m_sizeSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::NoTextBox);
    m_sizeSlider->setRange(0.1f, 1.0f, 0.05f);
    addAndMakeVisible(m_sizeSlider.get());
    m_sizeSliderLabel = std::make_unique<Label>("size", "Size:");
    addAndMakeVisible(m_sizeSliderLabel.get());

    m_acceptButton = std::make_unique<TextButton>("Accept");
    m_acceptButton->addListener(this);
    addAndMakeVisible(m_acceptButton.get());
}

ColourAndSizeCustomPopupContent::~ColourAndSizeCustomPopupContent()
{
}

void ColourAndSizeCustomPopupContent::resized()
{
    auto bounds = getLocalBounds();

    if(m_acceptButton)
        m_acceptButton->setBounds(bounds.removeFromBottom(25));
    bounds.removeFromBottom(15);
    if (m_sizeSlider)
        m_sizeSlider->setBounds(bounds.removeFromBottom(25));
    if (m_sizeSliderLabel)
        m_sizeSliderLabel->setBounds(bounds.removeFromBottom(15));
    bounds.removeFromBottom(20);
    if (m_colourSelectorLabel)
        m_colourSelectorLabel->setBounds(bounds.removeFromTop(15));
    if (m_colourSelector)
        m_colourSelector->setBounds(bounds);
}

void ColourAndSizeCustomPopupContent::buttonClicked(Button* button)
{
    if (m_acceptButton && m_acceptButton.get() == button)
    {
        if (onColourAndSizeValuePicked)
            onColourAndSizeValuePicked(getColourValue(), getSizeValue());
        triggerMenuItem();
    }
}

void ColourAndSizeCustomPopupContent::getIdealSize(int& idealWidth, int& idealHeight)
{
    idealWidth = 240;
    idealHeight = 360;

    auto primaryDisplay = Desktop::getInstance().getDisplays().getPrimaryDisplay();
    if (primaryDisplay != nullptr)
    {
        auto screenBounds = primaryDisplay->userArea;
        auto usableScreenWidth = static_cast<int>(0.8f * screenBounds.getWidth());
        auto usableScreenHeight = static_cast<int>(0.8f * screenBounds.getHeight());

        if (idealWidth > usableScreenWidth)
            idealWidth = usableScreenWidth;

        if (idealHeight > usableScreenHeight)
            idealHeight = usableScreenHeight;
    }
}

void ColourAndSizeCustomPopupContent::setColourAndSizeValue(const juce::Colour& colour, double size)
{
    if (m_colourSelector)
        m_colourSelector->setCurrentColour(colour, juce::dontSendNotification);
    if (m_sizeSlider)
        m_sizeSlider->setValue(size, juce::dontSendNotification);
}

const juce::Colour ColourAndSizeCustomPopupContent::getColourValue()
{
    if (m_colourSelector)
        return m_colourSelector->getCurrentColour();
    else
        return juce::Colour();
}

const double ColourAndSizeCustomPopupContent::getSizeValue()
{
    if (m_sizeSlider)
        return m_sizeSlider->getValue();
    else
        return 0.0f;
}


//==============================================================================
ColourAndSizePickerComponent::ColourAndSizePickerComponent()
{
    m_pickButton = std::make_unique<DrawableButtonWithColourIndicator>("Pick colour and size", DrawableButton::ButtonStyle::ImageOnButtonBackground);
    m_pickButton->addListener(this);
	addAndMakeVisible(m_pickButton.get());
    lookAndFeelChanged();

    setCurrentColourAndSize(Colour::greyLevel(0.5f), 0.5f);
}

ColourAndSizePickerComponent::~ColourAndSizePickerComponent()
{

}

void ColourAndSizePickerComponent::resized()
{
    if (m_pickButton)
        m_pickButton->setBounds(getLocalBounds());
}

void ColourAndSizePickerComponent::buttonClicked(Button* button)
{
	if (m_pickButton && button == m_pickButton.get())
	{
        triggerPickColourAndSize();
	}
}

void ColourAndSizePickerComponent::lookAndFeelChanged()
{
    Component::lookAndFeelChanged();

    auto colourOn = getLookAndFeel().findColour(TextButton::ColourIds::textColourOnId);
    auto colourOff = getLookAndFeel().findColour(TextButton::ColourIds::textColourOffId);

    std::unique_ptr<juce::Drawable> NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage;
    JUCEAppBasics::Image_utils::getDrawableButtonImages(BinaryData::colorize_black_24dp_svg, NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage,
        colourOn, colourOff, colourOff, colourOff, colourOn, colourOn, colourOn, colourOn);

    if (m_pickButton)
        m_pickButton->setImages(NormalImage.get(), OverImage.get(), DownImage.get(), DisabledImage.get(), NormalOnImage.get(), OverOnImage.get(), DownOnImage.get(), DisabledOnImage.get());
}

void ColourAndSizePickerComponent::setCurrentColourAndSize(const juce::Colour& colour, double size)
{
    m_colour = colour;
    m_size = size;

    if (m_pickButton)
        m_pickButton->setIndicatorColour(m_colour);
}

void ColourAndSizePickerComponent::triggerPickColourAndSize()
{
    m_popup.dismissAllActiveMenus();
    m_popup.clear();

    std::unique_ptr<ColourAndSizeCustomPopupContent> customPopupContent = std::make_unique<ColourAndSizeCustomPopupContent>();
    customPopupContent->setColourAndSizeValue(m_colour, m_size);
    customPopupContent->onColourAndSizeValuePicked = [=](const juce::Colour& colour, double size) { 
        setCurrentColourAndSize(colour, size);
        if (onColourAndSizeSet)
            onColourAndSizeSet(colour, size);
    };
    m_popup.addCustomItem(-1, std::move(customPopupContent));

    m_popup.showMenuAsync(PopupMenu::Options(), [this](int resultingAssiIdx) {
        ignoreUnused(resultingAssiIdx);
        });
}

}
