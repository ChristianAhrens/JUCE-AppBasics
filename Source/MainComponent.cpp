/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#include "iOS_utils.hpp"
#include "Image_utils.hpp"

namespace AppBasicsDemo
{

//==============================================================================
MainComponent::MainComponent()
{
	m_header = std::make_unique<DemoHeaderFooterComponent>();
	addAndMakeVisible(m_header.get());
	m_body = std::make_unique<DemoBodyComponent>();
	addAndMakeVisible(m_body.get());
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
        {BinaryData::folder_open24px_svg} };

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

	setSize(300, 400);
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint (Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
	auto panelDefaultSize = 40.0f;

    auto safety = JUCEAppBasics::iOS_utils::getDeviceSafetyMargins();

	FlexBox fb;
	fb.flexDirection = FlexBox::Direction::column;
	fb.items.addArray({
        FlexItem(*m_header.get()).withFlex(1).withMaxHeight(panelDefaultSize + safety._top),
        FlexItem(*m_splitButton.get()).withFlex(1).withMargin(juce::FlexItem::Margin(1, 1, 1, 1)),
        FlexItem(*m_body.get()).withFlex(4),
        FlexItem(*m_footer.get()).withFlex(1).withMaxHeight(panelDefaultSize + safety._bottom) });

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
