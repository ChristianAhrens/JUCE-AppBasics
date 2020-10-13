#pragma once

#include <JuceHeader.h>

namespace JUCEAppBasics
{
namespace Image_utils
{
    
static void getDrawableButtonImages(juce::String BinaryDataString, 
    std::unique_ptr<juce::Drawable> &NormalImage, std::unique_ptr<juce::Drawable> &OverImage, std::unique_ptr<juce::Drawable> &DownImage, std::unique_ptr<juce::Drawable>& DisabledImage,
    std::unique_ptr<juce::Drawable> &NormalOnImage, std::unique_ptr<juce::Drawable> &OverOnImage, std::unique_ptr<juce::Drawable> &DownOnImage, std::unique_ptr<juce::Drawable>& DisabledOnImage,
    Colour normalColour = Colours::white, Colour overColour = Colours::lightgrey, Colour downColour = Colours::grey, Colour disabledColour = Colours::grey,
    Colour normalOnColour = Colours::white, Colour overOnColour = Colours::white, Colour downOnColour = Colours::white, Colour disabledOnColour = Colours::white)
{
    std::unique_ptr<XmlElement> svg_xml = XmlDocument::parse(BinaryDataString);

    // create svg images from resources for regular state
    NormalImage = Drawable::createFromSVG(*(svg_xml.get()));
    NormalImage->replaceColour(Colours::black, normalColour);
    OverImage = Drawable::createFromSVG(*(svg_xml.get()));
    OverImage->replaceColour(Colours::black, overColour);
    DownImage = Drawable::createFromSVG(*(svg_xml.get()));
    DownImage->replaceColour(Colours::black, downColour);
    DisabledImage = Drawable::createFromSVG(*(svg_xml.get()));
    DisabledImage->replaceColour(Colours::black, disabledColour);

    // create svg images from resources for ON state
    NormalOnImage = Drawable::createFromSVG(*(svg_xml.get()));
    NormalOnImage->replaceColour(Colours::black, normalOnColour);
    OverOnImage = Drawable::createFromSVG(*(svg_xml.get()));
    OverOnImage->replaceColour(Colours::black, overOnColour);
    DownOnImage = Drawable::createFromSVG(*(svg_xml.get()));
    DownOnImage->replaceColour(Colours::black, downOnColour);
    DisabledOnImage = Drawable::createFromSVG(*(svg_xml.get()));
    DisabledOnImage->replaceColour(Colours::black, disabledOnColour);
}

static void getDrawableButtonImages(juce::String BinaryDataString,
    std::unique_ptr<juce::Drawable>& NormalImage, std::unique_ptr<juce::Drawable>& OverImage, std::unique_ptr<juce::Drawable>& DownImage, std::unique_ptr<juce::Drawable>& DisabledImage,
    Colour normalColour = Colours::white, Colour overColour = Colours::lightgrey, Colour downColour = Colours::grey, Colour disabledColour = Colours::grey)
{
    std::unique_ptr<XmlElement> svg_xml = XmlDocument::parse(BinaryDataString);

    // create svg images from resources for regular state
    NormalImage = Drawable::createFromSVG(*(svg_xml.get()));
    NormalImage->replaceColour(Colours::black, normalColour);
    OverImage = Drawable::createFromSVG(*(svg_xml.get()));
    OverImage->replaceColour(Colours::black, overColour);
    DownImage = Drawable::createFromSVG(*(svg_xml.get()));
    DownImage->replaceColour(Colours::black, downColour);
    DisabledImage = Drawable::createFromSVG(*(svg_xml.get()));
    DisabledImage->replaceColour(Colours::black, disabledColour);
}
     
}; // namespace Image_utils
}; // namespace JUCE-AppBasics