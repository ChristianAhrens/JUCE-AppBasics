#pragma once

#include <JuceHeader.h>

#include <ToggleStateSlider.h>


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
		MeteringHoldColourId	= 0xf000003
	};

public:
	CustomLookAndFeel(PaletteStyle style = PS_Dark);
	~CustomLookAndFeel();

	void setPaletteStyle(PaletteStyle style);
	const PaletteStyle& getPaletteStyle();

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
	void drawCallOutBoxBackground(juce::CallOutBox& box, Graphics& g,
		const juce::Path& path, juce::Image& cachedImage) override;

	//==============================================================================
	int getSliderThumbRadius(juce::Slider&) override;
	void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		juce::Slider::SliderStyle style, juce::Slider& slider) override;

	//==============================================================================
	void setMouseCursor(const juce::MouseCursor& mouseCursor);
	juce::MouseCursor getMouseCursorFor(juce::Component& c) override;

	//==============================================================================
	void drawTreeviewPlusMinusBox(Graphics& g, const Rectangle<float>& area,
		Colour backgroundColour, bool isOpen, bool isMouseOver) override;
	bool areLinesDrawnForTreeView(TreeView&) override;
	int getTreeViewIndentSize(TreeView&) override;

	//==============================================================================
	void drawPopupMenuBackground(Graphics& g, int width, int height) override;

private:
	PaletteStyle m_paletteStyle = PS_Dark;
	
	juce::MouseCursor m_mouseCursor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomLookAndFeel)
};

}
