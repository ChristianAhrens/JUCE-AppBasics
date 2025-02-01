#include "CustomLookAndFeel.h"

namespace JUCEAppBasics
{

CustomLookAndFeel::CustomLookAndFeel(PaletteStyle style)
{
    setPaletteStyle(style);
}

void CustomLookAndFeel::setPaletteStyle(PaletteStyle style)
{
    switch (style)
    {
    case PS_Light:
        setColour(ColourScheme::windowBackground, Colours::lightgrey.brighter());
        setColour(ColourScheme::widgetBackground, Colours::grey.brighter());
        setColour(ColourScheme::menuBackground, Colours::lightgrey.brighter().brighter());
        setColour(ColourScheme::outline, Colours::darkgrey);
        setColour(ColourScheme::defaultText, Colours::darkgrey);
        setColour(ColourScheme::defaultFill, Colours::grey.brighter());
        setColour(ColourScheme::highlightedText, Colours::black);
        setColour(ColourScheme::highlightedFill, Colours::grey.brighter());
        setColour(ColourScheme::menuText, Colours::slategrey);

        setColour(ResizableWindow::ColourIds::backgroundColourId, Colours::lightgrey.brighter().brighter());

        setColour(AlertWindow::ColourIds::backgroundColourId, Colours::lightgrey.brighter());
        setColour(AlertWindow::ColourIds::outlineColourId, Colours::darkgrey);
        setColour(AlertWindow::ColourIds::textColourId, Colours::darkgrey);

        setColour(TextEditor::ColourIds::backgroundColourId, Colours::lightgrey.brighter());
        setColour(TextEditor::ColourIds::textColourId, Colours::black);
        setColour(TextEditor::ColourIds::highlightColourId, Colours::slategrey.brighter());
        setColour(TextEditor::ColourIds::highlightedTextColourId, Colours::darkgrey);
        setColour(TextEditor::ColourIds::outlineColourId, Colours::darkgrey);
        setColour(TextEditor::ColourIds::focusedOutlineColourId, Colours::darkgrey);
        setColour(TextEditor::ColourIds::shadowColourId, Colours::grey.brighter());

        setColour(ComboBox::ColourIds::arrowColourId, Colours::darkgrey);
        setColour(ComboBox::ColourIds::backgroundColourId, Colours::lightgrey.brighter());
        setColour(ComboBox::ColourIds::buttonColourId, Colours::grey.brighter());
        setColour(ComboBox::ColourIds::focusedOutlineColourId, Colours::lightgrey);
        setColour(ComboBox::ColourIds::outlineColourId, Colours::darkgrey);
        setColour(ComboBox::ColourIds::textColourId, Colours::black);

        setColour(PopupMenu::ColourIds::backgroundColourId, Colours::lightgrey.brighter());
        setColour(PopupMenu::ColourIds::textColourId, Colours::black);
        setColour(PopupMenu::ColourIds::headerTextColourId, Colours::darkgrey);
        setColour(PopupMenu::ColourIds::highlightedBackgroundColourId, Colours::black.brighter());
        setColour(PopupMenu::ColourIds::highlightedTextColourId, Colours::darkgrey);

        setColour(TextButton::ColourIds::buttonColourId, Colours::grey.brighter()); // this applies for DrawableButton as well
        setColour(TextButton::ColourIds::buttonOnColourId, Colours::darkgrey.brighter()); // this applies for DrawableButton as well
        setColour(TextButton::ColourIds::textColourOffId, Colours::black);
        setColour(TextButton::ColourIds::textColourOnId, Colours::black);

        setColour(DrawableButton::ColourIds::textColourId, Colours::black);
        setColour(DrawableButton::ColourIds::textColourOnId, Colours::black);
        setColour(DrawableButton::ColourIds::backgroundColourId, Colours::lightgrey.brighter());
        setColour(DrawableButton::ColourIds::backgroundOnColourId, Colours::lightgrey.brighter());

        setColour(ListBox::ColourIds::backgroundColourId, Colours::lightgrey.brighter());
        setColour(ListBox::ColourIds::outlineColourId, Colours::darkgrey);
        setColour(ListBox::ColourIds::textColourId, Colours::black);

        setColour(TableHeaderComponent::ColourIds::textColourId, Colours::black);
        setColour(TableHeaderComponent::ColourIds::backgroundColourId, Colours::lightgrey.brighter());
        setColour(TableHeaderComponent::ColourIds::outlineColourId, Colours::grey);
        setColour(TableHeaderComponent::ColourIds::highlightColourId, Colours::grey.brighter());

        setColour(ScrollBar::ColourIds::backgroundColourId, Colours::lightgrey.brighter());
        setColour(ScrollBar::ColourIds::thumbColourId, Colours::grey);
        setColour(ScrollBar::ColourIds::trackColourId, Colours::grey);

        setColour(TableListBox::ColourIds::backgroundColourId, Colours::lightgrey.brighter());
        setColour(TableListBox::ColourIds::outlineColourId, Colours::darkgrey);
        setColour(TableListBox::ColourIds::textColourId, Colours::black);

        setColour(CodeEditorComponent::ColourIds::backgroundColourId, Colours::lightgrey.brighter());
        setColour(CodeEditorComponent::ColourIds::defaultTextColourId, Colours::black);
        setColour(CodeEditorComponent::ColourIds::highlightColourId, Colours::darkgrey.brighter());
        setColour(CodeEditorComponent::ColourIds::lineNumberBackgroundId, Colours::grey.brighter());
        setColour(CodeEditorComponent::ColourIds::lineNumberTextId, Colours::black);

        setColour(Slider::ColourIds::backgroundColourId, Colours::lightgrey.brighter().brighter().brighter());
        setColour(Slider::ColourIds::thumbColourId, Colours::grey);
        setColour(Slider::ColourIds::trackColourId, Colours::lightgrey.brighter());
        setColour(Slider::ColourIds::rotarySliderFillColourId, Colours::lightgrey.brighter());
        setColour(Slider::ColourIds::rotarySliderOutlineColourId, Colours::lightgrey.brighter().brighter());
        setColour(Slider::ColourIds::textBoxTextColourId, Colours::black);
        setColour(Slider::ColourIds::textBoxBackgroundColourId, Colours::lightgrey.brighter());
        setColour(Slider::ColourIds::textBoxHighlightColourId, Colours::slategrey.brighter());
        setColour(Slider::ColourIds::textBoxOutlineColourId, Colours::darkgrey);

        setColour(ColourSelector::ColourIds::backgroundColourId, Colours::lightgrey.brighter());
        setColour(ColourSelector::ColourIds::labelTextColourId, Colours::black);

        setColour(ToggleButton::ColourIds::textColourId, Colours::black);
        setColour(ToggleButton::ColourIds::tickColourId, Colours::black);
        setColour(ToggleButton::ColourIds::tickDisabledColourId, Colours::black.brighter());

        setColour(Label::ColourIds::textColourId, Colours::black);
        setColour(Label::ColourIds::textWhenEditingColourId, Colours::black);

        setColour(FileSearchPathListComponent::ColourIds::backgroundColourId, Colours::lightgrey);

        setColour(ProgressBar::ColourIds::foregroundColourId, Colours::darkgrey);
        setColour(ProgressBar::ColourIds::backgroundColourId, Colours::lightgrey);

        break;
    case PS_Dark:
    default:
        setColour(ColourScheme::windowBackground, Colours::darkgrey.darker());
        setColour(ColourScheme::widgetBackground, Colours::grey.darker());
        setColour(ColourScheme::menuBackground, Colours::darkgrey.darker().darker());
        setColour(ColourScheme::outline, Colours::lightgrey);
        setColour(ColourScheme::defaultText, Colours::lightgrey);
        setColour(ColourScheme::defaultFill, Colours::grey.darker());
        setColour(ColourScheme::highlightedText, Colours::white);
        setColour(ColourScheme::highlightedFill, Colours::grey.darker());
        setColour(ColourScheme::menuText, Colours::dimgrey);

        setColour(ResizableWindow::ColourIds::backgroundColourId, Colours::darkgrey.darker().darker());

        setColour(AlertWindow::ColourIds::backgroundColourId, Colours::darkgrey.darker());
        setColour(AlertWindow::ColourIds::outlineColourId, Colours::lightgrey);
        setColour(AlertWindow::ColourIds::textColourId, Colours::lightgrey);

        setColour(TextEditor::ColourIds::backgroundColourId, Colours::darkgrey.darker());
        setColour(TextEditor::ColourIds::textColourId, Colours::white);
        setColour(TextEditor::ColourIds::highlightColourId, Colours::dimgrey.darker());
        setColour(TextEditor::ColourIds::highlightedTextColourId, Colours::lightgrey);
        setColour(TextEditor::ColourIds::outlineColourId, Colours::lightgrey);
        setColour(TextEditor::ColourIds::focusedOutlineColourId, Colours::lightgrey);
        setColour(TextEditor::ColourIds::shadowColourId, Colours::grey.darker());

        setColour(ComboBox::ColourIds::arrowColourId, Colours::lightgrey.darker());
        setColour(ComboBox::ColourIds::backgroundColourId, Colours::darkgrey.darker());
        setColour(ComboBox::ColourIds::buttonColourId, Colours::grey.darker());
        setColour(ComboBox::ColourIds::focusedOutlineColourId, Colours::darkgrey);
        setColour(ComboBox::ColourIds::outlineColourId, Colours::lightgrey);
        setColour(ComboBox::ColourIds::textColourId, Colours::white);

        setColour(PopupMenu::ColourIds::backgroundColourId, Colours::darkgrey.darker());
        setColour(PopupMenu::ColourIds::textColourId, Colours::white);
        setColour(PopupMenu::ColourIds::headerTextColourId, Colours::lightgrey);
        setColour(PopupMenu::ColourIds::highlightedBackgroundColourId, Colours::white.darker());
        setColour(PopupMenu::ColourIds::highlightedTextColourId, Colours::lightgrey);

        setColour(TextButton::ColourIds::buttonColourId, Colours::grey.darker()); // this applies for DrawableButton as well
        setColour(TextButton::ColourIds::buttonOnColourId, Colours::lightgrey.darker()); // this applies for DrawableButton as well
        setColour(TextButton::ColourIds::textColourOffId, Colours::white);
        setColour(TextButton::ColourIds::textColourOnId, Colours::white);

        setColour(DrawableButton::ColourIds::textColourId, Colours::white);
        setColour(DrawableButton::ColourIds::textColourOnId, Colours::white);
        setColour(DrawableButton::ColourIds::backgroundColourId, Colours::darkgrey.darker());
        setColour(DrawableButton::ColourIds::backgroundOnColourId, Colours::darkgrey.darker());

        setColour(ListBox::ColourIds::backgroundColourId, Colours::darkgrey.darker());
        setColour(ListBox::ColourIds::outlineColourId, Colours::lightgrey);
        setColour(ListBox::ColourIds::textColourId, Colours::white);

        setColour(TableHeaderComponent::ColourIds::textColourId, Colours::white);
        setColour(TableHeaderComponent::ColourIds::backgroundColourId, Colours::darkgrey.darker());
        setColour(TableHeaderComponent::ColourIds::outlineColourId, Colours::grey);
        setColour(TableHeaderComponent::ColourIds::highlightColourId, Colours::grey.darker());

        setColour(ScrollBar::ColourIds::backgroundColourId, Colours::darkgrey.darker());
        setColour(ScrollBar::ColourIds::thumbColourId, Colours::grey);
        setColour(ScrollBar::ColourIds::trackColourId, Colours::grey);

        setColour(TableListBox::ColourIds::backgroundColourId, Colours::darkgrey.darker());
        setColour(TableListBox::ColourIds::outlineColourId, Colours::lightgrey);
        setColour(TableListBox::ColourIds::textColourId, Colours::white);

        setColour(CodeEditorComponent::ColourIds::backgroundColourId, Colours::darkgrey.darker());
        setColour(CodeEditorComponent::ColourIds::defaultTextColourId, Colours::white);
        setColour(CodeEditorComponent::ColourIds::highlightColourId, Colours::lightgrey.darker());
        setColour(CodeEditorComponent::ColourIds::lineNumberBackgroundId, Colours::grey.darker());
        setColour(CodeEditorComponent::ColourIds::lineNumberTextId, Colours::white);

        setColour(Slider::ColourIds::backgroundColourId, Colours::darkgrey.darker().darker().darker());
        setColour(Slider::ColourIds::thumbColourId, Colours::grey);
        setColour(Slider::ColourIds::trackColourId, Colours::darkgrey.darker());
        setColour(Slider::ColourIds::rotarySliderFillColourId, Colours::darkgrey.darker());
        setColour(Slider::ColourIds::rotarySliderOutlineColourId, Colours::darkgrey.darker().darker());
        setColour(Slider::ColourIds::textBoxTextColourId, Colours::white);
        setColour(Slider::ColourIds::textBoxBackgroundColourId, Colours::darkgrey.darker());
        setColour(Slider::ColourIds::textBoxHighlightColourId, Colours::dimgrey.darker());
        setColour(Slider::ColourIds::textBoxOutlineColourId, Colours::lightgrey);

        setColour(ColourSelector::ColourIds::backgroundColourId, Colours::darkgrey.darker());
        setColour(ColourSelector::ColourIds::labelTextColourId, Colours::white);

        setColour(ToggleButton::ColourIds::textColourId, Colours::white);
        setColour(ToggleButton::ColourIds::tickColourId, Colours::white);
        setColour(ToggleButton::ColourIds::tickDisabledColourId, Colours::white.darker());

        setColour(Label::ColourIds::textColourId, Colours::white);
        setColour(Label::ColourIds::textWhenEditingColourId, Colours::white);

        setColour(FileSearchPathListComponent::ColourIds::backgroundColourId, Colours::darkgrey.darker().darker());

        setColour(ProgressBar::ColourIds::foregroundColourId, Colours::lightgrey);
        setColour(ProgressBar::ColourIds::backgroundColourId, Colours::darkgrey);

        break;
    }
	
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

void CustomLookAndFeel::drawGroupComponentOutline(Graphics& g,
	int width, 
	int height, 
	const String& text,
	const Justification& position, 
	GroupComponent& group)
{
	// code from LookAndFeel_V2 but without rounded edges

    const float textH = 15.0f;
    const float indent = 3.0f;
    const float textEdgeGap = 4.0f;

    auto f = Font(FontOptions(textH));

    Path p;
    auto x = indent;
    auto y = f.getAscent() - 3.0f;
    auto w = jmax(0.0f, (float)width - x * 2.0f);
    auto h = jmax(0.0f, (float)height - y - indent);

    auto textW = text.isEmpty() ? 0
        : jlimit(0.0f,
            jmax(0.0f, w - textEdgeGap * 2),
            juce::GlyphArrangement::getStringWidth(f, text)+ textEdgeGap * 2.0f);
    auto textX = textEdgeGap;

    if (position.testFlags(Justification::horizontallyCentred))
        textX = (w - textW) * 0.5f;
    else if (position.testFlags(Justification::right))
        textX = w - textW - textEdgeGap;

    p.startNewSubPath(x + textX + textW, y);
    p.lineTo(x + w, y);

    p.lineTo(x + w, y + h);

    p.lineTo(x , y + h);

    p.lineTo(x, y);

    p.lineTo(x + textX, y);

    auto alpha = group.isEnabled() ? 1.0f : 0.5f;

    g.setColour(group.findColour(GroupComponent::outlineColourId)
        .withMultipliedAlpha(alpha));

    g.strokePath(p, PathStrokeType(2.0f));

    g.setColour(group.findColour(GroupComponent::textColourId)
        .withMultipliedAlpha(alpha));
    g.setFont(f);
    g.drawText(text,
        roundToInt(x + textX), 0,
        roundToInt(textW),
        roundToInt(textH),
        Justification::centred, true);
}

void CustomLookAndFeel::drawTickBox(Graphics& g, Component& component,
    float x, float y, float w, float h,
    const bool ticked,
    const bool isEnabled,
    const bool shouldDrawButtonAsHighlighted,
    const bool shouldDrawButtonAsDown)
{
    ignoreUnused(isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

    Rectangle<float> tickBounds(x, y, w, h);

    g.setColour(component.findColour(ToggleButton::tickDisabledColourId));
    g.drawRect(tickBounds, 1.0f);

    if (ticked)
    {
        g.setColour(component.findColour(ToggleButton::tickColourId));
        auto tick = getTickShape(0.75f);
        g.fillPath(tick, tick.getTransformToScaleToFit(tickBounds.reduced(4, 5).toFloat(), false));
    }
}

void CustomLookAndFeel::drawComboBox(Graphics& g, int width, int height, bool,
    int, int, int, int, ComboBox& box)
{
    Rectangle<int> boxBounds(0, 0, width, height);

    g.setColour(box.findColour(ComboBox::backgroundColourId));
    g.fillRect(boxBounds.toFloat());

    g.setColour(box.findColour(ComboBox::outlineColourId));
    g.drawRect(boxBounds.toFloat().reduced(0.5f, 0.5f), 1.0f);

    Rectangle<int> arrowZone(width - 30, 0, 20, height);
    Path path;
    path.startNewSubPath((float)arrowZone.getX() + 3.0f, (float)arrowZone.getCentreY() - 2.0f);
    path.lineTo((float)arrowZone.getCentreX(), (float)arrowZone.getCentreY() + 3.0f);
    path.lineTo((float)arrowZone.getRight() - 3.0f, (float)arrowZone.getCentreY() - 2.0f);

    g.setColour(box.findColour(ComboBox::arrowColourId).withAlpha((box.isEnabled() ? 0.9f : 0.2f)));
    g.strokePath(path, PathStrokeType(2.0f));
}

void CustomLookAndFeel::drawCallOutBoxBackground(CallOutBox& box, Graphics& g,
    const Path& path, Image& cachedImage)
{
    if (cachedImage.isNull())
    {
        cachedImage = Image(Image::ARGB, box.getWidth(), box.getHeight(), true);
        Graphics g2(cachedImage);

        DropShadow(Colours::black.withAlpha(0.7f), 8, Point<int>(0, 2)).drawForPath(g2, path);
    }

    g.setColour(Colours::black);
    g.drawImageAt(cachedImage, 0, 0);

    g.setColour(findColour(ResizableWindow::ColourIds::backgroundColourId));// Colour::greyLevel(0.23f).withAlpha(0.9f));
    g.fillPath(path);

    g.setColour(Colours::white.withAlpha(0.8f));
    g.strokePath(path, PathStrokeType(2.0f));
}

}
