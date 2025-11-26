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


//==============================================================================
class ServiceTreeViewItem : public juce::TreeViewItem
{
public:
    //==============================================================================
    ServiceTreeViewItem() : juce::TreeViewItem()
    {
    };
    ~ServiceTreeViewItem() override
    {
    };

    void setServiceInfo(const SessionMasterAwareService& service)
    {
        m_service = service;
    };
    const SessionMasterAwareService& getServiceInfo()
    {
        return m_service;
    };

    virtual bool mightContainSubItems() override
    {
        return false;
    };

    virtual std::unique_ptr<Component> createItemComponent() override
    {
        auto label = std::make_unique<juce::Label>("ServiceTreeViewItem", getServiceInfo().description);
        label->setJustificationType(juce::Justification::centredLeft);
        return std::move(label);
    };
    int getItemHeight() const override
    {
        return ServiceTreeViewItem::getHeight();
    };

    static int getHeight();

protected:
    //==============================================================================

private:
    //==============================================================================
    SessionMasterAwareService  m_service;
};
int ServiceTreeViewItem::getHeight() { return 25; };

class MasterServiceTreeViewItem : public ServiceTreeViewItem
{
public:
    //==============================================================================
    MasterServiceTreeViewItem() : ServiceTreeViewItem()
    {
    };
    ~MasterServiceTreeViewItem() override
    {
    };

    bool mightContainSubItems() override
    {
        return true;
    };

    virtual std::unique_ptr<Component> createItemComponent() override
    {
        auto label = std::make_unique<juce::Label>("MasterServiceTreeViewItem", getServiceInfo().description);
        auto font = label->getFont();
        font.setBold(true);
        label->setFont(font);
        label->setJustificationType(juce::Justification::centredLeft);
        return std::move(label);
    };
    int getItemHeight() const override
    {
        return MasterServiceTreeViewItem::getHeight();
    };

    static int getHeight();

protected:
    //==============================================================================

private:
    //==============================================================================
};
int MasterServiceTreeViewItem::getHeight() { return 27; };


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
    m_serviceTopology = topology;

    if (!m_rootItem)
    {
        jassertfalse;
        return;
    }

    m_rootItem->clearSubItems();
    for (auto const& masterService : m_serviceTopology)
    {
        auto masterServiceItem = std::make_unique<MasterServiceTreeViewItem>();
        masterServiceItem->setServiceInfo(masterService.first);
        for (auto const& service : masterService.second)
        {
            auto serviceItem = std::make_unique<ServiceTreeViewItem>();
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
