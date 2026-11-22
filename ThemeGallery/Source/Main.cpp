#include <JuceHeader.h>

#include "ThemeGalleryComponent.h"

//==============================================================================
/**
 * Small host application for the ThemeGalleryComponent: shows the building blocks of the shared design
 * foundation (JUCEAppBasics::CustomLookAndFeel) in the dark and the light palette side by side.
 */
class ThemeGalleryApplication : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "ThemeGallery"; }
    const juce::String getApplicationVersion() override { return "0.1.0"; }
    bool moreThanOneInstanceAllowed() override { return true; }

    void initialise(const juce::String&) override
    {
        m_mainWindow = std::make_unique<MainWindow>(getApplicationName());
    }

    void shutdown() override { m_mainWindow.reset(); }
    void systemRequestedQuit() override { quit(); }

private:
    class MainWindow : public juce::DocumentWindow
    {
    public:
        explicit MainWindow(juce::String name) : DocumentWindow(name, juce::Colours::darkgrey, DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new JUCEAppBasics::ThemeGalleryComponent(), true);
            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
            setVisible(true);
        }

        void closeButtonPressed() override { juce::JUCEApplication::getInstance()->systemRequestedQuit(); }
    };

    std::unique_ptr<MainWindow> m_mainWindow;
};

START_JUCE_APPLICATION(ThemeGalleryApplication)
