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
ZeroconfDiscoverComponent::ZeroconfDiscoverComponent(const juce::String& name)
	: juce::DrawableButton(name, ButtonStyle::ImageOnButtonBackground)
{
	setPopupActive(false);

	// Being a global mouse listner and at the same time deriving 
	// from a button breaks the usual button onclick std::function 
	// and listener callback pattern useless. Therefor we have to detect 
	// our button click by hand in ::mouseDown processing!
	juce::Desktop::getInstance().addGlobalMouseListener(this);

	// Initially prep. all lookandfeel dependant - esp. button icon in this case.
	lookAndFeelChanged();
}

ZeroconfDiscoverComponent::~ZeroconfDiscoverComponent()
{
	juce::Desktop::getInstance().removeGlobalMouseListener(this);
}

/**
 * Reimplemented method to create the custom
 * image position information using the internal
 * m_imageSize information that was set on object construction.
 * @return	The custom image position based on the internal m_imageSize setting.
 */
juce::Rectangle<float> ZeroconfDiscoverComponent::getImageBounds() const
{
	jassert(getStyle() == ButtonStyle::ImageOnButtonBackground); // this method implementation assumes the button style is set to ImageOnButtonBackground

	auto bounds = getLocalBounds();
	return bounds.reduced(3, 3).toFloat();
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

    addSearcher(serviceName, serviceDescriptor);
}

void ZeroconfDiscoverComponent::removeDiscoverService(ZeroconfServiceType serviceType)
{
	auto serviceName = getServiceName(serviceType);

	removeSearcher(serviceName);
}

bool ZeroconfDiscoverComponent::addPopupCategory(const juce::String& categoryName, const std::pair<std::pair<ZeroconfServiceCategoryType, juce::String>, std::pair<ZeroconfServiceCategoryMatch, juce::String>>& categoryProcessingData)
{
	if (categoryProcessingData.first.first <= ZSCT_Unknown || categoryProcessingData.first.first >= ZSCT_Max)
		return false;
	if (categoryProcessingData.second.first <= ZSCM_Unknown || categoryProcessingData.second.first >= ZSCM_Max)
		return false;

	m_currentPopupCategories[categoryName] = categoryProcessingData;

	return true;
}

void ZeroconfDiscoverComponent::removePopupCategory(const juce::String& categoryName)
{
	m_currentPopupCategories.erase(categoryName);
}

void ZeroconfDiscoverComponent::addSearcher(const juce::String& name, const juce::String& serviceName)
{
	ZeroconfSearcher::ZeroconfSearcher * searcher = getSearcherByName(name);

	if (searcher == nullptr)
	{
		m_searchers.push_back(std::make_unique<ZeroconfSearcher::ZeroconfSearcher>(name.toStdString(), serviceName.toStdString()));
		m_searchers.back()->AddListener(this);
	}
}

void ZeroconfDiscoverComponent::removeSearcher(const juce::String& name)
{
	ZeroconfSearcher::ZeroconfSearcher* s = getSearcherByName(name);
	auto const zsit = std::find_if(m_searchers.begin(), m_searchers.end(), [&](const auto& val) { return val.get() == s; });
	if (zsit != m_searchers.end())
		m_searchers.erase(zsit);
}

