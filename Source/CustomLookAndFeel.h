#pragma once

#include <JuceHeader.h>

#include "ToggleStateSlider.h"
#include "Theme.h"


namespace JUCEAppBasics
{

class CustomLookAndFeel : public juce::LookAndFeel_V4
{

public:
	enum PaletteStyle
	{
		PS_Dark,
		PS_Light
	};

	enum ColourIds
	{
		MeteringPeakColourId	= 0xf000001,
		MeteringRmsColourId		= 0xf000002,
		MeteringHoldColourId	= 0xf000003,

		// colour roles of the shared design foundation; values equal the literals
		// that the apps used before
		StateMuteColourId			= 0xf000010,	///< mute on (fixed red)
		StateWarningColourId		= 0xf000011,	///< load bar warning section
		StateCriticalColourId		= 0xf000012,	///< load bar critical section
		StateAlertColourId			= 0xf000013,	///< network load alert (clipped)
		StateErrorColourId			= 0xf000014,	///< error text
		OverlayShadowColourId		= 0xf000015,	///< call out box shadow
		OverlayBorderColourId		= 0xf000016,	///< call out box border
		SurfaceRecessedDeepColourId	= 0xf000017,	///< deepest surface (audio setup background)
		SurfaceGapColourId			= 0xf000018		///< gaps between strips
	};

public:
	CustomLookAndFeel(PaletteStyle style = PS_Dark);
	~CustomLookAndFeel();

	void setPaletteStyle(PaletteStyle style);
	const PaletteStyle& getPaletteStyle();

	//==============================================================================
	/** Sets the accent colour: the metering colour ids (rms = accent, peak = accent darker in the dark and brighter in the
	 *  light palette). The accent is kept when the palette style changes. Does not notify, see applyAccentColour(). */
	void setAccentColour(const juce::Colour& accent);
	const juce::Colour& getAccentColour() const { return m_accentColour; }

	/** Sets the accent colour of the look and feel and, if it is the default look and feel and the colour changed,
	 *  notifies all components (Desktop::setDefaultLookAndFeel sends the look and feel change). A call with the current
	 *  colour does nothing, so it is safe to call it from lookAndFeelChanged(). Does nothing for other look and feels. */
	static void applyAccentColour(juce::LookAndFeel& laf, const juce::Colour& accent);
	static void applyAccentColour(juce::LookAndFeel& laf, Theme::AccentColour accent) { applyAccentColour(laf, Theme::getAccentColour(accent)); }

	/** Replaces the look and feel in the holder by a new one with the given palette style that keeps the accent colour
	 *  of the previous one, and makes it the default look and feel of the desktop (which notifies all components). */
	static void applyPaletteStyle(std::unique_ptr<CustomLookAndFeel>& holder, PaletteStyle style);

	//==============================================================================
	void drawButtonBackground(juce::Graphics&, juce::Button&, const juce::Colour& backgroundColour,
		bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

	//==============================================================================
	void drawGroupComponentOutline(juce::Graphics&, int w, int h, const juce::String& text,
		const juce::Justification&, juce::GroupComponent&) override;

	//==============================================================================
	void drawTickBox(juce::Graphics&, juce::Component&,
		float x, float y, float w, float h,
		bool ticked, bool isEnabled,
		bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

	//==============================================================================
	void drawComboBox(juce::Graphics&, int width, int height, bool isButtonDown,
		int buttonX, int buttonY, int buttonW, int buttonH,
		juce::ComboBox&) override;

	//==============================================================================
	void drawCallOutBoxBackground(juce::CallOutBox& box, juce::Graphics& g,
		const juce::Path& path, juce::Image& cachedImage) override;

	//==============================================================================
	int getSliderThumbRadius(juce::Slider&) override;
	void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		juce::Slider::SliderStyle style, juce::Slider& slider) override;
	void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
		float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
		juce::Slider& slider) override;

	//==============================================================================
	void setMouseCursor(const juce::MouseCursor& mouseCursor);
	juce::MouseCursor getMouseCursorFor(juce::Component& c) override;

	//==============================================================================
	void drawTreeviewPlusMinusBox(juce::Graphics& g, const juce::Rectangle<float>& area,
		juce::Colour backgroundColour, bool isOpen, bool isMouseOver) override;
	bool areLinesDrawnForTreeView(juce::TreeView&) override;
	int getTreeViewIndentSize(juce::TreeView&) override;

	//==============================================================================
	void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override;

private:
	PaletteStyle m_paletteStyle = PS_Dark;
	juce::Colour m_accentColour = juce::Colours::forestgreen;
	
	juce::MouseCursor m_mouseCursor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomLookAndFeel)
};

}
