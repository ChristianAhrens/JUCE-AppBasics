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

MidiCommandRangeAssignment::MidiCommandRangeAssignment(const std::vector<std::uint8_t>& commandData)
{
    setCommandData(commandData);
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
    return getCommandDescription(m_commandData);
}

juce::String MidiCommandRangeAssignment::getCommandDescription(const std::vector<std::uint8_t>& commandData)
{
    if (isNoteOnCommand(commandData) && (commandData.size() > 1))
        return "NoteOn " + MidiMessage::getMidiNoteName(commandData[1], true, true, 3);

    else if (isNoteOffCommand(commandData) && (commandData.size() > 1))
        return "NoteOff " + MidiMessage::getMidiNoteName(commandData[1], true, true, 3);

    else if (isProgramChangeCommand(commandData) && (commandData.size() > 1))
        return "ProgChange" + String(commandData[1]);

    else if (isAftertouchCommand(commandData) && (commandData.size() > 1))
        return "Aftertouch" + String(commandData[1]);
    // aftertouch value
    //data[2]

    else if (isControllerCommand(commandData) && (commandData.size() > 1))
        return "Ctrl" + String(commandData[1]);
    //// controller value
    //data[2]
    
    else if (isPitchCommand(commandData))
        return "Pitch";
    //// pitch wheel value
    //data[1] | (data[2] << 7)
    
    else if (isChannelPressureCommand(commandData))
        return "Ch. pressure";
    //// ch. pres. value
    //data[1]

    else
        return "Unassigned";
}

bool MidiCommandRangeAssignment::isNoteOnCommand() const
{
    return isNoteOnCommand(m_commandData);
}

bool MidiCommandRangeAssignment::isNoteOffCommand() const
{
    return isNoteOffCommand(m_commandData);
}

bool MidiCommandRangeAssignment::isProgramChangeCommand() const
{
    return isProgramChangeCommand(m_commandData);
}

bool MidiCommandRangeAssignment::isPitchCommand() const
{
    return isPitchCommand(m_commandData);
}

bool MidiCommandRangeAssignment::isAftertouchCommand() const
{
    return isAftertouchCommand(m_commandData);
}

bool MidiCommandRangeAssignment::isControllerCommand() const
{
    return isControllerCommand(m_commandData);
}

bool MidiCommandRangeAssignment::isChannelPressureCommand() const
{
    return isChannelPressureCommand(m_commandData);
}

bool MidiCommandRangeAssignment::isNoteOnCommand(const std::vector<std::uint8_t>& commandData)
{
    if (commandData.empty())
        return false;

    return ((commandData[0] & 0xf0) == 0x90);
}

bool MidiCommandRangeAssignment::isNoteOffCommand(const std::vector<std::uint8_t>& commandData)
{
    if (commandData.empty())
        return false;

    return ((commandData[0] & 0xf0) == 0x80);
}

bool MidiCommandRangeAssignment::isProgramChangeCommand(const std::vector<std::uint8_t>& commandData)
{
    if (commandData.empty())
        return false;

    return ((commandData[0] & 0xf0) == 0xc0);
}

bool MidiCommandRangeAssignment::isPitchCommand(const std::vector<std::uint8_t>& commandData)
{
    if (commandData.empty())
        return false;

    return ((commandData[0] & 0xf0) == 0xe0);
}

bool MidiCommandRangeAssignment::isAftertouchCommand(const std::vector<std::uint8_t>& commandData)
{
    if (commandData.empty())
        return false;

    return ((commandData[0] & 0xf0) == 0xa0);
}

bool MidiCommandRangeAssignment::isControllerCommand(const std::vector<std::uint8_t>& commandData)
{
    if (commandData.empty())
        return false;

    return ((commandData[0] & 0xf0) == 0xb0);
}

bool MidiCommandRangeAssignment::isChannelPressureCommand(const std::vector<std::uint8_t>& commandData)
{
    if (commandData.empty())
        return false;

    return ((commandData[0] & 0xf0) == 0xd0);
}

MidiCommandRangeAssignment::CommandType MidiCommandRangeAssignment::getCommandType() const
{
    auto commandType = CT_Invalid;

    if (isNoteOnCommand())
        commandType = CT_NoteOn;
    else if (isNoteOffCommand())
        commandType = CT_NoteOff;
    else if (isProgramChangeCommand())
        commandType = CT_ProgramChange;
    else if (isPitchCommand())
        commandType = CT_Pitch;
    else if (isAftertouchCommand())
        commandType = CT_Aftertouch;
    else if (isControllerCommand())
        commandType = CT_Controller;
    else if (isChannelPressureCommand())
        commandType = CT_ChannelPressure;

    return commandType;
}

MidiCommandRangeAssignment::CommandType MidiCommandRangeAssignment::getCommandType(const juce::MidiMessage& m)
{
    return MidiCommandRangeAssignment(m).getCommandType();
}

MidiCommandRangeAssignment::CommandType MidiCommandRangeAssignment::getCommandType(const std::vector<std::uint8_t>& commandData)
{
    auto mcra = MidiCommandRangeAssignment();
    mcra.setCommandData(commandData);
    
    return mcra.getCommandType();
}

int MidiCommandRangeAssignment::getCommandValue(const juce::MidiMessage& m)
{
    if (m.isNoteOn())
    {
        return m.getNoteNumber();
    }
    else if (m.isNoteOff())
    {
        return m.getNoteNumber();
    }
    else if (m.isProgramChange())
    {
        return m.getProgramChangeNumber();
    }
    else if (m.isController())
    {
        return m.getControllerNumber();
    }
    else
    {
        return 0;
    }
}

