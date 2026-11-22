#pragma once

#include <JuceHeader.h>

#include <CustomLookAndFeel.h>

namespace JUCEAppBasics
{

/**
 * Shows the building blocks of the shared design foundation (without the list box and the vertical scroll bar, see README.md) in one
 * palette. The panel owns its own CustomLookAndFeel, so two panels can show dark and light side by side.
 */
class ThemeGalleryPanel : public juce::Component
{
public:
    ThemeGalleryPanel(CustomLookAndFeel::PaletteStyle style, const juce::String& title);
    ~ThemeGalleryPanel() override;

    /** Applies the accent colour the same way the apps do: via the metering colour ids of the look and feel. */
    void setAccentColour(const juce::Colour& accent);

    void resized() override;
    void paint(juce::Graphics& g) override;

    /** @return The height the content needs for the current width. */
    int getRequiredHeight() const;

private:
    struct Row;

    void addRow(const juce::String& title, std::vector<juce::Component*> components, int height = 34, int componentWidth = 110);
    template<typename T, typename... Args> T* own(Args&&... args)
    {
        auto item = std::make_unique<T>(std::forward<Args>(args)...);
        auto* raw = item.get();
        m_owned.push_back(std::move(item));
        addAndMakeVisible(raw);
        return raw;
    }

    CustomLookAndFeel::PaletteStyle                     m_style;
    CustomLookAndFeel                                   m_lookAndFeel;
    juce::String                                        m_title;
    juce::Colour                                        m_accent = juce::Colours::forestgreen;
    std::vector<std::unique_ptr<juce::Component>>       m_owned;
    std::vector<std::unique_ptr<Row>>                   m_rows;
    std::vector<juce::Slider*>                          m_accentSliders;
    std::unique_ptr<juce::TooltipWindow>                m_tooltipWindow;
    std::unique_ptr<juce::TreeViewItem>                 m_treeRoot;
};

/**
 * Dark and light panel side by side with an accent selector on top.
 */
class ThemeGalleryComponent : public juce::Component
{
public:
    ThemeGalleryComponent();
    ~ThemeGalleryComponent() override;

    void resized() override;

private:
    juce::ComboBox      m_accentSelector;
    ThemeGalleryPanel   m_darkPanel;
    ThemeGalleryPanel   m_lightPanel;
    juce::Viewport      m_darkViewport;
    juce::Viewport      m_lightViewport;
};

}
