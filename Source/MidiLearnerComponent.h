/*
  ==============================================================================

    MidiLearnerComponent.h
    Created: 14 Jan 2021 12:06:32pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../submodules/JUCE-AppBasics/Source/Midi_utils.h"

namespace JUCEAppBasics
{

class MidiLearnerComponent :
    public Component,
    public Button::Listener,
    private MidiInputCallback,
    private MessageListener
{
public:
    MidiLearnerComponent();
    ~MidiLearnerComponent();
	
    //==============================================================================
    void resized() override;

    //==============================================================================
    void buttonClicked(Button*) override;
    
    //==============================================================================
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
    
    //==============================================================================
    void handleMessage(const Message& msg) override;

    //==============================================================================
    std::function<void(Component*, JUCEAppBasics::Midi_utils::MidiCommandRangeAssignment)> onMidiAssiSet;
    
    //==============================================================================
    void setSelectedDeviceIdx(int deviceIdx);
    void setCurrentMidiAssi(const JUCEAppBasics::Midi_utils::MidiCommandRangeAssignment& currentAssi);

private:
    class CallbackMidiMessage : public Message
    {
    public:
        /**
        * Constructor with default initialization of message and source.
        * @param m	The midi message to handle.
        * @param s	The source the message was received from.
        */
        CallbackMidiMessage(const juce::MidiMessage& m, juce::MidiInput* s) : _message(m), _source(s) {}

        juce::MidiMessage _message;
        juce::MidiInput* _source;
    };
    
private:
    void triggerLearning();
    void updatePopupMenu();
    void handlePopupResult(int resultingAssiIdx);
    void activateMidiInputCallback();
    void deactivateMidiInputCallback();

    std::unique_ptr<TextEditor>                                             m_currentMidiAssiEdit;
    std::unique_ptr<DrawableButton>                                         m_learnButton;
    String                                                                  m_deviceIdentifier;
    String                                                                  m_deviceName;
    PopupMenu                                                               m_popup;
    std::map<int, JUCEAppBasics::Midi_utils::MidiCommandRangeAssignment>    m_learnedAssiMap;
    std::unique_ptr<AudioDeviceManager>                                     m_deviceManager;
    JUCEAppBasics::Midi_utils::MidiCommandRangeAssignment                   m_currentMidiAssi;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiLearnerComponent)
};

};
