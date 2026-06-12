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
    std::unique_ptr<juce::XmlElement> svg_xml = juce::XmlDocument::parse(BinaryDataString);

    // create svg images from resources for regular state
    NormalImage = juce::Drawable::createFromSVG(*(svg_xml.get()));
    NormalImage->replaceColour(juce::Colours::black, normalColour);
    OverImage = juce::Drawable::createFromSVG(*(svg_xml.get()));
    OverImage->replaceColour(juce::Colours::black, overColour);
    DownImage = juce::Drawable::createFromSVG(*(svg_xml.get()));
    DownImage->replaceColour(juce::Colours::black, downColour);
    DisabledImage = juce::Drawable::createFromSVG(*(svg_xml.get()));
    DisabledImage->replaceColour(juce::Colours::black, disabledColour);

    // create svg images from resources for ON state
    NormalOnImage = juce::Drawable::createFromSVG(*(svg_xml.get()));
    NormalOnImage->replaceColour(juce::Colours::black, normalOnColour);
    OverOnImage = juce::Drawable::createFromSVG(*(svg_xml.get()));
    OverOnImage->replaceColour(juce::Colours::black, overOnColour);
    DownOnImage = juce::Drawable::createFromSVG(*(svg_xml.get()));
    DownOnImage->replaceColour(juce::Colours::black, downOnColour);
    DisabledOnImage = juce::Drawable::createFromSVG(*(svg_xml.get()));
    DisabledOnImage->replaceColour(juce::Colours::black, disabledOnColour);
}

void getDrawableButtonImages(juce::String BinaryDataString,
    std::unique_ptr<juce::Drawable>& NormalImage, std::unique_ptr<juce::Drawable>& OverImage, std::unique_ptr<juce::Drawable>& DownImage, std::unique_ptr<juce::Drawable>& DisabledImage,
    juce::Colour normalColour, juce::Colour overColour, juce::Colour downColour, juce::Colour disabledColour)
{
    std::unique_ptr<juce::XmlElement> svg_xml = juce::XmlDocument::parse(BinaryDataString);

    // create svg images from resources for regular state
    NormalImage = juce::Drawable::createFromSVG(*(svg_xml.get()));
    NormalImage->replaceColour(juce::Colours::black, normalColour);
    OverImage = juce::Drawable::createFromSVG(*(svg_xml.get()));
    OverImage->replaceColour(juce::Colours::black, overColour);
    DownImage = juce::Drawable::createFromSVG(*(svg_xml.get()));
    DownImage->replaceColour(juce::Colours::black, downColour);
    DisabledImage = juce::Drawable::createFromSVG(*(svg_xml.get()));
    DisabledImage->replaceColour(juce::Colours::black, disabledColour);
}
     
}; // namespace Image_utils
}; // namespace JUCE-AppBasics