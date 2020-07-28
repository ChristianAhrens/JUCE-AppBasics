/*
  ==============================================================================

    AppConfig.cpp
    Created: 28 Jul 2020 11:21:42am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "AppConfig.h"

AppConfig::AppConfig(const File& file)
    : JUCEAppBasics::AppConfigurationBase(file)
{
}

AppConfig::~AppConfig()
{

}

bool AppConfig::isValid()
{
    auto valid = JUCEAppBasics::AppConfigurationBase::isValid();

    XmlElement* demoSectionElement = m_xml->getChildByName(m_demoTag);
    valid &= (demoSectionElement != nullptr);

    return valid;
}