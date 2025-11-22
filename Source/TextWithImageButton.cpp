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
	: DrawableButton("", ImageOnButtonBackground), m_imagePosition(Justification::centredRight)
{
}

TextWithImageButton::TextWithImageButton(const String& buttonName)
	: DrawableButton(buttonName, ImageOnButtonBackground), m_imagePosition(Justification::centredRight)
{
}

TextWithImageButton::~TextWithImageButton()
{

}

void TextWithImageButton::setImagePosition(Justification position)
{
    m_imagePosition = position;
}

Rectangle<float> TextWithImageButton::getImageBounds() const
{
    auto bounds = getLocalBounds();
    bounds.reduce(3, 3);

    switch (m_imagePosition.getFlags())
    {
    case Justification::centredRight:
        return bounds.removeFromRight(bounds.getHeight()).toFloat();
    case Justification::centredLeft:
        return bounds.removeFromLeft(bounds.getHeight()).toFloat();
    default:
        return DrawableButton::getImageBounds();
    }
}

void TextWithImageButton::paint(Graphics& g)
{
    DrawableButton::paint(g);

    // text drawing taken from LookAndFeel_V2::drawButtonText
    juce::Font font(juce::FontOptions(jmin(15.0f, static_cast<float>(getHeight()) * 0.6f)));
    g.setFont(font);
    g.setColour(findColour(getToggleState() ? TextButton::textColourOnId
        : TextButton::textColourOffId)
        .withMultipliedAlpha(isEnabled() ? 1.0f : 0.5f));

    const int yIndent = jmin(4, proportionOfHeight(0.3f));
    const int cornerSize = jmin(getHeight(), getWidth()) / 2;

    auto fontHeight = roundToInt(font.getHeight() * 0.6f);
    auto leftIndent = jmin(fontHeight, 2 + cornerSize / (isConnectedOnLeft() ? 4 : 2));
    auto rightIndent = jmin(fontHeight, 2 + cornerSize / (isConnectedOnRight() ? 4 : 2));
    auto textWidth = getWidth() - leftIndent - rightIndent;

    Justification::Flags textPosition;
    switch (m_imagePosition.getFlags())
    {
    case Justification::centredRight:
        textPosition = Justification::centredLeft;
        textWidth -= getImageBounds().toNearestInt().getWidth();
        break;
    case Justification::centredLeft:
        textPosition = Justification::centredRight;
        leftIndent += getImageBounds().toNearestInt().getWidth();
        textWidth -= getImageBounds().toNearestInt().getWidth();
        break;
    default:
        textPosition = Justification::centred;
        break;
    }

    if (textWidth > 0)
        g.drawFittedText(getButtonText(),
            leftIndent, yIndent, textWidth, getHeight() - yIndent * 2,
            textPosition, 2);
}

void TextWithImageButton::resized()
{
    DrawableButton::resized();
}

}
