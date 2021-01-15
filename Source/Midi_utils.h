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
    MidiCommandRangeAssignment();
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

    juce::String getCommandDescription() const;
    juce::String getRangeDescription() const;
    juce::String getCommandRangeDescription() const;

    const std::vector<std::uint8_t>& getCommandData() const;
    static std::vector<std::uint8_t> getCommandData(const juce::MidiMessage& m);
    void setCommandData(const juce::MidiMessage& m);
    int getCommandDataExpectedBytes() const;
    static int getCommandDataExpectedBytes(const juce::MidiMessage& m);

    const juce::Range<int>& getValueRange() const;
    void setValueRange(const juce::Range<int>& r);
    bool extendValueRange(int value);
    bool extendValueRange(const juce::MidiMessage& m);

private:
    std::vector<std::uint8_t>   m_commandData;
    juce::Range<int>            m_valueRange;
    bool                        m_valueRangeEmpty{ true };

    JUCE_LEAK_DETECTOR(MidiCommandRangeAssignment)
};
    
}

}
