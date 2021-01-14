/*
  ==============================================================================

    MidiLearnerComponent.h
    Created: 14 Jan 2021 12:06:32pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace JUCEAppBasics
{

class MidiLearnerComponent :
    public Component,
    public Button::Listener,
    private MidiInputCallback,
    private MessageListener
{
public:
    struct MidiAssi
    {
        String _rawMsg;
    };

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
    std::function<void(MidiAssi)> onMidiAssiSet;
    
    //==============================================================================
    void setSelectedDeviceIdx(int deviceIdx);
    void setCurrentMidiAssi(MidiAssi currentAssi);

private:
    class CallbackMidiMessage : public Message
    {
    public:
        /**
        * Constructor with default initialization of message and source.
        * @param m    The midi message to handle.
        */
        CallbackMidiMessage(const juce::MidiMessage& m) : _message(m) {}

        juce::MidiMessage _message;
    };
    
private:
    void showMenu();
    void handlePopupResult(int resultingAssiIdx);

    std::unique_ptr<TextEditor>         m_currentMidiAssiEdit;
    std::unique_ptr<DrawableButton>     m_learnButton;
    int                                 m_deviceIdx{ -1 };
    PopupMenu                           m_popup;
    std::map<int, MidiAssi>             m_learnedAssiMap;
    std::unique_ptr<AudioDeviceManager> m_deviceManager;
    MidiAssi                            m_currentMidiAssi;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiLearnerComponent)
};

};
