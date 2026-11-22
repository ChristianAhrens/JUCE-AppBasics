#include "CustomLookAndFeel.h"

namespace JUCEAppBasics
{

CustomLookAndFeel::CustomLookAndFeel(CustomLookAndFeel::PaletteStyle style)
{
    setPaletteStyle(style);
    setMouseCursor(juce::MouseCursor(juce::MouseCursor::StandardCursorType::NoCursor));
}

CustomLookAndFeel::~CustomLookAndFeel()
{
}

void CustomLookAndFeel::setPaletteStyle(CustomLookAndFeel::PaletteStyle style)
{
    m_paletteStyle = style;

    switch (style)
    {
    case PS_Light:
        setColour(juce::LookAndFeel_V4::ColourScheme::windowBackground, juce::Colours::lightgrey.brighter());
        setColour(juce::LookAndFeel_V4::ColourScheme::widgetBackground, juce::Colours::grey.brighter());
        setColour(juce::LookAndFeel_V4::ColourScheme::menuBackground, juce::Colours::lightgrey.brighter().brighter());
        setColour(juce::LookAndFeel_V4::ColourScheme::outline, juce::Colours::darkgrey);
        setColour(juce::LookAndFeel_V4::ColourScheme::defaultText, juce::Colours::darkgrey);
        setColour(juce::LookAndFeel_V4::ColourScheme::defaultFill, juce::Colours::grey.brighter());
        setColour(juce::LookAndFeel_V4::ColourScheme::highlightedText, juce::Colours::black);
        setColour(juce::LookAndFeel_V4::ColourScheme::highlightedFill, juce::Colours::grey.brighter());
        setColour(juce::LookAndFeel_V4::ColourScheme::menuText, juce::Colours::slategrey);

        setColour(juce::ResizableWindow::ColourIds::backgroundColourId, juce::Colours::lightgrey.brighter().brighter());

        setColour(juce::AlertWindow::ColourIds::backgroundColourId, juce::Colours::lightgrey.brighter());
        setColour(juce::AlertWindow::ColourIds::outlineColourId, juce::Colours::darkgrey);
        setColour(juce::AlertWindow::ColourIds::textColourId, juce::Colours::darkgrey);

        setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colours::lightgrey.brighter());
        setColour(juce::TextEditor::ColourIds::textColourId, juce::Colours::black);
        setColour(juce::TextEditor::ColourIds::highlightColourId, juce::Colours::slategrey.brighter());
        setColour(juce::TextEditor::ColourIds::highlightedTextColourId, juce::Colours::darkgrey);
        setColour(juce::TextEditor::ColourIds::outlineColourId, juce::Colours::darkgrey);
        setColour(juce::TextEditor::ColourIds::focusedOutlineColourId, juce::Colours::darkgrey);
        setColour(juce::TextEditor::ColourIds::shadowColourId, juce::Colours::grey.brighter());

        setColour(juce::ComboBox::ColourIds::arrowColourId, juce::Colours::darkgrey);
        setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colours::lightgrey.brighter());
        setColour(juce::ComboBox::ColourIds::buttonColourId, juce::Colours::grey.brighter());
        setColour(juce::ComboBox::ColourIds::focusedOutlineColourId, juce::Colours::lightgrey);
        setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colours::darkgrey);
        setColour(juce::ComboBox::ColourIds::textColourId, juce::Colours::black);

        setColour(juce::PopupMenu::ColourIds::backgroundColourId, juce::Colours::lightgrey.brighter());
        setColour(juce::PopupMenu::ColourIds::textColourId, juce::Colours::black);
        setColour(juce::PopupMenu::ColourIds::headerTextColourId, juce::Colours::darkgrey);
        // the highlighted item is the pressed overlay of the text colour, the text keeps its colour
        setColour(juce::PopupMenu::ColourIds::highlightedBackgroundColourId, juce::Colours::black.withAlpha(Theme::Alpha::pressed));
        setColour(juce::PopupMenu::ColourIds::highlightedTextColourId, juce::Colours::black);

        setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::grey.brighter()); // this applies for DrawableButton as well
        setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkgrey.brighter()); // this applies for DrawableButton as well
        setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::black);
        setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);

        setColour(juce::DrawableButton::ColourIds::textColourId, juce::Colours::black);
        setColour(juce::DrawableButton::ColourIds::textColourOnId, juce::Colours::black);
        setColour(juce::DrawableButton::ColourIds::backgroundColourId, juce::Colours::lightgrey.brighter());
        setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colours::lightgrey.brighter());

        setColour(juce::ListBox::ColourIds::backgroundColourId, juce::Colours::lightgrey.brighter());
        setColour(juce::ListBox::ColourIds::outlineColourId, juce::Colours::darkgrey);
        setColour(juce::ListBox::ColourIds::textColourId, juce::Colours::black);

        setColour(juce::TableHeaderComponent::ColourIds::textColourId, juce::Colours::black);
        setColour(juce::TableHeaderComponent::ColourIds::backgroundColourId, juce::Colours::lightgrey.brighter());
        setColour(juce::TableHeaderComponent::ColourIds::outlineColourId, juce::Colours::grey);
        setColour(juce::TableHeaderComponent::ColourIds::highlightColourId, juce::Colours::grey.brighter());

        setColour(juce::ScrollBar::ColourIds::backgroundColourId, juce::Colours::lightgrey.brighter());
        setColour(juce::ScrollBar::ColourIds::thumbColourId, juce::Colours::grey);
        setColour(juce::ScrollBar::ColourIds::trackColourId, juce::Colours::grey);

        setColour(juce::TableListBox::ColourIds::backgroundColourId, juce::Colours::lightgrey.brighter());
        setColour(juce::TableListBox::ColourIds::outlineColourId, juce::Colours::darkgrey);
        setColour(juce::TableListBox::ColourIds::textColourId, juce::Colours::black);

