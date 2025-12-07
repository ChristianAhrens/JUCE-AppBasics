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
#pragma once

#include <JuceHeader.h>

#include <ServiceTopologyManager.h>

namespace JUCEAppBasics
{

class ServiceTreeViewItem : public juce::TreeViewItem
{
public:
    //==============================================================================
    ServiceTreeViewItem(bool canBeSelected = false);
    ~ServiceTreeViewItem() override;

    void setServiceInfo(const SessionMasterAwareService& service);
    const SessionMasterAwareService& getServiceInfo();

    virtual bool mightContainSubItems() override;
    virtual std::unique_ptr<Component> createItemComponent() override;
    int getItemHeight() const override;
    bool canBeSelected() const override;
    bool customComponentUsesTreeViewMouseHandler() const override;
    juce::String getTooltip() override;
    
    void setLabelTextColour(const juce::Colour& labelTextColour);

    static int getHeight();

protected:
    //==============================================================================
    std::unique_ptr<Component> createItemComponent(bool bold);

private:
    //==============================================================================
    SessionMasterAwareService  m_service;
    bool m_canBeSelected = false;
    juce::Colour m_labelTextColour;
};

class MasterServiceTreeViewItem : public ServiceTreeViewItem
{
public:
    //==============================================================================
    MasterServiceTreeViewItem(bool canBeSelected = false);
    ~MasterServiceTreeViewItem() override;

    bool mightContainSubItems() override;

    virtual std::unique_ptr<Component> createItemComponent() override;
    int getItemHeight() const override;

    static int getHeight();

};

/**
 * ServiceTopologyTreeView is a class derived from juce::TreeVew
 * to show the topology detected by ServiceTopologyManager and
 * optionally to interact with it.
 */
class ServiceTopologyTreeView : public juce::TreeView
{
public:
    //==============================================================================
    ServiceTopologyTreeView(bool allowSelection);
    ~ServiceTopologyTreeView() override;

    void setServiceTopology(const SessionServiceTopology& topology);

    void recreateTree();

    std::pair<int, int> getIdealSize();

    void lookAndFeelChanged() override;

private:
    //==============================================================================
    SessionServiceTopology              m_serviceTopology;
    bool                                m_allowSelection;
    std::unique_ptr<juce::TreeViewItem> m_rootItem;

};

} // namespace JUCEAppBasics