ZeroconfSearcher::ZeroconfSearcher* ZeroconfDiscoverComponent::getSearcherByName(const juce::String& name)
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
					if (std::find_if(m_currentServiceBrowsingList.begin(), m_currentServiceBrowsingList.end(), [=](const auto& val) { return std::get<1>(val) == service; }) == m_currentServiceBrowsingList.end())
						m_currentServiceBrowsingList.push_back(std::make_tuple(searcher->GetServiceName(), service));
					else
						continue;
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
			if (std::find_if(m_currentServiceBrowsingList.begin(), m_currentServiceBrowsingList.end(), [&](const auto& val) { return std::get<1>(val) == service; }) == m_currentServiceBrowsingList.end())
				m_currentServiceBrowsingList.push_back(std::make_tuple(searcher->GetServiceName(), service));
			else
				continue;
		}
	}

	if (m_currentServiceBrowsingList.empty())
	{
		m_currentServiceBrowsingPopup.addCustomItem(-1, std::make_unique<CustomLabelItemComponent>("CustomItem-1", "No service found"), nullptr, "No service found");
	}
	else
	{
		auto currentCategorizedServicesMap = std::map<juce::String, std::map<int, juce::String>>();

		auto itemNo = 0;
		for (auto const& serviceEntry : m_currentServiceBrowsingList)
		{
			auto& serviceEntryName = std::get<0>(serviceEntry);
			auto& service = std::get<1>(serviceEntry);

			auto name = String(service.name).upToFirstOccurrenceOf(String("." + serviceEntryName), false, true);
			auto serviceItemString = juce::String(name + " (" + String(service.ip) + ")");

			itemNo++;

			auto categoryMatch = false;
			for (auto const& popupCategory : m_currentPopupCategories)
			{
				auto& categoryType = popupCategory.second.first.first;
				auto& categoryIdentStr = popupCategory.second.first.second;
				auto& matchType = popupCategory.second.second.first;
				auto& matchVal = popupCategory.second.second.second;
				if (categoryType == ZSCT_MetaInfo)
				{
					juce::String refVal;
					if (categoryIdentStr == "NAME")
						refVal = service.name;
					else if (categoryIdentStr == "HOST")
						refVal = service.host;
					else if (categoryIdentStr == "IP")
						refVal = service.ip;
					else if (categoryIdentStr == "PORT")
						refVal = juce::String(service.port);

					if (refVal.isNotEmpty())
					{
						categoryMatch = true;
						if (matchType == ZSCM_Match && refVal == matchVal)
							currentCategorizedServicesMap[popupCategory.first][itemNo] = serviceItemString;
						else if (matchType == ZSCM_Contain && juce::String(refVal).contains(matchVal))
							currentCategorizedServicesMap[popupCategory.first][itemNo] = serviceItemString;
						else
							categoryMatch = false;
					}
				}
				else if (categoryType == ZSCT_TxtRecord)
				{
					auto recordToMatch = service.txtRecords.find(categoryIdentStr.toStdString());
					if (recordToMatch != service.txtRecords.end())
					{
						categoryMatch = true;
						if (matchType == ZSCM_Match && recordToMatch->second == matchVal.toStdString())
							currentCategorizedServicesMap[popupCategory.first][itemNo] = serviceItemString;
						else if (matchType == ZSCM_Contain && juce::String(recordToMatch->second).contains(matchVal))
							currentCategorizedServicesMap[popupCategory.first][itemNo] = serviceItemString;
						else
							categoryMatch = false;
					}
				}
			}
			if (!categoryMatch)
				currentCategorizedServicesMap["Other " + serviceEntryName + " services"][itemNo] = serviceItemString;
		}

		for (auto const& serviceCategory : currentCategorizedServicesMap)
		{
			if (currentCategorizedServicesMap.size() > 1)
			{
				m_currentServiceBrowsingPopup.addSeparator();
				m_currentServiceBrowsingPopup.addSectionHeader(serviceCategory.first);
			}
			for (auto const& serviceEntry : serviceCategory.second)
			{
				m_currentServiceBrowsingPopup.addCustomItem(serviceEntry.first, std::make_unique<CustomLabelItemComponent>("CustomItem" + juce::String(serviceEntry.first), serviceEntry.second), nullptr, serviceEntry.second);
			}
		}
	}

	m_ignoreBlankPopupResultCount = 1; /** An initial empty popup result (0) is expected 
										* before actual searcher entries are being added 
										* to the menu that shall not be processed and 
										* cause the searching to be aborted.
										* Therefor we set an initial ignore count 1 to be
										* used and decremented to enable processing all further results. */
	auto margin = 3;
	auto popupPosOffsetX = getWidth() + margin;
	auto popupPosOffsetY = getHeight() / 2;
	m_currentServiceBrowsingPopup.showMenuAsync(juce::PopupMenu::Options()
		.withTargetScreenArea(getScreenBounds().translated(popupPosOffsetX, popupPosOffsetY))
		.withPreferredPopupDirection(juce::PopupMenu::Options::PopupDirection::upwards), 
		[this](int result) {
			if (m_ignoreBlankPopupResultCount == 0 || result > 0)
				handlePopupResult(result);
			else
                m_ignoreBlankPopupResultCount--;
		});

    setPopupActive(true);
}

void ZeroconfDiscoverComponent::handlePopupResult(int result)
{
	// handle result
	if (result > 0 && onServiceSelected)
	{
		auto serviceIndex = result - 1;

		if (m_currentServiceBrowsingList.size() >= serviceIndex)
		{
			auto service = std::get<1>(m_currentServiceBrowsingList.at(serviceIndex));
			auto searcher = getSearcherByServiceName(service.name);

			if (searcher != nullptr)
				onServiceSelected(getServiceType(searcher->GetName()), &service);
			else
				onServiceSelected(ZST_Unkown, nullptr);
		}
		else
			onServiceSelected(ZST_Unkown, nullptr);
	}

	shutdownSearching();

	setPopupActive(false);
}

void ZeroconfDiscoverComponent::lookAndFeelChanged()
{
	juce::DrawableButton::lookAndFeelChanged();

	auto colourOn = getLookAndFeel().findColour(TextButton::ColourIds::textColourOnId);
	auto colourOff = getLookAndFeel().findColour(TextButton::ColourIds::textColourOffId);

	std::unique_ptr<juce::Drawable> NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage;
	JUCEAppBasics::Image_utils::getDrawableButtonImages(BinaryData::find_replace24px_svg, NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage,
		colourOn, colourOff, colourOff, colourOff, colourOn, colourOn, colourOn, colourOn);

	setImages(NormalImage.get(), OverImage.get(), DownImage.get(), DisabledImage.get(), NormalOnImage.get(), OverOnImage.get(), DownOnImage.get(), DisabledOnImage.get());
}