#if JUCE_MODULE_AVAILABLE_juce_gui_extra
        setColour(juce::CodeEditorComponent::ColourIds::backgroundColourId, juce::Colours::lightgrey.brighter());
        setColour(juce::CodeEditorComponent::ColourIds::defaultTextColourId, juce::Colours::black);
        setColour(juce::CodeEditorComponent::ColourIds::highlightColourId, juce::Colours::darkgrey.brighter());
        setColour(juce::CodeEditorComponent::ColourIds::lineNumberBackgroundId, juce::Colours::grey.brighter());
        setColour(juce::CodeEditorComponent::ColourIds::lineNumberTextId, juce::Colours::black);
#endif

        setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::lightgrey.brighter().brighter().brighter());
        setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::grey);
        setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::lightgrey.brighter());
        setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::lightgrey.brighter());
        setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::lightgrey.brighter().brighter());
        setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::black);
        setColour(juce::Slider::ColourIds::textBoxBackgroundColourId, juce::Colours::lightgrey.brighter());
        setColour(juce::Slider::ColourIds::textBoxHighlightColourId, juce::Colours::slategrey.brighter());
        setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::darkgrey);

#if JUCE_MODULE_AVAILABLE_juce_gui_extra
        setColour(juce::ColourSelector::ColourIds::backgroundColourId, juce::Colours::lightgrey.brighter());
        setColour(juce::ColourSelector::ColourIds::labelTextColourId, juce::Colours::black);
#endif

        setColour(juce::ToggleButton::ColourIds::textColourId, juce::Colours::black);
        setColour(juce::ToggleButton::ColourIds::tickColourId, juce::Colours::black);
        setColour(juce::ToggleButton::ColourIds::tickDisabledColourId, juce::Colours::black.brighter());

        setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
        setColour(juce::Label::ColourIds::textWhenEditingColourId, juce::Colours::black);

        setColour(juce::FileSearchPathListComponent::ColourIds::backgroundColourId, juce::Colours::lightgrey);

        setColour(juce::ProgressBar::ColourIds::foregroundColourId, juce::Colours::darkgrey);
        setColour(juce::ProgressBar::ColourIds::backgroundColourId, juce::Colours::lightgrey);

        setColour(CustomLookAndFeel::MeteringHoldColourId, juce::Colours::grey);

        setColour(juce::TreeView::ColourIds::backgroundColourId, juce::Colours::lightgrey);
        setColour(juce::TreeView::ColourIds::evenItemsColourId, juce::Colours::lightgrey);
        setColour(juce::TreeView::ColourIds::oddItemsColourId, juce::Colours::lightgrey);
        setColour(juce::TreeView::ColourIds::dragAndDropIndicatorColourId, juce::Colours::lightgrey.brighter());
        setColour(juce::TreeView::ColourIds::linesColourId, juce::Colours::darkgrey);
        setColour(juce::TreeView::ColourIds::selectedItemBackgroundColourId, juce::Colours::grey.brighter());

        break;
    case PS_Dark:
    default:
        setColour(juce::LookAndFeel_V4::ColourScheme::windowBackground, juce::Colours::darkgrey.darker());
        setColour(juce::LookAndFeel_V4::ColourScheme::widgetBackground, juce::Colours::grey.darker());
        setColour(juce::LookAndFeel_V4::ColourScheme::menuBackground, juce::Colours::darkgrey.darker().darker());
        setColour(juce::LookAndFeel_V4::ColourScheme::outline, juce::Colours::lightgrey);
        setColour(juce::LookAndFeel_V4::ColourScheme::defaultText, juce::Colours::lightgrey);
        setColour(juce::LookAndFeel_V4::ColourScheme::defaultFill, juce::Colours::grey.darker());
        setColour(juce::LookAndFeel_V4::ColourScheme::highlightedText, juce::Colours::white);
        setColour(juce::LookAndFeel_V4::ColourScheme::highlightedFill, juce::Colours::grey.darker());
        setColour(juce::LookAndFeel_V4::ColourScheme::menuText, juce::Colours::dimgrey);

        setColour(juce::ResizableWindow::ColourIds::backgroundColourId, juce::Colours::darkgrey.darker().darker());

        setColour(juce::AlertWindow::ColourIds::backgroundColourId, juce::Colours::darkgrey.darker());
        setColour(juce::AlertWindow::ColourIds::outlineColourId, juce::Colours::lightgrey);
        setColour(juce::AlertWindow::ColourIds::textColourId, juce::Colours::lightgrey);

        setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colours::darkgrey.darker());
        setColour(juce::TextEditor::ColourIds::textColourId, juce::Colours::white);
        setColour(juce::TextEditor::ColourIds::highlightColourId, juce::Colours::dimgrey.darker());
        setColour(juce::TextEditor::ColourIds::highlightedTextColourId, juce::Colours::lightgrey);
        setColour(juce::TextEditor::ColourIds::outlineColourId, juce::Colours::lightgrey);
        setColour(juce::TextEditor::ColourIds::focusedOutlineColourId, juce::Colours::lightgrey);
        setColour(juce::TextEditor::ColourIds::shadowColourId, juce::Colours::grey.darker());

        setColour(juce::ComboBox::ColourIds::arrowColourId, juce::Colours::lightgrey.darker());
        setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colours::darkgrey.darker());
        setColour(juce::ComboBox::ColourIds::buttonColourId, juce::Colours::grey.darker());
        setColour(juce::ComboBox::ColourIds::focusedOutlineColourId, juce::Colours::darkgrey);
        setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colours::lightgrey);
        setColour(juce::ComboBox::ColourIds::textColourId, juce::Colours::white);

        setColour(juce::PopupMenu::ColourIds::backgroundColourId, juce::Colours::darkgrey.darker());
        setColour(juce::PopupMenu::ColourIds::textColourId, juce::Colours::white);
        setColour(juce::PopupMenu::ColourIds::headerTextColourId, juce::Colours::lightgrey);
        setColour(juce::PopupMenu::ColourIds::highlightedBackgroundColourId, juce::Colours::white.withAlpha(Theme::Alpha::pressed));
        setColour(juce::PopupMenu::ColourIds::highlightedTextColourId, juce::Colours::white);

        setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::grey.darker()); // this applies for DrawableButton as well
        setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::lightgrey.darker()); // this applies for DrawableButton as well
        setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
        setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::white);

        setColour(juce::DrawableButton::ColourIds::textColourId, juce::Colours::white);
        setColour(juce::DrawableButton::ColourIds::textColourOnId, juce::Colours::white);
        setColour(juce::DrawableButton::ColourIds::backgroundColourId, juce::Colours::darkgrey.darker());
        setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colours::darkgrey.darker());

        setColour(juce::ListBox::ColourIds::backgroundColourId, juce::Colours::darkgrey.darker());
        setColour(juce::ListBox::ColourIds::outlineColourId, juce::Colours::lightgrey);
        setColour(juce::ListBox::ColourIds::textColourId, juce::Colours::white);

        setColour(juce::TableHeaderComponent::ColourIds::textColourId, juce::Colours::white);
        setColour(juce::TableHeaderComponent::ColourIds::backgroundColourId, juce::Colours::darkgrey.darker());
        setColour(juce::TableHeaderComponent::ColourIds::outlineColourId, juce::Colours::grey);
        setColour(juce::TableHeaderComponent::ColourIds::highlightColourId, juce::Colours::grey.darker());

        setColour(juce::ScrollBar::ColourIds::backgroundColourId, juce::Colours::darkgrey.darker());
        setColour(juce::ScrollBar::ColourIds::thumbColourId, juce::Colours::grey);
        setColour(juce::ScrollBar::ColourIds::trackColourId, juce::Colours::grey);

        setColour(juce::TableListBox::ColourIds::backgroundColourId, juce::Colours::darkgrey.darker());
        setColour(juce::TableListBox::ColourIds::outlineColourId, juce::Colours::lightgrey);
        setColour(juce::TableListBox::ColourIds::textColourId, juce::Colours::white);

