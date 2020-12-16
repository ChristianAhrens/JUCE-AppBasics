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
class TextWithImageButton : public DrawableButton
{
public:
    TextWithImageButton();
    explicit TextWithImageButton(const String& buttonName);
	~TextWithImageButton() override;

    //==============================================================================
    Rectangle<float> getImageBounds() const override;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    //==============================================================================
    void setImagePosition(Justification position);

private:
    Justification m_imagePosition;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TextWithImageButton)
};

}
