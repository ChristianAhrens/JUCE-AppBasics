/*
  ==============================================================================

    DualPointMultitouchCatcherComponent.h
    Created: 30 Aug 2022 20:02:00pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace JUCEAppBasics
{

//==============================================================================
/**
 *
 */
class DualPointMultitouchCatcherComponent : public juce::Component
{
public:
    enum InputState
    {
        IS_None = 0,
        IS_SingleTouchEntered,
        IS_SingleTouch,
        IS_SingleTouchLeft,
        IS_DualTouchEntered,
        IS_DualTouch,
        IS_DualTouchLeft,
        IS_TripleOrMoreTouchEntered,
        IS_TripleOrMoreTouch,
        IS_TripleOrMoreTouchLeft
    };

public:
    DualPointMultitouchCatcherComponent();
    ~DualPointMultitouchCatcherComponent() override;

    virtual void dualPointMultitouchStarted(const juce::Point<int>& p1, const juce::Point<int>& p2) = 0;
    virtual void dualPointMultitouchUpdated(const juce::Point<int>& p1, const juce::Point<int>& p2) = 0;
    virtual void dualPointMultitouchFinished() = 0;

protected:
    virtual void mouseDown(const juce::MouseEvent&) override;
    virtual void mouseDrag(const juce::MouseEvent&) override;
    virtual void mouseUp(const juce::MouseEvent&) override;

    virtual void modifierKeysChanged (const juce::ModifierKeys& modifiers) override;

    std::map<int, juce::Point<int>>& GetActiveMouseInputSources();
    int GetPrimaryMouseInputSourceIndex();
    
    bool IsInFakeALTMultiTouch();

    void ProcessMultitouchState();

private:
    std::map<int, juce::Point<int>>   m_activeMouseInputSources;
    InputState                  m_inputState{ IS_None };
    bool                        m_fakeDualMultiTouchWithALTModifier{ false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DualPointMultitouchCatcherComponent)
};

}
