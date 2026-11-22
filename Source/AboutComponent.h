#pragma once

#include <JuceHeader.h>

namespace JUCEAppBasics
{

/**
 * @class AboutComponent
 * @brief Small overlay panel showing the application icon, version info, and a
 *        link to the project repository.
 *
 * Embedded inside a `CustomAboutItem` and shown as a custom item inside a
 * JUCE PopupMenu, triggered by the about button of the app.
 */
class AboutComponent : public juce::Component
{
public:
    /**
     * @param imageData     Raw bytes of the app icon image (SVG or PNG, compiled into the binary as a JUCE BinaryData resource).
     * @param imageDataSize Byte count of `imageData`.
     * @param repoUrl       Address the link to the project page opens.
     */
    AboutComponent(const char* imageData, int imageDataSize, const juce::String& repoUrl);
    ~AboutComponent() override;

    //========================================================================*
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    std::unique_ptr<juce::DrawableButton>   m_appIcon;      ///< App icon rendered as a drawable.
    std::unique_ptr<juce::Label>            m_appInfoLabel; ///< Version and build info text.
    std::unique_ptr<juce::HyperlinkButton>  m_appRepoLink;  ///< Clickable link to the project page.

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AboutComponent)
};

}
