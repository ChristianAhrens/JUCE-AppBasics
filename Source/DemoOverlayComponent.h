/*
  ==============================================================================

    DemoOverlayComponent.h
    Created: 11 Jul 2020 10:23:17am
    Author:  musah

  ==============================================================================
*/

#pragma once

#include "OverlayToggleComponentBase.h"

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
    void changeState() override;

private:
    std::unique_ptr<Label>  m_stateLabel;

};