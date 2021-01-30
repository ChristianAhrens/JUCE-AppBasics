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
MidiLearnerComponent::MidiLearnerComponent(std::int16_t refId, AssignmentType assignmentTypesToBeLearned)
{
    m_assignmentTypesToBeLearned = assignmentTypesToBeLearned;

    setReferredId(refId);

	m_currentMidiAssiEdit = std::make_unique<TextEditor>();
    m_currentMidiAssiEdit->setReadOnly(true);
	addAndMakeVisible(m_currentMidiAssiEdit.get());

	m_learnButton = std::make_unique<DrawableButton>("Learn MIDI command", DrawableButton::ButtonStyle::ImageOnButtonBackground);
	m_learnButton->addListener(this);
	addAndMakeVisible(m_learnButton.get());
    lookAndFeelChanged();
    
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
        if (m_midiInput && m_midiInput->getDeviceInfo().identifier != m_deviceIdentifier)
        {
            m_midiInput->stop();
            m_midiInput.reset();
        }
        else if (m_midiInput && m_midiInput->getDeviceInfo().identifier == m_deviceIdentifier)
        {
            DBG("Midi input " + m_deviceIdentifier + " is already active.");
        }
        else
        {
            m_midiInput = juce::MidiInput::openDevice(m_deviceIdentifier, this);
            m_midiInput->start();
        }
    }
}

void MidiLearnerComponent::deactivateMidiInputCallback()
{
    if (m_midiInput)
    {
        m_midiInput->stop();
        m_midiInput.reset();
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
        auto& midiMessage = callbackMessage->_message;

        DBG(String(__FUNCTION__) + " MIDI received: " + midiMessage.getDescription());

        // sanity check if the incoming message comes from the device we want to listen to
        if (m_deviceIdentifier.isEmpty() || (m_deviceIdentifier != callbackMessage->_source->getDeviceInfo().identifier))
            return;

        // start handling of the incoming data
        auto commandRangeAssi = JUCEAppBasics::MidiCommandRangeAssignment(midiMessage);
        auto commandType = commandRangeAssi.getCommandType();
        auto& commandData = commandRangeAssi.getCommandData();

        // direct command assignments without value or command range
        {
            // iterate through already known command data/type to find if existing entries can be extended
            auto directAssisKeyIndex = -1;
            if (m_learnedDirectAssis.count(commandType) > 0)
                for (auto& learnedAssiKV : m_learnedDirectAssis.at(commandType))
                    if (learnedAssiKV.second.getCommandData() == commandData && learnedAssiKV.second.isMatchingValueRange(midiMessage))
                        directAssisKeyIndex = learnedAssiKV.first;

            // if the command data/type is not yet represented, create new entries for it
            if (directAssisKeyIndex == -1)
            {
                auto nextKey = ++m_popupItemIndexCounter;
                m_learnedDirectAssis[commandType][nextKey] = commandRangeAssi;
            }
        }

        // command assignments with value range
        {
            // iterate through already known command data/type to find if existing entries can be extended
            auto commandAssisKeyIndex = -1;
            if (m_learnedValueRangeAssis.count(commandType) > 0)
            {
                for (auto& learnedAssiKV : m_learnedValueRangeAssis.at(commandType))
                {
                    if (learnedAssiKV.second.getCommandData() == commandData)
                    {
                        learnedAssiKV.second.extendValueRange(midiMessage);
                        commandAssisKeyIndex = learnedAssiKV.first;
                    }
                }
            }

            // if the command data/type is not yet represented, create new entries for it
            if (commandAssisKeyIndex == -1)
            {
                auto nextKey = ++m_popupItemIndexCounter;
                m_learnedValueRangeAssis[commandType][nextKey] = commandRangeAssi;
            }
        }

        // assignment of a range of command and range of values
        {
            // iterate through already known command data/type to find if existing entries can be extended
            auto commandAndValueAssisKeyIndex = -1;
            if (m_learnedCommandAndValueRangeAssis.count(commandType) > 0)
            {
                jassert(m_learnedCommandAndValueRangeAssis.at(commandType).size() == 1);
                auto learnedAssiKV = m_learnedCommandAndValueRangeAssis.at(commandType).begin();
                
                learnedAssiKV->second.extendCommandRange(midiMessage);
                learnedAssiKV->second.extendValueRange(midiMessage);
                commandAndValueAssisKeyIndex = learnedAssiKV->first;
            }

            // if the command data/type is not yet represented, create new entries for it
            if (commandAndValueAssisKeyIndex == -1)
            {
                auto nextKey = ++m_popupItemIndexCounter;
                m_learnedCommandAndValueRangeAssis[commandType][nextKey] = commandRangeAssi;
            }
        }

        // if the cyclical updating of the popup menu contents is not active, start now to display the new available assignments
        if (!isTimerUpdatingPopup())
            startTimerUpdatingPopup();
    }
}