int MidiCommandRangeAssignment::getCommandValue(const std::vector<std::uint8_t>& commandData)
{
    if (commandData.size() < 2)
        return 0;

    auto mcra = MidiCommandRangeAssignment(commandData);

    if (mcra.isNoteOnCommand())
    {
        return commandData.at(1);
    }
    else if (mcra.isNoteOffCommand())
    {
        return commandData.at(1);
    }
    else if (mcra.isProgramChangeCommand())
    {
        return commandData.at(1);
    }
    else if (mcra.isControllerCommand())
    {
        return commandData.at(1);
    }
    else
    {
        return 0;
    }
}

juce::String MidiCommandRangeAssignment::getValueRangeDescription() const
{
    if (m_valueRange.isEmpty())
        return String(m_valueRange.getStart());
    else
        return String(m_valueRange.getStart()) + " - " + String(m_valueRange.getEnd());
}

juce::String MidiCommandRangeAssignment::getCommandRangeDescription() const
{
    if (m_commandRange.isEmpty())
        return getCommandDescription(m_commandRange.getStart());
    else
        return getCommandDescription(m_commandRange.getStart()) + " - " + getCommandDescription(m_commandRange.getEnd());
}

juce::String MidiCommandRangeAssignment::getNiceDescription() const
{
    if (isCommandRangeAssignment() && isValueRangeAssignment())
        return getCommandRangeDescription() + " (" + getValueRangeDescription() + ")";
    else if (isCommandRangeAssignment())
        return getCommandRangeDescription();
    else if (isValueRangeAssignment())
        return getCommandDescription() + " (" + getValueRangeDescription() + ")";
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

void MidiCommandRangeAssignment::setCommandData(const std::vector<std::uint8_t>& commandData)
{
    m_commandData = commandData;
}

void MidiCommandRangeAssignment::setCommandData(const juce::MidiMessage& m)
{
    m_commandData = getCommandData(m);
}

int MidiCommandRangeAssignment::getValue(const juce::MidiMessage& m)
{
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

    return value;
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
    else if (getCommandType(m) != getCommandType())
        return false;

    return extendValueRange(getValue(m));
}

bool MidiCommandRangeAssignment::isValueRangeAssignment() const
{
    return (m_valueRange.getStart() != 0 || m_valueRange.getEnd() != 0);
}

bool MidiCommandRangeAssignment::isMatchingValueRange(int v) const
{
    return ((m_valueRange.getStart() <= v) && (m_valueRange.getEnd() >= v));
}

bool MidiCommandRangeAssignment::isMatchingValueRange(const juce::MidiMessage& m) const
{
    return isMatchingValueRange(getValue(m));
}

const juce::Range<std::vector<std::uint8_t>>& MidiCommandRangeAssignment::getCommandRange() const
{
    return m_commandRange;
}

void MidiCommandRangeAssignment::setCommandRange(const juce::Range<std::vector<std::uint8_t>>& cr)
{
    m_commandRange = cr;
}

bool MidiCommandRangeAssignment::extendCommandRange(const std::vector<std::uint8_t>& c)
{
    if (isMatchingCommandRange(c))
        return false;

    if (isCommandRangeAssignment())
    {
        auto commandValue = getCommandValue(c);
        auto commandRangeLowerValue = getCommandValue(m_commandRange.getStart());
        auto commandRangeUpperValue = getCommandValue(m_commandRange.getEnd());

        if (commandRangeLowerValue > commandValue)
        {
            m_commandRange.setStart(c);
            return true;
        }
        if (commandRangeUpperValue < commandValue)
        {
            m_commandRange.setEnd(c);
            return true;
        }
        else
            return false;
    }
    else
    {
        setCommandRange(juce::Range<std::vector<std::uint8_t>>(c, c));
        return true;
    }
}

bool MidiCommandRangeAssignment::extendCommandRange(const juce::MidiMessage& m)
{
    return extendCommandRange(MidiCommandRangeAssignment(m).getCommandData());
}

bool MidiCommandRangeAssignment::isCommandRangeAssignment() const
{
    return (!m_commandRange.getStart().empty() && !m_commandRange.getEnd().empty());
}

bool MidiCommandRangeAssignment::isMatchingCommand(const juce::MidiMessage& m) const
{
    return (m_commandData == getCommandData(m));
}

bool MidiCommandRangeAssignment::isMatchingCommandRange(const std::vector<std::uint8_t>& c) const
{
    if (!isCommandRangeAssignment())
        return false;

    auto commandType = getCommandType(getCommandRange().getStart());
    if (commandType != getCommandType(getCommandRange().getEnd()))
        return false;

    auto commandRange = juce::Range<int>(getCommandValue(getCommandRange().getStart()), getCommandValue(getCommandRange().getEnd()));;
    auto inputCommandValue = getCommandValue(c);

    return ((commandRange.getStart() <= inputCommandValue) && (commandRange.getEnd() >= inputCommandValue));
}

bool MidiCommandRangeAssignment::isMatchingCommandRange(const juce::MidiMessage& m) const
{
    if (!isCommandRangeAssignment())
        return false;

    auto commandType = getCommandType(getCommandRange().getStart());
    if (commandType != getCommandType(getCommandRange().getEnd()))
        return false;

    auto commandRange = juce::Range<int>(getCommandValue(getCommandRange().getStart()), getCommandValue(getCommandRange().getEnd()));;
    auto inputCommandValue = getCommandValue(m);

    return ((commandRange.getStart() <= inputCommandValue) && (commandRange.getEnd() >= inputCommandValue));
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
    if (isValueRangeAssignment())
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