#if JUCE_MODULE_AVAILABLE_juce_gui_extra
        setColour(juce::CodeEditorComponent::ColourIds::backgroundColourId, juce::Colours::darkgrey.darker());
        setColour(juce::CodeEditorComponent::ColourIds::defaultTextColourId, juce::Colours::white);
        setColour(juce::CodeEditorComponent::ColourIds::highlightColourId, juce::Colours::lightgrey.darker());
        setColour(juce::CodeEditorComponent::ColourIds::lineNumberBackgroundId, juce::Colours::grey.darker());
        setColour(juce::CodeEditorComponent::ColourIds::lineNumberTextId, juce::Colours::white);
#endif

        setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::darkgrey.darker().darker().darker());
        setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::grey);
        setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::darkgrey.darker());
        setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::darkgrey.darker());
        setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::darkgrey.darker().darker());
        setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::white);
        setColour(juce::Slider::ColourIds::textBoxBackgroundColourId, juce::Colours::darkgrey.darker());
        setColour(juce::Slider::ColourIds::textBoxHighlightColourId, juce::Colours::dimgrey.darker());
        setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::lightgrey);

#if JUCE_MODULE_AVAILABLE_juce_gui_extra
        setColour(juce::ColourSelector::ColourIds::backgroundColourId, juce::Colours::darkgrey.darker());
        setColour(juce::ColourSelector::ColourIds::labelTextColourId, juce::Colours::white);
