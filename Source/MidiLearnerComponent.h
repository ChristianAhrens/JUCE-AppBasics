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
    public Button::Listener
{
public:
    struct MidiAssi
    {
        String _undef;
    };

    class MidiAssiLearningPopupMenu : public PopupMenu
    {
    public:
        MidiAssiLearningPopupMenu()
        {
        }

        ~MidiAssiLearningPopupMenu()
        {
        }

        MidiAssi show(int itemIDThatMustBeVisible = 0,
            int minimumWidth = 0,
            int maximumNumColumns = 0,
            int standardItemHeight = 0,
            ModalComponentManager::Callback* callback = nullptr)
        {
            auto itemId = PopupMenu::show(itemIDThatMustBeVisible, minimumWidth, maximumNumColumns, standardItemHeight, callback);

            return MidiAssi();
        }
    };

public:
    MidiLearnerComponent();
    ~MidiLearnerComponent();
	
    //==============================================================================
    void resized() override;

    //==============================================================================
    void buttonClicked(Button*) override;

    //==============================================================================
    std::function<void(MidiAssi)> onMidiAssiSet;

private:
    MidiAssi showMenuAndGetMidiAssi();

    std::unique_ptr<TextEditor>         m_currentMidiAssiEdit;
    std::unique_ptr<DrawableButton>     m_learnButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiLearnerComponent)
};

};
