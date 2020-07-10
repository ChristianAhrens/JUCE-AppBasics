#pragma once

#include <JuceHeader.h>

namespace JUCEAppBasics
{
namespace Image_utils
{
    
static void getDrawableButtonImages(juce::String BinaryDataString, 
    std::unique_ptr<juce::Drawable> &NormalImage, std::unique_ptr<juce::Drawable> &OverImage, std::unique_ptr<juce::Drawable> &DownImage, std::unique_ptr<juce::Drawable>& DisabledImage,
    std::unique_ptr<juce::Drawable> &NormalOnImage, std::unique_ptr<juce::Drawable> &OverOnImage, std::unique_ptr<juce::Drawable> &DownOnImage, std::unique_ptr<juce::Drawable>& DisabledOnImage)
{
    std::unique_ptr<XmlElement> svg_xml = XmlDocument::parse(BinaryDataString);

    // create svg images from resources for regular state
    NormalImage = Drawable::createFromSVG(*(svg_xml.get()));
    NormalImage->replaceColour(Colours::black, Colours::white);
    OverImage = Drawable::createFromSVG(*(svg_xml.get()));
    OverImage->replaceColour(Colours::black, Colours::lightgrey);
    DownImage = Drawable::createFromSVG(*(svg_xml.get()));
    DownImage->replaceColour(Colours::black, Colours::grey);
    DisabledImage = Drawable::createFromSVG(*(svg_xml.get()));
    DisabledImage->replaceColour(Colours::black, Colours::grey);

    // create svg images from resources for ON state
    NormalOnImage = Drawable::createFromSVG(*(svg_xml.get()));
    NormalOnImage->replaceColour(Colours::black, Colours::white);
    OverOnImage = Drawable::createFromSVG(*(svg_xml.get()));
    OverOnImage->replaceColour(Colours::black, Colours::white);
    DownOnImage = Drawable::createFromSVG(*(svg_xml.get()));
    DownOnImage->replaceColour(Colours::black, Colours::white);
    DisabledOnImage = Drawable::createFromSVG(*(svg_xml.get()));
    DisabledOnImage->replaceColour(Colours::black, Colours::white);
}
     
}; // namespace Image_utils
}; // namespace JUCE-AppBasics