/**
 * Reimplemnted from Component to correctly adjust button drawable colouring
 */
void MidiLearnerComponent::lookAndFeelChanged()
{
    Component::lookAndFeelChanged();

    auto colourOn = getLookAndFeel().findColour(TextButton::ColourIds::textColourOnId);
    auto colourOff = getLookAndFeel().findColour(TextButton::ColourIds::textColourOffId);

    std::unique_ptr<juce::Drawable> NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage;
    JUCEAppBasics::Image_utils::getDrawableButtonImages(BinaryData::school24px_svg, NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage,
        colourOn, colourOff, colourOff, colourOff, colourOn, colourOn, colourOn, colourOn);

    m_learnButton->setImages(NormalImage.get(), OverImage.get(), DownImage.get(), DisabledImage.get(), NormalOnImage.get(), OverOnImage.get(), DownOnImage.get(), DisabledOnImage.get());
}

void MidiLearnerComponent::updatePopupMenu()
{
    m_popup.dismissAllActiveMenus();
    m_popup.clear();

    auto maxItemsBeforeSubmenu = 5;

    if (m_deviceIdentifier.isEmpty())
        m_popup.addItem(-1, "No MIDI Input Selected", false);
    else
    {
        m_popup.addItem(-1, "Waiting for input from " + m_deviceName, false);

        if (((m_assignmentTypesToBeLearned & AT_Trigger) == AT_Trigger) && !m_learnedDirectAssis.empty())
        {
            m_popup.addSeparator();

            auto itemCount = 0;
            for (auto const& learnedDirectAssiKV : m_learnedDirectAssis)
                    itemCount += static_cast<int>(learnedDirectAssiKV.second.size());

            if (itemCount > maxItemsBeforeSubmenu)
            {
                PopupMenu subMenu;
                for (auto const& learnedDirectAssiKV : m_learnedDirectAssis)
                    for (auto const& learnedAssiKV : learnedDirectAssiKV.second)
                        subMenu.addItem(learnedAssiKV.first, learnedAssiKV.second.getNiceDescription());
                m_popup.addSubMenu("Single Trigger Commands", subMenu);
            }
            else
            {
                m_popup.addItem(-1, "Single Trigger Commands", false);
                for (auto const& learnedDirectAssiKV : m_learnedDirectAssis)
                    for (auto const& learnedAssiKV : learnedDirectAssiKV.second)
                        m_popup.addItem(learnedAssiKV.first, learnedAssiKV.second.getNiceDescription());
            }
        }

        if (((m_assignmentTypesToBeLearned & AT_ValueRange) == AT_ValueRange) && !m_learnedValueRangeAssis.empty())
        {
            m_popup.addSeparator();

            auto itemCount = 0;
            for (auto const& learnedValueRangeAssiKV : m_learnedValueRangeAssis)
                for (auto const& learnedAssiKV : learnedValueRangeAssiKV.second)
                    if (learnedAssiKV.second.isValueRangeAssignment())
                        itemCount++;

            if (itemCount > maxItemsBeforeSubmenu)
            {
                PopupMenu subMenu;
                for (auto const& learnedValueRangeAssiKV : m_learnedValueRangeAssis)
                    for (auto const& learnedAssiKV : learnedValueRangeAssiKV.second)
                        if (learnedAssiKV.second.isValueRangeAssignment())
                            subMenu.addItem(learnedAssiKV.first, learnedAssiKV.second.getNiceDescription());
                m_popup.addSubMenu("Value Range Commands", subMenu);
            }
            else
            {
                m_popup.addItem(-1, "Value Range Commands", false);
                for (auto const& learnedValueRangeAssiKV : m_learnedValueRangeAssis)
                    for (auto const& learnedAssiKV : learnedValueRangeAssiKV.second)
                        if (learnedAssiKV.second.isValueRangeAssignment())
                            m_popup.addItem(learnedAssiKV.first, learnedAssiKV.second.getNiceDescription());
            }
        }

        if (((m_assignmentTypesToBeLearned & AT_CommandRange) == AT_CommandRange) && !m_learnedCommandAndValueRangeAssis.empty())
        {
            m_popup.addSeparator();

            auto itemCount = 0;
            for (auto const& learnedCommandAndValueRangeAssiKV : m_learnedCommandAndValueRangeAssis)
                for (auto const& learnedAssiKV : learnedCommandAndValueRangeAssiKV.second)
                    if (learnedAssiKV.second.isCommandRangeAssignment())
                        itemCount++;

            if (itemCount > maxItemsBeforeSubmenu)
            {
                PopupMenu subMenu;
                for (auto const& learnedCommandAndValueRangeAssiKV : m_learnedCommandAndValueRangeAssis)
                    for (auto const& learnedAssiKV : learnedCommandAndValueRangeAssiKV.second)
                        if (learnedAssiKV.second.isCommandRangeAssignment())
                            subMenu.addItem(learnedAssiKV.first, learnedAssiKV.second.getNiceDescription());
                m_popup.addSubMenu("Command + Value Range Commands", subMenu);
            }
            else
            {
                m_popup.addItem(-1, "Command+Value Range Commands", false);
                for (auto const& learnedCommandAndValueRangeAssiKV : m_learnedCommandAndValueRangeAssis)
                    for (auto const& learnedAssiKV : learnedCommandAndValueRangeAssiKV.second)
                        if (learnedAssiKV.second.isCommandRangeAssignment())
                            m_popup.addItem(learnedAssiKV.first, learnedAssiKV.second.getNiceDescription());
            }
        }
    }

    addPopupResultMutedOnce();
    m_popup.showMenuAsync(PopupMenu::Options(), [this](int resultingAssiIdx) { 
        handlePopupResult(resultingAssiIdx); });
    
    stopTimerUpdatingPopup();
}

