#include "Image_utils.h"

namespace JUCEAppBasics
{
namespace Image_utils
{
    
void getDrawableButtonImages(juce::String BinaryDataString, 
    std::unique_ptr<juce::Drawable> &NormalImage, std::unique_ptr<juce::Drawable> &OverImage, std::unique_ptr<juce::Drawable> &DownImage, std::unique_ptr<juce::Drawable>& DisabledImage,
    std::unique_ptr<juce::Drawable> &NormalOnImage, std::unique_ptr<juce::Drawable> &OverOnImage, std::unique_ptr<juce::Drawable> &DownOnImage, std::unique_ptr<juce::Drawable>& DisabledOnImage,
    juce::Colour normalColour, juce::Colour overColour, juce::Colour downColour, juce::Colour disabledColour,
    juce::Colour normalOnColour, juce::Colour overOnColour, juce::Colour downOnColour, juce::Colour disabledOnColour)
{
    // create svg images from resources for regular state
    NormalImage = juce::Drawable::createFromSVGString(BinaryDataString);
    NormalImage->replaceColour(juce::Colours::black, normalColour);
    OverImage = juce::Drawable::createFromSVGString(BinaryDataString);
    OverImage->replaceColour(juce::Colours::black, overColour);
    DownImage = juce::Drawable::createFromSVGString(BinaryDataString);
    DownImage->replaceColour(juce::Colours::black, downColour);
    DisabledImage = juce::Drawable::createFromSVGString(BinaryDataString);
    DisabledImage->replaceColour(juce::Colours::black, disabledColour);

    // create svg images from resources for ON state
    NormalOnImage = juce::Drawable::createFromSVGString(BinaryDataString);
    NormalOnImage->replaceColour(juce::Colours::black, normalOnColour);
    OverOnImage = juce::Drawable::createFromSVGString(BinaryDataString);
    OverOnImage->replaceColour(juce::Colours::black, overOnColour);
    DownOnImage = juce::Drawable::createFromSVGString(BinaryDataString);
    DownOnImage->replaceColour(juce::Colours::black, downOnColour);
    DisabledOnImage = juce::Drawable::createFromSVGString(BinaryDataString);
    DisabledOnImage->replaceColour(juce::Colours::black, disabledOnColour);
}

void getDrawableButtonImages(juce::String BinaryDataString,
    std::unique_ptr<juce::Drawable>& NormalImage, std::unique_ptr<juce::Drawable>& OverImage, std::unique_ptr<juce::Drawable>& DownImage, std::unique_ptr<juce::Drawable>& DisabledImage,
    juce::Colour normalColour, juce::Colour overColour, juce::Colour downColour, juce::Colour disabledColour)
{
    // create svg images from resources for regular state
    NormalImage = juce::Drawable::createFromSVGString(BinaryDataString);
    NormalImage->replaceColour(juce::Colours::black, normalColour);
    OverImage = juce::Drawable::createFromSVGString(BinaryDataString);
    OverImage->replaceColour(juce::Colours::black, overColour);
    DownImage = juce::Drawable::createFromSVGString(BinaryDataString);
    DownImage->replaceColour(juce::Colours::black, downColour);
    DisabledImage = juce::Drawable::createFromSVGString(BinaryDataString);
    DisabledImage->replaceColour(juce::Colours::black, disabledColour);
}
     
}; // namespace Image_utils
}; // namespace JUCE-AppBasics
