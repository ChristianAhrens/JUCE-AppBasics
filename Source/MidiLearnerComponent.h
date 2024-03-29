/*
  ==============================================================================

    MidiLearnerComponent.h
    Created: 14 Jan 2021 12:06:32pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "MidiCommandRangeAssignment.h"

namespace JUCEAppBasics
{

class MidiLearnerComponent :
    public Component,
    public Timer,
    private MidiInputCallback,
    private MessageListener
{
public:
    typedef std::uint8_t AssignmentType;
    static constexpr AssignmentType AT_Invalid      = 0x00;
    static constexpr AssignmentType AT_Trigger      = 0x01;
    static constexpr AssignmentType AT_ValueRange   = 0x02;
    static constexpr AssignmentType AT_CommandRange = 0x04;

public:
    MidiLearnerComponent(std::int16_t refId = -1, AssignmentType assignmentTypesToBeLearned = AT_Trigger|AT_ValueRange|AT_CommandRange, bool showClearButton = true);
    ~MidiLearnerComponent();
	
    //==============================================================================
    void resized() override;
    
    //==============================================================================
    void timerCallback() override;
    
    //==============================================================================
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
    
    //==============================================================================
    void handleMessage(const Message& msg) override;

    //==============================================================================
    void lookAndFeelChanged() override;

    //==============================================================================
    std::function<void(Component*, JUCEAppBasics::MidiCommandRangeAssignment)> onMidiAssiSet;
    
    //==============================================================================
    void setSelectedDeviceIdentifier(const String& deviceIdentifier);
    void setCurrentMidiAssi(const JUCEAppBasics::MidiCommandRangeAssignment& currentAssi);
    void clearCurrentMidiAssi();
    const JUCEAppBasics::MidiCommandRangeAssignment& getCurrentMidiAssi();

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
    void activateMidiInput();
    void deactivateMidiInput();

    std::unique_ptr<TextEditor>     m_currentMidiAssiEdit;
    std::unique_ptr<DrawableButton> m_learnButton;
    std::unique_ptr<DrawableButton> m_clearButton;
    bool                            m_showClearButton;
    String                          m_deviceIdentifier;
    String                          m_deviceName;
    PopupMenu                       m_popup;
    std::map<JUCEAppBasics::MidiCommandRangeAssignment::CommandType, std::map<int, JUCEAppBasics::MidiCommandRangeAssignment>>    m_learnedDirectAssis;
    std::map<JUCEAppBasics::MidiCommandRangeAssignment::CommandType, std::map<int, JUCEAppBasics::MidiCommandRangeAssignment>>    m_learnedValueRangeAssis;
    std::map<JUCEAppBasics::MidiCommandRangeAssignment::CommandType, std::map<int, JUCEAppBasics::MidiCommandRangeAssignment>>    m_learnedCommandAndValueRangeAssis;
    
    std::unique_ptr<MidiInput>                  m_midiInput;
    JUCEAppBasics::MidiCommandRangeAssignment   m_currentMidiAssi;
    std::int16_t                                m_referredId{ -1 };
    int                                         m_popupItemIndexCounter{ 0 };
    AssignmentType                              m_assignmentTypesToBeLearned{ AT_Invalid };

    //==============================================================================
    bool isTimerUpdatingPopup();
    void startTimerUpdatingPopup();
    void stopTimerUpdatingPopup();
    bool m_timerUpdatingPopup{ false };

    //==============================================================================
    bool isPopupResultMuted(bool clear = true);
    void addPopupResultMutedOnce();
    std::uint32_t m_popupResultMutedCount{ 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiLearnerComponent)
};

};
