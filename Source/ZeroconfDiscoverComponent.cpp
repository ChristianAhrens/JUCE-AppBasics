/*
  ==============================================================================

    ZeroconfDiscoverComponent.cpp
    Created: 20 Sept 2020 20:20:00pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "ZeroconfDiscoverComponent.h"

#include "Image_utils.h"

#ifdef JUCE_WINDOWS
#include <Winsock2.h>
#include <Ws2tcpip.h>
#else
#include <netdb.h>
#endif /* _WIN32 */


namespace JUCEAppBasics
{

//==============================================================================
ZeroconfDiscoverComponent::ZeroconfDiscoverComponent(bool useSeparateServiceSearchers, bool showServiceNameLabels)
{
	m_useSeparateServiceSearchers = useSeparateServiceSearchers;

	setShowServiceNameLabels(showServiceNameLabels);
}

ZeroconfDiscoverComponent::~ZeroconfDiscoverComponent()
{
    for(auto const& serviceDiscoverButtonKV : m_discoveryButtons)
        removeSearcher(serviceDiscoverButtonKV.first);
}

void ZeroconfDiscoverComponent::setShowServiceNameLabels(bool show)
{
    m_showServiceNameLabels = show;
    
    for (auto const& labelKV : m_serviceNameLabels)
        labelKV.second->setVisible(show);
};

void ZeroconfDiscoverComponent::clearServices()
{
	for (int i = ZST_Unkown + 1; i < ZST_Max; ++i)
		removeDiscoverService(static_cast<ZeroconfServiceType>(i));
}

void ZeroconfDiscoverComponent::addDiscoverService(ZeroconfServiceType serviceType)
{
    auto serviceName = getServiceName(serviceType);
    auto serviceDescriptor = getServiceDescriptor(serviceType);
    
	if(m_useSeparateServiceSearchers || m_discoveryButtons.empty())
	{
		m_discoveryButtons.insert(std::make_pair(serviceName, std::make_unique<DrawableButton>(serviceName, DrawableButton::ButtonStyle::ImageOnButtonBackground)));
		m_discoveryButtons.at(serviceName)->addListener(this);
		m_discoveryButtons.at(serviceName)->setButtonText(serviceName);
		addAndMakeVisible(m_discoveryButtons.at(serviceName).get());
    
		m_serviceNameLabels.insert(std::make_pair(serviceName, std::make_unique<Label>(serviceName, serviceName + ":")));
		addAndMakeVisible(m_serviceNameLabels.at(serviceName).get());

		lookAndFeelChanged();
	}

    addSearcher(serviceName, serviceDescriptor);
}

void ZeroconfDiscoverComponent::removeDiscoverService(ZeroconfServiceType serviceType)
{
	auto serviceName = getServiceName(serviceType);

	if (m_discoveryButtons.count(serviceName) > 0)
	{
		removeChildComponent(m_discoveryButtons.at(serviceName).get());
		m_discoveryButtons.erase(serviceName);
	}

	if (m_serviceNameLabels.count(serviceName) > 0)
	{
		removeChildComponent(m_serviceNameLabels.at(serviceName).get());
		m_serviceNameLabels.erase(serviceName);
	}

	removeSearcher(serviceName);
}

void ZeroconfDiscoverComponent::addSearcher(String name, String serviceName)
{
	ZeroconfSearcher::ZeroconfSearcher * searcher = getSearcherByName(name);

	if (searcher == nullptr)
	{
		m_searchers.push_back(std::make_unique<ZeroconfSearcher::ZeroconfSearcher>(name.toStdString(), serviceName.toStdString()));
	}
}

void ZeroconfDiscoverComponent::removeSearcher(String name)
{
	ZeroconfSearcher::ZeroconfSearcher * s = getSearcherByName(name);
	auto const zsit = std::find_if(m_searchers.begin(), m_searchers.end(), [&](const auto& val) { return val.get() == s; });
	if (zsit != m_searchers.end())
		m_searchers.erase(zsit);
}

ZeroconfSearcher::ZeroconfSearcher * ZeroconfDiscoverComponent::getSearcherByName(const juce::String& name)
{
	for (auto const& searcher : m_searchers)
	{
		if (searcher)
			if (name.contains(searcher->GetName()))
				return searcher.get();
	}
	
	return nullptr;
}

ZeroconfSearcher::ZeroconfSearcher* ZeroconfDiscoverComponent::getSearcherByServiceName(const juce::String& serviceName)
{
	for (auto const& searcher : m_searchers)
	{
		if (searcher)
			if (serviceName.contains(searcher->GetServiceName()))
				return searcher.get();
	}

	return nullptr;
}

void ZeroconfDiscoverComponent::showMenuAndGetService(const juce::String& serviceName)
{
	m_currentServiceBrowsingPopup.clear();

	if (serviceName.isEmpty())
	{
		if (m_searchers.empty())
		{
			DBG("No searcher found");
			return;
		}

		for (auto& searcher : m_searchers)
		{
			if (searcher)
			{
				for (auto& service : searcher->GetServices())
				{
					if (service)
					{
						m_currentServiceBrowsingList.push_back(service);
						String serviceItemString = service->name + " on " + (service->host.empty() ? service->ip : service->host);
#ifdef DEBUG
						serviceItemString += " (" + String(service->ip) + ":" + String(service->port) + ")";
#endif
						m_currentServiceBrowsingPopup.addItem(static_cast<int>(m_currentServiceBrowsingList.size()), serviceItemString);
					}
				}
			}
		}
	}
	else
	{
		auto searcher = getSearcherByServiceName(serviceName);

		if (searcher == nullptr)
		{
			DBG("No searcher found for service " << serviceName);
			return;
		}

		for (auto& service : searcher->GetServices())
		{
			m_currentServiceBrowsingList.push_back(service);

			String serviceItemString;
			//serviceItemString = String(service->name) + " on " + (service->host.empty() ? service->ip : service->host) + " (" + String(service->ip) + ":" + String(service->port) + ")";
			auto name = String(service->name).upToFirstOccurrenceOf(String("." + searcher->GetServiceName()), false, true);
			serviceItemString = name + " (" + String(service->ip) + ")";

			m_currentServiceBrowsingPopup.addItem(static_cast<int>(m_currentServiceBrowsingList.size()), serviceItemString);
		}
	}

	if (m_currentServiceBrowsingList.empty())
	{
		m_currentServiceBrowsingPopup.addItem(-1, "No service found", false);
	}

	m_currentServiceBrowsingPopup.showMenuAsync(juce::PopupMenu::Options(), [this](int result)
		{
			if (result > 0)
			{
				if (onServiceSelected)
				{
					auto serviceIndex = result - 1;
					auto service = (m_currentServiceBrowsingList.size() >= serviceIndex) ? m_currentServiceBrowsingList.at(serviceIndex) : static_cast<ZeroconfSearcher::ZeroconfSearcher::ServiceInfo*>(nullptr);

					if (service != nullptr)
					{
						auto searcher = getSearcherByServiceName(service->name);

						if (searcher != nullptr)
							onServiceSelected(getServiceType(searcher->GetName()), service);
						else
							onServiceSelected(ZST_Unkown, nullptr);
					}
					else
						onServiceSelected(ZST_Unkown, nullptr);
				}
			}

			m_currentServiceBrowsingList.clear();
		});
}

void ZeroconfDiscoverComponent::resized()
{
	if (m_useSeparateServiceSearchers)
	{
		auto buttonBounds = getLocalBounds().reduced(2);

		FlexBox fb;
		fb.flexDirection = FlexBox::Direction::row;
		for (auto const& buttonKV : m_discoveryButtons)
		{
			if (m_showServiceNameLabels && m_serviceNameLabels.count(buttonKV.first) != 0)
			{
				fb.items.add(FlexItem().withFlex(1));
				fb.items.add(FlexItem(*m_serviceNameLabels.at(buttonKV.first).get()).withFlex(2).withMargin(FlexItem::Margin(2, 2, 2, 2)));
			}
			fb.items.add(FlexItem(*buttonKV.second.get()).withFlex(2).withMargin(FlexItem::Margin(2, 2, 2, 2)));
		}
		fb.performLayout(buttonBounds);
	}
	else
	{
		jassert(m_discoveryButtons.size() == 1);
		if (m_discoveryButtons.size() == 1)
			m_discoveryButtons.begin()->second->setBounds(getLocalBounds());
	}
}

/**
 * Reimplemnted from Component to correctly adjust button drawable colouring
 */
void ZeroconfDiscoverComponent::lookAndFeelChanged()
{
	Component::lookAndFeelChanged();

	auto colourOn = getLookAndFeel().findColour(TextButton::ColourIds::textColourOnId);
	auto colourOff = getLookAndFeel().findColour(TextButton::ColourIds::textColourOffId);

	std::unique_ptr<juce::Drawable> NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage;
	JUCEAppBasics::Image_utils::getDrawableButtonImages(BinaryData::find_replace24px_svg, NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage,
		colourOn, colourOff, colourOff, colourOff, colourOn, colourOn, colourOn, colourOn);

	for (auto const& button : m_discoveryButtons)
		button.second->setImages(NormalImage.get(), OverImage.get(), DownImage.get(), DisabledImage.get(), NormalOnImage.get(), OverOnImage.get(), DownOnImage.get(), DisabledOnImage.get());
}

void ZeroconfDiscoverComponent::buttonClicked(Button* button)
{
    for (auto const& buttonKV : m_discoveryButtons)
    {
		if (button == buttonKV.second.get())
		{
			auto searcherName = buttonKV.first;
			auto searcher = getSearcherByName(searcherName);
			if(searcher)
				showMenuAndGetService(searcher->GetServiceName());
			return;
		}
    }
}

void ZeroconfDiscoverComponent::handleServicesChanged()
{
	std::cout << __FUNCTION__;
}

ZeroconfDiscoverComponent::ZeroconfServiceType ZeroconfDiscoverComponent::getServiceType(String name)
{
    if (name.contains(getServiceName(ZST_OSC)))
        return ZST_OSC;
    else if (name.contains(getServiceName(ZST_OCA)))
        return ZST_OCA;
    else
        return ZST_Unkown;
}

String ZeroconfDiscoverComponent::getServiceName(ZeroconfServiceType type)
{
    switch (type)
    {
        case ZST_OCA:
            return "OCA";
        case ZST_OSC:
            return "OSC";
        default:
            return String();
    };
}

String ZeroconfDiscoverComponent::getServiceDescriptor(ZeroconfServiceType type)
{
    switch (type)
    {
        case ZST_OCA:
            return "_oca._tcp";
        case ZST_OSC:
            return "_osc._udp";
        default:
            return String();
    };
}

}
