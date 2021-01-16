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
MidiLearnerComponent::MidiLearnerComponent(std::int16_t refId)
{
    setReferredId(refId);

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
    
    startTimer(200);
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
        triggerLearning();
	}
}

void MidiLearnerComponent::timerCallback()
{
    if (isTimerUpdatingPopup())
        updatePopupMenu();
}

void MidiLearnerComponent::activateMidiInputCallback()
{
    if (m_deviceIdentifier.isNotEmpty())
    {
        if (!m_deviceManager->isMidiInputDeviceEnabled(m_deviceIdentifier))
            m_deviceManager->setMidiInputDeviceEnabled(m_deviceIdentifier, true);

        m_deviceManager->addMidiInputDeviceCallback(m_deviceIdentifier, this);
    }
}

void MidiLearnerComponent::deactivateMidiInputCallback()
{
    if (m_deviceIdentifier.isNotEmpty())
    {
        if (m_deviceManager->isMidiInputDeviceEnabled(m_deviceIdentifier))
            m_deviceManager->setMidiInputDeviceEnabled(m_deviceIdentifier, false);

        m_deviceManager->removeMidiInputCallback(m_deviceIdentifier, this);
    }
}

void MidiLearnerComponent::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    // dispatch message to queue
    postMessage(std::make_unique<CallbackMidiMessage>(message, source).release());
}

/**
 * Reimplemented from MessageListener to handle messages posted to queue.
 * This implementation tries to interpred incoming messages as midi callback, 
 * asynchronously posted to queue from midi callback method above.
 * @param msg    The incoming message to handle
 */
void MidiLearnerComponent::handleMessage(const Message& msg)
{
    if (auto* callbackMessage = dynamic_cast<const CallbackMidiMessage*> (&msg))
    {
        if (m_deviceIdentifier.isEmpty() || (m_deviceIdentifier != callbackMessage->_source->getDeviceInfo().identifier))
            return;

        auto commandRangeAssi = JUCEAppBasics::Midi_utils::MidiCommandRangeAssignment(callbackMessage->_message);

        auto mapKeyIndex = -1;
        for (auto& learnedAssiKV : m_learnedAssiMap)
        {
            if (learnedAssiKV.second.getCommandData() == commandRangeAssi.getCommandData())
            {
                learnedAssiKV.second.extendValueRange(callbackMessage->_message);
                mapKeyIndex = learnedAssiKV.first;
            }
        }
        
        if (mapKeyIndex == -1)
        {
            auto nextKey = static_cast<int>(m_learnedAssiMap.size() + 1);
            m_learnedAssiMap[nextKey] = commandRangeAssi;
        }

        if (!isTimerUpdatingPopup())
            startTimerUpdatingPopup();
    }
}

void MidiLearnerComponent::updatePopupMenu()
{
    m_popup.dismissAllActiveMenus();
    m_popup.clear();

    if (m_deviceIdentifier.isEmpty())
        m_popup.addItem(-1, "No MIDI Input Selected", false);
    else
    {
        m_popup.addItem(-1, "Waiting for input from " + m_deviceName, false);

        for (auto const& learnedAssiKV : m_learnedAssiMap)
            m_popup.addItem(learnedAssiKV.first, learnedAssiKV.second.getNiceDescription());
    }

    m_popup.showMenuAsync(PopupMenu::Options(), [this](int resultingAssiIdx) { 
        handlePopupResult(resultingAssiIdx); });
    
    stopTimerUpdatingPopup();
}

void MidiLearnerComponent::triggerLearning()
{
    m_learnedAssiMap.clear();
    m_popup.clear();

    activateMidiInputCallback();

    updatePopupMenu();
}

void MidiLearnerComponent::handlePopupResult(int resultingAssiIdx)
{
    if (m_learnedAssiMap.count(resultingAssiIdx) > 0)
    {
        auto resultingAssi = m_learnedAssiMap[resultingAssiIdx];

        setCurrentMidiAssi(resultingAssi);

        if (onMidiAssiSet)
            onMidiAssiSet(this, resultingAssi);

        deactivateMidiInputCallback();

        m_learnedAssiMap.clear();
        m_popup.clear();
    }
}

void MidiLearnerComponent::setSelectedDeviceIdx(int deviceIdx)
{
    // a new deviceIdx cancels all ongoing action
    deactivateMidiInputCallback();
    m_popup.dismissAllActiveMenus();

    // sanity check of incoming deviceIdx
    auto list = juce::MidiInput::getAvailableDevices();
    if (list.size() > deviceIdx)
    {
        m_deviceIdentifier = list[deviceIdx].identifier;
        m_deviceName = list[deviceIdx].name;
    }
    else
    {
        m_deviceIdentifier.clear();
        m_deviceName.clear();
    }
}

void MidiLearnerComponent::setCurrentMidiAssi(const JUCEAppBasics::Midi_utils::MidiCommandRangeAssignment& currentAssi)
{
    m_currentMidiAssi = currentAssi;
    
    if (m_currentMidiAssiEdit)
        m_currentMidiAssiEdit->setText(m_currentMidiAssi.getNiceDescription());
}

void MidiLearnerComponent::setReferredId(std::int16_t refId)
{
    m_referredId = refId;
}

std::int16_t MidiLearnerComponent::getReferredId() const
{
    return m_referredId;
}

}
