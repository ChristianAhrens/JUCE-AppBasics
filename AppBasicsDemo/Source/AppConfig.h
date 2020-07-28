/*
  ==============================================================================

    AppConfig.h
    Created: 28 Jul 2020 11:21:42am
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include "../../Source/AppConfigurationBase.h"

//==============================================================================
class AppConfig : public JUCEAppBasics::AppConfigurationBase
{
public:
    AppConfig(const File& file);
    ~AppConfig() override;

    //==============================================================================
    bool isValid() override;

private:
    String m_demoTag{ "DemoTagName" };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppConfig)
};