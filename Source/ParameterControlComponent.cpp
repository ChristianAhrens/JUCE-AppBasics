/* Copyright (c) 2026, Christian Ahrens
 *
 * This file is part of JUCE-AppBasics <https://github.com/ChristianAhrens/JUCE-AppBasics>
 *
 * This tool is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This tool is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this tool; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "ParameterControlComponent.h"

#include "CustomLookAndFeel.h"
#include "ToggleStateSlider.h"


namespace JUCEAppBasics
{


ParameterControlComponent::ParameterControlComponent()
{
    m_parameterControlsGrid = std::make_unique<juce::Grid>();
}

ParameterControlComponent::~ParameterControlComponent() = default;

void ParameterControlComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void ParameterControlComponent::resized()
{
    m_parameterBounds = getLocalBounds();
    rebuildLayout();
}

void ParameterControlComponent::lookAndFeelChanged()
{
    auto accentColour = getLookAndFeel().findColour(
        JUCEAppBasics::CustomLookAndFeel::ColourIds::MeteringRmsColourId);

    for (auto& [idx, combo] : m_parameterValueComboBoxes)
        combo->setColour(juce::ComboBox::ColourIds::focusedOutlineColourId, accentColour);

    for (auto& [idx, slider] : m_parameterValueSliders)
        slider->setColour(juce::Slider::ColourIds::trackColourId, accentColour);

    for (auto& [idx, btn] : m_parameterValueButtons)
        btn->setColour(juce::TextButton::ColourIds::buttonOnColourId, accentColour);
}

void ParameterControlComponent::setParameters(const std::vector<ParameterControlInfo>& parameters)
{
    if (m_parameters == parameters)
        return;

    // Destroy all existing widgets — handles both removals and type changes on the same index.
    // Component destructors automatically remove children from this parent.
    m_parameterNameLabels.clear();
    m_parameterValueButtons.clear();
    m_parameterValueSliders.clear();
    m_parameterValueComboBoxes.clear();

    m_parameters = parameters;
    rebuildControls();
    rebuildLayout();
}

const std::vector<ParameterControlInfo>& ParameterControlComponent::getParameters() const
{
    return m_parameters;
}

void ParameterControlComponent::setParameterValue(int index, float nativeValue)
{
    // Update stored value
    for (auto& p : m_parameters)
    {
        if (p.index == index)
        {
            p.currentValue = nativeValue;
            break;
        }
    }

    if (m_parameterValueComboBoxes.count(index))
    {
        auto stepIdx = juce::roundToInt(nativeValue);
        m_parameterValueComboBoxes.at(index)->setSelectedId(stepIdx + 1,
                                                            juce::dontSendNotification);
    }
    else if (m_parameterValueSliders.count(index))
    {
        m_parameterValueSliders.at(index)->setValue(nativeValue, juce::dontSendNotification);
    }
    else if (m_parameterValueButtons.count(index))
    {
        m_parameterValueButtons.at(index)->setToggleState(nativeValue > 0.5f,
                                                          juce::dontSendNotification);
    }
}

void ParameterControlComponent::setControlsSize(ControlsSize size)
{
    m_controlsSize = size;
    rebuildLayout();
}

void ParameterControlComponent::rebuildControls()
{
    for (auto& info : m_parameters)
    {
        if (info.type != ParameterControlType::Toggle)
        {
            if (m_parameterNameLabels.count(info.index))
                m_parameterNameLabels.at(info.index)->setText(info.name,
                                                               juce::dontSendNotification);
            else
            {
                m_parameterNameLabels[info.index] = std::make_unique<juce::Label>(info.name);
                m_parameterNameLabels[info.index]->setJustificationType(
                    juce::Justification::centredBottom);
                m_parameterNameLabels[info.index]->setText(info.name,
                                                            juce::dontSendNotification);
                addAndMakeVisible(m_parameterNameLabels[info.index].get());
            }
        }

        if (info.type == ParameterControlType::Discrete)
        {
            if (m_parameterValueComboBoxes.count(info.index))
            {
                auto stepIdx = juce::roundToInt(info.currentValue);
                m_parameterValueComboBoxes.at(info.index)->setSelectedId(
                    stepIdx + 1, juce::dontSendNotification);
            }
            else
            {
                m_parameterValueComboBoxes[info.index] =
                    std::make_unique<juce::ComboBox>(info.name);

                auto& combo = m_parameterValueComboBoxes[info.index];
                if (info.stepCount > 0 && !info.stepNames.empty())
                {
                    jassert(info.stepCount == int(info.stepNames.size()));
                    for (int s = 0; s < info.stepCount; ++s)
                        combo->addItem(info.stepNames[s], s + 1);
                }
                else
                {
                    for (int s = 0; s < juce::jmax(1, info.stepCount); ++s)
                        combo->addItem("Step " + juce::String(s + 1), s + 1);
                }

                auto stepIdx = juce::roundToInt(info.currentValue);
                combo->setSelectedId(stepIdx + 1, juce::dontSendNotification);

                const int capturedIdx = info.index;
                combo->onChange = [this, capturedIdx]() {
                    if (!m_parameterValueComboBoxes.count(capturedIdx)) return;
                    auto stepIndex = m_parameterValueComboBoxes.at(capturedIdx)->getSelectedId() - 1;

                    for (auto& p : m_parameters)
                        if (p.index == capturedIdx) { p.currentValue = float(stepIndex); break; }

                    if (onParameterValueChanged)
                        onParameterValueChanged(capturedIdx, float(stepIndex));
                };

                addAndMakeVisible(combo.get());
            }
        }
        else if (info.type == ParameterControlType::Continuous)
        {
            if (m_parameterValueSliders.count(info.index))
            {
                m_parameterValueSliders.at(info.index)->setValue(info.currentValue,
                                                                  juce::dontSendNotification);
            }
            else
            {
                m_parameterValueSliders[info.index] =
                    std::make_unique<JUCEAppBasics::ToggleStateSlider>(
                        juce::Slider::Rotary, juce::Slider::NoTextBox);
                auto& sl = m_parameterValueSliders[info.index];
                sl->setTogglalbe(false);
                sl->setRange(info.minValue, info.maxValue,
                             info.stepSize > 0.0f ? info.stepSize : 0.0);
                sl->setValue(info.currentValue, juce::dontSendNotification);

                const int capturedIdx = info.index;
                sl->onValueChange = [this, capturedIdx]() {
                    if (!m_parameterValueSliders.count(capturedIdx)) return;
                    auto value = float(m_parameterValueSliders.at(capturedIdx)->getValue());

                    for (auto& p : m_parameters)
                        if (p.index == capturedIdx) { p.currentValue = value; break; }

                    if (onParameterValueChanged)
                        onParameterValueChanged(capturedIdx, value);
                };

                addAndMakeVisible(sl.get());
            }
        }
        else if (info.type == ParameterControlType::Toggle)
        {
            if (m_parameterValueButtons.count(info.index))
            {
                m_parameterValueButtons.at(info.index)->setToggleState(
                    info.currentValue > 0.5f, juce::dontSendNotification);
            }
            else
            {
                m_parameterValueButtons[info.index] =
                    std::make_unique<juce::TextButton>(info.name);
                auto& btn = m_parameterValueButtons[info.index];
                btn->setClickingTogglesState(true);
                btn->setToggleState(info.currentValue > 0.5f, juce::dontSendNotification);

                const int capturedIdx = info.index;
                btn->onStateChange = [this, capturedIdx]() {
                    if (!m_parameterValueButtons.count(capturedIdx)) return;
                    auto value = m_parameterValueButtons.at(capturedIdx)->getToggleState()
                                     ? 1.0f : 0.0f;

                    for (auto& p : m_parameters)
                        if (p.index == capturedIdx) { p.currentValue = value; break; }

                    if (onParameterValueChanged)
                        onParameterValueChanged(capturedIdx, value);
                };

                addAndMakeVisible(btn.get());
            }
        }
    }

    lookAndFeelChanged();
}

void ParameterControlComponent::rebuildLayout()
{
    auto bounds = m_parameterBounds;
    if (bounds.isEmpty())
        return;

    auto itemCount = int(m_parameterValueComboBoxes.size()
                         + m_parameterValueSliders.size()
                         + m_parameterValueButtons.size());
    if (itemCount == 0)
        return;

    const auto gridItemControlSize = 3 * int(m_controlsSize);
    const int  labelHeight         = 30;
    const int  controlMargin       = 5;
    const int  rowPairHeight       = labelHeight + gridItemControlSize;

    const int   availableWidth  = bounds.getWidth();
    const int   availableHeight = bounds.getHeight();
    const float availableAspect =
        float(availableWidth) / float(juce::jmax(1, availableHeight));

    // Choose itemsPerRow whose grid aspect ratio best matches available area
    const int maxItemsPerRow =
        juce::jmin(itemCount, juce::jmax(1, availableWidth / gridItemControlSize));
    int   itemsPerRow    = 1;
    float bestAspectDiff = std::numeric_limits<float>::max();
    for (int n = 1; n <= maxItemsPerRow; ++n)
    {
        int   rows        = static_cast<int>(std::ceil(float(itemCount) / float(n)));
        float layoutAspect =
            float(n * gridItemControlSize) / float(juce::jmax(1, rows * rowPairHeight));
        float diff = std::abs(layoutAspect - availableAspect);
        if (diff < bestAspectDiff) { bestAspectDiff = diff; itemsPerRow = n; }
    }

    const int numRowPairs     = static_cast<int>(std::ceil(float(itemCount) / float(itemsPerRow)));
    const int totalGridWidth  = itemsPerRow * gridItemControlSize;
    const int totalGridHeight = numRowPairs  * rowPairHeight;

    auto gridBounds = juce::Rectangle<int>(
        bounds.getX() + juce::jmax(0, (availableWidth  - totalGridWidth)  / 2),
        bounds.getY() + juce::jmax(0, (availableHeight - totalGridHeight) / 2),
        juce::jmin(totalGridWidth,  availableWidth),
        juce::jmin(totalGridHeight, availableHeight));

    m_parameterControlsGrid->templateRows.clear();
    m_parameterControlsGrid->templateColumns.clear();
    m_parameterControlsGrid->items.clear();

    for (int i = 0; i < numRowPairs; ++i)
    {
        m_parameterControlsGrid->templateRows.add(
            juce::Grid::TrackInfo(juce::Grid::Px(labelHeight)));
        m_parameterControlsGrid->templateRows.add(
            juce::Grid::TrackInfo(juce::Grid::Px(gridItemControlSize)));
    }
    for (int i = 0; i < itemsPerRow; ++i)
        m_parameterControlsGrid->templateColumns.add(
            juce::Grid::TrackInfo(juce::Grid::Px(gridItemControlSize)));

    int currentItem = 0;
    for (auto& info : m_parameters)
    {
        int row = (currentItem / itemsPerRow) * 2;
        int col =  currentItem % itemsPerRow;

        if (info.type == ParameterControlType::Toggle)
        {
            auto it = m_parameterValueButtons.find(info.index);
            if (it == m_parameterValueButtons.end() || !it->second) continue;
            auto buttonSize = float(gridItemControlSize - controlMargin * 2);
            m_parameterControlsGrid->items.add(
                juce::GridItem(*it->second)
                    .withArea(row + 2, col + 1)
                    .withMargin(juce::GridItem::Margin(float(controlMargin)))
                    .withWidth(buttonSize).withHeight(buttonSize)
                    .withJustifySelf(juce::GridItem::JustifySelf::center)
                    .withAlignSelf(juce::GridItem::AlignSelf::center));
        }
        else if (info.type == ParameterControlType::Discrete)
        {
            auto comboIt = m_parameterValueComboBoxes.find(info.index);
            auto labelIt = m_parameterNameLabels.find(info.index);
            if (comboIt == m_parameterValueComboBoxes.end() || !comboIt->second) continue;
            if (labelIt == m_parameterNameLabels.end() || !labelIt->second) continue;
            m_parameterControlsGrid->items.add(
                juce::GridItem(*labelIt->second)
                    .withArea(row + 1, col + 1)
                    .withMargin(juce::GridItem::Margin(2.0f)));
            m_parameterControlsGrid->items.add(
                juce::GridItem(*comboIt->second)
                    .withArea(row + 2, col + 1)
                    .withMargin(juce::GridItem::Margin(float(controlMargin)))
                    .withHeight(30)
                    .withJustifySelf(juce::GridItem::JustifySelf::center)
                    .withAlignSelf(juce::GridItem::AlignSelf::center));
        }
        else // Continuous
        {
            auto sliderIt = m_parameterValueSliders.find(info.index);
            auto labelIt  = m_parameterNameLabels.find(info.index);
            if (sliderIt == m_parameterValueSliders.end() || !sliderIt->second) continue;
            if (labelIt  == m_parameterNameLabels.end()  || !labelIt->second)  continue;
            auto sliderSize = float(gridItemControlSize - controlMargin * 2);
            m_parameterControlsGrid->items.add(
                juce::GridItem(*labelIt->second)
                    .withArea(row + 1, col + 1)
                    .withMargin(juce::GridItem::Margin(2.0f)));
            m_parameterControlsGrid->items.add(
                juce::GridItem(*sliderIt->second)
                    .withArea(row + 2, col + 1)
                    .withMargin(juce::GridItem::Margin(float(controlMargin)))
                    .withWidth(sliderSize).withHeight(sliderSize)
                    .withJustifySelf(juce::GridItem::JustifySelf::center)
                    .withAlignSelf(juce::GridItem::AlignSelf::center));
        }

        currentItem++;
    }

    m_parameterControlsGrid->performLayout(gridBounds);
}


} // namespace JUCEAppBasics
