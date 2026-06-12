/*
  ==============================================================================

    ColourAndSizePickerComponent.h
    Created: 22 Oct 2021 12:07:32pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace JUCEAppBasics
{


class DrawableButtonWithColourIndicator : public juce::DrawableButton
{
public:
    DrawableButtonWithColourIndicator(const juce::String buttonName, const juce::DrawableButton::ButtonStyle buttonStyle);
    ~DrawableButtonWithColourIndicator();

    void setIndicatorColour(const juce::Colour& indicatorColour);

    //==============================================================================
    void paint(juce::Graphics& g) override;

    //==============================================================================
    juce::Rectangle<float> getImageBounds() const override;

private:
    juce::Colour    m_indicatorColour;
};
    
class ColourAndSizeCustomPopupContent :
    public juce::PopupMenu::CustomComponent,
    public juce::Button::Listener
{
public:
    ColourAndSizeCustomPopupContent();
    ~ColourAndSizeCustomPopupContent();

    //==============================================================================
    void resized() override;

    //==============================================================================
    void buttonClicked(juce::Button*) override;

    //==============================================================================
    void getIdealSize(int& idealWidth, int& idealHeight) override;

    //==============================================================================
    void setColourAndSizeValue(const juce::Colour& colour, double size);
    const juce::Colour getColourValue();
    const double getSizeValue();

    //==============================================================================
    std::function<void(const juce::Colour& colour, double size)> onColourAndSizeValuePicked;

private:
    std::unique_ptr<juce::ColourSelector> m_colourSelector;
    std::unique_ptr<juce::Label>          m_colourSelectorLabel;
    std::unique_ptr<juce::Slider>         m_sizeSlider;
    std::unique_ptr<juce::Label>          m_sizeSliderLabel;
    std::unique_ptr<juce::TextButton>     m_acceptButton;

};

class ColourAndSizePickerComponent :
    public juce::Component,
    public juce::Button::Listener
{
public:
    ColourAndSizePickerComponent();
    ~ColourAndSizePickerComponent();
	
    //==============================================================================
    void resized() override;

    //==============================================================================
    void buttonClicked(juce::Button*) override;

    //==============================================================================
    void lookAndFeelChanged() override;

    //==============================================================================
    std::function<void(const juce::Colour& colour, double size)> onColourAndSizeSet;
    
    //==============================================================================
    void setCurrentColourAndSize(const juce::Colour &colour, double size);
    
private:
    void triggerPickColourAndSize();

    std::unique_ptr<DrawableButtonWithColourIndicator>  m_pickButton;
    juce::String                                        m_deviceIdentifier;
    juce::String                                        m_deviceName;
    juce::PopupMenu                                     m_popup;

    juce::Colour                                        m_colour;
    double                                              m_size;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourAndSizePickerComponent)
};

};
