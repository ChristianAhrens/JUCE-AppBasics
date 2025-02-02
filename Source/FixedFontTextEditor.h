/* Copyright(c) 2023, Christian Ahrens
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
class FixedFontTextEditor : public juce::TextEditor
{
public:    
    explicit FixedFontTextEditor(const juce::String& componentName = String(),
        juce_wchar passwordCharacter = 0, bool autoResizeFont = false);
    virtual ~FixedFontTextEditor() override;

    //==============================================================================
    void resized() override;
    void lookAndFeelChanged() override;

private:
    //==============================================================================
    bool m_autoResizeFont = false;
};

}
