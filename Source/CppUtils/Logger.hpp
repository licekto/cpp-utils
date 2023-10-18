#pragma once

#include <LogLevel.hpp>

#include <fstream>
#include <mutex>
#include <source_location>
#include <sstream>
#include <string>
#include <string.h>
#include <string_view>
#include <thread>
#include <unordered_map>

#include "Exception.hpp"

//#include <boost/lockfree/spsc_queue.hpp>

namespace Logger
{
class LoggerException : public CppUtils::CppUtilsException
{
public:
    LoggerException(std::string msg);
};

/**
 * @brief The Logger Level enum
 */
enum class Level
{
    Data = 0, /// Data level - prints contents of variables etc.
    Trace = 1, /// Trace level
    Debug = 2, /// Debug level
    Info = 3, /// Info level
    Warning = 4, /// Warning level
    Error = 5 /// Error level
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

#define log_data log_on_level(Logger::Level::Data)
#define log_trace log_on_level(Logger::Level::Trace)
#define log_debug log_on_level(Logger::Level::Debug)
#define log_info log_on_level(Logger::Level::Info)
#define log_warning log_on_level(Logger::Level::Warning)
#define log_error log_on_level(Logger::Level::Error)