#endif

        setColour(juce::ToggleButton::ColourIds::textColourId, juce::Colours::white);
        setColour(juce::ToggleButton::ColourIds::tickColourId, juce::Colours::white);
        setColour(juce::ToggleButton::ColourIds::tickDisabledColourId, juce::Colours::white.darker());

        setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
        setColour(juce::Label::ColourIds::textWhenEditingColourId, juce::Colours::white);

        setColour(juce::FileSearchPathListComponent::ColourIds::backgroundColourId, juce::Colours::darkgrey.darker().darker());

        setColour(juce::ProgressBar::ColourIds::foregroundColourId, juce::Colours::lightgrey);
        setColour(juce::ProgressBar::ColourIds::backgroundColourId, juce::Colours::darkgrey);

        setColour(CustomLookAndFeel::MeteringHoldColourId, juce::Colours::grey);

        setColour(juce::TreeView::ColourIds::backgroundColourId, juce::Colours::darkgrey.darker());
        setColour(juce::TreeView::ColourIds::evenItemsColourId, juce::Colours::darkgrey.darker());
        setColour(juce::TreeView::ColourIds::oddItemsColourId, juce::Colours::darkgrey.darker());
        setColour(juce::TreeView::ColourIds::dragAndDropIndicatorColourId, juce::Colours::darkgrey);
        setColour(juce::TreeView::ColourIds::linesColourId, juce::Colours::lightgrey);
        setColour(juce::TreeView::ColourIds::selectedItemBackgroundColourId, juce::Colours::grey.darker());

        break;
    }

    // colour roles of the shared design foundation, derived from the palette colours set above
    setColour(CustomLookAndFeel::StateMuteColourId, juce::Colours::red);
    setColour(CustomLookAndFeel::StateWarningColourId, juce::Colour(0xff, 0xe8, 0x00).withAlpha(0.5f));
    setColour(CustomLookAndFeel::StateCriticalColourId, juce::Colour(0xff, 0x40, 0x02).withAlpha(0.5f));
    setColour(CustomLookAndFeel::StateAlertColourId, juce::Colour(0xff, 0x30, 0x02).withAlpha(0.8f));
    setColour(CustomLookAndFeel::StateErrorColourId, juce::Colours::red);
    setColour(CustomLookAndFeel::OverlayShadowColourId, juce::Colours::black.withAlpha(0.7f));
    setColour(CustomLookAndFeel::OverlayBorderColourId, (PS_Light == m_paletteStyle ? juce::Colours::darkgrey : juce::Colours::white).withAlpha(0.8f));

    // tooltip and hyperlink follow the palette (were the dark scheme of LookAndFeel_V4 in both palettes)
    setColour(juce::TooltipWindow::ColourIds::backgroundColourId, findColour(juce::ResizableWindow::ColourIds::backgroundColourId));
    setColour(juce::TooltipWindow::ColourIds::textColourId, findColour(juce::Label::ColourIds::textColourId));
    setColour(juce::TooltipWindow::ColourIds::outlineColourId, findColour(juce::TextEditor::ColourIds::outlineColourId));
    setColour(juce::HyperlinkButton::ColourIds::textColourId, findColour(juce::Label::ColourIds::textColourId));
    setColour(CustomLookAndFeel::SurfaceRecessedDeepColourId, findColour(juce::ResizableWindow::ColourIds::backgroundColourId).darker());
    setColour(CustomLookAndFeel::SurfaceGapColourId, findColour(juce::LookAndFeel_V4::ColourScheme::defaultFill));

    // the accent colour survives a palette change
    setAccentColour(m_accentColour);
}

void CustomLookAndFeel::setAccentColour(const juce::Colour& accent)
{
    m_accentColour = accent;
    setColour(CustomLookAndFeel::MeteringRmsColourId, accent);
    setColour(CustomLookAndFeel::MeteringPeakColourId, PS_Light == m_paletteStyle ? accent.brighter() : accent.darker());
}

void CustomLookAndFeel::applyAccentColour(juce::LookAndFeel& laf, const juce::Colour& accent)
{
    auto* customLookAndFeel = dynamic_cast<CustomLookAndFeel*>(&laf);
    if (nullptr == customLookAndFeel || customLookAndFeel->getAccentColour() == accent)
        return;

    customLookAndFeel->setAccentColour(accent);

    auto& desktop = juce::Desktop::getInstance();
    if (&desktop.getDefaultLookAndFeel() == customLookAndFeel)
        desktop.setDefaultLookAndFeel(customLookAndFeel);
}

void CustomLookAndFeel::applyPaletteStyle(std::unique_ptr<CustomLookAndFeel>& holder, PaletteStyle style)
{
    const auto accent = holder ? holder->getAccentColour() : juce::Colours::forestgreen;
    holder = std::make_unique<CustomLookAndFeel>(style);
    holder->setAccentColour(accent);
    juce::Desktop::getInstance().setDefaultLookAndFeel(holder.get());
}

const CustomLookAndFeel::PaletteStyle& CustomLookAndFeel::getPaletteStyle()
{
    return m_paletteStyle;
}

void CustomLookAndFeel::setMouseCursor(const juce::MouseCursor& mouseCursor)
{
    m_mouseCursor = mouseCursor;

    juce::Desktop::getInstance().getMainMouseSource().forceMouseCursorUpdate();
}

juce::MouseCursor CustomLookAndFeel::getMouseCursorFor(juce::Component& c)
{
    if (m_mouseCursor != juce::MouseCursor(juce::MouseCursor::StandardCursorType::NoCursor) && m_mouseCursor != c.getMouseCursor())
        return m_mouseCursor;
    else
        return c.getMouseCursor();
}

void CustomLookAndFeel::drawButtonBackground(juce::Graphics& g,
    juce::Button& button,
    const juce::Colour& backgroundColour,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);

    auto baseColour = backgroundColour.withMultipliedAlpha(button.isEnabled() ? 1.0f : Theme::Alpha::disabled);

    if (button.isEnabled() && (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted))
        baseColour = baseColour.contrasting(shouldDrawButtonAsDown ? Theme::Alpha::pressed : Theme::Alpha::hover);

    g.setColour(baseColour);
    g.fillRect(bounds);

    // keyboard focus is shown as a 1 px frame in the shared focus outline role
    if (button.isEnabled() && button.hasKeyboardFocus(true))
    {
        g.setColour(findColour(juce::TextEditor::ColourIds::focusedOutlineColourId));
        g.drawRect(button.getLocalBounds(), Theme::Size::hairline);
    }
}

