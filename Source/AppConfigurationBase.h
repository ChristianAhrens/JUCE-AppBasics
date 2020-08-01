/*
  ==============================================================================

    AppConfigurationBase.h
    Created: 28 Jul 2020 10:42:42am
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace JUCEAppBasics
{

class AppConfigurationBase
{
public:
	class XmlConfigurableElement
	{
	public:
        virtual ~XmlConfigurableElement(){};
        
		virtual std::unique_ptr<XmlElement> createStateXml() = 0;
		virtual bool setStateXml(XmlElement* stateXml) = 0;

		void triggerConfigurationUpdate()
		{
			auto config = AppConfigurationBase::getInstance();
			if (config != nullptr)
				config->triggerConfigurationDump();
		};
	};

	class Dumper
	{
	public:
        virtual ~Dumper(){};
        
		virtual void performConfigurationDump() = 0;
	};

	class Watcher
	{
	public:
		virtual ~Watcher(){};

		virtual void onConfigUpdated() = 0;
	};

public:
	AppConfigurationBase(const File &file);
	virtual ~AppConfigurationBase();

	static AppConfigurationBase* getInstance() noexcept;
	static AppConfigurationBase*	m_singleton;
	static String getDefaultConfigFilePath() noexcept;

	virtual bool isValid();

	void addDumper(AppConfigurationBase::Dumper* d);
	void triggerConfigurationDump();
	void clearDumpers();

	void addWatcher(AppConfigurationBase::Watcher* w);
	void triggerWatcherUpdate();
	void clearWatchers();

	std::unique_ptr<XmlElement> getConfigState(StringRef tagName = StringRef());
	bool setConfigState(std::unique_ptr<XmlElement> stateXml, StringRef attributeName = StringRef());

protected:
	std::unique_ptr<File>		m_file;
	std::unique_ptr<XmlElement>	m_xml{ nullptr };

private:
	bool initializeFromDisk();
	bool exists();
	bool create();
	bool flush();
	void debugPrintXmlTree();

	std::vector<Dumper*>		m_dumpers;
	std::vector<Watcher*>		m_watchers;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppConfigurationBase)
};

}

