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
class DualPointMultitouchCatcherComponent : public Component
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

    virtual void dualPointMultitouchStarted(const Point<int>& p1, const Point<int>& p2) = 0;
    virtual void dualPointMultitouchUpdated(const Point<int>& p1, const Point<int>& p2) = 0;
    virtual void dualPointMultitouchFinished() = 0;

protected:
    virtual void mouseDown(const MouseEvent&) override;
    virtual void mouseDrag(const MouseEvent&) override;
    virtual void mouseUp(const MouseEvent&) override;

    std::map<int, Point<int>>& GetActiveMouseInputSources();
    int GetPrimaryMouseInputSourceIndex();

    void ProcessMultitouchState();

private:
    std::map<int, Point<int>>   m_activeMouseInputSources;
    InputState                  m_inputState{ IS_None };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DualPointMultitouchCatcherComponent)
};

}