void CustomLookAndFeel::drawDrawableButton(juce::Graphics& g,
    juce::DrawableButton& button,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown)
{
    // background and caption as in LookAndFeel_V2
    LookAndFeel_V4::drawDrawableButton(g, button, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

    if (!button.isEnabled())
        return;

    // hover and pressed as overlay of the text colour, so that it works on transparent backgrounds
    if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
    {
        g.setColour(button.findColour(juce::DrawableButton::ColourIds::textColourId)
            .withAlpha(shouldDrawButtonAsDown ? Theme::Alpha::pressed : Theme::Alpha::hover));
        g.fillRect(button.getLocalBounds());
    }

    // keyboard focus as 1 px frame, same role as for text buttons
    if (button.hasKeyboardFocus(true))
    {
        g.setColour(findColour(juce::TextEditor::ColourIds::focusedOutlineColourId));
        g.drawRect(button.getLocalBounds(), Theme::Size::hairline);
    }
}

void CustomLookAndFeel::drawGroupComponentOutline(juce::Graphics& g,
	int width,
	int height,
	const juce::String& text,
	const juce::Justification& position,
	juce::GroupComponent& group)
{
	// code from LookAndFeel_V2 but without rounded edges

    const float textH = 15.0f;
    const float indent = 3.0f;
    const float textEdgeGap = 4.0f;

    auto f = juce::Font(juce::FontOptions(textH));

    juce::Path p;
    auto x = indent;
    auto y = f.getAscent() - 3.0f;
    auto w = juce::jmax(0.0f, (float)width - x * 2.0f);
    auto h = juce::jmax(0.0f, (float)height - y - indent);

    auto textW = text.isEmpty() ? 0
        : juce::jlimit(0.0f,
            juce::jmax(0.0f, w - textEdgeGap * 2),
            juce::GlyphArrangement::getStringWidth(f, text)+ textEdgeGap * 2.0f);
    auto textX = textEdgeGap;

    if (position.testFlags(juce::Justification::horizontallyCentred))
        textX = (w - textW) * 0.5f;
    else if (position.testFlags(juce::Justification::right))
        textX = w - textW - textEdgeGap;

    p.startNewSubPath(x + textX + textW, y);
    p.lineTo(x + w, y);

    p.lineTo(x + w, y + h);

    p.lineTo(x , y + h);

    p.lineTo(x, y);

    p.lineTo(x + textX, y);

    auto alpha = group.isEnabled() ? 1.0f : 0.5f;

    g.setColour(group.findColour(juce::GroupComponent::outlineColourId)
        .withMultipliedAlpha(alpha));

    g.strokePath(p, juce::PathStrokeType(2.0f));

    g.setColour(group.findColour(juce::GroupComponent::textColourId)
        .withMultipliedAlpha(alpha));
    g.setFont(f);
    g.drawText(text,
        juce::roundToInt(x + textX), 0,
        juce::roundToInt(textW),
        juce::roundToInt(textH),
        juce::Justification::centred, true);
}

void CustomLookAndFeel::drawTickBox(juce::Graphics& g, juce::Component& component,
    float x, float y, float w, float h,
    const bool ticked,
    const bool isEnabled,
    const bool shouldDrawButtonAsHighlighted,
    const bool shouldDrawButtonAsDown)
{
    juce::Rectangle<float> tickBounds(x, y, w, h);
    const auto alpha = isEnabled ? 1.0f : Theme::Alpha::disabled;
    const auto textColour = component.findColour(juce::ToggleButton::textColourId);

    if (isEnabled && (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted))
    {
        g.setColour(textColour.withAlpha(shouldDrawButtonAsDown ? Theme::Alpha::pressed : Theme::Alpha::hover));
        g.fillRect(tickBounds);
    }

    g.setColour(component.findColour(juce::ToggleButton::tickDisabledColourId).withMultipliedAlpha(alpha));
    g.drawRect(tickBounds, 1.0f);

    if (ticked)
    {
        g.setColour(component.findColour(juce::ToggleButton::tickColourId).withMultipliedAlpha(alpha));
        auto tick = getTickShape(0.75f);
        g.fillPath(tick, tick.getTransformToScaleToFit(tickBounds.reduced(4, 5).toFloat(), false));
    }

    // keyboard focus as 1 px frame around the box, same role as for the buttons
    if (isEnabled && component.hasKeyboardFocus(false))
    {
        g.setColour(findColour(juce::TextEditor::ColourIds::focusedOutlineColourId));
        g.drawRect(tickBounds.expanded(2.0f), static_cast<float>(Theme::Size::hairline));
    }
}

void CustomLookAndFeel::drawScrollbar(juce::Graphics& g, juce::ScrollBar& scrollbar, int x, int y, int width, int height,
    bool isScrollbarVertical, int thumbStartPosition, int thumbSize, bool isMouseOver, bool isMouseDown)
{
    const auto thumbBounds = (isScrollbarVertical ? juce::Rectangle<int>(x, thumbStartPosition, width, thumbSize)
        : juce::Rectangle<int>(thumbStartPosition, y, thumbSize, height)).reduced(Theme::Size::hairline).toFloat();

    g.setColour(scrollbar.findColour(juce::ScrollBar::ColourIds::thumbColourId));
    g.fillRoundedRectangle(thumbBounds, 4.0f);

    // hover and pressed as overlay of the text colour, same as for the other building blocks
    if (isMouseOver || isMouseDown)
    {
        g.setColour(findColour(juce::Label::ColourIds::textColourId).withAlpha(isMouseDown ? Theme::Alpha::pressed : Theme::Alpha::hover));
        g.fillRoundedRectangle(thumbBounds, 4.0f);
    }
}

void CustomLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
    int, int, int, int, juce::ComboBox& box)
{
    juce::Rectangle<int> boxBounds(0, 0, width, height);
    const auto alpha = box.isEnabled() ? 1.0f : Theme::Alpha::disabled;

    g.setColour(box.findColour(juce::ComboBox::backgroundColourId).withMultipliedAlpha(alpha));
    g.fillRect(boxBounds.toFloat());

    // hover and pressed as overlay of the text colour
    if (box.isEnabled() && (isButtonDown || box.isMouseOver(true)))
    {
        g.setColour(box.findColour(juce::ComboBox::textColourId).withAlpha(isButtonDown ? Theme::Alpha::pressed : Theme::Alpha::hover));
        g.fillRect(boxBounds.toFloat());
    }

    g.setColour(box.findColour(juce::ComboBox::outlineColourId).withMultipliedAlpha(alpha));
    g.drawRect(boxBounds.toFloat().reduced(0.5f, 0.5f), 1.0f);

    juce::Rectangle<int> arrowZone(width - 30, 0, 20, height);
    juce::Path path;
    path.startNewSubPath((float)arrowZone.getX() + 3.0f, (float)arrowZone.getCentreY() - 2.0f);
    path.lineTo((float)arrowZone.getCentreX(), (float)arrowZone.getCentreY() + 3.0f);
    path.lineTo((float)arrowZone.getRight() - 3.0f, (float)arrowZone.getCentreY() - 2.0f);

    g.setColour(box.findColour(juce::ComboBox::arrowColourId).withAlpha(box.isEnabled() ? 0.9f : Theme::Alpha::disabled));
    g.strokePath(path, juce::PathStrokeType(2.0f));

    // keyboard focus as 1 px frame in the shared focus role
    if (box.isEnabled() && box.hasKeyboardFocus(true))
    {
        g.setColour(box.findColour(juce::ComboBox::focusedOutlineColourId));
        g.drawRect(boxBounds, Theme::Size::hairline);
    }
}

