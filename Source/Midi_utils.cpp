/*
  ==============================================================================

    Midi_utils.cpp
    Created: 15 Jan 2021 1:15:57pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "Midi_utils.h"

namespace JUCEAppBasics
{
    
namespace Midi_utils
{
    
MidiCommandRangeAssignment::MidiCommandRangeAssignment()
{
}

MidiCommandRangeAssignment::MidiCommandRangeAssignment(const juce::MidiMessage& m)
{
    extendValueRange(m);
}

MidiCommandRangeAssignment::~MidiCommandRangeAssignment()
{
}

bool MidiCommandRangeAssignment::operator==(const MidiCommandRangeAssignment& rhs) const
{
    return (m_commandData == rhs.m_commandData && m_valueRange == rhs.m_valueRange);
}

bool MidiCommandRangeAssignment::operator!=(const MidiCommandRangeAssignment& rhs) const
{
    return !(*this == rhs);
}

bool MidiCommandRangeAssignment::operator<(const MidiCommandRangeAssignment& rhs) const
{
    return (!(*this > rhs) && (*this != rhs));
}

bool MidiCommandRangeAssignment::operator>(const MidiCommandRangeAssignment& rhs) const
{
    auto greaterThanRhs = true;
    for (int i = 0; i < m_commandData.size() && i < rhs.m_commandData.size(); ++i)
    {
        if (m_commandData[i] <= rhs.m_commandData[i])
        {
            greaterThanRhs = false;
            break;
        }
    }

    return greaterThanRhs || ((m_commandData == rhs.m_commandData) && (m_valueRange.intersects(rhs.m_valueRange) ? m_valueRange.getLength() > rhs.m_valueRange.getLength() : m_valueRange.getStart() > rhs.m_valueRange.getStart()));
}

MidiCommandRangeAssignment& MidiCommandRangeAssignment::operator=(const MidiCommandRangeAssignment& rhs)
{
    if (this != &rhs)
    {
        m_valueRange = rhs.m_valueRange;
        m_commandData = rhs.m_commandData;
    }

    return *this;
}

juce::String MidiCommandRangeAssignment::getCommandDescription() const
{
    if (m_commandData.size() > 1)
    {
        // note on
        if ((m_commandData[0] & 0xf0) == 0x90)
            return "NoteOn" + String(m_commandData[1]);

        // note off
        if ((m_commandData[0] & 0xf0) == 0x80)
            return "NoteOff" + String(m_commandData[1]);

        // program change
        if ((m_commandData[0] & 0xf0) == 0xc0)
            return "ProgChange" + String(m_commandData[1]);

        // pitch wheel
        if ((m_commandData[0] & 0xf0) == 0xe0)
            return "Pitch";
        //// pitch wheel value
        //data[1] | (data[2] << 7)

        // aftertouch
        if ((m_commandData[0] & 0xf0) == 0xa0)
            return "Aftertouch" + String(m_commandData[1]);
        // aftertouch value
        //data[2]

        // controller
        if ((m_commandData[0] & 0xf0) == 0xb0)
            return "Ctrl" + String(m_commandData[1]);
        //// controller value
        //data[2]

        // channel pressure
        if ((m_commandData[0] & 0xf0) == 0xd0)
            return "Ch. pressure";
        //// ch. pres. value
        //data[1]
    }

    return "Unknown";
}

juce::String MidiCommandRangeAssignment::getRangeDescription() const
{
    return String(m_valueRange.getStart()) + "..." + String(m_valueRange.getEnd());
}

juce::String MidiCommandRangeAssignment::getCommandRangeDescription() const
{
    return getCommandDescription() + " " + getRangeDescription();
}

const std::vector<std::uint8_t>& MidiCommandRangeAssignment::getCommandData() const
{
    return m_commandData;
}

std::vector<std::uint8_t> MidiCommandRangeAssignment::getCommandData(const juce::MidiMessage& m)
{
    auto dataBytes = m.getRawData();
    auto dataByteLength = m.getRawDataSize();
    auto commandDataByteLength = dataByteLength - 1;
    auto commandData = std::vector<std::uint8_t>();
    for (int i = 0; i < commandDataByteLength; ++i)
        commandData.push_back(dataBytes[i]);

    return commandData;
}

void MidiCommandRangeAssignment::setCommandData(const juce::MidiMessage& m)
{
    m_commandData = getCommandData(m);
}

const juce::Range<int>& MidiCommandRangeAssignment::getValueRange() const
{
    return m_valueRange;
}

void MidiCommandRangeAssignment::setValueRange(const juce::Range<int>& r)
{
    m_valueRange = r;
}

bool MidiCommandRangeAssignment::extendValueRange(int value)
{
    if (m_valueRangeEmpty)
    {
        m_valueRange.setStart(value);
        m_valueRange.setEnd(value);
        m_valueRangeEmpty = false;
    }
    else
        m_valueRange = m_valueRange.getUnionWith(value);

    return true;
}

bool MidiCommandRangeAssignment::extendValueRange(const juce::MidiMessage& m)
{
    if (getCommandData().empty())
        setCommandData(m);
    else if (getCommandData(m) != getCommandData())
        return false;

    auto value = 0;
    if (m.isAftertouch())
        value = m.getAfterTouchValue();
    else if (m.isChannelPressure())
        value = m.getChannelPressureValue();
    else if (m.isController())
        value = m.getControllerValue();
    else if (m.isPitchWheel())
        value = m.getPitchWheelValue();
    else if (m.isQuarterFrame())
        value = m.getQuarterFrameValue();
    else
        return false;

    return extendValueRange(value);
}

    
}

}