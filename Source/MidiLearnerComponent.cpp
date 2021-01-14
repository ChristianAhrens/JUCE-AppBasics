/*
  ==============================================================================

    MidiLearnerComponent.cpp
    Created: 14 Jan 2021 12:06:32pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "MidiLearnerComponent.h"

#include "Image_utils.h"

namespace JUCEAppBasics
{

//==============================================================================
MidiLearnerComponent::MidiLearnerComponent()
{
	m_currentMidiAssiEdit = std::make_unique<TextEditor>();
	m_currentMidiAssiEdit->setEnabled(false);
	addAndMakeVisible(m_currentMidiAssiEdit.get());

	std::unique_ptr<juce::Drawable> NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage;
	JUCEAppBasics::Image_utils::getDrawableButtonImages(BinaryData::school24px_svg, NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage);

	m_learnButton = std::make_unique<DrawableButton>("Learn MIDI command", DrawableButton::ButtonStyle::ImageOnButtonBackground);
	m_learnButton->setImages(NormalImage.get(), OverImage.get(), DownImage.get(), DisabledImage.get(), NormalOnImage.get(), OverOnImage.get(), DownOnImage.get(), DisabledOnImage.get());
	m_learnButton->addListener(this);
	addAndMakeVisible(m_learnButton.get());
}

MidiLearnerComponent::~MidiLearnerComponent()
{

}

void MidiLearnerComponent::resized()
{
	auto bounds = getLocalBounds();

	m_learnButton->setBounds(bounds.removeFromRight(bounds.getHeight()));
	bounds.removeFromRight(4);
	m_currentMidiAssiEdit->setBounds(bounds);
}

void MidiLearnerComponent::buttonClicked(Button* button)
{
	if (button == m_learnButton.get())
	{
		MidiLearnerComponent::MidiAssi selectedMidiAssignment = showMenuAndGetMidiAssi();

		if (onMidiAssiSet)
			onMidiAssiSet(selectedMidiAssignment);

		return;
	}
}

MidiLearnerComponent::MidiAssi MidiLearnerComponent::showMenuAndGetMidiAssi()
{
	MidiAssiLearningPopupMenu p;

	if (p.getNumItems() == 0)
		p.addItem(-1, "Internal Error", false);

	MidiLearnerComponent::MidiAssi result = p.show();

	return result;
}

}