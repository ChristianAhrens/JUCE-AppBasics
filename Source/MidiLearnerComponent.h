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
    public Timer,
    private MidiInputCallback,
    private MessageListener
{
public:
    MidiLearnerComponent(std::int16_t refId = -1);
    ~MidiLearnerComponent();
	
    //==============================================================================
    void resized() override;

    //==============================================================================
    void buttonClicked(Button*) override;
    
    //==============================================================================
    void timerCallback() override;
    
    //==============================================================================
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
    
    //==============================================================================
    void handleMessage(const Message& msg) override;

    //==============================================================================
    void lookAndFeelChanged() override;

    //==============================================================================
    std::function<void(Component*, JUCEAppBasics::Midi_utils::MidiCommandRangeAssignment)> onMidiAssiSet;
    
    //==============================================================================
    void setSelectedDeviceIdx(int deviceIdx);
    void setCurrentMidiAssi(const JUCEAppBasics::Midi_utils::MidiCommandRangeAssignment& currentAssi);

    void setReferredId(std::int16_t refId);
    std::int16_t getReferredId() const;

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
    std::int16_t                                                            m_referredId{ -1 };

    //==============================================================================
    bool isTimerUpdatingPopup()     { return m_timerUpdatingPopup; };
    void startTimerUpdatingPopup()  { m_timerUpdatingPopup = true; };
    void stopTimerUpdatingPopup()   { m_timerUpdatingPopup = false; };
    bool m_timerUpdatingPopup{ false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiLearnerComponent)
};

};
