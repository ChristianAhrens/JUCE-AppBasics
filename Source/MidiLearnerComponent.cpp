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
    m_currentMidiAssiEdit->setReadOnly(true);
	addAndMakeVisible(m_currentMidiAssiEdit.get());

	std::unique_ptr<juce::Drawable> NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage;
	JUCEAppBasics::Image_utils::getDrawableButtonImages(BinaryData::school24px_svg, NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage);

	m_learnButton = std::make_unique<DrawableButton>("Learn MIDI command", DrawableButton::ButtonStyle::ImageOnButtonBackground);
	m_learnButton->setImages(NormalImage.get(), OverImage.get(), DownImage.get(), DisabledImage.get(), NormalOnImage.get(), OverOnImage.get(), DownOnImage.get(), DisabledOnImage.get());
	m_learnButton->addListener(this);
	addAndMakeVisible(m_learnButton.get());
    
    m_deviceManager = std::make_unique<AudioDeviceManager>();
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
		showMenu();
	}
}

void MidiLearnerComponent::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    // dispatch message to queue
    postMessage(std::make_unique<CallbackMidiMessage>(message).release());
}

/**
 * Reimplemented from MessageListener to handle messages posted to queue.
 * @param msg    The incoming message to handle
 */
void MidiLearnerComponent::handleMessage(const Message& msg)
{
    if (auto* callbackMessage = dynamic_cast<const CallbackMidiMessage*> (&msg))
    {
        auto midiMessage = callbackMessage->_message;
        auto messageHexString = juce::String::toHexString(midiMessage.getRawData(), midiMessage.getRawDataSize());
        
        auto nextKey = static_cast<int>(m_learnedAssiMap.size() + 1);
        m_learnedAssiMap[nextKey]._rawMsg = messageHexString;
        
        m_popup.addItem(nextKey, messageHexString);
        m_popup.showMenuAsync(PopupMenu::Options(), [=](int resultingAssiIdx) { handlePopupResult(resultingAssiIdx); });
    }
}

void MidiLearnerComponent::showMenu()
{
    auto inputIdentifier = String();
    
    if (m_deviceIdx == -1)
        m_popup.addItem(-1, "No MIDI Input Selected", false);
    else
    {
        auto list = juce::MidiInput::getAvailableDevices();
        if (list.size() > m_deviceIdx)
        {
            inputIdentifier = list[m_deviceIdx].identifier;
            
            m_popup.addItem(-1, "Waiting for input from " + list[m_deviceIdx].name, false);
        }
    }
    
    if (inputIdentifier.isNotEmpty())
    {
        if (!m_deviceManager->isMidiInputDeviceEnabled(inputIdentifier))
            m_deviceManager->setMidiInputDeviceEnabled(inputIdentifier, true);

        m_deviceManager->addMidiInputDeviceCallback(inputIdentifier, this);
    }

    m_popup.showMenuAsync(PopupMenu::Options(), [=](int resultingAssiIdx) { handlePopupResult(resultingAssiIdx); });
}

void MidiLearnerComponent::handlePopupResult(int resultingAssiIdx)
{
    auto resultingAssi = MidiLearnerComponent::MidiAssi();
    if (m_learnedAssiMap.count(resultingAssiIdx) > 0)
        resultingAssi = m_learnedAssiMap.at(resultingAssiIdx);
    
    setCurrentMidiAssi(resultingAssi);
    
    if (onMidiAssiSet)
        onMidiAssiSet(resultingAssi);
    
    if (m_deviceIdx != -1)
    {
        auto list = juce::MidiInput::getAvailableDevices();
        if (list.size() > m_deviceIdx)
        {
            auto inputIdentifier = list[m_deviceIdx].identifier;
            if (inputIdentifier.isNotEmpty())
                m_deviceManager->removeMidiInputCallback(inputIdentifier, this);
        }
    }
    m_popup.clear();
}

void MidiLearnerComponent::setSelectedDeviceIdx(int deviceIdx)
{
    // sanity check of incoming device id
    auto list = juce::MidiInput::getAvailableDevices();
    if (list.size() > deviceIdx)
        m_deviceIdx = deviceIdx;
    else
        m_deviceIdx = -1;
}

void MidiLearnerComponent::setCurrentMidiAssi(MidiAssi currentAssi)
{
    m_currentMidiAssi = currentAssi;
    
    if (m_currentMidiAssiEdit)
        m_currentMidiAssiEdit->setText(m_currentMidiAssi._rawMsg);
}


}
