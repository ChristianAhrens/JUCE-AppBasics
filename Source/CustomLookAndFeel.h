#pragma once

#include <JuceHeader.h>

namespace JUCEAppBasics
{

class CustomLookAndFeel : public LookAndFeel_V4
{

public:
	enum PaletteStyle
	{
		PS_Dark,
		PS_Light
	};

public:
	CustomLookAndFeel(PaletteStyle style = PS_Dark);
	~CustomLookAndFeel();

	void setPaletteStyle(PaletteStyle style);

	//==============================================================================
	void drawButtonBackground(Graphics&, Button&, const Colour& backgroundColour,
		bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

	//==============================================================================
	void drawGroupComponentOutline(Graphics&, int w, int h, const String& text,
		const Justification&, GroupComponent&) override;

	//==============================================================================
	void drawTickBox(Graphics&, Component&,
		float x, float y, float w, float h,
		bool ticked, bool isEnabled,
		bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

	//==============================================================================
	void drawComboBox(Graphics&, int width, int height, bool isButtonDown,
		int buttonX, int buttonY, int buttonW, int buttonH,
		ComboBox&) override;

	//==============================================================================
	void drawCallOutBoxBackground(CallOutBox& box, Graphics& g,
		const Path& path, Image& cachedImage) override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomLookAndFeel)
};

}
