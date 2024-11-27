#pragma once

#include <filesystem>
#include <fstream>
#include <format>
#include <iostream>
#include <ts_queue/tsqueue.hpp>
#include <toolbox/Vector3.h>

#include "utils/flag.h"

/// <summary>
/// A Class for a Logger to debug
/// </summary>
class Logger
{
public:
	UNDEFINED_ENGINE Logger();
	UNDEFINED_ENGINE ~Logger();

public:
	/// <summary>
	/// Enum where we put different level of warning/error
	/// </summary>
	enum class LogLevel
	{
		DEBUG = 0,
		INFO,
		WARNING,
		ERROR,
		FATALERROR,
	};

private:
	/// <summary>
	/// Struct used to set the level, the string log and the time offset of the Entry
	/// </summary>
	struct LogEntry
	{
		LogLevel Level;
		std::string Log;
		std::chrono::system_clock::time_point TimeOffset;
	};

public:
	/// <summary>
	/// Stop the Logger thread
	/// </summary>
	UNDEFINED_ENGINE static void Stop();
	UNDEFINED_ENGINE static void CheckForExit();
	UNDEFINED_ENGINE static void Sync();
	UNDEFINED_ENGINE static void Desync();

	static void DisableEntry(LogLevel level);
	static void EnableEntry(LogLevel level);

	template<class... Types>
	static void Debug(std::string string, Types... args)
	{
		if (!isDebugEnabled)
			return;
		constexpr std::size_t n = sizeof...(args);
		size_t nFormat = 0;
		std::string log;

		for (size_t i = 0; (i = string.find("{}", i)) != std::string::npos; i += 2)
		{
			nFormat++;
		}

		for (size_t i = nFormat; i < n; i++)
		{
			string += "{} ";
		}

		if (nFormat <= n)
		{
			log = std::vformat(string, std::make_format_args(args...));
			SetupLogEntry(LogLevel::DEBUG, log);
		}

		else
		{
			Logger::Error("Logger::Debug Attempt too many arguments, not enough {}", "{}");

		}
	}

	template<class... Types>
	static void Info(std::string string, Types... args)
	{
		if (!isInfoEnabled)
			return;
		constexpr std::size_t n = sizeof...(args);
		size_t nFormat = 0;
		std::string log;

		for (size_t i = 0; (i = string.find("{}", i)) != std::string::npos; i += 2)
		{
			nFormat++;
		}

		for (size_t i = nFormat; i < n; i++)
		{
			string += "{} ";
		}

		if (nFormat <= n)
		{
			log = std::vformat(string, std::make_format_args(args...));
			SetupLogEntry(LogLevel::INFO, log);
		}

		else
		{
			Logger::Error("Logger::Info Attempt too many arguments, not enough {}", "{}");
		}
	}

	template<class... Types>
	static void Warning(std::string string, Types... args)
	{
		if (!isWarningEnabled)
			return;
		constexpr std::size_t n = sizeof...(args);
		size_t nFormat = 0;
		std::string log;

		for (size_t i = 0; (i = string.find("{}", i)) != std::string::npos; i += 2)
		{
			nFormat++;
		}

		for (size_t i = nFormat; i < n; i++)
		{
			string += "{} ";
		}

		if (nFormat <= n)
		{
			log = std::vformat(string, std::make_format_args(args...));
			SetupLogEntry(LogLevel::WARNING, log);
		}

		else
		{
			Logger::Error("Too many arguments, not enough {}", "{}");
		}
	}

	template<class... Types>
	static void Error(std::string string, Types... args)
	{
		if (!isErrorEnabled)
			return;
		constexpr std::size_t n = sizeof...(args);
		size_t nFormat = 0;
		std::string log;

		for (size_t i = 0; (i = string.find("{}", i)) != std::string::npos; i += 2)
		{
			nFormat++;
		}

		for (size_t i = nFormat; i < n; i++)
		{
			string += "{} ";
		}

		if (nFormat <= n)
		{
			log = std::vformat(string, std::make_format_args(args...));
			SetupLogEntry(LogLevel::ERROR, log);
		}

		else
		{
			Logger::Error("Too many arguments, not enough {}", "{}");
		}
	}

	template<class... Types>
	static void FatalError(std::string string, Types... args)
	{
		constexpr std::size_t n = sizeof...(args);
		size_t nFormat = 0;
		std::string log;

		for (size_t i = 0; (i = string.find("{}", i)) != std::string::npos; i += 2)
		{
			nFormat++;
		}

		for (size_t i = nFormat; i < n; i++)
		{
			string += "{} ";
		}

		if (nFormat <= n)
		{
			log = std::vformat(string, std::make_format_args(args...));
			SetupLogEntry(LogLevel::FATALERROR, log);
		}

		else
		{
			Logger::Error("Too many arguments, not enough {}", "{}");
		}
	}

private:
	static std::string CurrentDateTime();
	static void CreateDebugFile(const std::string& path, const std::string& mName);
	static void SetupLogEntry(LogLevel level, const std::string& log);

	static void Start();
	static void PrintEntry(LogEntry entry);


	UNDEFINED_ENGINE static inline bool mIsRunning = true;
	UNDEFINED_ENGINE static inline bool mIsSync = true;

	UNDEFINED_ENGINE static inline std::fstream mFile;

	UNDEFINED_ENGINE static inline std::condition_variable mSleep;
	UNDEFINED_ENGINE static inline std::thread mThread = std::thread(&Logger::Start);
	UNDEFINED_ENGINE static inline TsQueue<LogEntry> mEntryList;

	static inline bool isInfoEnabled = true;
	static inline bool isDebugEnabled = true;
	static inline bool isWarningEnabled = true;
	static inline bool isErrorEnabled = true;
};
