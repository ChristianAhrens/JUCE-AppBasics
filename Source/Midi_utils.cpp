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
    setCommandData(m);
    extendValueRange(m);
}

MidiCommandRangeAssignment::~MidiCommandRangeAssignment()
{
}

bool MidiCommandRangeAssignment::operator==(const MidiCommandRangeAssignment& rhs) const
{
    return (m_commandData == rhs.m_commandData && m_valueRange == rhs.m_valueRange && m_commandRange == rhs.m_commandRange);
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
    if (getAsValue(m_commandData) > getAsValue(rhs.m_commandData))
        return true;

    auto valueRangeGreaterThanRhs = false;
    if (m_valueRange.intersects(rhs.m_valueRange))
        valueRangeGreaterThanRhs = m_valueRange.getLength() > rhs.m_valueRange.getLength();
    else
        valueRangeGreaterThanRhs = m_valueRange.getStart() > rhs.m_valueRange.getStart();

    auto commandRangeGreaterThanRhs = false;
    auto commandRangeV = juce::Range<std::uint64_t>(getAsValue(m_commandRange.getStart()), getAsValue(m_commandRange.getEnd()));
    auto rhsCommandRangeV = juce::Range<std::uint64_t>(getAsValue(rhs.m_commandRange.getStart()), getAsValue(rhs.m_commandRange.getEnd()));
    if (commandRangeV.intersects(rhsCommandRangeV))
        commandRangeGreaterThanRhs = commandRangeV.getLength() > rhsCommandRangeV.getLength();
    else
        commandRangeGreaterThanRhs = commandRangeV.getStart() > rhsCommandRangeV.getStart();

    return ((m_commandData == rhs.m_commandData) && (valueRangeGreaterThanRhs || commandRangeGreaterThanRhs));
}

MidiCommandRangeAssignment& MidiCommandRangeAssignment::operator=(const MidiCommandRangeAssignment& rhs)
{
    if (this != &rhs)
    {
        m_valueRange = rhs.m_valueRange;
        m_commandRange = rhs.m_commandRange;
        m_commandData = rhs.m_commandData;
    }

    return *this;
}

std::uint64_t MidiCommandRangeAssignment::getAsValue(const std::vector<std::uint8_t>& data)
{
    jassert(data.size() <= 8); // more than 8 bytes won't fit into a uint64

    auto retVal = std::uint64_t();

    auto shiftMask = 0;
    for (auto iter = data.end() - 1; iter > data.begin(); iter--)
    {
        retVal += (*iter << shiftMask);
        shiftMask += 8;
    }

    return retVal;
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

    if (((commandData[0] & 0xf0) == 0x90) && ((commandData.size() > 2) && (commandData[2] != 0)))
        return true;
    else
        return false;
}

bool MidiCommandRangeAssignment::isNoteOffCommand(const std::vector<std::uint8_t>& commandData)
{
    if (commandData.empty())
        return false;

    if ((commandData[0] & 0xf0) == 0x80)
        return true;
    else if (((commandData[0] & 0xf0) == 0x90) && ((commandData.size() > 2) && (commandData[2] == 0)))
        return true;
    else
        return false;
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
    auto commandType = CT_Invalid;

    if (m.isNoteOn())
        commandType = CT_NoteOn;
    else if (m.isNoteOff())
        commandType = CT_NoteOff;
    else if (m.isProgramChange())
        commandType = CT_ProgramChange;
    else if (m.isPitchWheel())
        commandType = CT_Pitch;
    else if (m.isAftertouch())
        commandType = CT_Aftertouch;
    else if (m.isController())
        commandType = CT_Controller;
    else if (m.isChannelPressure())
        commandType = CT_ChannelPressure;

    return commandType;
}

MidiCommandRangeAssignment::CommandType MidiCommandRangeAssignment::getCommandType(const std::vector<std::uint8_t>& commandData)
{
    auto mcra = MidiCommandRangeAssignment();
    mcra.setCommandData(commandData);
    
    return mcra.getCommandType();
}

bool MidiCommandRangeAssignment::isMatchingCommandType(const juce::MidiMessage& m) const
{
    return getCommandType() == getCommandType(m);
}

bool MidiCommandRangeAssignment::isMatchingCommandType(const std::vector<std::uint8_t>& commandData) const
{
    return getCommandType() == getCommandType(commandData);
}

int MidiCommandRangeAssignment::getCommandValue() const
{
    if (m_commandData.size() < 2)
        return -1;

    if (isNoteOnCommand())
    {
        return m_commandData.at(1);
    }
    else if (isNoteOffCommand())
    {
        return m_commandData.at(1);
    }
    else if (isProgramChangeCommand())
    {
        return m_commandData.at(1);
    }
    else if (isControllerCommand())
    {
        return m_commandData.at(1);
    }
    else
    {
        return -1;
    }
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
        return -1;
    }
}

