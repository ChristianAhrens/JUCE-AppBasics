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

#include "ToggleStateSlider.h"


namespace JUCEAppBasics
{


/**
 * Constructor that simply acts as proxy to Slider base class constructor
 */
ToggleStateSlider::ToggleStateSlider()
	: juce::Slider()
{
}

/**
 * Constructor that simply acts as proxy to Slider base class constructor
 */
ToggleStateSlider::ToggleStateSlider(const juce::String& componentName)
	: juce::Slider(componentName)
{
}

/**
 * Constructor that simply acts as proxy to Slider base class constructor
 */
ToggleStateSlider::ToggleStateSlider(juce::Slider::SliderStyle style, juce::Slider::TextEntryBoxPosition textBoxPosition)
	: juce::Slider(style, textBoxPosition)
{
	
}

/**
 * Destructor
 */
ToggleStateSlider::~ToggleStateSlider()
{

}

/**
 * Setter for the on/off toggle state of the slider
 * @param	shouldBeOn	Boolean on/off value
 * @param	notification	Notification type to define if the change should be promoted onwards
 */
void ToggleStateSlider::setToggleState(bool shouldBeOn, juce::NotificationType notification)
{
	m_isToggledOn = shouldBeOn;

	if (juce::dontSendNotification != notification && onToggleStateChange)
		onToggleStateChange();

	repaint();
}

/**
 * Getter for the boolean on/off state
 * @return True if the fader is toggled on, false if not
 */
bool ToggleStateSlider::getToggleState() const noexcept
{
	return m_isToggledOn;
}

/**
 * Reimplemented to optionally also resize the texteditor font used
 */
void ToggleStateSlider::paint(juce::Graphics& g)
{
    juce::Slider::paint(g);
}

/**
 * Reimplemented to optionally also resize the texteditor font used
 */
void ToggleStateSlider::resized()
{
	juce::Slider::resized();
}

/**
 * Reimplemented method that extends the base class implementation
 * with setting the text colour to default of changed lookAndFeel
 */
void ToggleStateSlider::lookAndFeelChanged()
{
	juce::Slider::lookAndFeelChanged();
}

/**
 * Reimplemented to catch the drag distance and if it is considerably small
 * initiate a toggle state flip
 * @param	e	The mouse event for the mouseup
 */
void ToggleStateSlider::mouseUp(const MouseEvent& e)
{
	if (e.getDistanceFromDragStart() <= 1)
		setToggleState(!getToggleState(), juce::sendNotification);

	juce::Slider::mouseUp(e);
}


}
