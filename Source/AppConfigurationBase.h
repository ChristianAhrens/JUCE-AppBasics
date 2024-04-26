/*
  ==============================================================================

    AppConfigurationBase.h
    Created: 28 Jul 2020 10:42:42am
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <thread>

namespace JUCEAppBasics
{

class AppConfigurationBase
{
public:
	class XmlConfigurableElement
	{
	public:
		class ScopedXmlChangeLock
		{
		public:
			inline explicit ScopedXmlChangeLock(bool& locked, bool acquireLockOnInitialisation = true) noexcept
				: m_locked(locked), m_lockWasSuccessful(acquireLockOnInitialisation && !locked)
			{
				if (m_lockWasSuccessful)
					m_locked = true;
			}

			inline ~ScopedXmlChangeLock() noexcept
			{
				if (m_lockWasSuccessful)
					m_locked = false;
			}

			bool isLocked() const noexcept
			{
				return m_lockWasSuccessful;
			}

		private:
			//==============================================================================
			bool& m_locked;
			bool m_lockWasSuccessful;

			JUCE_DECLARE_NON_COPYABLE(ScopedXmlChangeLock)
		};

	public:
        virtual ~XmlConfigurableElement(){};
        
		virtual std::unique_ptr<XmlElement> createStateXml() = 0;
		virtual bool setStateXml(XmlElement* stateXml) = 0;

		void triggerConfigurationUpdate(bool includeWatcherUpdate)
		{
			auto config = AppConfigurationBase::getInstance();
			if (config != nullptr)
				config->triggerConfigurationDump(includeWatcherUpdate);
		};

		bool& IsXmlChangeLocked()
		{
			return m_lockState;
		};

	private:
		bool m_lockState{ false };
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

	class Version
	{
	public:
		Version()
		{
		};
		Version(const juce::String& versionString)
		{
			auto other = FromString(versionString);
			m_major = other.GetMajor();
			m_minor = other.GetMinor();
			m_fixlevel = other.GetFixlevel();
		};
		Version(const Version& other)
		{
			m_major = other.GetMajor();
			m_minor = other.GetMinor();
			m_fixlevel = other.GetFixlevel();
		};
		Version(int major, int minor, int fixlevel)
		{
			m_major = major;
			m_minor = minor;
			m_fixlevel = fixlevel;
		};
		~Version()
		{
		};

		bool operator==(const Version& other) const
		{
			return GetMajor() == other.GetMajor()
				&& GetMinor() == other.GetMinor()
				&& GetFixlevel() == other.GetFixlevel();
		};
		bool operator!=(const Version& other) const
		{
			return !(*this == other);
		};

		bool IsValid() const
		{
			return m_major != 0 || m_minor != 0 || m_fixlevel != 0;
		};

		static Version FromString(const juce::String& versionString)
		{
			auto sa = juce::StringArray();
			auto cnt = sa.addTokens(versionString, ".", "");
			jassert(3 == cnt);
			if (3 != cnt)
				return Version();

			return Version(sa[0].getIntValue(), sa[1].getIntValue(), sa[2].getIntValue());
		};

		static juce::String ToString(const Version& version)
		{
			juce::String rv;
			rv << version.GetMajor() << "." << version.GetMinor() << "." << version.GetFixlevel();
			return rv;
		};

		juce::String ToString() const
		{
			return Version::ToString(*this);
		};

		int GetMajor() const
		{
			return m_major;
		};
		int GetMinor() const
		{
			return m_minor;
		};
		int GetFixlevel() const
		{
			return m_fixlevel;
		};

	private:
		int m_major { 0 };
		int m_minor{ 0 };
		int m_fixlevel{ 0 };
	};

public:
	AppConfigurationBase();
	virtual ~AppConfigurationBase();

	static AppConfigurationBase* getInstance() noexcept;
	static AppConfigurationBase*	m_singleton;
	static String getDefaultConfigFilePath() noexcept;

	void InitializeBase(const juce::File& file, const Version& configVersion = Version());

	bool UsesConfigVersion() { return m_configVersion.IsValid(); };

	virtual bool isValid();
	static bool isValid(const std::unique_ptr<XmlElement>& xmlConfiguration);

	void addDumper(AppConfigurationBase::Dumper* d);
	void triggerConfigurationDump(bool includeWatcherUpdate = true);
	void clearDumpers();

	void addWatcher(AppConfigurationBase::Watcher* w, bool initialUpdate = false);
	void triggerWatcherUpdate();
	void clearWatchers();

	std::unique_ptr<XmlElement> getConfigState(StringRef tagName = StringRef());
	bool setConfigState(std::unique_ptr<XmlElement> stateXml, StringRef attributeName = StringRef());
	bool resetConfigState(std::unique_ptr<XmlElement> fullStateXml);

	bool IsFlushAndUpdateDisabled() const;
	const std::pair<bool, bool>& GetFlushAndUpdateDisabled() const;
	void SetFlushAndUpdateDisabled(bool disableFlush = true, bool disableUpdate = true);
	void ResetFlushAndUpdateDisabled(bool flushAndUpdateNow = true);

protected:
	virtual bool HandleConfigVersionConflict(const Version& configVersionFound);

protected:
	std::unique_ptr<XmlElement>	m_xml{ nullptr };
	std::unique_ptr<XmlElement>	m_xmlFileFlushCopy{ nullptr };
	std::mutex							m_xmlCopyAccessMutex;
	
	std::unique_ptr<File>			m_file{ nullptr };
	std::unique_ptr<std::thread>	m_fileFlushThread;
	std::atomic<bool>				m_fileFlushThreadActive;
	std::condition_variable			m_fileFlushCV;
	std::mutex						m_fileFlushCVMutex;

private:
	void SetupFileFlushThread();
	void TeardownFileFlushThread();

private:
	bool initializeFromDisk();
	bool exists();
	bool create();
	bool flush(bool includeWatcherUpdate);

#ifdef DEBUG
	void debugPrintXmlTree();
#endif

	std::vector<Dumper*>		m_dumpers;
	std::vector<Watcher*>		m_watchers;

	std::pair<bool, bool>		m_flushAndUpdateDisabled{ false, false };

	Version						m_configVersion;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppConfigurationBase)
};

}

