/*
  ==============================================================================

    MainComponent.cpp
    Created: 28 Jul 2020 11:21:42am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "MainComponent.h"

#include "../../Source/iOS_utils.hpp"
#include "../../Source/Image_utils.hpp"

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
    m_zeroconf->addDiscoverService(JUCEAppBasics::ZeroconfDiscoverComponent::ZeroconfServiceType::ZST_OSC, 50013);
    m_zeroconf->addDiscoverService(JUCEAppBasics::ZeroconfDiscoverComponent::ZeroconfServiceType::ZST_OCA, 50014);
    m_zeroconf->onServiceSelected = [=](JUCEAppBasics::ZeroconfDiscoverComponent::ZeroconfServiceType type, JUCEAppBasics::ZeroconfDiscoverComponent::ServiceInfo* info) { handleServiceSelected(type, info); };
    addAndMakeVisible(m_zeroconf.get());
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
        {BinaryData::fast_rewind24px_svg},
        {BinaryData::skip_previous24px_svg},
        {BinaryData::play_arrow24px_svg},
        {BinaryData::pause24px_svg},
        {BinaryData::stop24px_svg},
        {BinaryData::skip_next24px_svg},
        {BinaryData::fast_forward24px_svg},
        {BinaryData::find_replace24px_svg},
        {BinaryData::open_in_full24px_svg},
        {BinaryData::close_fullscreen24px_svg},
        {BinaryData::equalizer24px_svg},
        {BinaryData::graphic_eq24px_svg},
        {BinaryData::grid_on24px_svg},
        {BinaryData::mediation24px_svg},
        {BinaryData::radio_button_checked24px_svg},
        {BinaryData::radio_button_unchecked24px_svg},
        {BinaryData::settings24px_svg},
        {BinaryData::show_chart24px_svg},
        {BinaryData::track_changes24px_svg},
        {BinaryData::waves24px_svg},
        {BinaryData::folder_open24px_svg},
        {BinaryData::grain24px_svg},
        {BinaryData::vertical_split24px_svg} };

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

	setSize(300, 420);
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
	auto panelDefaultSize = 40.0f;

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
            FlexItem(*m_header.get())       .withFlex(1).withMaxHeight(panelDefaultSize + safety._top),
            FlexItem(*m_splitButton.get())  .withFlex(1).withMargin(juce::FlexItem::Margin(1, 1, 1, 1)),
            FlexItem(*m_body.get())         .withFlex(5),
            FlexItem(*m_zeroconf.get())     .withFlex(1),
            FlexItem(*m_overlay.get())      .withFlex(1).withMargin(juce::FlexItem::Margin(10, 10, 10, 10)),
            FlexItem(*m_footer.get())       .withFlex(1).withMaxHeight(panelDefaultSize + safety._bottom) });

        fb.performLayout(getLocalBounds().toFloat());

        Grid buttonGrid;
        buttonGrid.templateRows = { Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr) };
        buttonGrid.templateColumns = { Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr), Grid::TrackInfo(1_fr) };
        for (auto const& button : m_buttons)
        {
            buttonGrid.items.add(GridItem(button.get()).withMargin(juce::GridItem::Margin(1, 1, 1, 1)));
        }
        buttonGrid.performLayout(m_body->getBounds().reduced(15));
    }
}

void MainComponent::handleServiceSelected(JUCEAppBasics::ZeroconfDiscoverComponent::ZeroconfServiceType type, JUCEAppBasics::ZeroconfDiscoverComponent::ServiceInfo* info)
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