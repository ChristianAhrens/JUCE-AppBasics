#include "ThemeGalleryComponent.h"

#include <ToggleStateSlider.h>

namespace JUCEAppBasics
{

namespace
{

/** Icon button the way the apps build them: SVG recoloured from black to TextButton::textColourOnId on every look and feel change. */
class GalleryIconButton : public juce::DrawableButton
{
public:
    GalleryIconButton(const juce::String& name, juce::DrawableButton::ButtonStyle style, const char* svgData)
        : juce::DrawableButton(name, style), m_svgData(svgData)
    {
        setClickingTogglesState(juce::DrawableButton::ImageOnButtonBackground == style);
        lookAndFeelChanged();
    }

    void lookAndFeelChanged() override
    {
        auto drawable = juce::Drawable::createFromSVGString(m_svgData);
        if (nullptr != drawable)
        {
            drawable->replaceColour(juce::Colours::black, findColour(juce::TextButton::ColourIds::textColourOnId));
            setImages(drawable.get());
        }
        setColour(juce::DrawableButton::ColourIds::backgroundColourId, juce::Colours::transparentBlack);
        setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colours::transparentBlack);
    }

private:
    juce::String m_svgData;
};

/** Colour swatches of the main roles of the current palette. */
class SwatchStrip : public juce::Component
{
public:
    SwatchStrip()
    {
        m_swatches = {
            { "window",     juce::LookAndFeel_V4::ColourScheme::windowBackground },
            { "recessed",   juce::ResizableWindow::ColourIds::backgroundColourId },
            { "control",    juce::TextButton::ColourIds::buttonColourId },
            { "controlOn",  juce::TextButton::ColourIds::buttonOnColourId },
            { "text",       juce::TextButton::ColourIds::textColourOnId },
            { "outline",    juce::LookAndFeel_V4::ColourScheme::outline },
            { "accent",     CustomLookAndFeel::MeteringRmsColourId },
            { "peak",       CustomLookAndFeel::MeteringPeakColourId },
            { "hold",       CustomLookAndFeel::MeteringHoldColourId },
        };
    }

    void paint(juce::Graphics& g) override
    {
        const auto w = getWidth() / static_cast<int>(m_swatches.size());
        auto x = 0;
        for (const auto& s : m_swatches)
        {
            auto area = juce::Rectangle<int>(x, 0, w - 2, getHeight());
            g.setColour(findColour(s.second));
            g.fillRect(area);
            g.setColour(findColour(juce::LookAndFeel_V4::ColourScheme::outline));
            g.drawRect(area, 1);
            g.setColour(findColour(s.second).contrasting());
            g.setFont(juce::FontOptions(11.0f));
            g.drawText(s.first, area, juce::Justification::centred);
            x += w;
        }
    }

private:
    std::vector<std::pair<juce::String, int>> m_swatches;
};

/** Tree item that paints with the colours of its owning TreeView, like the service topology in Mema. */
class TreeItem : public juce::TreeViewItem
{
public:
    TreeItem(const juce::String& text, int numChildren) : m_text(text), m_numChildren(numChildren) {}
    bool mightContainSubItems() override { return 0 < m_numChildren; }
    void itemOpennessChanged(bool isNowOpen) override
    {
        if (isNowOpen && 0 == getNumSubItems())
            for (auto i = 0; i < m_numChildren; ++i)
                addSubItem(new TreeItem(m_text + " / child " + juce::String(i + 1), 0));
    }
    void paintItem(juce::Graphics& g, int width, int height) override
    {
        if (nullptr == getOwnerView())
            return;
        g.setColour(getOwnerView()->findColour(juce::ListBox::textColourId));
        g.drawText(m_text, 2, 0, width - 2, height, juce::Justification::centredLeft);
    }
private:
    juce::String m_text;
    int          m_numChildren;
};

}

struct ThemeGalleryPanel::Row
{
    juce::Label                 title;
    std::vector<juce::Component*> components;
    int                         height = 34;
    int                         componentWidth = 110;
};

