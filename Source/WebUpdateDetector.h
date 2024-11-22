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

    /**
     * @brief Constructor. Thread base class is initialized but not started here.
     */
    WebUpdateDetector();

    /**
     * @brief Destructor. Base class thread is stopped and singleton cleaned up.
     */
    ~WebUpdateDetector() override;

    /**
     * @brief Sets the internal member m_referenceVersion value.
     *
     * @param referenceVersion String defining the current version in semantic versioning format major.minor.fixlevel.
     */
    void SetReferenceVersion(const juce::String& referenceVersion);

    /**
     * @brief Sets the internal member m_downloadUpdateWebAddress value.
     *
     * @param downloadUpdateWebAddress String defining the web address to offer to the user as download location for new versions.
     */
    void SetDownloadUpdateWebAddress(const juce::String& downloadUpdateWebAddress);

    /**
     * @brief Method to start the base class thread for version checking. 
     *
     * @details Calling this with a new webUpdateServerAddress when already started before results in an assert.
     * This is due to the running thread not being thread-secured for runtime modification.
     * @param   isBackgroundCheck  Flag to set if the check should round in background mode (not verbose - no info popups)
     * @param   webUpdateServerAddress  The address of the webserver to use to check for updates.
     */
    void CheckForNewVersion(bool isBackgroundCheck, const juce::String& webUpdateServerAddress);

    //==============================================================================
    std::function<void(bool)> onCheckAtProgramStartChangedCallback;

    /**
     * @brief Sets whether the checkbox in the popup dialog should be shown as checked or not.
     * 
     * @details This function determines whether the checkbox in the popup dialog should be shown as checked or unchecked.
     * @param checkForUpdates Boolean value indicating whether to check for updates at program start.
     */
    void SetToggleStateForUpdatesPopup(const bool checkForUpdates);


    JUCE_DECLARE_SINGLETON_SINGLETHREADED_MINIMAL(WebUpdateDetector)

private:
    /**
     * @brief Thread callback function for webserver version checking and notification
     * 
     * @details Reimplemented thread callback function that cyclically checks the webserver
     * for availability of new versions, downloads asset information and prompts
     * the user with a notification on new versions if any were found.
     */
    void run() override;

    //==============================================================================
    bool m_backgroundCheck{ false };

    juce::String                        m_webUpdateServerAddress;
    std::unique_ptr<juce::Component>    m_dialogWindow;
    bool                                m_checkForUpdatesAtProgramStart{true};
    juce::String                        m_referenceVersion;
    juce::String                        m_downloadUpdateWebAddress;

    JUCE_DECLARE_WEAK_REFERENCEABLE(WebUpdateDetector)
};


} // namespace JUCEAppBasics
