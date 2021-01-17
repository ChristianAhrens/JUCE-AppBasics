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
    if (isNoteOnCommand() && (m_commandData.size() > 1))
        return "NoteOn " + MidiMessage::getMidiNoteName(m_commandData[1], true, true, 3);

    else if (isNoteOffCommand() && (m_commandData.size() > 1))
        return "NoteOff " + MidiMessage::getMidiNoteName(m_commandData[1], true, true, 3);

    else if (isProgramChangeCommand() && (m_commandData.size() > 1))
        return "ProgChange" + String(m_commandData[1]);

    else if (isAftertouchCommand() && (m_commandData.size() > 1))
        return "Aftertouch" + String(m_commandData[1]);
    // aftertouch value
    //data[2]

    else if (isControllerCommand() && (m_commandData.size() > 1))
        return "Ctrl" + String(m_commandData[1]);
    //// controller value
    //data[2]
    
    else if (isPitchCommand())
        return "Pitch";
    //// pitch wheel value
    //data[1] | (data[2] << 7)
    
    else if (isChannelPressureCommand())
        return "Ch. pressure";
    //// ch. pres. value
    //data[1]

    else
        return "Unassigned";
}

bool MidiCommandRangeAssignment::isNoteOnCommand() const
{
    if (m_commandData.empty())
        return false;
    
    return ((m_commandData[0] & 0xf0) == 0x90);
}

bool MidiCommandRangeAssignment::isNoteOffCommand() const
{
    if (m_commandData.empty())
        return false;
    
    return ((m_commandData[0] & 0xf0) == 0x80);
}

bool MidiCommandRangeAssignment::isProgramChangeCommand() const
{
    if (m_commandData.empty())
        return false;
    
    return ((m_commandData[0] & 0xf0) == 0xc0);
}

bool MidiCommandRangeAssignment::isPitchCommand() const
{
    if (m_commandData.empty())
        return false;
    
    return ((m_commandData[0] & 0xf0) == 0xe0);
}

bool MidiCommandRangeAssignment::isAftertouchCommand() const
{
    if (m_commandData.empty())
        return false;
    
    return ((m_commandData[0] & 0xf0) == 0xa0);
}

bool MidiCommandRangeAssignment::isControllerCommand() const
{
    if (m_commandData.empty())
        return false;
    
    return ((m_commandData[0] & 0xf0) == 0xb0);
}

bool MidiCommandRangeAssignment::isChannelPressureCommand() const
{
    if (m_commandData.empty())
        return false;
    
    return ((m_commandData[0] & 0xf0) == 0xd0);
}

juce::String MidiCommandRangeAssignment::getRangeDescription() const
{
    return "(" + String(m_valueRange.getStart()) + " - " + String(m_valueRange.getEnd()) + ")";
}

juce::String MidiCommandRangeAssignment::getCommandRangeDescription() const
{
    return getCommandDescription() + " " + getRangeDescription();
}

juce::String MidiCommandRangeAssignment::getNiceDescription() const
{
    if (isRangedCommandAssignment())
        return getCommandRangeDescription();
    else
        return getCommandDescription();
}

const std::vector<std::uint8_t>& MidiCommandRangeAssignment::getCommandData() const
{
    return m_commandData;
}

std::vector<std::uint8_t> MidiCommandRangeAssignment::getCommandData(const juce::MidiMessage& m)
{
    auto dataBytes = m.getRawData();
    auto dataBytesLength = m.getRawDataSize();
    auto commandData = std::vector<std::uint8_t>();
    auto commandDataExpectedBytesLength = getCommandDataExpectedBytes(m);
    
    if (commandDataExpectedBytesLength > dataBytesLength)
        return commandData;
    
    for (int i = 0; i < commandDataExpectedBytesLength; ++i)
        commandData.push_back(dataBytes[i]);

    return commandData;
}

