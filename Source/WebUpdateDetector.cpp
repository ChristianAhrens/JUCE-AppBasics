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

#include "WebUpdateDetector.h"


namespace JUCEAppBasics
{


//==============================================================================
/**
 * @brief Utility class encapsulating functionality to parse a CHANGELOG.md following
 * semantic versioning and keepachangelog rules and derive from it if it points at
 * that a newer version of <this> tool than currently running could be fetched.
 * Could look like
 * https://raw.githubusercontent.com/ChristianAhrens/Mema/refs/heads/main/CHANGELOG.md
 */
class ChangeLogInfo
{
public:
    struct VersionNumber
    {
        int _Major;
        int _Minor;
        int _Fixlevel;

        VersionNumber() { _Major = 0; _Minor = 0; _Fixlevel = 0; };
        VersionNumber(int major, int minor, int fixlevel) { _Major = major; _Minor = minor; _Fixlevel = fixlevel; };
        VersionNumber(juce::String mmfs) {
            _Major = 0;
            _Minor = 0;
            _Fixlevel = 0;

            auto sl = juce::StringArray();
            if (sl.addTokens(mmfs, ".", "") == 3)
            {
                _Major = sl[0].getIntValue();
                _Minor = sl[1].getIntValue();
                _Fixlevel = sl[2].getIntValue();
            }
        };

        bool operator==(const VersionNumber& rhs) const
        {
            return (_Major == rhs._Major && _Minor == rhs._Minor && _Fixlevel == rhs._Fixlevel);
        };
        bool operator<(const VersionNumber& rhs) const
        {
            if (_Major < rhs._Major 
                || (_Major == rhs._Major && _Minor < rhs._Minor) 
                || (_Major == rhs._Major && _Minor == rhs._Minor && _Fixlevel < rhs._Fixlevel))
            {
                return true;
            }
            else
                return false;
        };
        bool operator>(const VersionNumber& rhs) const
        {
            return (!(*this == rhs) && !(*this < rhs));
        };

        bool IsValid() { return !(_Major == 0 && _Minor == 0 && _Fixlevel == 0); };
        void Clear() { _Major = 0; _Minor = 0; _Fixlevel = 0; };
        const juce::String ToString() const { return juce::StringArray(juce::String(_Major), juce::String(_Minor), juce::String(_Fixlevel)).joinIntoString("."); };
    };
    struct VersionEntry
    {
        // ##[0.2.0] 2024-11-01
        VersionNumber _version;
        juce::String _date;
        // ### Added
        // - Added improved README contents
        // - Added AppVeyor CI for macOS / Windows / Linux
        juce::StringArray _additions;
        // 
        // ### Changed
        // - Changed macOS app Sandbox to on
        // - Improved performance
        juce::StringArray _changes;
        // 
        // ### Fixed
        // - Fixed macOS, iOS provisioning
        // - Fixed typo and UI sizing issues
        juce::StringArray _fixes;

        bool IsEmpty() { return (!_version.IsValid() && _date.isEmpty() && _additions.isEmpty() && _changes.isEmpty() && _fixes.isEmpty()); };
        void Clear() { _version.Clear(); _date.clear(); _additions.clear(); _changes.clear(); _fixes.clear(); };
    };

public:
    ChangeLogInfo() = default;
    virtual ~ChangeLogInfo() = default;

    bool FetchLatestFromUpdateServer(const juce::String& webUpdateServerAddress)
    {
        auto endpoint = juce::String("CHANGELOG.md");

        juce::URL latestVersionURL(webUpdateServerAddress + endpoint);

        std::unique_ptr<InputStream> inStream(
            latestVersionURL.createInputStream(juce::URL::InputStreamOptions(juce::URL::ParameterHandling::inAddress).withConnectionTimeoutMs(5000)));

        if(inStream == nullptr)
            return false;

        auto content = inStream->readEntireStreamAsString();
        
        return ReadVersionEntriesFromChangeLogString(content);
    }

    bool ContainsNewerVersionThan(const VersionNumber& otherVersion)
    {
        jassert(!m_versionEntries.isEmpty());

        for (auto const& versionEntry : m_versionEntries)
            if (versionEntry._version > otherVersion)
                return true;

        return false;
    }

