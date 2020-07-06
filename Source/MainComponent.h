/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "SplitButtonComponent.h"
#include "CustomLookAndFeel.h"

namespace AppBasicsDemo
{

//==============================================================================
class DemoHeaderFooterComponent : public Component
{
public:
    //==============================================================================
    DemoHeaderFooterComponent() {};
    ~DemoHeaderFooterComponent() {};

    //==============================================================================
    void paint(Graphics &g)
    {
        g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
    };
    void resized() {};
};

//==============================================================================
class DemoBodyComponent : public Component
{
public:
    //==============================================================================
    DemoBodyComponent() {};
    ~DemoBodyComponent() {};

    //==============================================================================
    void paint(Graphics& g)
    {
        g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    };
    void resized() {};
};

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
private:
    //==============================================================================
    std::unique_ptr<DemoHeaderFooterComponent>  m_header;
    std::unique_ptr<DemoBodyComponent>          m_body;
    std::unique_ptr<DemoHeaderFooterComponent>  m_footer;

    std::vector<std::unique_ptr<DrawableButton>>            m_buttons;
    std::unique_ptr<JUCEAppBasics::SplitButtonComponent>    m_splitButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

}