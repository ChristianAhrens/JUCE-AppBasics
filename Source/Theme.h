#pragma once

#include <JuceHeader.h>

#include <optional>

namespace JUCEAppBasics
{

/**
 * Shared design foundation tokens: the accent colours, alpha, size and type
 * values that the apps used to define on their own. Header only; the colour roles of the palettes live in
 * CustomLookAndFeel (see CustomLookAndFeel::ColourIds).
 */
namespace Theme
{

//==============================================================================
/** The five accent colours, the same in every app. */
enum class AccentColour
{
    Green,  ///< forestgreen, the default
    Red,    ///< orangered
    Blue,   ///< dodgerblue
    Pink,   ///< deeppink ("Anni Pink")
    Laser   ///< high visibility laser green
};

inline constexpr int numAccentColours = 5;

inline AccentColour getAccentColourByIndex(int index)
{
    return static_cast<AccentColour>(juce::jlimit(0, numAccentColours - 1, index));
}

inline juce::Colour getAccentColour(AccentColour accent)
{
    switch (accent)
    {
    case AccentColour::Red:     return juce::Colours::orangered;
    case AccentColour::Blue:    return juce::Colours::dodgerblue;
    case AccentColour::Pink:    return juce::Colours::deeppink;
    case AccentColour::Laser:   return juce::Colour(0xd1, 0xff, 0x4f);
    case AccentColour::Green:
    default:                    return juce::Colours::forestgreen;
    }
}

/** Stable name used for persistence (configuration files). */
inline juce::String getAccentName(AccentColour accent)
{
    switch (accent)
    {
    case AccentColour::Red:     return "Red";
    case AccentColour::Blue:    return "Blue";
    case AccentColour::Pink:    return "Pink";
    case AccentColour::Laser:   return "Laser";
    case AccentColour::Green:
    default:                    return "Green";
    }
}

/** Name shown in the settings menus. */
inline juce::String getAccentMenuName(AccentColour accent)
{
    return AccentColour::Pink == accent ? juce::String("Anni Pink") : getAccentName(accent);
}

inline std::optional<AccentColour> findAccentByName(const juce::String& name)
{
    for (auto i = 0; i < numAccentColours; ++i)
        if (getAccentName(getAccentColourByIndex(i)) == name)
            return getAccentColourByIndex(i);
    return std::nullopt;
}

inline std::optional<AccentColour> findAccentByColour(const juce::Colour& colour)
{
    for (auto i = 0; i < numAccentColours; ++i)
        if (getAccentColour(getAccentColourByIndex(i)) == colour)
            return getAccentColourByIndex(i);
    return std::nullopt;
}

//==============================================================================
/** Opacity tokens (values of the literals that were spread over the code). */
namespace Alpha
{
inline constexpr float disabled     = 0.5f;     ///< disabled buttons (drawButtonBackground)
inline constexpr float panel        = 0.93f;    ///< panel and divider background
inline constexpr float separator    = 0.35f;    ///< separator line between content and grab strip
inline constexpr float grip         = 0.75f;    ///< grab pills
inline constexpr float emptyText    = 0.45f;    ///< hint texts of empty panels
inline constexpr float flashDim     = 0.25f;    ///< dim phase of blinking elements
}

//==============================================================================
/** Size tokens in pixels. */
namespace Size
{
inline constexpr int toolbarIcon    = 30;       ///< toolbar button edge length (Mema.Mo, Mema.Re, Umsci)
inline constexpr int toolbarIconCompact = 26;   ///< toolbar button edge length of the Mema menu bar UI
inline constexpr int toolbarRow     = 35;       ///< toolbar row height of the client apps
inline constexpr int iconInset      = 3;        ///< JUCE DrawableButton edge indent
inline constexpr int hairline       = 1;        ///< borders and gaps
inline constexpr int strongLine     = 2;        ///< strong lines (combo arrow, call out box border)
inline constexpr int calloutRadius  = 8;        ///< call out box corner radius
inline constexpr int treeIndent     = 15;       ///< tree view indent
}

//==============================================================================
/** Type size tokens (the effective sizes of the JUCE defaults that the apps rely on). */
namespace Type
{
inline constexpr float visuCaption  = 9.0f;     ///< small captions of the visualisations (meter ranges, field legends)
inline constexpr float compact      = 13.0f;    ///< compact bold dialog section captions
inline constexpr float visuLegend   = 12.0f;    ///< legends of the spectrum visualisation
inline constexpr float body         = 14.0f;    ///< text editor, label, tooltip, visualisation labels
inline constexpr float menu         = 17.0f;    ///< popup menu
inline constexpr float dialogBody   = 16.0f;    ///< alert window text
inline constexpr float dialogTitle  = 18.0f;    ///< alert window title (bold)
inline constexpr float heading      = 25.0f;    ///< plug-in name heading
}

}

}
