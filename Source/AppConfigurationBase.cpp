/*
  ==============================================================================

    AppConfigurationBaseBase.cpp
    Created: 28 Jul 2020 10:42:42am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "AppConfigurationBase.h"

namespace JUCEAppBasics
{

AppConfigurationBase* AppConfigurationBase::m_singleton = nullptr;


AppConfigurationBase::AppConfigurationBase(const File& file)
	: m_file(std::make_unique<File>(file))
{
	if (m_singleton)
		jassertfalse;
	m_singleton = this;

	if (!exists() && !create())
		jassertfalse;

	initializeFromDisk();
}

AppConfigurationBase::~AppConfigurationBase()
{
}

AppConfigurationBase* AppConfigurationBase::getInstance() noexcept
{
	if (m_singleton == nullptr)
	{
		DBG("The configuration object was not yet created! Please make shure to do so in your derived class!");
		jassertfalse;
	}

	return m_singleton;
}

String AppConfigurationBase::getDefaultConfigFilePath() noexcept
{
	static String s_configPath{ File::getSpecialLocation(File::SpecialLocationType::userApplicationDataDirectory).getFullPathName() + "/"
										+ JUCEApplication::getInstance()->getApplicationName() + "/"
										+ JUCEApplication::getInstance()->getApplicationName() + ".config" };

	return s_configPath;
}

bool AppConfigurationBase::isValid()
{
	return isValid(m_xml);
}

bool AppConfigurationBase::isValid(const std::unique_ptr<XmlElement>& xmlConfiguration)
{
	if (!xmlConfiguration)
		return false;

	if (!xmlConfiguration->hasTagName(JUCEApplication::getInstance()->getApplicationName()))
		return false;

	return true;
}

bool AppConfigurationBase::exists()
{
	return m_file->exists();
}

bool AppConfigurationBase::create()
{
	if (exists())
		return true;

	if (!m_file->create().wasOk())
		return false;

	return true;
}

bool AppConfigurationBase::initializeFromDisk()
{
	m_xml = juce::parseXML(*m_file.get());

	if (m_xml && m_xml->hasTagName(JUCEApplication::getInstance()->getApplicationName()))
	{
		return true;
	}
	else
	{
		m_xml = std::make_unique<XmlElement>(JUCEApplication::getInstance()->getApplicationName());
	}
		
	return false;
}

bool AppConfigurationBase::flush(bool includeWatcherUpdate)
{
	if (!m_xml)
		return false;

	if (GetFlushAndUpdateDisabled().first) // check if config flushing to disk is globally disabled
	{
#ifdef DEBUG
		DBG(String(__FUNCTION__) + " config flushing to disk is globally disabled");
#endif
		return true;
	}

//#ifdef DEBUG
//	debugPrintXmlTree();
//#endif

	if (!m_xml->writeTo(*m_file.get()))
		jassertfalse;

	if (includeWatcherUpdate)
		triggerWatcherUpdate();

	return true;
}

void AppConfigurationBase::addDumper(AppConfigurationBase::Dumper* d)
{
	m_dumpers.push_back(d);
}

void AppConfigurationBase::triggerConfigurationDump(bool includeWatcherUpdate)
{

	for (auto d : m_dumpers)
		d->performConfigurationDump();

	flush(includeWatcherUpdate);
}

void AppConfigurationBase::clearDumpers()
{
	m_dumpers.clear();
}

void AppConfigurationBase::addWatcher(AppConfigurationBase::Watcher* w, bool initialUpdate)
{
	if (initialUpdate)
		w->onConfigUpdated();

	m_watchers.push_back(w);
}

void AppConfigurationBase::triggerWatcherUpdate()
{
	if (GetFlushAndUpdateDisabled().second) // check if watcher updating is globally disabled
	{
#ifdef DEBUG
		DBG(String(__FUNCTION__) + " config watcher updating is globally disabled");
#endif
		return;
	}

	for (auto w : m_watchers)
		w->onConfigUpdated();
}

void AppConfigurationBase::clearWatchers()
{
	m_watchers.clear();
}

std::unique_ptr<XmlElement> AppConfigurationBase::getConfigState(StringRef tagName)
{
	if (m_xml)
	{
		if (tagName.isEmpty())
			return std::make_unique<XmlElement>(*m_xml);

		XmlElement *tagNameElement = m_xml->getChildByName(tagName);
		if (tagNameElement)
			return std::make_unique<XmlElement>(*tagNameElement);
		else
			return nullptr;
	}
	else
		return nullptr;
}

bool AppConfigurationBase::setConfigState(std::unique_ptr<XmlElement> stateXml, StringRef attributeName)
{
	if (stateXml && m_xml)
	{
		XmlElement *existingChildElement = m_xml->getChildByName(stateXml->getTagName());
		XmlElement* childElement = existingChildElement;
		while (childElement != nullptr && attributeName.isNotEmpty())
		{
			if (childElement->getIntAttribute(attributeName) == stateXml->getIntAttribute(attributeName))
			{
				existingChildElement = childElement;
				break;
			}

			childElement = childElement->getNextElementWithTagName(stateXml->getTagName());
		}

		if(!existingChildElement || existingChildElement->getIntAttribute(attributeName) != stateXml->getIntAttribute(attributeName))
			m_xml->addChildElement(std::make_unique<XmlElement>(*stateXml).release());
		else
			m_xml->replaceChildElement(existingChildElement, new XmlElement(*stateXml));

		return true;
	}
		
	return false;
}

bool AppConfigurationBase::resetConfigState(std::unique_ptr<XmlElement> fullStateXml)
{
	m_xml.reset(fullStateXml.release());

	triggerWatcherUpdate();

	return true;
}

const std::pair<bool, bool>& AppConfigurationBase::GetFlushAndUpdateDisabled() const
{
	return m_flushAndUpdateDisabled;
}

void AppConfigurationBase::SetFlushAndUpdateDisabled(bool disableFlush, bool disableUpdate)
{
	m_flushAndUpdateDisabled.first = disableFlush;
	m_flushAndUpdateDisabled.second = disableUpdate;
}

void AppConfigurationBase::ResetFlushAndUpdateDisabled(bool flushAndUpdateNow)
{
	SetFlushAndUpdateDisabled(false, false);

	if (flushAndUpdateNow)
		flush(true);
}

#ifdef DEBUG
void AppConfigurationBase::debugPrintXmlTree()
{
	DBG("#####################################");
	DBG("###### saving xml tree to file ######");
	DBG("#####################################");
	DBG(m_xml->getTagName());
    for (auto childElement : m_xml->getChildIterator())
	{
		auto childDbgString = String("- " + childElement->getTagName());
		for (int i = 0; i < childElement->getNumAttributes(); ++i)
			childDbgString += " " + childElement->getAttributeName(i) + "=" + childElement->getAttributeValue(i);
		DBG(childDbgString);

        for (auto childchildElement : childElement->getChildIterator())
		{
			auto childchildDbgString = String("-- " + childchildElement->getTagName());
			for (int i = 0; i < childchildElement->getNumAttributes(); ++i)
				childchildDbgString += " " + childchildElement->getAttributeName(i) + "=" + childchildElement->getAttributeValue(i);
			DBG(childchildDbgString);

            for (auto childchildchildElement : childchildElement->getChildIterator())
			{
				auto childchildchildDbgString = String("--- " + childchildchildElement->getTagName());
				for (int i = 0; i < childchildchildElement->getNumAttributes(); ++i)
					childchildchildDbgString += " " + childchildchildElement->getAttributeName(i) + "=" + childchildchildElement->getAttributeValue(i);
				DBG(childchildchildDbgString);

                for (auto childchildchildchildElement : childchildchildElement->getChildIterator())
				{
					auto childchildchildchildDbgString = String("---- " + childchildchildchildElement->getTagName());
					for (int i = 0; i < childchildchildchildElement->getNumAttributes(); ++i)
						childchildchildchildDbgString += " " + childchildchildchildElement->getAttributeName(i) + "=" + childchildchildchildElement->getAttributeValue(i);
					DBG(childchildchildchildDbgString);

                    for (auto childchildchildchildchildElement : childchildchildchildElement->getChildIterator())
					{
						auto childchildchildchildchildDbgString = String("----- " + childchildchildchildchildElement->getTagName());
						for (int i = 0; i < childchildchildchildchildElement->getNumAttributes(); ++i)
							childchildchildchildchildDbgString += " " + childchildchildchildchildElement->getAttributeName(i) + "=" + childchildchildchildchildElement->getAttributeValue(i) + " value=" + childchildchildchildchildElement->getAllSubText();
						DBG(childchildchildchildchildDbgString);
					}
				}
			}
		}
	}
	DBG("#####################################\n");
}
#endif

}
