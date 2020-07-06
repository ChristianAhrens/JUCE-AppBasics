#include "CustomLookAndFeel.h"

namespace JUCEAppBasics
{

CustomLookAndFeel::CustomLookAndFeel()
{
	setColour(ColourScheme::windowBackground, Colours::darkgrey);
	setColour(ColourScheme::widgetBackground, Colours::grey);
	setColour(ColourScheme::menuBackground, Colours::darkgrey.darker());
	setColour(ColourScheme::outline, Colours::lightgrey);
	setColour(ColourScheme::defaultText, Colours::lightgrey);
	setColour(ColourScheme::defaultFill, Colours::grey);
	setColour(ColourScheme::highlightedText, Colours::white);
	setColour(ColourScheme::highlightedFill, Colours::grey);
	setColour(ColourScheme::menuText, Colours::dimgrey);

	setColour(ResizableWindow::ColourIds::backgroundColourId, Colours::darkgrey);

	setColour(TextEditor::ColourIds::backgroundColourId, Colours::darkgrey);
	setColour(TextEditor::ColourIds::textColourId, Colours::dimgrey);
	setColour(TextEditor::ColourIds::highlightColourId, Colours::white);
	setColour(TextEditor::ColourIds::highlightedTextColourId, Colours::lightgrey);
	setColour(TextEditor::ColourIds::outlineColourId, Colours::lightgrey);
	setColour(TextEditor::ColourIds::focusedOutlineColourId, Colours::lightgrey);
	setColour(TextEditor::ColourIds::shadowColourId, Colours::grey);

	setColour(ComboBox::ColourIds::arrowColourId, Colours::lightgrey);
	setColour(ComboBox::ColourIds::backgroundColourId, Colours::darkgrey);
	setColour(ComboBox::ColourIds::buttonColourId, Colours::grey);
	setColour(ComboBox::ColourIds::focusedOutlineColourId, Colours::darkgrey);
	setColour(ComboBox::ColourIds::outlineColourId, Colours::lightgrey);
	setColour(ComboBox::ColourIds::textColourId, Colours::white);
    
    setColour(PopupMenu::ColourIds::backgroundColourId, Colours::darkgrey);
    setColour(PopupMenu::ColourIds::textColourId, Colours::white);
    setColour(PopupMenu::ColourIds::headerTextColourId, Colours::lightgrey);
    setColour(PopupMenu::ColourIds::highlightedBackgroundColourId, Colours::white);
    setColour(PopupMenu::ColourIds::highlightedTextColourId, Colours::lightgrey);

	setColour(TextButton::ColourIds::buttonColourId, Colours::darkgrey.brighter());
	setColour(TextButton::ColourIds::buttonOnColourId, Colours::lightgrey);
	setColour(TextButton::ColourIds::textColourOffId, Colours::white);
	setColour(TextButton::ColourIds::textColourOnId, Colours::white);

	setColour(DrawableButton::ColourIds::backgroundColourId, Colours::darkgrey);
	setColour(DrawableButton::ColourIds::backgroundOnColourId, Colours::darkgrey);
    
    setColour(ListBox::ColourIds::backgroundColourId, Colours::darkgrey);
    setColour(ListBox::ColourIds::outlineColourId, Colours::lightgrey);
    setColour(ListBox::ColourIds::textColourId, Colours::white);
}

CustomLookAndFeel::~CustomLookAndFeel()
{
}

void CustomLookAndFeel::drawButtonBackground(Graphics& g,
    Button& button,
    const Colour& backgroundColour,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);

    auto baseColour = backgroundColour.withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.3f : 0.9f)
        .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f);

    if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
        baseColour = baseColour.contrasting(shouldDrawButtonAsDown ? 0.2f : 0.05f);

    g.setColour(baseColour);
	g.fillRoundedRectangle(bounds, 0);
}

}