void MidiLearnerComponent::triggerLearning()
{
    m_learnedDirectAssis.clear();
    m_learnedValueRangeAssis.clear();
    m_learnedCommandAndValueRangeAssis.clear();
    m_popup.clear();

    activateMidiInputCallback();

    updatePopupMenu();
}

void MidiLearnerComponent::handlePopupResult(int resultingAssiIdx)
{
    DBG(String(__FUNCTION__));
    if (isPopupResultMuted())
        return;
    DBG(String(__FUNCTION__) + " not muted");

    auto resultingAssi = JUCEAppBasics::MidiCommandRangeAssignment();
    auto resultingAssiFound = false;

    for (auto const& commandTypeKV : m_learnedDirectAssis)
    {
        for (auto const& learnedAssiKV : m_learnedDirectAssis[commandTypeKV.first])
        {
            if (learnedAssiKV.first == resultingAssiIdx)
            {
                resultingAssi = learnedAssiKV.second;
                resultingAssiFound = true;
                break;
            }
        }
        for (auto const& learnedAssiKV : m_learnedValueRangeAssis[commandTypeKV.first])
        {
            if (learnedAssiKV.first == resultingAssiIdx)
            {
                resultingAssi = learnedAssiKV.second;
                resultingAssiFound = true;
                break;
            }
        }
        for (auto const& learnedAssiKV : m_learnedCommandAndValueRangeAssis[commandTypeKV.first])
        {
            if (learnedAssiKV.first == resultingAssiIdx)
            {
                resultingAssi = learnedAssiKV.second;
                resultingAssiFound = true;
                break;
            }
        }

        if (resultingAssiFound)
            break;
    }

    if (resultingAssiFound)
    {
        setCurrentMidiAssi(resultingAssi);

        if (onMidiAssiSet)
            onMidiAssiSet(this, resultingAssi);

        deactivateMidiInputCallback();

        m_learnedDirectAssis.clear();
        m_learnedValueRangeAssis.clear();
        m_learnedCommandAndValueRangeAssis.clear();
        m_popup.clear();
        m_popupItemIndexCounter = 0;
    }
}

void MidiLearnerComponent::setSelectedDeviceIdentifier(const String& deviceIdentifier)
{
    // a new deviceIdx cancels all ongoing action
    deactivateMidiInputCallback();
    m_popup.dismissAllActiveMenus();

    // sanity check of incoming deviceIdx
    auto midiDevicesInfos = juce::MidiInput::getAvailableDevices();
    bool newMidiDeviceFound = false;
    for (auto const& midiDeviceInfo : midiDevicesInfos)
    {
        if (midiDeviceInfo.identifier == deviceIdentifier)
        {
            newMidiDeviceFound = true;
            m_deviceIdentifier = midiDeviceInfo.identifier;
            m_deviceName = midiDeviceInfo.name;
            break;
        }
    }

    if (!newMidiDeviceFound)
    {
        m_deviceIdentifier.clear();
        m_deviceName.clear();
    }
}

void MidiLearnerComponent::setCurrentMidiAssi(const JUCEAppBasics::MidiCommandRangeAssignment& currentAssi)
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

bool MidiLearnerComponent::isTimerUpdatingPopup()
{
    return m_timerUpdatingPopup;
}

void MidiLearnerComponent::startTimerUpdatingPopup()
{
    m_timerUpdatingPopup = true;
}

void MidiLearnerComponent::stopTimerUpdatingPopup()
{
    m_timerUpdatingPopup = false;
}

bool MidiLearnerComponent::isPopupResultMuted(bool clear)
{
    if (m_popupResultMutedCount > 0) {
        if (clear)
            m_popupResultMutedCount = 0;
        else
            m_popupResultMutedCount--;
        return true;
    }
    else
        return false;
}

void MidiLearnerComponent::addPopupResultMutedOnce()
{
    m_popupResultMutedCount++;
}

}
