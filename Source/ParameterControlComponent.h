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

#pragma once

#include <JuceHeader.h>


namespace JUCEAppBasics
{

class ToggleStateSlider;

/** Control widget type for a single parameter entry in ParameterControlComponent. */
enum class ParameterControlType
{
    Toggle,      ///< Two-state toggle button.
    Discrete,    ///< Combo box for a finite set of named steps.
    Continuous   ///< Rotary slider for a continuous value range.
};

/**
 * @brief Describes a single parameter rendered inside ParameterControlComponent.
 *
 * All values are expressed in the parameter's native range (not normalised 0–1).
 * For Toggle:   currentValue is 0.0 (off) or 1.0 (on).
 * For Discrete: currentValue is the zero-based step index as a float.
 * For Continuous: currentValue is in [minValue, maxValue].
 */
struct ParameterControlInfo
{
    int           index        = 0;
    juce::String  name         = "";
    ParameterControlType type  = ParameterControlType::Continuous;
    float         minValue     = 0.0f;
    float         maxValue     = 1.0f;
    float         stepSize     = 0.0f;  ///< Non-zero enables slider snap; 0 = free.
    int           stepCount    = 0;     ///< Number of discrete steps (Discrete only).
    std::vector<std::string> stepNames; ///< Display name for each discrete step.
    float         currentValue = 0.0f;

    bool operator==(const ParameterControlInfo& o) const noexcept
    {
        return index == o.index && name == o.name && type == o.type
            && minValue == o.minValue && maxValue == o.maxValue
            && stepSize == o.stepSize && stepCount == o.stepCount
            && stepNames == o.stepNames;
    }
    bool operator!=(const ParameterControlInfo& o) const noexcept { return !(*this == o); }
};


/**
 * @class ParameterControlComponent
 * @brief Renders a dynamically laid-out grid of parameter controls built from a
 *        list of ParameterControlInfo descriptors.
 *
 * Continuous parameters → rotary ToggleStateSlider  (value in native range).
 * Discrete parameters   → juce::ComboBox            (value = step index 0-based).
 * Toggle parameters     → juce::TextButton           (value = 0.0 or 1.0).
 *
 * The grid aspect ratio adapts to the component's current bounds so that the
 * layout fills the available area as efficiently as possible.
 *
 * ## Usage
 * ```cpp
 * m_paramCtrl = std::make_unique<ParameterControlComponent>();
 * m_paramCtrl->setParameters(infos);
 * m_paramCtrl->onParameterValueChanged = [](int idx, float v) { ... };
 * addAndMakeVisible(m_paramCtrl.get());
 * ```
 */
class ParameterControlComponent : public juce::Component
{
public:
    /** Pixel-based size that governs all control widget dimensions. */
    enum class ControlsSize { S = 35, M = 50, L = 65 };

    ParameterControlComponent();
    ~ParameterControlComponent() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
    void lookAndFeelChanged() override;

    //==============================================================================
    void setParameters(const std::vector<ParameterControlInfo>& parameters);
    const std::vector<ParameterControlInfo>& getParameters() const;

    /** Updates one control's displayed value without firing onParameterValueChanged. */
    void setParameterValue(int index, float nativeValue);

    //==============================================================================
    void setControlsSize(ControlsSize size);
    ControlsSize getControlsSize() const { return m_controlsSize; }

    //==============================================================================
    /** Fired on the message thread when the user moves a control.
     *  @param index       ParameterControlInfo::index of the changed parameter.
     *  @param nativeValue New value in native range (see ParameterControlInfo docs). */
    std::function<void(int index, float nativeValue)> onParameterValueChanged;

protected:
    void rebuildControls();
    void rebuildLayout();

private:
    std::unique_ptr<juce::Grid> m_parameterControlsGrid;

    std::map<int, std::unique_ptr<juce::Label>>                       m_parameterNameLabels;
    std::map<int, std::unique_ptr<juce::TextButton>>                  m_parameterValueButtons;
    std::map<int, std::unique_ptr<JUCEAppBasics::ToggleStateSlider>>  m_parameterValueSliders;
    std::map<int, std::unique_ptr<juce::ComboBox>>                    m_parameterValueComboBoxes;

    std::vector<ParameterControlInfo> m_parameters;
    ControlsSize                      m_controlsSize = ControlsSize::M;
    juce::Rectangle<int>              m_parameterBounds;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterControlComponent)
};

} // namespace JUCEAppBasics
