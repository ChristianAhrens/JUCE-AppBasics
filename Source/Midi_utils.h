/*
  ==============================================================================

    Midi_utils.h
    Created: 15 Jan 2021 1:15:57pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace JUCEAppBasics
{
    
namespace Midi_utils
{
    
class MidiCommandRangeAssignment
{
public:
    enum CommandType
    {
        CT_Invalid = 0,
        CT_NoteOn,
        CT_NoteOff,
        CT_ProgramChange,
        CT_Pitch,
        CT_Aftertouch,
        CT_Controller,
        CT_ChannelPressure,
    };

public:
    MidiCommandRangeAssignment();
    MidiCommandRangeAssignment(const std::vector<std::uint8_t>& commandData);
    MidiCommandRangeAssignment(const juce::MidiMessage& m);
    ~MidiCommandRangeAssignment();

    bool operator==(const MidiCommandRangeAssignment& rhs) const;
    bool operator!=(const MidiCommandRangeAssignment& rhs) const;
    bool operator<(const MidiCommandRangeAssignment& rhs) const;
	bool operator>(const MidiCommandRangeAssignment& rhs) const;
    MidiCommandRangeAssignment& operator=(const MidiCommandRangeAssignment& rhs);

    bool isNoteOnCommand() const;
    bool isNoteOffCommand() const;
    bool isProgramChangeCommand() const;
    bool isPitchCommand() const;
    bool isAftertouchCommand() const;
    bool isControllerCommand() const;
    bool isChannelPressureCommand() const;

    static bool isNoteOnCommand(const std::vector<std::uint8_t>& commandData);
    static bool isNoteOffCommand(const std::vector<std::uint8_t>& commandData);
    static bool isProgramChangeCommand(const std::vector<std::uint8_t>& commandData);
    static bool isPitchCommand(const std::vector<std::uint8_t>& commandData);
    static bool isAftertouchCommand(const std::vector<std::uint8_t>& commandData);
    static bool isControllerCommand(const std::vector<std::uint8_t>& commandData);
    static bool isChannelPressureCommand(const std::vector<std::uint8_t>& commandData);

    CommandType getCommandType() const;
    static CommandType getCommandType(const juce::MidiMessage& m);
    static CommandType getCommandType(const std::vector<std::uint8_t>& commandData);


    juce::String getCommandDescription() const;
    static juce::String getCommandDescription(const std::vector<std::uint8_t>& commandData);
    juce::String getValueRangeDescription() const;
    juce::String getCommandRangeDescription() const;
    juce::String getNiceDescription() const;

    const std::vector<std::uint8_t>& getCommandData() const;
    static std::vector<std::uint8_t> getCommandData(const juce::MidiMessage& m);
    void setCommandData(const std::vector<std::uint8_t>& commandData);
    void setCommandData(const juce::MidiMessage& m);
    int getCommandDataExpectedBytes() const;
    static int getCommandDataExpectedBytes(const juce::MidiMessage& m);

    static int getValue(const juce::MidiMessage& m);

    const juce::Range<int>& getValueRange() const;
    void setValueRange(const juce::Range<int>& r);
    bool extendValueRange(int value);
    bool extendValueRange(const juce::MidiMessage& m);
    bool isValueRangeAssignment() const;
    bool isMatchingValueRange(int v) const;
    bool isMatchingValueRange(const juce::MidiMessage& m) const;

    static int getCommandValue(const juce::MidiMessage& m);
    static int getCommandValue(const std::vector<std::uint8_t>& commandData);

    const juce::Range<std::vector<std::uint8_t>>& getCommandRange() const;
    void setCommandRange(const juce::Range<std::vector<std::uint8_t>>& cr);
    bool extendCommandRange(const std::vector<std::uint8_t>& c);
    bool extendCommandRange(const juce::MidiMessage& m);
    bool isCommandRangeAssignment() const;
    bool isMatchingCommand(const juce::MidiMessage& m) const;
    bool isMatchingCommandRange(const std::vector<std::uint8_t>& c) const;
    bool isMatchingCommandRange(const juce::MidiMessage& m) const;

    juce::String serializeToHexString() const;
    bool deserializeFromHexString(const juce::String& serialData);

private:
    std::vector<std::uint8_t>               m_commandData;
    juce::Range<int>                        m_valueRange;
    juce::Range<std::vector<std::uint8_t>>  m_commandRange;
    bool                                    m_valueRangeEmpty{ true };

    JUCE_LEAK_DETECTOR(MidiCommandRangeAssignment)
};
    
}

}
