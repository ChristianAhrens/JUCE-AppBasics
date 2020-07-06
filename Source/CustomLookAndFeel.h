#pragma once

#include <JuceHeader.h>

namespace JUCEAppBasics
{

class CustomLookAndFeel : public LookAndFeel_V4
{

public:
	CustomLookAndFeel();
	~CustomLookAndFeel();

	//==============================================================================
	void drawButtonBackground(Graphics&, Button&, const Colour& backgroundColour,
		bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomLookAndFeel)
};

}