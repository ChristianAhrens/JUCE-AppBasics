/*
  ==============================================================================

    OverlayToggleComponentBase.cpp
    Created: 11 Jul 2020 10:10:30am
    Author:  musah

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace JUCEAppBasics
{

class OverlayToggleComponentBase  :   public Component
{
public:
    enum OverlayState
    {
        invalid   = 0,
        minimized = 1,
        maximized = 2,
    };

public:
    class OverlayParent
    {
    public:
        virtual ~OverlayParent() {};
        void activateOverlayComponent(OverlayToggleComponentBase* overlay)
        {
            m_activeOverlay = overlay;
        };
        bool isOverlayActive()
        {
            return (m_activeOverlay != nullptr);
        };
        OverlayToggleComponentBase* getOverlay()
        {
            return m_activeOverlay;
        };

    protected:
        OverlayToggleComponentBase *m_activeOverlay{ nullptr };
    };

public:
    //==============================================================================
    OverlayToggleComponentBase();
    ~OverlayToggleComponentBase() override;

    void addOverlayParent(OverlayParent *p);

    Rectangle<int>  getOverlayBounds() const;
    void toggleClicked();

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

    //==============================================================================
    std::function<void()> parentResize;

protected:
    OverlayState getCurrentOverlayState() const;
    virtual void changeOverlayState();

private:
    void setMinimized();
    void setMaximized();

    //==============================================================================
    OverlayParent *m_overlayParent{ nullptr };

    //==============================================================================
    std::unique_ptr<DrawableButton> m_toggleOverlayButton;

    //==============================================================================
    OverlayState    m_overlayState{ invalid };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OverlayToggleComponentBase)
};

}; // namespace JUCEAppBasics