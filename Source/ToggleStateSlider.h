/* Copyright(c) 2025, Christian Ahrens
 *
 * This file is part of JUCE-AppBasics <https://github.com/ChristianAhrens/JUCE-AppBasics>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#pragma once

#include <JuceHeader.h>

namespace JUCEAppBasics
{

//==============================================================================
/**
 * FixedFontTextEditor is a reimplementation of a juce:TextEditor with the only
 * differing behaviour to reset all text colour to what is set as default in
 * current juce::Desktop lookAndFeel on every lookAndFeelChanged call.
 */
class ToggleStateSlider : public juce::Slider
{
public:
    ToggleStateSlider();
    explicit ToggleStateSlider(const juce::String& componentName = juce::String());
    ToggleStateSlider(juce::Slider::SliderStyle style, juce::Slider::TextEntryBoxPosition textBoxPosition);
    virtual ~ToggleStateSlider() override;

    //==============================================================================
    void setToggleState(bool shouldBeOn, NotificationType notification);
    bool getToggleState() const noexcept;

    //==============================================================================
    std::function<void()> onToggleStateChange;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;
    void lookAndFeelChanged() override;

    //==============================================================================
    void mouseUp(const MouseEvent& e) override;

private:
    //==============================================================================
    bool m_isToggledOn = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToggleStateSlider)
};

}
