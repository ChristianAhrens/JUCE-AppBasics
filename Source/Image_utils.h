#pragma once

#include <JuceHeader.h>

namespace JUCEAppBasics
{
namespace Image_utils
{
    
void getDrawableButtonImages(juce::String BinaryDataString, 
    std::unique_ptr<juce::Drawable> &NormalImage, std::unique_ptr<juce::Drawable> &OverImage, std::unique_ptr<juce::Drawable> &DownImage, std::unique_ptr<juce::Drawable>& DisabledImage,
    std::unique_ptr<juce::Drawable> &NormalOnImage, std::unique_ptr<juce::Drawable> &OverOnImage, std::unique_ptr<juce::Drawable> &DownOnImage, std::unique_ptr<juce::Drawable>& DisabledOnImage,
    Colour normalColour = Colours::white, Colour overColour = Colours::lightgrey, Colour downColour = Colours::grey, Colour disabledColour = Colours::grey,
    Colour normalOnColour = Colours::white, Colour overOnColour = Colours::white, Colour downOnColour = Colours::white, Colour disabledOnColour = Colours::white);

void getDrawableButtonImages(juce::String BinaryDataString,
    std::unique_ptr<juce::Drawable>& NormalImage, std::unique_ptr<juce::Drawable>& OverImage, std::unique_ptr<juce::Drawable>& DownImage, std::unique_ptr<juce::Drawable>& DisabledImage,
    Colour normalColour = Colours::white, Colour overColour = Colours::lightgrey, Colour downColour = Colours::grey, Colour disabledColour = Colours::grey);
     
}; // namespace Image_utils
}; // namespace JUCE-AppBasics