/* Copyright (c) 2024, Christian Ahrens
 *
 * This file is part of JUCEAppBasics <https://github.com/ChristianAhrens/JUCE-AppBasics>
 *
 * This module is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This module is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this tool; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#pragma once

#include <JuceHeader.h>


namespace JUCEAppBasics
{


/**
 * @brief Singleton class providing update detection and user information functionality.
 */
class WebUpdateDetector : public juce::DeletedAtShutdown, private juce::Thread
{
public:
    //==============================================================================
    WebUpdateDetector();
    ~WebUpdateDetector() override;

    void SetReferenceVersion(const juce::String& referenceVersion);
    void SetDownloadUpdateWebAddress(const juce::String& downloadUpdateWebAddress);
    void CheckForNewVersion(bool isBackgroundCheck, const juce::String& webUpdateServerAddress);
    void SetToggleStateForUpdatesPopup(const bool checkForUpdates);

    //==============================================================================
    std::function<void(bool)> onCheckAtProgramStartChangedCallback;

    JUCE_DECLARE_SINGLETON_SINGLETHREADED_MINIMAL(WebUpdateDetector)

private:
    //==============================================================================
    void run() override;

    //==============================================================================
    bool                                m_backgroundCheck{ false };
    juce::String                        m_webUpdateServerAddress;
    std::unique_ptr<juce::Component>    m_dialogWindow;
    bool                                m_checkForUpdatesAtProgramStart{true};
    juce::String                        m_referenceVersion;
    juce::String                        m_downloadUpdateWebAddress;

    JUCE_DECLARE_WEAK_REFERENCEABLE(WebUpdateDetector)
};


} // namespace JUCEAppBasics