    const VersionNumber GetLatestContainedVersion()
    {
        auto version = VersionNumber();
        for (auto const& versionEntry : m_versionEntries)
            if (versionEntry._version > version)
                version = versionEntry._version;

        return version;
    }

    juce::String GetStringInfoOnVersionsBelow(const VersionNumber& otherVersion)
    {
        jassert(!m_versionEntries.isEmpty());

        juce::StringArray additions;
        juce::StringArray changes;
        juce::StringArray fixes;
        for (auto const& versionEntry : m_versionEntries)
        {
            if (versionEntry._version > otherVersion)
            {
                for (auto const& addInfo : versionEntry._additions)
                    additions.add(addInfo);
                for (auto const& changeInfo : versionEntry._changes)
                    changes.add(changeInfo);
                for (auto const& fixInfo : versionEntry._fixes)
                    fixes.add(fixInfo);
            }
        }

        auto addedString = juce::String(additions.isEmpty() ? juce::String("") : juce::String("Added:\n") + additions.joinIntoString("\n") + juce::String("\n"));
        auto changedString = juce::String(changes.isEmpty() ? juce::String("") : juce::String("Changed:\n") + changes.joinIntoString("\n") + juce::String("\n"));
        auto fixedString = juce::String(fixes.isEmpty() ? juce::String("") : juce::String("Fixed:\n") + fixes.joinIntoString("\n") + juce::String("\n"));

        return addedString + (addedString.isEmpty() ? "" : "\n") + changedString + (changedString.isEmpty() ? "" : "\n") + fixedString;
    }

    const juce::Array<VersionEntry>& GetVersions() { return m_versionEntries; }

private:
    bool ReadVersionEntriesFromChangeLogString(const juce::String& changelogAsString)
    {
        m_versionEntries.clear();

        auto versionEntry = VersionEntry();
        juce::StringArray* currentACFscope = nullptr;

        auto changelogLines = juce::StringArray::fromLines(changelogAsString);

        auto hasValidFirstLine = false;
        if (changelogLines.size() > 0)
        {
            auto& firstLine = changelogLines[0];
            auto goodStart = firstLine.startsWith("# ");
            auto goodEnd = firstLine.endsWith(" Changelog");
            hasValidFirstLine = goodStart && goodEnd;
        }

        for (auto const& changelogLine : changelogLines)
        {
            if (changelogLine.startsWith("## ["))
            {
                if (!versionEntry.IsEmpty())
                    m_versionEntries.add(versionEntry);
                versionEntry.Clear();

                versionEntry._version = {changelogLine.fromFirstOccurrenceOf("[", false, false).upToFirstOccurrenceOf("]", false, false)};
                versionEntry._date = changelogLine.fromLastOccurrenceOf("]", false, false).removeCharacters(" ");
                currentACFscope = nullptr;
            }
            else if (changelogLine.startsWith("### Added"))
            {
                currentACFscope = &versionEntry._additions;
            }
            else if (changelogLine.startsWith("### Changed"))
            {
                currentACFscope = &versionEntry._changes;
            }
            else if (changelogLine.startsWith("### Fixed"))
            {
                currentACFscope = &versionEntry._fixes;
            }
            else if (changelogLine.startsWith("-"))
            {
                if (nullptr != currentACFscope)
                {
                    currentACFscope->add(changelogLine);
                }
                else
                    jassertfalse;
            }
        }

        if (!versionEntry.IsEmpty())
            m_versionEntries.add(versionEntry);

        return hasValidFirstLine && !m_versionEntries.isEmpty();
    };

