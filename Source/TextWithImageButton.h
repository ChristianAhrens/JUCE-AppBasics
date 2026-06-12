/*
  ==============================================================================

    TextWithImageButton.h
    Created: 15 Dec 2020 22:02:00pm
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
class TextWithImageButton : public juce::DrawableButton
{
public:
    TextWithImageButton();
    explicit TextWithImageButton(const juce::String& buttonName);
	~TextWithImageButton() override;

    //==============================================================================
    juce::Rectangle<float> getImageBounds() const override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    //==============================================================================
    void setImagePosition(juce::Justification position);

private:
    juce::Justification m_imagePosition;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TextWithImageButton)
};

}
