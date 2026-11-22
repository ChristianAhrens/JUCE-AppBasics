#pragma once

#include <JuceHeader.h>

#include "AppConfigurationBase.h"
#include "CustomLookAndFeel.h"

namespace JUCEAppBasics
{

/**
 * Start-up scaffold of the client apps (Mema.Mo, Mema.Re, Umsci): the application with its tooltip window and the main
 * window that owns the global CustomLookAndFeel, follows the dark mode setting of the operating system and hands the
 * palette and fullscreen requests of the main component to the window.
 *
 * The traits type of the app provides:
 * - `using MainComponent = ...;` with the members `onPaletteStyleChange` (`std::function<void(int, bool)>`),
 *   `onSetFullscreenWindow` (`std::function<void(bool)>`) and `applySettingsOption(...)`.
 * - `static void activateDarkPalette(MainComponent&)` that selects the dark look and feel through the settings menu infrastructure.
 * - `static constexpr bool paletteChangeFollowsSystem` true if a palette change request while "follow" is active re-evaluates
 *   the operating system dark mode setting instead of applying the requested style (Umsci).
 * - optionally `static constexpr bool hasResizeLimits` with `minWidth`, `minHeight`, `maxWidth`, `maxHeight`.
 * - optionally `static void onShutdown()` called when the application shuts down.
 */
template<typename Traits>
class ClientApplication : public juce::JUCEApplication
{
public:
    ClientApplication() {}

    const juce::String getApplicationName() override { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override { return true; }

    //==============================================================================
    void initialise(const juce::String& commandLine) override
    {
        m_mainWindow.reset(std::make_unique<MainWindow>(getApplicationName(), commandLine).release());

#if JUCE_MAC
        // Ignore SIGPIPE globally, to prevent occasional unexpected app termination when the peer disconnects while
        // sending by writing to socket is ongoing
        signal(SIGPIPE, SIG_IGN);
#endif

        // a single instance of tooltip window is required and used by JUCE everywhere a tooltip is required.
        m_toolTipWindowInstance = std::make_unique<juce::TooltipWindow>();
    }

    void shutdown() override
    {
        callOnShutdown<Traits>(0);
        m_mainWindow.reset();
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted(const juce::String& commandLine) override
    {
        juce::ignoreUnused(commandLine);
    }

    //==============================================================================
    /** The desktop window that contains an instance of the main component. */
    class MainWindow : public juce::DocumentWindow, juce::DarkModeSettingListener
    {
    public:
        MainWindow(const juce::String& name, const juce::String& commandLine) : juce::DocumentWindow(name,
            juce::Desktop::getInstance().getDefaultLookAndFeel()
            .findColour(juce::ResizableWindow::backgroundColourId),
            juce::DocumentWindow::allButtons)
        {
            juce::ignoreUnused(commandLine);

            setUsingNativeTitleBar(true);
            auto mainComponent = std::make_unique<typename Traits::MainComponent>();
            mainComponent->onPaletteStyleChange = [=](int paletteStyle, bool followLocalStyle) {
                m_followLocalStyle = followLocalStyle;
                if (Traits::paletteChangeFollowsSystem && m_followLocalStyle)
                    darkModeSettingChanged();
                else
                    applyPaletteStyle(static_cast<CustomLookAndFeel::PaletteStyle>(paletteStyle));
            };
            mainComponent->onSetFullscreenWindow = [=](bool fullscreenWindow) { setFullscreenWindow(fullscreenWindow); };
            setContentOwned(mainComponent.release(), true);

#if JUCE_IOS || JUCE_ANDROID
            setFullScreen(true);
            juce::Desktop::getInstance().setScreenSaverEnabled(false);
#elif JUCE_LINUX
            juce::Desktop::getInstance().setKioskModeComponent(getTopLevelComponent(), false);
#else
            setResizable(true, true);
            applyResizeLimits<Traits>(0);
            centreWithSize(getWidth(), getHeight());
#endif

            setVisible(true);

            juce::Desktop::getInstance().addDarkModeSettingListener(this);
            darkModeSettingChanged(); // initially trigger correct colourscheme

            // use the settings menu item call infrastructure to activate dark mode per default
            if (auto mc = dynamic_cast<typename Traits::MainComponent*>(getContentComponent()))
                Traits::activateDarkPalette(*mc);

            AppConfigurationBase::getInstance()->triggerWatcherUpdate();
        }

        void closeButtonPressed() override
        {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }

        void darkModeSettingChanged() override
        {
            if (!m_followLocalStyle)
                return;

            if (juce::Desktop::getInstance().isDarkModeActive())
                applyPaletteStyle(CustomLookAndFeel::PS_Dark); // go dark
            else
                applyPaletteStyle(CustomLookAndFeel::PS_Light); // go light

            lookAndFeelChanged();
        }

        void applyPaletteStyle(CustomLookAndFeel::PaletteStyle paletteStyle)
        {
            CustomLookAndFeel::applyPaletteStyle(m_lookAndFeel, paletteStyle);
        }

        void setFullscreenWindow(bool fullscreenWindow)
        {
#if JUCE_WINDOWS
            if (fullscreenWindow)
                juce::Desktop::getInstance().setKioskModeComponent(getTopLevelComponent(), false);
            else
                juce::Desktop::getInstance().setKioskModeComponent(nullptr, false);
#elif JUCE_MAC
            if (auto* topLevel = getTopLevelComponent())
            {
                if (auto* peer = topLevel->getPeer())
                {
                    peer->setFullScreen(fullscreenWindow);
                    return;
                }
            }
            jassertfalse;
#else
            juce::ignoreUnused(fullscreenWindow);
#endif
        }

    private:
        // optional traits members, selected by overload resolution (the int argument prefers the template that exists)
        template<typename T> auto applyResizeLimits(int) -> decltype(T::hasResizeLimits, void())
        {
            if (T::hasResizeLimits)
                setResizeLimits(T::minWidth, T::minHeight, T::maxWidth, T::maxHeight);
        }
        template<typename T> void applyResizeLimits(long) {}

        std::unique_ptr<CustomLookAndFeel>  m_lookAndFeel;
        bool m_followLocalStyle = true;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

private:
    template<typename T> auto callOnShutdown(int) -> decltype(T::onShutdown(), void())
    {
        T::onShutdown();
    }
    template<typename T> void callOnShutdown(long) {}

    std::unique_ptr<MainWindow>             m_mainWindow;
    std::unique_ptr<juce::TooltipWindow>    m_toolTipWindowInstance;
};

}
