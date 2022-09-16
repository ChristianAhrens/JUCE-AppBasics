/*
  ==============================================================================

    MainComponent.cpp
    Created: 28 Jul 2020 11:21:42am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "MainComponent.h"

#include "../../Source/iOS_utils.h"
#include "../../Source/Image_utils.h"

namespace AppBasicsDemo
{

//==============================================================================
MainComponent::MainComponent()
{
	m_header = std::make_unique<DemoHeaderFooterComponent>();
	addAndMakeVisible(m_header.get());

	m_body = std::make_unique<DemoBodyComponent>();
	addAndMakeVisible(m_body.get());

    m_zeroconf = std::make_unique<JUCEAppBasics::ZeroconfDiscoverComponent>(true, true);
    m_zeroconf->addDiscoverService(JUCEAppBasics::ZeroconfDiscoverComponent::ZeroconfServiceType::ZST_OSC);
    m_zeroconf->addDiscoverService(JUCEAppBasics::ZeroconfDiscoverComponent::ZeroconfServiceType::ZST_OCA);
    m_zeroconf->onServiceSelected = [=](JUCEAppBasics::ZeroconfDiscoverComponent::ZeroconfServiceType type, ZeroconfSearcher::ZeroconfSearcher::ServiceInfo* info) { handleServiceSelected(type, info); };
    addAndMakeVisible(m_zeroconf.get());

    m_midiLearner = std::make_unique<JUCEAppBasics::MidiLearnerComponent>();
    addAndMakeVisible(m_midiLearner.get());

    m_colourAndSizePicker = std::make_unique<JUCEAppBasics::ColourAndSizePickerComponent>();
    int randNr = std::rand();
    m_colourAndSizePicker->setCurrentColourAndSize(juce::Colour(juce::uint8(randNr * 111), juce::uint8(randNr * 222), juce::uint8(randNr * 333)), 0.5f);
    m_colourAndSizePicker->onColourAndSizeSet = [=](const juce::Colour& colour, double size) {
        DBG("Colour and size picker selected c:" + colour.toDisplayString(true) + " s:" + String(size));
    };
    addAndMakeVisible(m_colourAndSizePicker.get());

    m_overlay = std::make_unique<DemoOverlayComponent>();
    m_overlay->addOverlayParent(this);
    m_overlay->parentResize = [this] { resized(); };
    addAndMakeVisible(m_overlay.get());

	m_footer = std::make_unique<DemoHeaderFooterComponent>();
	addAndMakeVisible(m_footer.get());

    m_splitButton = std::make_unique<JUCEAppBasics::SplitButtonComponent>();
    m_splitButton->addButtons({ "A", "B", "C" });
    m_splitButton->setButtonDown("A");
    addAndMakeVisible(m_splitButton.get());

    std::vector<String> svgImages{
        {BinaryData::add_batch24dp_svg},
        {BinaryData::add24px_svg},
        {BinaryData::add_circle24px_svg},
        {BinaryData::add_circle_outline24px_svg},
        {BinaryData::add_photo_alternate_black_24dp_svg},
        {BinaryData::adjust_black_24dp_svg},
        {BinaryData::art_track_black_24dp_svg},
        {BinaryData::brush_black_24dp_svg},
        {BinaryData::call_made24px_svg},
        {BinaryData::call_received24px_svg},
        {BinaryData::cancel24px_svg},
        {BinaryData::cast24px_svg},
        {BinaryData::clear_black_24dp_svg},
        {BinaryData::close_fullscreen24px_svg},
        {BinaryData::color_lens_black_24dp_svg},
        {BinaryData::colorize_black_24dp_svg},
        {BinaryData::compare_black_24dp_svg},
        {BinaryData::download_black_24dp_svg},
        {BinaryData::equalizer24px_svg},
        {BinaryData::fast_forward24px_svg},
        {BinaryData::fast_rewind24px_svg},
        {BinaryData::find_replace24px_svg},
        {BinaryData::flip_black_24dp_svg},
        {BinaryData::folder_open24px_svg},
        {BinaryData::grain24px_svg},
        {BinaryData::graphic_eq24px_svg},
        {BinaryData::grid_on24px_svg},
        {BinaryData::height24px_svg},
        {BinaryData::help24px_svg},
        {BinaryData::hide_image_black_24dp_svg},
        {BinaryData::image_black_24dp_svg},
        {BinaryData::input24px_svg},
        {BinaryData::insert_chart_outlined24px_svg},
        {BinaryData::keyboard_arrow_down24px_svg},
        {BinaryData::keyboard_arrow_left24px_svg},
        {BinaryData::keyboard_arrow_right24px_svg},
        {BinaryData::keyboard_arrow_up24px_svg},
        {BinaryData::mediation24px_svg},
        {BinaryData::mobiledata_off24px_svg},
        {BinaryData::open_in_full24px_svg},
        {BinaryData::open_in_new24px_svg},
        {BinaryData::outbond24px_svg},
        {BinaryData::pause24px_svg},
        {BinaryData::play_arrow24px_svg},
        {BinaryData::push_pin_black_24dp_svg},
        {BinaryData::radio_button_checked24px_svg},
        {BinaryData::radio_button_unchecked24px_svg},
        {BinaryData::remove24px_svg},
        {BinaryData::remove_circle24px_svg},
        {BinaryData::remove_circle_outline24px_svg},
        {BinaryData::rule24px_svg},
        {BinaryData::rule_checked24px_svg},
        {BinaryData::rule_unchecked24px_svg},
        {BinaryData::rule_one24px_svg},
        {BinaryData::school24px_svg},
        {BinaryData::select_all24px_svg},
        {BinaryData::sensors_black_24dp_svg},
        {BinaryData::settings24px_svg},
        {BinaryData::show_chart24px_svg},
        {BinaryData::skip_next24px_svg},
        {BinaryData::skip_previous24px_svg},
        {BinaryData::slideshow_black_24dp_svg},
        {BinaryData::stop24px_svg},
        {BinaryData::text_fields_black_24dp_svg},
        {BinaryData::track_changes24px_svg},
        {BinaryData::tune24px_svg},
        {BinaryData::vertical_split24px_svg},
        {BinaryData::video_library_black_24dp_svg},
        {BinaryData::volume_off24px_svg},
        {BinaryData::waves24px_svg}, };

    for(auto imageName : svgImages)
    {
        m_buttons.push_back(std::make_unique<DrawableButton>(String(), DrawableButton::ButtonStyle::ImageFitted));
        m_buttons.back()->setClickingTogglesState(true);

        std::unique_ptr<juce::Drawable> NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage;
        JUCEAppBasics::Image_utils::getDrawableButtonImages(imageName, NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage);

        // set the images to button
        m_buttons.back()->setImages(NormalImage.get(), OverImage.get(), DownImage.get(), DisabledImage.get(), NormalOnImage.get(), OverOnImage.get(), DownOnImage.get(), DisabledOnImage.get());
        addAndMakeVisible(m_buttons.back().get());
    }

    m_config = std::make_unique<AppConfig>(AppConfig::getDefaultConfigFilePath());
    m_config->triggerWatcherUpdate();

	setSize(300, 440);
}

MainComponent::~MainComponent()
{
    m_config->triggerWatcherUpdate();
}

void MainComponent::paint (Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
}

void MainComponent::resized()
{
	auto panelDefaultSize = 45.0f;

    auto safety = JUCEAppBasics::iOS_utils::getDeviceSafetyMargins();

    if (isOverlayActive())
    {
        auto safeBounds = getLocalBounds();
        safeBounds.removeFromTop(safety._top);
        safeBounds.removeFromBottom(safety._bottom);
        safeBounds.removeFromLeft(safety._left);
        safeBounds.removeFromRight(safety._right);
        m_activeOverlay->setBounds(safeBounds);
    }
    else
    {
        FlexBox fb;
        fb.flexDirection = FlexBox::Direction::column;
        fb.items.addArray({
            FlexItem(*m_header.get())               .withFlex(1).withMaxHeight(panelDefaultSize + safety._top),
            FlexItem(*m_splitButton.get())          .withFlex(1).withMargin(juce::FlexItem::Margin(1, 1, 1, 1)),
            FlexItem(*m_body.get())                 .withFlex(5),
            FlexItem(*m_zeroconf.get())             .withFlex(1),
            FlexItem(*m_midiLearner.get())          .withFlex(1),
            FlexItem(*m_colourAndSizePicker.get())  .withFlex(1),
            FlexItem(*m_overlay.get())              .withFlex(1).withMargin(juce::FlexItem::Margin(10, 10, 10, 10)),
            FlexItem(*m_footer.get())               .withFlex(1).withMaxHeight(panelDefaultSize + safety._bottom) });

        fb.performLayout(getLocalBounds().toFloat());

        Grid buttonGrid;
        buttonGrid.templateRows = { Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr) };
        buttonGrid.templateColumns = { Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr) };
        for (auto const& button : m_buttons)
        {
            buttonGrid.items.add(GridItem(button.get()).withMargin(juce::GridItem::Margin(1, 1, 1, 1)));
        }
        buttonGrid.performLayout(m_body->getBounds().reduced(15));
    }
}

void MainComponent::handleServiceSelected(JUCEAppBasics::ZeroconfDiscoverComponent::ZeroconfServiceType type, ZeroconfSearcher::ZeroconfSearcher::ServiceInfo* info)
{
    auto serviceName = JUCEAppBasics::ZeroconfDiscoverComponent::getServiceName(type);
    if (info)
    {
        DBG(serviceName + " clicked, "
            + String(info->host) + " "
            + String(info->ip) + " "
            + String(info->name) + " "
            + String(info->port) + " "
            + " selected.");
    }
    else
    {
        DBG(serviceName + " clicked, no service selected.");
    }
}

}
