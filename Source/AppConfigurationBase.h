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
				config->triggerListenersUpdate();
		};
	};

	class Listener
	{
	public:
        virtual ~Listener(){};
        
		virtual void performConfigurationDump() = 0;
	};

public:
	AppConfigurationBase(const File &file);
	virtual ~AppConfigurationBase();

	static AppConfigurationBase* getInstance() noexcept;
	static AppConfigurationBase*	m_singleton;
	static String getDefaultConfigFilePath() noexcept;

	virtual bool isValid();

	void addListener(AppConfigurationBase::Listener* l);
	void triggerListenersUpdate();

	std::unique_ptr<XmlElement> getConfigState(StringRef tagName);
	bool setConfigState(std::unique_ptr<XmlElement> stateXml);

protected:
	std::unique_ptr<File>		m_file;
	std::unique_ptr<XmlElement>	m_xml{ nullptr };

private:
	bool initializeFromDisk();
	bool exists();
	bool create();
	bool flush();
	void debugPrintXmlTree();

	std::vector<Listener*>		m_listeners;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppConfigurationBase)
};

}