ThemeGalleryPanel::ThemeGalleryPanel(CustomLookAndFeel::PaletteStyle style, const juce::String& title)
    : m_style(style), m_lookAndFeel(style), m_title(title)
{
    setLookAndFeel(&m_lookAndFeel);
    m_tooltipWindow = std::make_unique<juce::TooltipWindow>(this, 300);

    const auto sampleItems = [](juce::ComboBox& box) {
        box.addItem("Combo item 1", 1);
        box.addItem("Combo item 2", 2);
        box.setSelectedId(1, juce::dontSendNotification);
    };

    // roles
    addRow("Roles", { own<SwatchStrip>() }, 34, 440);

    // text button
    auto* normal = own<juce::TextButton>("Normal");
    auto* on = own<juce::TextButton>("On");
    on->setClickingTogglesState(true);
    on->setToggleState(true, juce::dontSendNotification);
    auto* disabled = own<juce::TextButton>("Disabled");
    disabled->setEnabled(false);
    auto* mute = own<juce::TextButton>("M");
    mute->setClickingTogglesState(true);
    mute->setToggleState(true, juce::dontSendNotification);
    mute->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::red);
    addRow("Text button", { normal, on, disabled, mute }, 34, 80);

    // icon button
    auto* settings = own<GalleryIconButton>("settings", juce::DrawableButton::ImageFitted, BinaryData::settings_24dp_svg);
    auto* help = own<GalleryIconButton>("help", juce::DrawableButton::ImageFitted, BinaryData::help24px_svg);
    auto* power = own<GalleryIconButton>("power", juce::DrawableButton::ImageOnButtonBackground, BinaryData::power_settings_24dp_svg);
    auto* linkOff = own<GalleryIconButton>("linkoff", juce::DrawableButton::ImageFitted, BinaryData::link_off_24dp_svg);
    linkOff->setEnabled(false);
    settings->setTooltip("Tooltip of an icon button");
    addRow("Icon button", { settings, help, power, linkOff }, 40, 40);

    // toggle state slider
    auto* rotaryOn = own<ToggleStateSlider>(juce::Slider::Rotary, juce::Slider::NoTextBox);
    auto* rotaryOff = own<ToggleStateSlider>(juce::Slider::Rotary, juce::Slider::NoTextBox);
    auto* linearOn = own<ToggleStateSlider>(juce::Slider::LinearVertical, juce::Slider::NoTextBox);
    auto* linearOff = own<ToggleStateSlider>(juce::Slider::LinearVertical, juce::Slider::NoTextBox);
    for (auto* s : { rotaryOn, rotaryOff, linearOn, linearOff })
    {
        s->setRange(0.0, 1.0, 0.01);
        s->setValue(0.6, juce::dontSendNotification);
        m_accentSliders.push_back(s);
    }
    rotaryOff->setToggleState(false, juce::dontSendNotification);
    linearOff->setToggleState(false, juce::dontSendNotification);
    addRow("Toggle slider", { rotaryOn, rotaryOff, linearOn, linearOff }, 70, 70);

    // plain slider
    auto* slider = own<juce::Slider>(juce::Slider::LinearHorizontal, juce::Slider::NoTextBox);
    slider->setRange(0.0, 1.0, 0.01);
    slider->setValue(0.4, juce::dontSendNotification);
    addRow("Slider", { slider }, 34, 200);

    // combo box
    auto* combo = own<juce::ComboBox>("combo");
    sampleItems(*combo);
    auto* comboDisabled = own<juce::ComboBox>("comboDisabled");
    sampleItems(*comboDisabled);
    comboDisabled->setEnabled(false);
    addRow("Combo box", { combo, comboDisabled }, 34, 140);

    // tick box
    auto* tickOn = own<juce::ToggleButton>("On");
    tickOn->setToggleState(true, juce::dontSendNotification);
    auto* tickOff = own<juce::ToggleButton>("Off");
    auto* tickDisabled = own<juce::ToggleButton>("Disabled");
    tickDisabled->setEnabled(false);
    addRow("Tick box", { tickOn, tickOff, tickDisabled }, 34, 100);

    // text editor
    auto* editor = own<juce::TextEditor>("editor");
    editor->setText("Text editor", false);
    addRow("Text editor", { editor }, 34, 200);

    // popup menu, callout, alert
    auto* menuButton = own<juce::TextButton>("Menu");
    menuButton->onClick = [menuButton]() {
        juce::PopupMenu sub;
        sub.addItem(10, "Sub item 1", true, true);
        sub.addItem(11, "Sub item 2");
        juce::PopupMenu menu;
        menu.addItem(1, "Item");
        menu.addItem(2, "Ticked item", true, true);
        menu.addItem(3, "Disabled item", false);
        menu.addSeparator();
        menu.addSubMenu("Sub menu", sub);
        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(menuButton));
    };
    auto* calloutButton = own<juce::TextButton>("Callout");
    calloutButton->onClick = [this, calloutButton]() {
        auto content = std::make_unique<juce::Label>(juce::String(), "Callout content");
        content->setSize(200, 60);
        content->setJustificationType(juce::Justification::centred);
        juce::CallOutBox::launchAsynchronously(std::move(content), getLocalArea(calloutButton, calloutButton->getLocalBounds()), this);
    };
    auto* alertButton = own<juce::TextButton>("Alert");
    alertButton->onClick = [this]() {
        auto* aw = new juce::AlertWindow("Alert title", "Alert message text.", juce::MessageBoxIconType::InfoIcon, this);
        aw->setLookAndFeel(&m_lookAndFeel);
        aw->addButton("Ok", 1, juce::KeyPress(juce::KeyPress::returnKey));
        aw->addButton("Cancel", 0, juce::KeyPress(juce::KeyPress::escapeKey));
        aw->enterModalState(true, nullptr, true);
    };
    addRow("Popups", { menuButton, calloutButton, alertButton }, 34, 80);

    // tooltip, hyperlink
    auto* tipButton = own<juce::TextButton>("Hover me");
    tipButton->setTooltip("Tooltip text");
    auto* link = own<juce::HyperlinkButton>("Hyperlink", juce::URL("https://github.com/ChristianAhrens/Mema"));
    addRow("Tooltip, link", { tipButton, link }, 34, 130);

    // scroll bar
    auto* hBar = own<juce::ScrollBar>(false);
    hBar->setRangeLimits(0.0, 100.0);
    hBar->setCurrentRange(10.0, 30.0);
    hBar->setAutoHide(false);
    addRow("Scroll bar", { hBar }, 34, 160);

    // tree view (the list box is not shown yet, see ThemeGallery/README.md)
    m_treeRoot = std::make_unique<TreeItem>("Tree root", 2);
    auto* tree = own<juce::TreeView>("tree");
    tree->setRootItem(m_treeRoot.get());
    tree->setRootItemVisible(true);
    m_treeRoot->setOpen(true);
    addRow("Tree", { tree }, 90, 260);
}

