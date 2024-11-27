#include "engine_debug/logger.h"

#include <stdarg.h>
#include <mutex>
#include <Windows.h>

#include "memory_leak.h"

#undef ERROR

Logger::Logger()
{
    CreateDebugFile(std::string("../log/"), CurrentDateTime());
}

Logger::~Logger()
{
    mFile.close();
}

std::string Logger::CurrentDateTime()
{
    std::time_t t = std::time(nullptr);

    std::tm now;
    localtime_s(std::addressof(now), std::addressof(t));

    char buffer[128];
    strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H-%M-%S", &now);
    return buffer;
}

void Logger::CreateDebugFile(const std::string& path, const std::string& mName)
{
    std::filesystem::path const& mFilename = path + mName + ".txt";
    int fileCount = 0;

    if (!std::filesystem::exists(path))
    {
        std::filesystem::create_directory(path);

        Logger::Info("Directory {} created", path);
    }

    for ([[maybe_unused]]auto& p : std::filesystem::directory_iterator(path))
        ++fileCount;

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (fileCount >= 4)
        {
            remove(entry);
            break;
        }
    }

    mFile.open(mFilename, std::fstream::out);
    if (mFile.is_open())
    {
        Logger::Info("File {} created", mFilename.string());
    }
    else
    {
        Logger::Warning("File {} could not be created", mFilename.string());
    }
}

void Logger::SetupLogEntry(LogLevel level, const std::string& log)
{
    LogEntry entry;
    entry.Level = level;
    entry.Log = log;
    entry.TimeOffset = std::chrono::system_clock::now();
    if (mIsSync)
    {
        PrintEntry(entry);
    }
    else
    {
        mEntryList.Push(entry);
        mSleep.notify_one();
    }
}

void Logger::Start()
{
    MemoryLeak::CheckMemoryLeak(true);

    std::mutex mutex;
    (void)SetThreadDescription(mThread.native_handle(), L"Logger Thread");

    std::unique_lock lock(mutex);
    while (mIsRunning)
    {
        mSleep.wait(lock, [] {return (!mEntryList.Empty() || !mIsRunning); });

        while (!mEntryList.Empty())
        {
            PrintEntry(mEntryList.Pop());
        }

        std::cout.flush();
    }
}

void Logger::PrintEntry(LogEntry entry)
{
    std::string log = entry.Log + '\n';
    const char* color = ANSI_RESET;
    std::chrono::milliseconds t = std::chrono::duration_cast<std::chrono::milliseconds, long long>(entry.TimeOffset.time_since_epoch());
    std::string time = std::format("[{:%T}] ", t);

    switch (entry.Level)
    {
    case LogLevel::DEBUG:
        color = ANSI_RESET;
        log = "[DEBUG] " + log;
        break;

    case LogLevel::INFO:
        color = ANSI_COLOR_GRAY;
        log = "[INFO] " + log;
        break;

    case LogLevel::WARNING:
        color = ANSI_COLOR_YELLOW;
        log = "[WARNING] " + log;
        break;

    case LogLevel::ERROR:
        color = ANSI_COLOR_RED;
        log = "[ERROR] " + log;
        break;

    case LogLevel::FATALERROR:
        color = ANSI_COLOR_RED ANSI_STYLE_BOLD;
        log = "[FATAL ERROR] " + log;
        break;
    }

    std::cout << color << time << log << ANSI_RESET;
    
    if (mFile.is_open())
    {
        mFile << time << log;
    }
}

void Logger::DisableEntry(LogLevel level)
{
    switch (level)
    {
    case LogLevel::DEBUG:
        isDebugEnabled = false;
        break;

    case LogLevel::INFO:
        isInfoEnabled = false;
        break;

    case LogLevel::WARNING:
        isWarningEnabled = false;
        break;

    case LogLevel::ERROR:
        isErrorEnabled = false;
        break;

    case LogLevel::FATALERROR:
        break;
    }
}

void Logger::EnableEntry(LogLevel level)
{
    switch (level)
    {
    case LogLevel::DEBUG:
        isDebugEnabled = true;
        break;

    case LogLevel::INFO:
        isInfoEnabled = true;
        break;

    case LogLevel::WARNING:
        isWarningEnabled = true;
        break;

    case LogLevel::ERROR:
        isErrorEnabled = true;
        break;

    case LogLevel::FATALERROR:
        break;
    }
}

void Logger::Stop()
{
    mIsRunning = false;
    mSleep.notify_one();

    if (mThread.joinable())
    {
        mThread.join();
    }
}

void Logger::CheckForExit()
{
    std::atexit(Logger::Stop);
    std::at_quick_exit(Logger::Stop);
}

void Logger::Sync()
{
    mIsSync = true;
}

void Logger::Desync()
{
    mIsSync = false;
}