void CustomLookAndFeel::drawCallOutBoxBackground(juce::CallOutBox& box, juce::Graphics& g,
    const juce::Path& path, juce::Image& cachedImage)
{
    if (cachedImage.isNull())
    {
        cachedImage = juce::Image(juce::Image::ARGB, box.getWidth(), box.getHeight(), true);
        juce::Graphics g2(cachedImage);

        juce::DropShadow(findColour(CustomLookAndFeel::OverlayShadowColourId), 8, juce::Point<int>(0, 2)).drawForPath(g2, path);
    }

    g.setColour(juce::Colours::black);
    g.drawImageAt(cachedImage, 0, 0);

    g.setColour(findColour(juce::ResizableWindow::ColourIds::backgroundColourId));
    g.fillPath(path);

    g.setColour(findColour(CustomLookAndFeel::OverlayBorderColourId));
    g.strokePath(path, juce::PathStrokeType(2.0f));
}

namespace
{
// hover, pressed and focus of a ToggleStateSlider, drawn over its frame shape (on and drag drawing is not touched)
void drawToggleSliderStates(juce::Graphics& g, juce::Slider& slider, const juce::Path& frame, const juce::Colour& focusColour)
{
    if (!slider.isEnabled())
        return;

    if (slider.isMouseOverOrDragging() || slider.isMouseButtonDown())
    {
        g.setColour(slider.findColour(juce::Slider::ColourIds::textBoxTextColourId)
            .withAlpha(slider.isMouseButtonDown() ? Theme::Alpha::pressed : Theme::Alpha::hover));
        g.fillPath(frame);
    }

    if (slider.hasKeyboardFocus(true))
    {
        g.setColour(focusColour);
        g.strokePath(frame, juce::PathStrokeType(static_cast<float>(Theme::Size::hairline)));
    }
}
}

int CustomLookAndFeel::getSliderThumbRadius(juce::Slider& slider)
{
    return slider.isHorizontal() ? static_cast<int> ((float)slider.getHeight() * 0.6f)
        : static_cast<int> ((float)slider.getWidth() * 0.6f);
}

void CustomLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    auto isTwoVal = (style == juce::Slider::SliderStyle::TwoValueVertical || style == juce::Slider::SliderStyle::TwoValueHorizontal);
    auto isThreeVal = (style == juce::Slider::SliderStyle::ThreeValueVertical || style == juce::Slider::SliderStyle::ThreeValueHorizontal);
    auto tss = dynamic_cast<ToggleStateSlider*>(&slider);
    if (tss && !(isTwoVal || isThreeVal || slider.isBar()))
    {
        // disabled is drawn at reduced opacity as a whole
        if (!slider.isEnabled())
            g.beginTransparencyLayer(Theme::Alpha::disabled);

        // oval surrounding frame
        juce::Path framing, framingOutline;
        if (slider.isHorizontal())
        {
            framing.startNewSubPath({ (float)x + 1, (float)y + (float)height * 0.5f });
            framing.lineTo({ (float)x + (float)width - 2, (float)y + (float)height * 0.5f });
            juce::PathStrokeType opt((float)height - 2, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);
            opt.createStrokedPath(framingOutline, framing);
        }
        else
        {
            framing.startNewSubPath({ (float)x + (float)width * 0.5f, (float)y + 1 });
            framing.lineTo({ (float)x + (float)width * 0.5f, (float)y + (float)height - 2 });
            juce::PathStrokeType opt((float)width - 2, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);
            opt.createStrokedPath(framingOutline, framing);
        }
        g.setColour(slider.findColour(juce::Slider::thumbColourId));
        g.strokePath(framingOutline, juce::PathStrokeType(1.0f));
        drawToggleSliderStates(g, slider, framingOutline, findColour(juce::TextEditor::ColourIds::focusedOutlineColourId));

        // slider itself
        auto trackWidth = slider.isHorizontal() ? (float)height * 0.25f : (float)width * 0.25f;

        juce::Point<float> startPoint(slider.isHorizontal() ? (float)x : (float)x + (float)width * 0.5f,
            slider.isHorizontal() ? (float)y + (float)height * 0.5f : (float)(height + y));

        juce::Point<float> endPoint(slider.isHorizontal() ? (float)(width + x) : startPoint.x,
            slider.isHorizontal() ? startPoint.y : (float)y);

        juce::Path backgroundTrack;
        backgroundTrack.startNewSubPath(startPoint);
        backgroundTrack.lineTo(endPoint);
        g.setColour(slider.findColour(juce::Slider::backgroundColourId));
        g.strokePath(backgroundTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

        juce::Path valueTrack;
        juce::Point<float> minPoint, maxPoint;

        auto kx = slider.isHorizontal() ? sliderPos : ((float)x + (float)width * 0.5f);
        auto ky = slider.isHorizontal() ? ((float)y + (float)height * 0.5f) : sliderPos;

        minPoint = startPoint;
        maxPoint = { kx, ky };

        auto thumbWidth = getSliderThumbRadius(slider);

        if (tss->getToggleState())
        {
            valueTrack.startNewSubPath(minPoint);
            valueTrack.lineTo(maxPoint);
            g.setColour(slider.findColour(juce::Slider::trackColourId));
            g.strokePath(valueTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });
            g.setColour(slider.findColour(juce::Slider::thumbColourId));
            g.fillEllipse(juce::Rectangle<float>(static_cast<float>(thumbWidth), static_cast<float>(thumbWidth)).withCentre(maxPoint));

            // Draw title on thumb
            if (tss->getTitle().isNotEmpty())
            {
                g.setColour(slider.findColour(juce::TextButton::textColourOnId));
                g.drawText(tss->getTitle(), juce::Rectangle<float>(static_cast<float>(thumbWidth), static_cast<float>(thumbWidth)).withCentre(maxPoint), juce::Justification::centred);
            }
        }
        else
        {
            valueTrack.startNewSubPath(minPoint);
            valueTrack.lineTo(maxPoint);
            auto valueTrackOutline = valueTrack;
            juce::PathStrokeType pt(trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);
            pt.createStrokedPath(valueTrackOutline, valueTrack);
            g.setColour(slider.findColour(juce::Slider::trackColourId));
            g.strokePath(valueTrackOutline, juce::PathStrokeType(1.0f));
            g.setColour(slider.findColour(juce::ResizableWindow::ColourIds::backgroundColourId));
            g.fillEllipse(juce::Rectangle<float>(static_cast<float>(thumbWidth - 1), static_cast<float>(thumbWidth - 1)).withCentre(maxPoint));
            g.setColour(slider.findColour(juce::Slider::thumbColourId));
            g.drawEllipse(juce::Rectangle<float>(static_cast<float>(thumbWidth), static_cast<float>(thumbWidth)).withCentre(maxPoint), 1.0f);
            
            // Draw title on thumb
            if (tss->getTitle().isNotEmpty())
            {
                g.drawText(tss->getTitle(), juce::Rectangle<float>(static_cast<float> (thumbWidth), static_cast<float> (thumbWidth)).withCentre(maxPoint), juce::Justification::centred);
            }
        }

        if (tss->isDragging())
        {
            juce::Rectangle<int> area;
            if (slider.isHorizontal())
            {
                auto centerPoint = maxPoint.withX(maxPoint.getX() + slider.getHeight()).toInt();
                if (maxPoint.getX() >= (slider.getWidth() - 2 * slider.getHeight()))
                    centerPoint = maxPoint.withX(maxPoint.getX() - slider.getHeight()).toInt();
                area = juce::Rectangle<int>(slider.getHeight(), slider.getHeight()).withCentre(centerPoint);
            }
            else
            {
                area = juce::Rectangle<int>(slider.getWidth(), slider.getWidth()).withCentre(maxPoint.withY(maxPoint.getY() - slider.getWidth() / 2).toInt());
            }
            g.setColour(slider.findColour(juce::TextButton::textColourOnId));
            g.drawFittedText(tss->displayValueConverter ? tss->displayValueConverter(tss->getValue()) : juce::String(tss->getValue()),
                area,
                juce::Justification::centred, 1);
        }

        if (!slider.isEnabled())
            g.endTransparencyLayer();
    }
    else
    {
        // Fallback to default JUCE rotary slider
        juce::LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
    }
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    auto tss = dynamic_cast<ToggleStateSlider*>(&slider);
    if (tss)
    {
        // disabled is drawn at reduced opacity as a whole
        if (!slider.isEnabled())
            g.beginTransparencyLayer(Theme::Alpha::disabled);

        auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(2.0f);
        if (bounds.getAspectRatio() > 1.0f)
            bounds.reduce((bounds.getWidth() - bounds.getHeight()) / 2, 0.0f);
        else if (bounds.getAspectRatio() < 1.0f)
            bounds.reduce(0.0f, (bounds.getHeight() - bounds.getWidth()) / 2);
        auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
        auto center = bounds.getCentre();
        auto arcRadius = radius - radius * 0.35f;
        auto thumbRadius = radius * 0.32f;

        // Draw outer frame (like the oval surrounding frame in linear slider)
        juce::Path outerArc;
        outerArc.addEllipse(bounds);
        g.setColour(slider.findColour(juce::Slider::thumbColourId));
        g.strokePath(outerArc, juce::PathStrokeType(1.0f));
        drawToggleSliderStates(g, slider, outerArc, findColour(juce::TextEditor::ColourIds::focusedOutlineColourId));

        // Draw background track
        juce::Path backgroundArc;
        backgroundArc.addCentredArc(center.x, center.y, arcRadius, arcRadius, 0.0f,
            rotaryStartAngle, rotaryEndAngle, true);
        g.setColour(slider.findColour(juce::Slider::backgroundColourId));
        g.strokePath(backgroundArc, juce::PathStrokeType(radius * 0.3f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Draw value track
        if (tss->getToggleState())
        {
            // Filled track mode
            juce::Path valueArc;
            valueArc.addCentredArc(center.x, center.y, arcRadius, arcRadius, 0.0f,
                rotaryStartAngle, toAngle, true);
            g.setColour(slider.findColour(juce::Slider::trackColourId));
            g.strokePath(valueArc, juce::PathStrokeType(radius * 0.3f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }
        else
        {
            // Outline track mode
            juce::Path valueArc;
            valueArc.addCentredArc(center.x, center.y, arcRadius, arcRadius, 0.0f,
                rotaryStartAngle, toAngle, true);
            g.setColour(slider.findColour(juce::Slider::trackColourId));
            g.strokePath(valueArc, juce::PathStrokeType(1.0f));
        }

        // Calculate thumb position
        auto thumbPoint = center.getPointOnCircumference(arcRadius, toAngle);

        // Draw thumb
        if (tss->getToggleState())
        {
            // Filled thumb
            g.setColour(slider.findColour(juce::Slider::thumbColourId));
            g.fillEllipse(juce::Rectangle<float>(thumbRadius * 2.0f, thumbRadius * 2.0f).withCentre(thumbPoint));

            // Draw title on thumb
            if (tss->getTitle().isNotEmpty())
            {
                g.setColour(slider.findColour(juce::TextButton::textColourOnId));
                g.drawText(tss->getTitle(),
                    juce::Rectangle<float>(thumbRadius * 2.0f, thumbRadius * 2.0f).withCentre(thumbPoint),
                    juce::Justification::centred);
            }
        }
        else
        {
            // Outline thumb with background fill
            g.setColour(slider.findColour(juce::ResizableWindow::ColourIds::backgroundColourId));
            g.fillEllipse(juce::Rectangle<float>((thumbRadius - 0.5f) * 2.0f, (thumbRadius - 0.5f) * 2.0f).withCentre(thumbPoint));

            g.setColour(slider.findColour(juce::Slider::thumbColourId));
            g.drawEllipse(juce::Rectangle<float>(thumbRadius * 2.0f, thumbRadius * 2.0f).withCentre(thumbPoint), 1.0f);

            // Draw title on thumb
            g.drawText(tss->getTitle(),
                juce::Rectangle<float>(thumbRadius * 2.0f, thumbRadius * 2.0f).withCentre(thumbPoint),
                juce::Justification::centred);
        }

        // Draw value when dragging
        if (tss->isDragging())
        {
            auto valueText = tss->displayValueConverter ? tss->displayValueConverter(tss->getValue()) : juce::String(tss->getValue());

            // Position the value display in the center of the rotary slider
            auto valueArea = juce::Rectangle<float>(radius * 1.2f, radius * 0.4f).withCentre(center);

            g.setColour(slider.findColour(juce::TextButton::textColourOnId));
            g.drawFittedText(valueText, valueArea.toNearestInt(), juce::Justification::centred, 1);
        }

        if (!slider.isEnabled())
            g.endTransparencyLayer();
    }
    else
    {
        // Fallback to default JUCE rotary slider
        juce::LookAndFeel_V4::drawRotarySlider(g, x, y, width, height, sliderPosProportional, rotaryStartAngle, rotaryEndAngle, slider);
    }
}

void CustomLookAndFeel::drawTreeviewPlusMinusBox(juce::Graphics& g, const juce::Rectangle<float>& area,
    juce::Colour backgroundColour, bool isOpen, bool isMouseOver)
{
    juce::ignoreUnused(backgroundColour);

    juce::Path p;
    p.addTriangle(0.0f, 0.0f, 1.0f, isOpen ? 0.0f : 0.5f, isOpen ? 0.5f : 0.0f, 1.0f);
    //p = p.createPathWithRoundedCorners(1);
    g.setColour(findColour(juce::TreeView::ColourIds::linesColourId).withAlpha(isMouseOver ? 0.7f : 1.0f));
    //g.strokePath(p, juce::PathStrokeType(2), p.getTransformToScaleToFit(area.reduced(2, area.getHeight() / 4), true));
    g.fillPath(p, p.getTransformToScaleToFit(area.reduced(2, area.getHeight() / 4), true));
}

bool CustomLookAndFeel::areLinesDrawnForTreeView(juce::TreeView&)
{
    return true;
}

int CustomLookAndFeel::getTreeViewIndentSize(juce::TreeView&)
{
    return 15;
}

void CustomLookAndFeel::drawPopupMenuBackground(juce::Graphics& g, [[maybe_unused]] int width, [[maybe_unused]] int height)
{
    g.fillAll(findColour(juce::PopupMenu::backgroundColourId));
}


}
