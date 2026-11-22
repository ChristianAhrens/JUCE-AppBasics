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
 * Setter for the on/off toggle state of the slider. Will be ignored if toggling is globally disabled
 * @param	shouldBeOn	Boolean on/off value
 * @param	notification	Notification type to define if the change should be promoted onwards
 */
void ToggleStateSlider::setToggleState(bool shouldBeOn, juce::NotificationType notification)
{
	if (!m_isTogglable)
		return;

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
 * Setter for if the slider toggle capability should be enabled
 * @param	isToggleable	True if toggling shall be enabled
 */
void ToggleStateSlider::setTogglalbe(bool isTogglable)
{
	m_isTogglable = isTogglable;

	setToggleState(true, juce::dontSendNotification);
}

/**
 * Getter for the boolean is-dragging state
 * @return True if the fader is currently being dragged, false if not
 */
bool ToggleStateSlider::isDragging() const
{
	return m_isDragging;
}

/**
 * Reimplemented for fun
 */
void ToggleStateSlider::paint(juce::Graphics& g)
{
    juce::Slider::paint(g);
}

/**
 * Reimplemented for fun
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
 * Reimplemented to keep a plain click from also moving the value: juce::Slider snaps the value to the mouse
 * position already on mouseDown. A plain (unmodified, left button) click on a togglable slider is therefore
 * only remembered and handed to juce::Slider once the mouse has actually moved (see mouseDrag). A click that
 * is released without movement only toggles (see mouseUp).
 * @param	e	The mouse event for the mousedown
 */
void ToggleStateSlider::mouseDown(const juce::MouseEvent& e)
{
	m_pendingMouseDown.reset();

	if (m_isTogglable && isEnabled() && !e.mods.isPopupMenu() && e.mods.withoutMouseButtons() == juce::ModifierKeys())
	{
		m_pendingMouseDown.emplace(e);
		return;
	}

	juce::Slider::mouseDown(e);
}

/**
 * Reimplemented to catch the drag distance and if it is considerably small
 * initiate a toggle state flip.
 * Also ends any is-dragging states
 * @param	e	The mouse event for the mouseup
 */
void ToggleStateSlider::mouseUp(const juce::MouseEvent& e)
{
	if (e.getDistanceFromDragStart() <= 1)
		setToggleState(!getToggleState(), juce::sendNotification);

	if (m_pendingMouseDown.has_value())
	{
		// juce::Slider never saw the mousedown, so it must not see the mouseup either
		m_pendingMouseDown.reset();
		m_isDragging = false;
		return;
	}

    if (m_isDragging)
        m_isDragging = false;

	juce::Slider::mouseUp(e);
}

/**
 * Reimplemented to catch the drag distance and if it is considerably large
 * set the is-dragging status.
 * @param	e	The mouse event for the mouseDrag
 */
void ToggleStateSlider::mouseDrag(const juce::MouseEvent& e)
{
    auto offset = e.getOffsetFromDragStart();
    if (std::abs(offset.getDistanceFromOrigin()) > 1)
        m_isDragging = true;

    if (m_pendingMouseDown.has_value())
    {
        if (!m_isDragging)
            return; // still a click

        juce::Slider::mouseDown(*m_pendingMouseDown);
        m_pendingMouseDown.reset();
    }

	juce::Slider::mouseDrag(e);
}

/**
 * Reimplemented to repaint for the hover state of the lookAndFeel, juce::Slider does not repaint on its own
 * @param	e	The mouse event for the mouseEnter
 */
void ToggleStateSlider::mouseEnter(const juce::MouseEvent& e)
{
	juce::Slider::mouseEnter(e);
	repaint();
}

/**
 * Reimplemented to repaint for the hover state of the lookAndFeel
 * @param	e	The mouse event for the mouseExit
 */
void ToggleStateSlider::mouseExit(const juce::MouseEvent& e)
{
	juce::Slider::mouseExit(e);
	repaint();
}


}