    juce::Array<VersionEntry> m_versionEntries;

};

//==============================================================================
/**
 * @brief Constructor. Thread base class is initialized but not started here.
 */
WebUpdateDetector::WebUpdateDetector()
    : juce::Thread("WebUpdateDetectorThread")
{
}

/**
 * @brief Destructor. Base class thread is stopped and singleton cleaned up.
 */
WebUpdateDetector::~WebUpdateDetector()
{
    stopThread(5000);
    clearSingletonInstance();
}

/**
 * @brief Sets the internal member m_referenceVersion value.
 *
 * @param referenceVersion String defining the current version in semantic versioning format major.minor.fixlevel.
 */
void WebUpdateDetector::SetReferenceVersion(const juce::String& referenceVersion)
{
    m_referenceVersion = referenceVersion;
}

/**
 * @brief Sets the internal member m_downloadUpdateWebAddress value.
 *
 * @param downloadUpdateWebAddress String defining the web address to offer to the user as download location for new versions.
 */
void WebUpdateDetector::SetDownloadUpdateWebAddress(const juce::String& downloadUpdateWebAddress)
{
    m_downloadUpdateWebAddress = downloadUpdateWebAddress;
}

/**
 * @brief Method to start the base class thread for version checking.
 *
 * @details Calling this with a new webUpdateServerAddress when already started before results in an assert.
 * This is due to the running thread not being thread-secured for runtime modification.
 * @param   isBackgroundCheck  Flag to set if the check should round in background mode (not verbose - no info popups)
 * @param   webUpdateServerAddress  The address of the webserver to use to check for updates.
 */
void WebUpdateDetector::CheckForNewVersion(bool isBackgroundCheck, const juce::String& webUpdateServerAddress)
{
    if(!isThreadRunning())
    {
        m_webUpdateServerAddress = webUpdateServerAddress;
        m_backgroundCheck        = isBackgroundCheck;
        startThread(Priority::low);
    }
    else
        jassert(webUpdateServerAddress.isEmpty()); // restarting the check with new webupdate address while thread is already running is not supported!
}

/**
 * @brief Thread callback function for webserver version checking and notification
 *
 * @details Reimplemented thread callback function that cyclically checks the webserver
 * for availability of new versions, downloads asset information and prompts
 * the user with a notification on new versions if any were found.
 */
void WebUpdateDetector::run()
{
    auto info = std::make_unique<ChangeLogInfo>();
    if (!info->FetchLatestFromUpdateServer(m_webUpdateServerAddress))
    {
        auto failureInfo = juce::String("Failed to communicate with the server providing update information.\nPlease try again in a few minutes.");
        std::cout << __FUNCTION__ << " " << failureInfo.toStdString() << std::endl;
        if(!m_backgroundCheck)
            juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon, "Update Server Communication Error", failureInfo);
        return;
    }

    if(!info->ContainsNewerVersionThan(ChangeLogInfo::VersionNumber(m_referenceVersion)))
    {
        auto noUpdateInfo = juce::String("Your " + juce::String(ProjectInfo::projectName) + " version is up to date.");
        std::cout << __FUNCTION__ << " " << noUpdateInfo.toStdString() << std::endl;
        if(!m_backgroundCheck)
            juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::NoIcon, "No New Version Available", noUpdateInfo);
        return;
    }
    else
    {
        auto availableUpdateInfo = juce::String("A new version " + info->GetLatestContainedVersion().ToString() + " of " + juce::String(ProjectInfo::projectName) + " is available.");
        availableUpdateInfo << "\n\n" << info->GetStringInfoOnVersionsBelow(m_referenceVersion);
        std::cout << __FUNCTION__ << " " << availableUpdateInfo.toStdString() << std::endl;
        auto options = juce::MessageBoxOptions()
            .withIconType(juce::MessageBoxIconType::InfoIcon)
            .withTitle("New Version Available")
            .withMessage(availableUpdateInfo)
            .withButton("Download")
            .withButton("Ignore");
        juce::AlertWindow::showAsync(options, [=](int buttonIndex) {
            switch (buttonIndex)
            {
            case 1:
                juce::URL(m_downloadUpdateWebAddress).launchInDefaultBrowser();
                break;
            case 0:
            default:
                break;
            }
        });
        return;
    }
}

/**
 * @brief Sets whether the checkbox in the popup dialog should be shown as checked or not.
 *
 * @details This function determines whether the checkbox in the popup dialog should be shown as checked or unchecked.
 * @param checkForUpdates Boolean value indicating whether to check for updates at program start.
 */
void WebUpdateDetector::SetToggleStateForUpdatesPopup(const bool checkForUpdates)
{
    m_checkForUpdatesAtProgramStart = checkForUpdates;
}

//==============================================================================
JUCE_IMPLEMENT_SINGLETON(WebUpdateDetector)


} // namespace JUCEAppBasics