void ZeroconfDiscoverComponent::handleServicesChanged(std::string serviceName)
{
	postMessage(new ServiceChangedMessage(serviceName));
}

void ZeroconfDiscoverComponent::handleMessage(const Message& message)
{
	auto serviceChangedMessage = dynamic_cast<const ServiceChangedMessage*>(&message);
	if (serviceChangedMessage != nullptr)
	{
		auto servicesChanged = false;
		for (auto& searcher : m_searchers)
		{
			if (searcher)
			{
				for (auto& service : searcher->GetServices())
				{
					if (std::find_if(m_currentServiceBrowsingList.begin(), m_currentServiceBrowsingList.end(), [&](const auto& val) { return std::get<1>(val) == service; }) == m_currentServiceBrowsingList.end())
					{
						servicesChanged = true;
						break;
					}
				}

				if (servicesChanged)
					break;
			}
		}

		if (servicesChanged && isPopupActive())
		{
			m_currentServiceBrowsingPopup.dismissAllActiveMenus();
            m_currentServiceBrowsingPopup.clear();
			setPopupActive(false);
			showMenuAndGetService(serviceChangedMessage->GetServiceName());
		}
	}
}

ZeroconfDiscoverComponent::ZeroconfServiceType ZeroconfDiscoverComponent::getServiceType(juce::String name)
{
    if (name.contains(getServiceName(ZST_OSC)))
        return ZST_OSC;
    else if (name.contains(getServiceName(ZST_OCA)))
        return ZST_OCA;
    else
        return ZST_Unkown;
}

juce::String ZeroconfDiscoverComponent::getServiceName(ZeroconfServiceType type)
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

juce::String ZeroconfDiscoverComponent::getServiceDescriptor(ZeroconfServiceType type)
{
    switch (type)
    {
        case ZST_OCA:
            return "_oca._tcp";
        case ZST_OSC:
            return "_osc._udp";
        default:
            return juce::String();
    };
}

bool ZeroconfDiscoverComponent::isPopupActive()
{
    return m_popupActive;
}

void ZeroconfDiscoverComponent::setPopupActive(bool active)
{
    m_popupActive = active;
}

void ZeroconfDiscoverComponent::mouseDown(const juce::MouseEvent& e)
{
	auto discoverButtonHit = getScreenBounds().contains(e.getScreenPosition());
	if (discoverButtonHit && !isSearchingActive())
	{
		startupSearching();
		juce::DrawableButton::mouseDown(e);
	}
	else if (isPopupActive() && isSearchingActive())
	{
		auto outsideWorldHit = true;
		auto iter = juce::PopupMenu::MenuItemIterator(m_currentServiceBrowsingPopup, true);
		if (!discoverButtonHit)
		{
			auto popupMenuHit = false;
			while (iter.next())
			{
				if (iter.getItem().customComponent.get() == e.originalComponent)
				{
					popupMenuHit = true;
					break;
				}
			}
			outsideWorldHit = !popupMenuHit;
		}

		if (discoverButtonHit)
		{
			m_ignoreBlankPopupResultCount = 0; // reset the ignore count for popup menu results. this enables the next popup result that is the direct cause of this mouseDown to be processed and not be ignored.
			return;
		}

		if (outsideWorldHit)
		{
			m_ignoreBlankPopupResultCount = 0; // reset the ignore count for popup menu results. this enables the next popup result that is the direct cause of this mouseDown to be processed and not be ignored.
		}

		juce::DrawableButton::mouseDown(e);
	}
}

bool ZeroconfDiscoverComponent::isSearchingActive()
{
	return m_searchingActive;
}

void ZeroconfDiscoverComponent::startupSearching()
{
	m_searchingActive = true;

	for (auto const& searcher : m_searchers)
	{
		if (searcher && !isPopupActive())
		{
			searcher->StartSearching();
			showMenuAndGetService(searcher->GetServiceName());
		}
	}

	return;
}

void ZeroconfDiscoverComponent::shutdownSearching()
{
	// shutdown searching for all searchers
	auto searchersToShutdown = std::vector<ZeroconfSearcher::ZeroconfSearcher*>();
	for (auto const& serviceEntry : m_currentServiceBrowsingList)
	{
		auto searcher = getSearcherByServiceName(std::get<0>(serviceEntry));
		if (searcher != nullptr && std::find(searchersToShutdown.begin(), searchersToShutdown.end(), searcher) == searchersToShutdown.end())
			searchersToShutdown.push_back(searcher);
	}
	for (auto const& searcher : searchersToShutdown)
	{
		if (nullptr != searcher)
			searcher->StopSearching();
	}
	m_currentServiceBrowsingList.clear();
	m_searchingActive = false;
}


}