int MidiCommandRangeAssignment::getCommandValue(const std::vector<std::uint8_t>& commandData)
{
    if (commandData.size() < 2)
        return -1;

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
        return -1;
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
    auto commandDataExpectedByteCount = getCommandDataExpectedBytes(m);

    commandData.reserve(commandDataExpectedByteCount);
    
    if (commandDataExpectedByteCount > dataBytesLength)
        return commandData;
    
    for (int i = 0; i < commandDataExpectedByteCount; ++i)
        commandData.push_back(dataBytes[i]);

    return commandData;
}

int MidiCommandRangeAssignment::getCommandDataExpectedBytes() const
{
    if (isNoteOnCommand()
        || isNoteOffCommand())
    {
        return 3;
    }
    else if (isProgramChangeCommand()
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
        || m.isNoteOff())
    {
        return 3;
    }
    else if (m.isProgramChange()
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
    if (isValueRangeAssignment() || isCommandRangeAssignment())
    {
        // The start/end int values are stored as two additional bytes each at the end of the data buffer
        auto vrstart = m_valueRange.getStart();
        auto vrend = m_valueRange.getEnd();
        serialData.push_back((vrstart & 0xff00) >> 8);
        serialData.push_back((vrstart & 0x00ff));
        serialData.push_back((vrend & 0xff00) >> 8);
        serialData.push_back((vrend & 0x00ff));

        if (isCommandRangeAssignment())
        {
            // The start/end int values are stored as two additional bytes each at the end of the data buffer
            auto crstart = m_commandRange.getStart();
            auto crend = m_commandRange.getEnd();
            jassert(crstart.size() == crend.size());
            auto rangeByteCount = static_cast<std::uint8_t>(crstart.size() + crend.size());
            serialData.push_back(rangeByteCount);
            serialData.insert(serialData.end(), crstart.begin(), crstart.end());
            serialData.insert(serialData.end(), crend.begin(), crend.end());
        }
    }

    // dump all the serialized bytes into a byte string array
    auto byteStrings = StringArray();
    for (auto const& byte : serialData)
        byteStrings.add(String::toHexString(byte));
    
    // join the byte strings into a single space separated string and return it
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
    // Read the given serial hex string data into byte vector
    auto byteData = std::vector<std::uint8_t>();
    auto byteStrings = StringArray::fromTokens(serialData, " ", StringRef());
    for (auto const& byteString : byteStrings)
        byteData.push_back(static_cast<std::uint8_t>(byteString.getHexValue32()));
    auto byteDataLength = byteData.size();
    
    // Save the current command data to be able to restore it, if something goes wrong
    auto commandDataStash = m_commandData;
    
    // Take over the just read byte vector into internal command data, to be able to use internal processing methods on ut
    m_commandData = byteData;
    auto newCommandDataByteLength = getCommandDataExpectedBytes(); // this relies on m_commandData (which is why we already modified it in the prev. line)
    // If the command data length is zero, something went wrong and we did not recognize the command
    jassert(newCommandDataByteLength != 0);
    if (newCommandDataByteLength != 0)
    {
        // Trim the copied byte vector data in commanddata to only contain the actual command data
        m_commandData.resize(newCommandDataByteLength);

        // Determine if the byte data contains four extra bytes for min-max range
        auto rangeDataLength = byteDataLength - newCommandDataByteLength;
        if (rangeDataLength != 0)
        {
            auto valRangeByteCount = 4;
            // Four additional bytes are interpreted as appended 2*2 bytes value range
            if (rangeDataLength >= valRangeByteCount)
            {
                auto valRangeBytePos = newCommandDataByteLength;
                m_valueRange.setStart(byteData.at(valRangeBytePos + 1) + (byteData.at(valRangeBytePos) << 8));
                m_valueRange.setEnd(byteData.at(valRangeBytePos + 3) + (byteData.at(valRangeBytePos + 2) << 8));

                // if only the four bytes were present, we are done
                if (rangeDataLength == valRangeByteCount)
                    return true;
                // otherwise, we expect the next byte to define the count of commandrange bytes that still follow
                else
                {
                    auto cmdRangeBytePos = valRangeBytePos + valRangeByteCount;
                    auto cmdRangeByteCount = byteData.at(cmdRangeBytePos);
                    cmdRangeBytePos++;

                    // If we have four additional bytes and a straight number of bytes on top, we assume a value range 
                    // followed by a command range being encoded in the additional bytes
                    if (((cmdRangeByteCount) % 2 == 0) && ((byteData.size() - cmdRangeBytePos) % 2 == 0))
                    {
                        // we assume that half of the command range bytes are start and half are end value
                        auto rangeValByteCount = cmdRangeByteCount / 2;
                        auto rangeStart = std::vector<std::uint8_t>();
                        rangeStart.reserve(rangeValByteCount);
                        auto rangeEnd = std::vector<std::uint8_t>();
                        rangeEnd.reserve(rangeValByteCount);
                        auto i = 0;
                        for (; i < rangeValByteCount; ++i)
                            rangeStart.push_back(byteData.at(cmdRangeBytePos + i));
                        for (; i < cmdRangeByteCount; ++i)
                            rangeEnd.push_back(byteData.at(cmdRangeBytePos + i));

                        m_commandRange.setStart(rangeStart);
                        m_commandRange.setEnd(rangeEnd);

                        return true;
                    }
                }

            }
        }
        else
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
