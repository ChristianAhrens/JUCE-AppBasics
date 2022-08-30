/*
  ==============================================================================

    DualPointMultitouchCatcherComponent.h
    Created: 30 Aug 2022 20:02:00pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "DualPointMultitouchCatcherComponent.h"

namespace JUCEAppBasics
{

//==============================================================================
/**
 *
 */
DualPointMultitouchCatcherComponent::DualPointMultitouchCatcherComponent(bool filterMouseDragsWhileActive)
	:   Component(),
        m_filterMouseDragsWhileActive(filterMouseDragsWhileActive)
{
}

/**
 *
 */
DualPointMultitouchCatcherComponent::~DualPointMultitouchCatcherComponent()
{
}

/**
 *
 */
void DualPointMultitouchCatcherComponent::paint(Graphics& g)
{
    Component::paint(g);
}

/**
 *
 */
void DualPointMultitouchCatcherComponent::resized()
{
    Component::resized();
}

/**
 * Called when a mouse button is pressed. 
 * @param e		Details about the position and status of the mouse event, including the source component in which it occurred 
 */
void DualPointMultitouchCatcherComponent::mouseDown(const MouseEvent& e)
{
    if (GetActiveMouseInputSources().empty())
    {
        m_inputState = IS_SingleTouchEntered;
        GetActiveMouseInputSources().insert(std::make_pair(e.source.getIndex(), e.getMouseDownPosition()));
    }
    else
    {
        auto iter = std::find_if(GetActiveMouseInputSources().begin(), GetActiveMouseInputSources().end(), [&](const auto& val) { return val.first == e.source.getIndex(); });
        if (iter == GetActiveMouseInputSources().end())
        {
            if (GetActiveMouseInputSources().size() == 1)
                m_inputState = IS_DualTouchEntered;
            else if (GetActiveMouseInputSources().size() > 1)
                m_inputState = IS_TripleOrMoreTouchEntered;
            GetActiveMouseInputSources().insert(std::make_pair(e.source.getIndex(), e.getMouseDownPosition()));
        }
        else
            jassertfalse; // A mouseDown for an already known inputIndex does not make sense. What went wrong?
    }

    ProcessMultitouchState();

    Component::mouseDown(e);
}

/**
 * Called when the mouse is moved while a button is held down. 
 * @param e		Details about the position and status of the mouse event, including the source component in which it occurred
 */
void DualPointMultitouchCatcherComponent::mouseDrag(const MouseEvent& e)
{
    if (GetActiveMouseInputSources().empty())
        jassertfalse;   // A mouseDrag for a not already known (mouseDown) inputIndex does not make sense. What went wrong?

    auto iter = std::find_if(GetActiveMouseInputSources().begin(), GetActiveMouseInputSources().end(), [&](const auto& val) { return val.first == e.source.getIndex(); });
    if (iter != GetActiveMouseInputSources().end())
        iter->second = e.getMouseDownPosition();
    else
        jassertfalse;   // A mouseDrag for a not already known (mouseDown) inputIndex does not make sense. What went wrong?

    if (GetActiveMouseInputSources().size() == 1)
        m_inputState = IS_SingleTouch;
    else if (GetActiveMouseInputSources().size() == 2)
        m_inputState = IS_DualTouch;
    else if (GetActiveMouseInputSources().size() > 2)
        m_inputState = IS_TripleOrMoreTouch;

    ProcessMultitouchState();

    Component::mouseDrag(e);
}

/**
 * Called when the mouse button is released.
 * @param e		Details about the position and status of the mouse event, including the source component in which it occurred
 */
void DualPointMultitouchCatcherComponent::mouseUp(const MouseEvent& e)
{
    if (GetActiveMouseInputSources().empty())
        jassertfalse;   // A mouseUp for a not already known (mouseDown) inputIndex does not make sense. What went wrong?

    auto iter = std::find_if(GetActiveMouseInputSources().begin(), GetActiveMouseInputSources().end(), [&](const auto& val) { return val.first == e.source.getIndex(); });
    if (iter != GetActiveMouseInputSources().end())
    {
        if (GetActiveMouseInputSources().size() == 1)
            m_inputState = IS_SingleTouchLeft;
        else if (GetActiveMouseInputSources().size() == 2)
            m_inputState = IS_DualTouchLeft;
        else if (GetActiveMouseInputSources().size() > 2)
            m_inputState = IS_TripleOrMoreTouchLeft;
        GetActiveMouseInputSources().erase(iter);
    }
    else
        jassertfalse;

    ProcessMultitouchState();

    Component::mouseUp(e);
}

/**
 * Getter for the internal bool state defining if filtering mousedrags while touch action is active
 * @return  True if filtering is active, false if not.
 */
bool DualPointMultitouchCatcherComponent::IsFilteringMouseDragsWhileActive()
{
    return m_filterMouseDragsWhileActive;
}

/**
 * Getter for the internal map of active inputSources
 * @return  The list of currently active ('mouseDown') inputSources
 */
std::map<int, Point<int>>& DualPointMultitouchCatcherComponent::GetActiveMouseInputSources()
{
    return m_activeMouseInputSources;
}

/**
 * Method to process current inputSources and their values and derive
 * what notifications must be broadcasted via pure virtual method calls.
 */
void DualPointMultitouchCatcherComponent::ProcessMultitouchState()
{
    switch (m_inputState)
    {
    case IS_None:
    case IS_SingleTouchEntered:
    case IS_SingleTouch:
    case IS_SingleTouchLeft:
        break;
    case IS_DualTouchEntered:
        if (GetActiveMouseInputSources().size() > 1)
            dualPointMultitouchStarted(GetActiveMouseInputSources().begin()->second, (GetActiveMouseInputSources().begin()++)->second);
        break;
    case IS_DualTouch:
        if(GetActiveMouseInputSources().size() > 1)
            dualPointMultitouchUpdated(GetActiveMouseInputSources().begin()->second, (GetActiveMouseInputSources().begin()++)->second);
        break;
    case IS_DualTouchLeft:
        dualPointMultitouchFinished();
        break;
    case IS_TripleOrMoreTouchEntered:
    case IS_TripleOrMoreTouch:
    case IS_TripleOrMoreTouchLeft:
        if (GetActiveMouseInputSources().size() > 1)
            dualPointMultitouchUpdated(GetActiveMouseInputSources().begin()->second, (GetActiveMouseInputSources().begin()++)->second);
        break;
    default:
        break;
    }
}

}