ThemeGalleryPanel::~ThemeGalleryPanel()
{
    for (auto& c : m_owned)
    {
        if (auto* tree = dynamic_cast<juce::TreeView*>(c.get()))
            tree->setRootItem(nullptr);
    }
    setLookAndFeel(nullptr);
}

void ThemeGalleryPanel::addRow(const juce::String& title, std::vector<juce::Component*> components, int height, int componentWidth)
{
    auto row = std::make_unique<Row>();
    row->title.setText(title, juce::dontSendNotification);
    row->title.setFont(juce::FontOptions(12.0f));
    row->components = std::move(components);
    row->height = height;
    row->componentWidth = componentWidth;
    addAndMakeVisible(row->title);
    m_rows.push_back(std::move(row));
}

void ThemeGalleryPanel::setAccentColour(const juce::Colour& accent)
{
    m_accent = accent;
    m_lookAndFeel.setColour(CustomLookAndFeel::MeteringRmsColourId, accent);
    m_lookAndFeel.setColour(CustomLookAndFeel::MeteringPeakColourId, CustomLookAndFeel::PS_Dark == m_style ? accent.darker() : accent.brighter());
    for (auto* s : m_accentSliders)
    {
        s->setColour(juce::Slider::trackColourId, accent);
        s->setColour(juce::Slider::thumbColourId, accent);
    }
    sendLookAndFeelChange();
    repaint();
}