int MidiCommandRangeAssignment::getCommandDataExpectedBytes() const
{
    if (isNoteOnCommand()
        || isNoteOffCommand()
        || isProgramChangeCommand()
        || isAftertouchCommand()
        || isControllerCommand())
    {
        return 2;
    }
    else if (isChannelPressureCommand()
        || isPitchCommand())
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int MidiCommandRangeAssignment::getCommandDataExpectedBytes(const juce::MidiMessage& m)
{
    if (m.isNoteOn()
        || m.isNoteOff()
        || m.isProgramChange()
        || m.isAftertouch()
        || m.isController())
    {
        return 2;
    }
    else if (m.isChannelPressure()
        || m.isPitchWheel())
    {
        return 1;
    }
    else
    {
        return 0;
    }
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

bool MidiCommandRangeAssignment::isRangedCommandAssignment() const
{
    return (m_valueRange.getStart() != 0 || m_valueRange.getEnd() != 0);
}

/**
 * Method to serialize the entire current assignment information
 * into a string that can be stored in config.
 * If the current assignment uses a range, the range start and end values
 * are included in the byte string representation as the last 4 bytes.
 * @return The resulting serialized byte data string.
 */
juce::String MidiCommandRangeAssignment::serializeToHexString() const
{
    auto serialData = m_commandData;
    if (isRangedCommandAssignment())
    {
        // The start/end int values are stored as two additional bytes each at the end of the data buffer
        auto start = m_valueRange.getStart();
        auto end = m_valueRange.getEnd();
        serialData.push_back((start & 0xff00) >> 8);
        serialData.push_back((start & 0x00ff));
        serialData.push_back((end & 0xff00) >> 8);
        serialData.push_back((end & 0x00ff));
    }

    auto byteStrings = StringArray();
    for (auto const& byte : serialData)
        byteStrings.add(String::toHexString(byte));
    
    return byteStrings.joinIntoString(" ");
}

/**
 * Method to read assignment data from a serialized string.
 * The string is expected as space-separated hex byte values that
 * are read into a uin8 buffer. The first bytes are then parsed for
 * information on the encapsulated command type, which then us used
 * to derive the expecte command byte count from. Depending on if four
 * bytes, that can be taken as range data, are available apart from the command data,
 * these are read into start/end range values as well.
 * @param serialData    The serial byte data string to read assignment data from.
 * @return  True on success, false if the input data was invalid.
 */
bool MidiCommandRangeAssignment::deserializeFromHexString(const juce::String& serialData)
{
    // read the given serial hex string data into byte vector
    auto byteData = std::vector<std::uint8_t>();
    auto byteStrings = StringArray::fromTokens(serialData, " ", StringRef());
    for (auto const& byteString : byteStrings)
        byteData.push_back(static_cast<std::uint8_t>(byteString.getHexValue32()));
    auto byteDataLength = byteData.size();
    
    // save the current command data to be able to restore it, if something goes wrong
    auto commandDataStash = m_commandData;
    
    // take over the just read byte vector into internal command data, to be able to use internal processing methods on ut
    m_commandData = byteData;
    auto newCommandDataByteLength = getCommandDataExpectedBytes(); // this relies on m_commandData (which is why we already modified it in the prev. line)
    // if the command data length is zero, something went wrong and we did not recognize the command
    jassert(newCommandDataByteLength != 0);
    if (newCommandDataByteLength != 0)
    {
        // trim the copied byte vector data in commanddata to only contain the actual command data
        m_commandData.resize(newCommandDataByteLength);

        // determine if the byte data contains four extra bytes for min-max range
        auto rangeDataLength = byteDataLength - newCommandDataByteLength;
        if (rangeDataLength == 4)
        {
            m_valueRange.setStart((byteData.at(byteDataLength - 4) << 8) | byteData.at(byteDataLength - 3));
            m_valueRange.setEnd((byteData.at(byteDataLength - 2) << 8) | byteData.at(byteDataLength - 1));

            return true;
        }
        else if (rangeDataLength == 0)
        {
            return true;
        }
    }

    // if we end up here, something went wrong and we need to restore the original commandData
    m_commandData = commandDataStash;
    return false;
}

    
} // namespace JUCEAppBasics

} // namespace Midi_utils
