/*
  ==============================================================================

    TextWithImageButton.cpp
    Created: 15 Dec 2020 22:02:00pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "TextWithImageButton.h"


namespace JUCEAppBasics
{

//==============================================================================
TextWithImageButton::TextWithImageButton()
	: juce::DrawableButton("", juce::DrawableButton::ImageOnButtonBackground), m_imagePosition(juce::Justification::centredRight)
{
}

TextWithImageButton::TextWithImageButton(const juce::String& buttonName)
	: juce::DrawableButton(buttonName, juce::DrawableButton::ImageOnButtonBackground), m_imagePosition(juce::Justification::centredRight)
{
}

TextWithImageButton::~TextWithImageButton()
{

}

void TextWithImageButton::setImagePosition(juce::Justification position)
{
    m_imagePosition = position;
}

juce::Rectangle<float> TextWithImageButton::getImageBounds() const
{
    auto bounds = getLocalBounds();
    bounds.reduce(3, 3);

    switch (m_imagePosition.getFlags())
    {
    case juce::Justification::centredRight:
        return bounds.removeFromRight(bounds.getHeight()).toFloat();
    case juce::Justification::centredLeft:
        return bounds.removeFromLeft(bounds.getHeight()).toFloat();
    default:
        return juce::DrawableButton::getImageBounds();
    }
}

void TextWithImageButton::paint(juce::Graphics& g)
{
    juce::DrawableButton::paint(g);

    // text drawing taken from LookAndFeel_V2::drawButtonText
    juce::Font font(juce::FontOptions(juce::jmin(15.0f, static_cast<float>(getHeight()) * 0.6f)));
    g.setFont(font);
    g.setColour(findColour(getToggleState() ? juce::TextButton::textColourOnId
        : juce::TextButton::textColourOffId)
        .withMultipliedAlpha(isEnabled() ? 1.0f : 0.5f));

    const int yIndent = juce::jmin(4, proportionOfHeight(0.3f));
    const int cornerSize = juce::jmin(getHeight(), getWidth()) / 2;

    auto fontHeight = juce::roundToInt(font.getHeight() * 0.6f);
    auto leftIndent = juce::jmin(fontHeight, 2 + cornerSize / (isConnectedOnLeft() ? 4 : 2));
    auto rightIndent = juce::jmin(fontHeight, 2 + cornerSize / (isConnectedOnRight() ? 4 : 2));
    auto textWidth = getWidth() - leftIndent - rightIndent;

    juce::Justification::Flags textPosition;
    switch (m_imagePosition.getFlags())
    {
    case juce::Justification::centredRight:
        textPosition = juce::Justification::centredLeft;
        textWidth -= getImageBounds().toNearestInt().getWidth();
        break;
    case juce::Justification::centredLeft:
        textPosition = juce::Justification::centredRight;
        leftIndent += getImageBounds().toNearestInt().getWidth();
        textWidth -= getImageBounds().toNearestInt().getWidth();
        break;
    default:
        textPosition = juce::Justification::centred;
        break;
    }

    if (textWidth > 0)
        g.drawFittedText(getButtonText(),
            leftIndent, yIndent, textWidth, getHeight() - yIndent * 2,
            textPosition, 2);
}

void TextWithImageButton::resized()
{
    juce::DrawableButton::resized();
}

}
