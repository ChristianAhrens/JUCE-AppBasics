/* Copyright (c) 2025, Christian Ahrens
 *
 * This file is part of JUCEAppBasics <https://github.com/ChristianAhrens/JUCE-AppBasics>
 *
 * This module is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This module is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this tool; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "ServiceTopologyTreeView.h"

namespace JUCEAppBasics
{


class DualLabelContainerComponent : public juce::Component
{
public:
    //==============================================================================
    DualLabelContainerComponent(juce::String labelString1, juce::String labelString2, int labe2Width = s_refWidth) :
        m_label1("Label1", labelString1),
        m_label2("Label2", labelString2)
    {
        m_labe2Width = labe2Width;

        m_label1.setJustificationType(juce::Justification::centredLeft);
        addAndMakeVisible(&m_label1);
        
        m_label2.setJustificationType(juce::Justification::centredLeft);
        addAndMakeVisible(&m_label2);
    };
    ~DualLabelContainerComponent() override {};

    void resized() override
    {
        auto bounds = getLocalBounds();
        
        if (bounds.getWidth() < int(1.5f * s_refWidth))
        {
            m_label1.setBounds(bounds);
            m_label2.setVisible(false);
        }
        else
        {
            m_label1.setBounds(bounds.removeFromLeft(bounds.getWidth() - m_labe2Width));
            m_label2.setVisible(true);
            m_label2.setBounds(bounds);
        }
    };
    
    const juce::Font getFont()
    {
        return m_label1.getFont();
    };
    void setFont (const Font& newFont)
    {
        m_label1.setFont(newFont);
        m_label2.setFont(newFont);
    };
    
    void setJustificationType(Justification justification)
    {
        m_label1.setJustificationType(justification);
        m_label2.setJustificationType(justification);
    };
    
    void setLabelTextColour(const juce::Colour& colour)
    {
        m_label1.setColour(juce::Label::ColourIds::textColourId, colour);
        m_label2.setColour(juce::Label::ColourIds::textColourId, colour);
    };
    
    static constexpr int s_refWidth = 200;

private:
    juce::Label m_label1;
    juce::Label m_label2;

    int m_labe2Width = 0;
};

//==============================================================================
ServiceTreeViewItem::ServiceTreeViewItem(bool canBeSelected) : juce::TreeViewItem()
{
    m_canBeSelected = canBeSelected;
}

ServiceTreeViewItem::~ServiceTreeViewItem()
{
}

void ServiceTreeViewItem::setServiceInfo(const SessionMasterAwareService& service)
{
    m_service = service;
}

const SessionMasterAwareService& ServiceTreeViewItem::getServiceInfo()
{
    return m_service;
}

bool ServiceTreeViewItem::mightContainSubItems()
{
    return false;
}

std::unique_ptr<Component> ServiceTreeViewItem::createItemComponent(bool bold)
{
    auto label1Text = getServiceInfo().description.upToFirstOccurrenceOf("@",false, true);
    if (getServiceInfo().description == JUCEAppBasics::ServiceTopologyManager::getServiceDescription())
        label1Text += " (this)";
    auto label2Text = getServiceInfo().description.fromFirstOccurrenceOf("@", true, true);
    
    auto item = std::make_unique<DualLabelContainerComponent>(label1Text, label2Text);
    auto font = item->getFont();
    font.setBold(bold);
    item->setLabelTextColour(m_labelTextColour);
    item->setFont(font);
    item->setJustificationType(juce::Justification::centredLeft);
    return std::move(item);
}

std::unique_ptr<Component> ServiceTreeViewItem::createItemComponent()
{
    return createItemComponent(false);
}

int ServiceTreeViewItem::getItemHeight() const
{
    return ServiceTreeViewItem::getHeight();
}

bool ServiceTreeViewItem::canBeSelected() const
{
    return m_canBeSelected;
}

bool ServiceTreeViewItem::customComponentUsesTreeViewMouseHandler() const
{
    return true;
}

void ServiceTreeViewItem::setLabelTextColour(const juce::Colour& labelTextColour)
{
    m_labelTextColour = labelTextColour;
}

int ServiceTreeViewItem::getHeight()
{
    return 25;
}

//==============================================================================
MasterServiceTreeViewItem::MasterServiceTreeViewItem(bool canBeSelected) : ServiceTreeViewItem(canBeSelected)
{
}

MasterServiceTreeViewItem::~MasterServiceTreeViewItem()
{
}

bool MasterServiceTreeViewItem::mightContainSubItems()
{
    return true;
}

std::unique_ptr<Component> MasterServiceTreeViewItem::createItemComponent()
{
    return ServiceTreeViewItem::createItemComponent(true);
}

int MasterServiceTreeViewItem::getItemHeight() const
{
    return MasterServiceTreeViewItem::getHeight();
}

int MasterServiceTreeViewItem::getHeight()
{
    return 27;
};


//==============================================================================
ServiceTopologyTreeView::ServiceTopologyTreeView(bool allowSelection)
    : juce::TreeView()
{
    m_allowSelection = allowSelection;
    m_rootItem = std::make_unique<MasterServiceTreeViewItem>();
    setRootItem(m_rootItem.get());
    setRootItemVisible(false);
    setMultiSelectEnabled(false);
}

ServiceTopologyTreeView::~ServiceTopologyTreeView()
{
    setRootItem(nullptr);
}

void ServiceTopologyTreeView::setServiceTopology(const JUCEAppBasics::SessionServiceTopology& topology)
{
    if (m_serviceTopology == topology)
        return;

    m_serviceTopology = topology;

    if (!m_rootItem)
    {
        jassertfalse;
        return;
    }

    m_rootItem->clearSubItems();
    for (auto const& masterService : m_serviceTopology)
    {
        auto masterServiceItem = std::make_unique<MasterServiceTreeViewItem>(m_allowSelection && masterService.first.description.contains("@"));
        masterServiceItem->setServiceInfo(masterService.first);
        masterServiceItem->setLabelTextColour(masterServiceItem->canBeSelected() ? getLookAndFeel().findColour(juce::TextButton::ColourIds::textColourOnId) : getLookAndFeel().findColour(juce::ToggleButton::ColourIds::tickDisabledColourId));
        for (auto const& service : masterService.second)
        {
            auto serviceItem = std::make_unique<ServiceTreeViewItem>(false);
            serviceItem->setServiceInfo(service);
            serviceItem->setLabelTextColour(getLookAndFeel().findColour(juce::ToggleButton::ColourIds::tickDisabledColourId));
            masterServiceItem->addSubItem(serviceItem.release());
        }

        m_rootItem->addSubItem(masterServiceItem.release());
    }
}

std::pair<int, int> ServiceTopologyTreeView::getIdealSize()
{
    auto height = 0;
    for (auto const& masterService : m_serviceTopology)
    {
        height += MasterServiceTreeViewItem::getHeight();
        for (auto const& service : masterService.second)
        {
            ignoreUnused(service);
            height += ServiceTreeViewItem::getHeight();
        }
    }

    return { int(1.8f * DualLabelContainerComponent::s_refWidth), height };
}


} // namespace JUCEAppBasics
