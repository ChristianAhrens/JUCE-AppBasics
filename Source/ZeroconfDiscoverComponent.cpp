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
ZeroconfDiscoverComponent::ZeroconfDiscoverComponent(bool useSeparateServiceSearchers, bool showServiceNameLabels) :
	Thread("Zeroconf")
{
	m_useSeparateServiceSearchers = useSeparateServiceSearchers;

	setShowServiceNameLabels(showServiceNameLabels);
	startThread();
	startTimer(5000); //every 5s
}

ZeroconfDiscoverComponent::~ZeroconfDiscoverComponent()
{
    for(auto const& serviceDiscoverButtonKV : m_discoveryButtons)
        removeSearcher(serviceDiscoverButtonKV.first);

	signalThreadShouldExit();
	waitForThreadToExit(5000);
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

void ZeroconfDiscoverComponent::addDiscoverService(ZeroconfServiceType serviceType, unsigned short announcementPort)
{
    auto serviceName = getServiceName(serviceType);
    auto serviceDescriptor = getServiceDescriptor(serviceType);
    
	if(m_useSeparateServiceSearchers || m_discoveryButtons.empty())
	{
		std::unique_ptr<juce::Drawable> NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage;
		JUCEAppBasics::Image_utils::getDrawableButtonImages(BinaryData::find_replace24px_svg, NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage);

		m_discoveryButtons.insert(std::make_pair(serviceName, std::make_unique<DrawableButton>(serviceName, DrawableButton::ButtonStyle::ImageOnButtonBackground)));
		m_discoveryButtons.at(serviceName)->addListener(this);
		m_discoveryButtons.at(serviceName)->setImages(NormalImage.get(), OverImage.get(), DownImage.get(), DisabledImage.get(), NormalOnImage.get(), OverOnImage.get(), DownOnImage.get(), DisabledOnImage.get());
		m_discoveryButtons.at(serviceName)->setButtonText(serviceName);
		addAndMakeVisible(m_discoveryButtons.at(serviceName).get());
    
		m_serviceNameLabels.insert(std::make_pair(serviceName, std::make_unique<Label>(serviceName, serviceName + ":")));
		addAndMakeVisible(m_serviceNameLabels.at(serviceName).get());
	}

    addSearcher(serviceName, serviceDescriptor, announcementPort);
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

void ZeroconfDiscoverComponent::addSearcher(StringRef name, StringRef serviceName, unsigned short announcementPort)
{
	ZeroconfSearcher * s = getSearcher(name);

	if (s == nullptr)
	{
		m_searchers.getLock().enter();
		m_searchers.add(new ZeroconfSearcher(name, serviceName, announcementPort));
		m_searchers.getLock().exit();
	}
}

void ZeroconfDiscoverComponent::removeSearcher(StringRef name)
{
	ZeroconfSearcher * s = getSearcher(name);
	if (s == nullptr)
	{
		m_searchers.getLock().enter();
		m_searchers.removeObject(s);
		m_searchers.getLock().exit();
	}
}

ZeroconfDiscoverComponent::ZeroconfSearcher * ZeroconfDiscoverComponent::getSearcher(StringRef name)
{
	for (auto &s : m_searchers)
		if (s->m_name == name)
			return s;

	return nullptr;
}

ZeroconfDiscoverComponent::ServiceInfo* ZeroconfDiscoverComponent::showMenuAndGetService()
{
	if (m_searchers.isEmpty())
	{
		DBG("No searcher found");
		return nullptr;
	}

	PopupMenu p;
	std::vector<ServiceInfo*> serviceList;
	for (auto& searcher : m_searchers)
	{
		for (auto& service : searcher->m_services)
		{
			serviceList.push_back(service);
			p.addItem(static_cast<int>(serviceList.size()), service->name + " on " + service->host + " (" + service->ip + ":" + String(service->port) + ")");
		}
	}

	if(p.getNumItems() == 0)
	{
		p.addItem(-1, "No service found", false);
	}

	int result = p.show();

	if (result <= 0)
		return nullptr;

	return serviceList[result - 1];
}

ZeroconfDiscoverComponent::ServiceInfo * ZeroconfDiscoverComponent::showMenuAndGetService(StringRef searcherName)
{
	auto searcher = getSearcher(searcherName);

	if (searcher == nullptr)
	{
		DBG("No searcher found for service " << searcherName);
		return nullptr;
	}

	PopupMenu p;
	if (searcher->m_services.isEmpty())
	{
		p.addItem(-1, "No service found", false);
	}
	else
	{
		for (int i = 0; i < searcher->m_services.size(); ++i)
		{
			auto info = searcher->m_services[i];
            String serviceItemString = info->name + " on " + (info->host.isEmpty() ? info->ip : info->host);
#ifdef DEBUG
            serviceItemString += " (" + info->ip + ":" + String(info->port) + ")";
#endif
			p.addItem(1 + i, serviceItemString);
		}
	}

	int result = p.show();

	if (result <= 0)
		return nullptr;
	
	return searcher->m_services[result - 1];
}

void ZeroconfDiscoverComponent::search()
{
	startThread();
}

void ZeroconfDiscoverComponent::timerCallback()
{
	search();
}


void ZeroconfDiscoverComponent::run()
{
	sleep(300);

	bool changed = false;

	m_searchers.getLock().enter();
	for (auto & se : m_searchers)
		changed |= se->search();
	m_searchers.getLock().exit();
}

ZeroconfDiscoverComponent::ZeroconfSearcher::ZeroconfSearcher(StringRef name, StringRef serviceName, unsigned short announcementPort) :
	m_name(name),
	m_serviceName(serviceName),
	m_servus(String(serviceName).toStdString())
{
    if (announcementPort != 0)
        m_servus.announce(announcementPort, JUCEApplication::getInstance()->getApplicationName().toStdString());
}

ZeroconfDiscoverComponent::ZeroconfSearcher::~ZeroconfSearcher()
{
	m_services.clear();
}

bool ZeroconfDiscoverComponent::ZeroconfSearcher::search()
{
	if (Thread::getCurrentThread()->threadShouldExit())
		return false;

	servus::Strings instances = m_servus.discover(servus::Servus::Interface::IF_ALL, 200);

	bool changed = false;

	StringArray servicesArray;
	for (auto &s : instances)
		servicesArray.add(s);

	Array<ServiceInfo *> servicesToRemove;

	for (auto &ss : m_services)
	{
		if (servicesArray.contains(ss->name))
		{
			String host = m_servus.get(ss->name.toStdString(), "servus_host");
			if (host.endsWithChar('.'))
				host = host.substring(0, host.length() - 1);
			int port = String(m_servus.get(ss->name.toStdString(), "servus_port")).getIntValue();

			if (ss->host != host || ss->port != port)
				servicesToRemove.add(ss);
		}
		else
		{
			servicesToRemove.add(ss);
		}
	}

	for (auto &ss : servicesToRemove)
		removeService(ss);

	for (auto &s : servicesArray)
	{
        if (Thread::getCurrentThread()->threadShouldExit())
			return false;
        
        String host = m_servus.get(s.toStdString(), "servus_host");
		if (host.endsWithChar('.'))
			host = host.substring(0, host.length() - 1);

		int port = String(m_servus.get(s.toStdString(), "servus_port")).getIntValue();
		String ip = getIPForHostAndPort(host, port);

		bool isLocal = false;
		if (ip.isNotEmpty())
		{
			Array<IPAddress> localIps;
			IPAddress::findAllAddresses(localIps);
			for (auto &lip : localIps)
			{
				if (ip == lip.toString())
				{
					isLocal = true;
					break;
				}
			}
		}

		if (isLocal)
			ip = IPAddress::local().toString();

		ServiceInfo * info = getService(s, host, port);
		if (info == nullptr)
		{
			changed = true;
			addService(s, host, ip, port);
		}
		else if (info->host != host || info->port != port || info->ip != ip)
		{
			changed = true;
			updateService(info, host, ip, port);
		}
	}

	return changed;
}


String ZeroconfDiscoverComponent::ZeroconfSearcher::getIPForHostAndPort(String host, int port)
{
	String ip;

	struct addrinfo hints = { 0 };
	hints.ai_family = AF_INET;

	struct addrinfo* info = nullptr;
	getaddrinfo(host.toRawUTF8(), String(port).toRawUTF8(), &hints, &info);
	if (info == nullptr)
	{
		DBG("Should not be null !");
		return "";
	}

	char * ipData = info->ai_addr->sa_data;
	if (info != nullptr)
		ip = String((uint8)ipData[2]) + "." + String((uint8)ipData[3]) + "." + String((uint8)ipData[4]) + "." + String((uint8)ipData[5]);
	
	freeaddrinfo(info);

	return ip;
}

ZeroconfDiscoverComponent::ServiceInfo * ZeroconfDiscoverComponent::ZeroconfSearcher::getService(StringRef sName, StringRef host, int port)
{
	for (auto &i : m_services)
	{
		if (Thread::getCurrentThread()->threadShouldExit())
			return nullptr;
		if (i->name == sName && i->host == host && i->port == port)
			return i;
	}
	return nullptr;
}

void ZeroconfDiscoverComponent::ZeroconfSearcher::addService(StringRef sName, StringRef host, StringRef ip, int port)
{
	if (Thread::getCurrentThread()->threadShouldExit())
		return;
	//NLOG("Zeroconf", "New " << name << " service discovered : " << sName << " on " << host << ", " << ip << ":" << port);
	jassert(getService(sName, host, port) == nullptr);
	m_services.add(new ServiceInfo{ sName, host, ip, port });

}

void ZeroconfDiscoverComponent::ZeroconfSearcher::removeService(ServiceInfo * s)
{
	jassert(s != nullptr);
	//NLOG("Zeroconf", name << " service removed : " << s->name);
	m_services.removeObject(s);
}

void ZeroconfDiscoverComponent::ZeroconfSearcher::updateService(ServiceInfo * service, StringRef host, StringRef ip, int port)
{
	jassert(service != nullptr);
	//NLOG("Zeroconf", name << "service updated changed : " << name << " : " << host << ", " << ip << ":" << port);
	service->host = host;
	service->ip = ip;
	service->port = port;
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

void ZeroconfDiscoverComponent::buttonClicked(Button* button)
{
    for (auto const& buttonKV : m_discoveryButtons)
    {
		if (!m_useSeparateServiceSearchers)
		{
			ZeroconfDiscoverComponent::ServiceInfo* selectedService = showMenuAndGetService();

			if (onServiceSelected)
			{
				if (selectedService)
					onServiceSelected(getServiceType(selectedService->name), selectedService);
				else
					onServiceSelected(ZST_Unkown, nullptr);
			}

			return;
		}
        else if (button == buttonKV.second.get())
        {
            ZeroconfDiscoverComponent::ServiceInfo* selectedService = showMenuAndGetService(buttonKV.first);
            
            if (onServiceSelected)
                onServiceSelected(getServiceType(buttonKV.first), selectedService);

			return;
        }
    }
}

ZeroconfDiscoverComponent::ZeroconfServiceType ZeroconfDiscoverComponent::getServiceType(String name)
{
    if (name == getServiceName(ZST_OSC))
        return ZST_OSC;
    else if (name == getServiceName(ZST_OCA))
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
