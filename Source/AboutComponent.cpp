#include "AboutComponent.h"
#include "Theme.h"

namespace JUCEAppBasics
{

AboutComponent::AboutComponent(const char* imageData, int imageDataSize, const juce::String& repoUrl)
    : juce::Component()
{
	m_appIcon = std::make_unique<juce::DrawableButton>("App Icon", juce::DrawableButton::ButtonStyle::ImageFitted);
	m_appIcon->setColour(juce::DrawableButton::ColourIds::backgroundColourId, juce::Colours::transparentBlack);
	m_appIcon->setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colours::transparentBlack);
	m_appIcon->setImages(juce::Drawable::createFromImageData(imageData, imageDataSize).get());
	addAndMakeVisible(m_appIcon.get());

	m_appInfoLabel = std::make_unique<juce::Label>("Version", juce::JUCEApplication::getInstance()->getApplicationName() + " " + juce::JUCEApplication::getInstance()->getApplicationVersion());
	m_appInfoLabel->setJustificationType(juce::Justification::centredBottom);
	m_appInfoLabel->setFont(juce::Font(juce::FontOptions(Theme::Type::dialogBody, juce::Font::plain)));
	addAndMakeVisible(m_appInfoLabel.get());

	m_appRepoLink = std::make_unique<juce::HyperlinkButton>(juce::JUCEApplication::getInstance()->getApplicationName() + juce::String(" on GitHub"), juce::URL(repoUrl));
	m_appRepoLink->setFont(juce::Font(juce::FontOptions(Theme::Type::dialogBody, juce::Font::plain).withUnderline()), false /* do not resize */); // underlined, since the link colour is the plain text colour
	m_appRepoLink->setJustificationType(juce::Justification::centredTop);
	addAndMakeVisible(m_appRepoLink.get());
}

AboutComponent::~AboutComponent()
{
}

void AboutComponent::paint(juce::Graphics &g)
{
	g.fillAll(getLookAndFeel().findColour(juce::DrawableButton::backgroundColourId));

	juce::Component::paint(g);
}

void AboutComponent::resized()
{
	auto bounds = getLocalBounds();
	auto margin = bounds.getHeight() / 8;
	bounds.reduce(margin, margin);
	auto iconBounds = bounds.removeFromTop(bounds.getHeight() / 2);
	auto infoBounds = bounds.removeFromTop(bounds.getHeight() / 2);
	auto& repoLinkBounds = bounds;

	m_appIcon->setBounds(iconBounds);
	m_appInfoLabel->setBounds(infoBounds);
	m_appRepoLink->setBounds(repoLinkBounds);
}

}
