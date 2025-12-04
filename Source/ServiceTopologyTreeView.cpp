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
    DualLabelContainerComponent(juce::String labelString1, juce::String labelString2, int labe2Width = 200) :
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
        m_label1.setBounds(bounds.removeFromLeft(bounds.getWidth() - m_labe2Width));
        m_label2.setBounds(bounds);
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

std::unique_ptr<Component> ServiceTreeViewItem::createItemComponent()
{
    return std::make_unique<DualLabelContainerComponent>(
        getServiceInfo().description.upToFirstOccurrenceOf("@",false, true),
        getServiceInfo().description.fromFirstOccurrenceOf("@", true, true));
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
    auto item = std::make_unique<DualLabelContainerComponent>(
        getServiceInfo().description.upToFirstOccurrenceOf("@",false, true),
        getServiceInfo().description.fromFirstOccurrenceOf("@", true, true));
    auto font = item->getFont();
    font.setBold(true);
    item->setFont(font);
    item->setJustificationType(juce::Justification::centredLeft);
    return std::move(item);
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
        for (auto const& service : masterService.second)
        {
            auto serviceItem = std::make_unique<ServiceTreeViewItem>(false);
            serviceItem->setServiceInfo(service);
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

    return { 300, height };
}


} // namespace JUCEAppBasics