int ThemeGalleryPanel::getRequiredHeight() const
{
    auto h = 40;
    for (const auto& r : m_rows)
        h += r->height + 6;
    return h + 10;
}

void ThemeGalleryPanel::paint(juce::Graphics& g)
{
    g.fillAll(findColour(juce::ResizableWindow::ColourIds::backgroundColourId));
    g.setColour(findColour(juce::TextButton::ColourIds::textColourOnId));
    g.setFont(juce::FontOptions(16.0f).withStyle("Bold"));
    g.drawText(m_title, 10, 6, getWidth() - 20, 28, juce::Justification::centredLeft);
}

void ThemeGalleryPanel::resized()
{
    auto y = 40;
    constexpr auto labelWidth = 120;
    for (auto& r : m_rows)
    {
        r->title.setBounds(8, y, labelWidth, r->height);
        r->title.setColour(juce::Label::textColourId, findColour(juce::Label::textColourId));
        auto x = labelWidth + 16;
        for (auto* c : r->components)
        {
            auto w = r->componentWidth;
            if (auto* bar = dynamic_cast<juce::ScrollBar*>(c))
            {
                if (bar->isVertical())
                {
                    w = 16;
                    c->setBounds(x, y, w, r->height);
                }
                else
                    c->setBounds(x, y + (r->height - 16) / 2, w, 16);
            }
            else
                c->setBounds(x, y, w, r->height);
            x += w + 8;
        }
        y += r->height + 6;
    }
}

//==============================================================================
ThemeGalleryComponent::ThemeGalleryComponent()
    : m_darkPanel(CustomLookAndFeel::PS_Dark, "Dark palette"), m_lightPanel(CustomLookAndFeel::PS_Light, "Light palette")
{
    m_accentSelector.addItem("Green", 1);
    m_accentSelector.addItem("Red", 2);
    m_accentSelector.addItem("Blue", 3);
    m_accentSelector.addItem("Anni Pink", 4);
    m_accentSelector.addItem("Laser", 5);
    m_accentSelector.onChange = [this]() {
        static const juce::Colour accents[] = { juce::Colours::forestgreen, juce::Colours::orangered, juce::Colours::dodgerblue,
                                                juce::Colours::deeppink, juce::Colour(0xd1, 0xff, 0x4f) };
        const auto idx = m_accentSelector.getSelectedId() - 1;
        if (0 <= idx && idx < 5)
        {
            m_darkPanel.setAccentColour(accents[idx]);
            m_lightPanel.setAccentColour(accents[idx]);
        }
    };
    addAndMakeVisible(m_accentSelector);

    m_darkViewport.setViewedComponent(&m_darkPanel, false);
    m_lightViewport.setViewedComponent(&m_lightPanel, false);
    m_darkViewport.setScrollBarsShown(true, false);
    m_lightViewport.setScrollBarsShown(true, false);
    addAndMakeVisible(m_darkViewport);
    addAndMakeVisible(m_lightViewport);

    setSize(1160, 900);
    m_accentSelector.setSelectedId(1);
}

ThemeGalleryComponent::~ThemeGalleryComponent()
{
    m_darkViewport.setViewedComponent(nullptr, false);
    m_lightViewport.setViewedComponent(nullptr, false);
}

void ThemeGalleryComponent::resized()
{
    auto area = getLocalBounds();
    m_accentSelector.setBounds(area.removeFromTop(30).reduced(4).removeFromLeft(160));
    auto half = area.getWidth() / 2;
    m_darkViewport.setBounds(area.removeFromLeft(half));
    m_lightViewport.setBounds(area);
    m_darkPanel.setSize(m_darkViewport.getMaximumVisibleWidth(), m_darkPanel.getRequiredHeight());
    m_lightPanel.setSize(m_lightViewport.getMaximumVisibleWidth(), m_lightPanel.getRequiredHeight());
}

}
