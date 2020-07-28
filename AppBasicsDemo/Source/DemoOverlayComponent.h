/*
  ==============================================================================

    DemoOverlayComponent.h
    Created: 11 Jul 2020 10:23:17am
    Author:  musah

  ==============================================================================
*/

#pragma once

#include "../../Source/OverlayToggleComponentBase.h"

//==============================================================================
class DemoOverlayComponent : public JUCEAppBasics::OverlayToggleComponentBase
{
public:
    DemoOverlayComponent();
    ~DemoOverlayComponent();

    //==============================================================================
    void resized() override;

    //==============================================================================
    String getCurrentStateAsText();

protected:
    void changeOverlayState() override;

private:
    std::unique_ptr<Label>  m_stateLabel;

};