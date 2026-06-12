#pragma once

#include <JuceHeader.h>

namespace JUCEAppBasics
{
namespace Image_utils
{
    
void getDrawableButtonImages(juce::String BinaryDataString, 
    std::unique_ptr<juce::Drawable> &NormalImage, std::unique_ptr<juce::Drawable> &OverImage, std::unique_ptr<juce::Drawable> &DownImage, std::unique_ptr<juce::Drawable>& DisabledImage,
    std::unique_ptr<juce::Drawable> &NormalOnImage, std::unique_ptr<juce::Drawable> &OverOnImage, std::unique_ptr<juce::Drawable> &DownOnImage, std::unique_ptr<juce::Drawable>& DisabledOnImage,
    juce::Colour normalColour = juce::Colours::white, juce::Colour overColour = juce::Colours::lightgrey, juce::Colour downColour = juce::Colours::grey, juce::Colour disabledColour = juce::Colours::grey,
    juce::Colour normalOnColour = juce::Colours::white, juce::Colour overOnColour = juce::Colours::white, juce::Colour downOnColour = juce::Colours::white, juce::Colour disabledOnColour = juce::Colours::white);

void getDrawableButtonImages(juce::String BinaryDataString,
    std::unique_ptr<juce::Drawable>& NormalImage, std::unique_ptr<juce::Drawable>& OverImage, std::unique_ptr<juce::Drawable>& DownImage, std::unique_ptr<juce::Drawable>& DisabledImage,
    juce::Colour normalColour = juce::Colours::white, juce::Colour overColour = juce::Colours::lightgrey, juce::Colour downColour = juce::Colours::grey, juce::Colour disabledColour = juce::Colours::grey);
     
}; // namespace Image_utils
}; // namespace JUCE-AppBasics