#pragma once

#include "LogLevel.hpp"

#include <fstream>
#include <mutex>
#include <source_location>
#include <sstream>
#include <string>
#include <string.h>
#include <string_view>
#include <thread>
#include <unordered_map>

//#include <boost/lockfree/spsc_queue.hpp>

namespace Logger
{
class LoggerException : std::exception
{
public:
    LoggerException(const std::string& msg_);
    const char * what() const noexcept override;

private:
    std::string msg;
};

/**
 * @brief The Logger Level enum
 */
enum class Level
{
    Trace = 0, /// Trace level
    Debug = 1, /// Debug level
    Info = 2, /// Info level
    Warning = 3, /// Warning level
    Error = 4 /// Error level
};

namespace Detail
{
/// Global log level
static constexpr Level GlobalLevel { Level::LOG_LEVEL };

/**
 * @brief enabledLevel
 * @param level
 * @return true if the log level is enabled
 */
static constexpr bool enabledLevel(const Level level) noexcept
{
    return (level >= GlobalLevel);
}

/**
 * @brief The SourceLocation class
 * Contains info about the current logged line
 */
struct SourceLocation
{
    const std::string_view filename, function;
    const size_t line;
};

/**
 * @brief operator << for SourceLocation to be printed in the log line
 * @param os
 * @param sourceLocation
 * @return
 */
std::ostream &operator<<(std::ostream &os, const SourceLocation &sourceLocation);

/**
 * @brief getSourceLocation Obtains source location for the current line
 * @param file
 * @param func
 * @param line
 * @return
 */
SourceLocation getSourceLocation(const char* file, const char* func, const size_t line);

inline std::unordered_map<std::thread::id, std::string> threadsNames;

const std::string& GetCurrentThreadName();

class LoggerImpl
{
public:
    class Line
    {
    public:
        Line(LoggerImpl &parent_);

        Line(Line &&other);

        ~Line();

        template<typename T>
        Line operator<<(T &&item) & = delete;

        template<typename T>
        Line operator<<(T &&item) &&
        {
            ss << item;
            moved = true;
            return std::move(*this);
        }

    private:
        bool moved{false};
        LoggerImpl &parent;
        std::stringstream ss;
    };

    ~LoggerImpl();

    void SetFile(std::string filename_);

    Line Log(const SourceLocation sourceLocation, const Level level);

private:

    static constexpr size_t flushLinesLimit = 1;
    void DumpLine(std::stringstream&& line);

    std::string filename;
    bool fileSet = false;
    std::ofstream fileStream;
    std::mutex mutex;
    size_t lineCounter{0};
};

inline LoggerImpl logger;
}

void SetCurrentThreadName(const std::string &name);
void SetLogFile(std::string filename);
}

#define log_on_level(level) for (bool tmp_ = Logger::Detail::enabledLevel(level); tmp_ != false; tmp_ = false) \
    Logger::Detail::logger.Log(Logger::Detail::getSourceLocation(__FILE__, __FUNCTION__, __LINE__), level)

#define log_trace log_on_level(Logger::Level::Trace)
#define log_debug log_on_level(Logger::Level::Debug)
#define log_info log_on_level(Logger::Level::Info)
#define log_warning log_on_level(Logger::Level::Warning)
#define log_error log_on_level(Logger::Level::Error)
