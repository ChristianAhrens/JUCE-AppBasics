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

#include "FixedFontTextEditor.h"


namespace JUCEAppBasics
{


/**
 * Constructor that simply acts as proxy to TextEditor base class constructor
 */
FixedFontTextEditor::FixedFontTextEditor(const juce::String& componentName, juce_wchar passwordCharacter, bool autoResizeFont)
	: juce::TextEditor(componentName, passwordCharacter)
{
	m_autoResizeFont = autoResizeFont;
}

/**
 * Destructor
 */
FixedFontTextEditor::~FixedFontTextEditor()
{

}

/**
 * Reimplemented to optionally also resize the texteditor font used
 */
void FixedFontTextEditor::resized()
{
	if (m_autoResizeFont)
		applyFontToAllText(getFont().withHeight(getLocalBounds().getHeight() * 0.9f));

	juce::TextEditor::resized();
}

/**
 * Reimplemented method that extends the base class implementation
 * with setting the text colour to default of changed lookAndFeel
 */
void FixedFontTextEditor::lookAndFeelChanged()
{
	applyColourToAllText(getLookAndFeel().findColour(TextEditor::textColourId), true);

	juce::TextEditor::lookAndFeelChanged();
}


}
