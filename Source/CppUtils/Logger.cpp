#include "Logger.hpp"

namespace Logger
{

LoggerException::LoggerException(std::string msg_)
    : CppUtils::CppUtilsException("Logger exception: '" + std::move(msg_) + "'")
{}

namespace Detail
{

namespace
{

static inline constexpr const char *LevelStrings[]{"DATA", "TRACE", "DEBUG", "INFO", "WARNING", "ERROR"};
//static constexpr size_t LevelsCount = sizeof(LevelStrings) / sizeof(LevelStrings[0]);

static constexpr const char *levelToString(const Level level)
{
    return LevelStrings[static_cast<size_t>(level)];
}

static constexpr bool debugLessOrEqualLevel(const Level level) noexcept
{
    return (level <= Level::Debug);
}

static constexpr std::string_view Delim = "::";

std::string getTimeString()
{
    auto timePoint = std::chrono::system_clock::now();
    // TODO: Time dumper object
    std::time_t currentTime = std::chrono::system_clock::to_time_t(timePoint);
    auto timeInfo = std::localtime(&currentTime);

    constexpr size_t bufferSize = 128;
    char buffer[bufferSize];

    int stringSize = strftime(buffer, bufferSize, "%Y-%m-%d %H:%M:%S", timeInfo);

    const auto duration = timePoint.time_since_epoch();
    const uint32_t ms = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()) % 1000;

    stringSize += std::snprintf(buffer + stringSize, bufferSize - stringSize, ".%03u", ms);

    return std::string(buffer, buffer + stringSize);
}
}

std::ostream &operator<<(std::ostream &os, const SourceLocation &sourceLocation)
{
    return os << sourceLocation.filename << Delim << sourceLocation.function << Delim << sourceLocation.line;
}

SourceLocation getSourceLocation(const char* file, const char* func, const size_t line)
{
#if defined(_WIN32) || defined(_WIN64)
    static constexpr char delim = '\\';
#else
    static constexpr char delim = '/';
#endif

    const char *filename = strrchr(file, delim);
    filename = filename != nullptr ? filename + 1 : file;

    return SourceLocation{filename, func, line};
}

const std::string& GetCurrentThreadName()
{
    if (const auto it = threadsNames.find(std::this_thread::get_id());
        it != std::end(threadsNames))
    {
        return it->second;
    }
    static std::string empty("");
    return empty;
}

LoggerImpl::Line::Line(LoggerImpl &parent_)
    : parent(parent_)
{}

LoggerImpl::Line::Line(Line &&other)
    : parent(other.parent)
    , ss(std::move(other.ss))
{}

LoggerImpl::Line::~Line()
{
    if (!moved)
    {
        ss << "\n";
        parent.DumpLine(std::move(ss));
    }
}

LoggerImpl::~LoggerImpl()
{
    fileStream.flush();
}

void LoggerImpl::SetFile(std::string filename_)
{
    filename = std::move(filename_);
    fileStream = std::ofstream(filename, std::fstream::in | std::fstream::app);

    if (!fileStream.good())
    {
        throw LoggerException("Invalid file '" + filename + "'");
    }
    fileSet = true;
}

void LoggerImpl::Disable()
{
    disabled = true;
}

LoggerImpl::Line LoggerImpl::Log(const SourceLocation sourceLocation, const Level level)
{
    if (!fileSet && !disabled)
    {
        throw LoggerException("The log file has not been set");
    }

    std::string threadName;
    if (debugLessOrEqualLevel(level))
    {
        if (const auto& currentThreadName = GetCurrentThreadName(); !currentThreadName.empty())
        {
            threadName = currentThreadName + std::string(Delim);
        }
    }

    return Line(*this) << levelToString(level) << Delim << threadName << "["
                       << getTimeString() << "]" << Delim << sourceLocation << " - ";
}

void LoggerImpl::DumpLine(std::stringstream&& line)
{
    const std::lock_guard<std::mutex> lock(mutex);
    fileStream << line.view();
    ++lineCounter;

    if (lineCounter >= flushLinesLimit)
    {
        lineCounter = 0;
        fileStream.flush();
    }
}

}

void SetCurrentThreadName(const std::string &name)
{
    Detail::threadsNames[std::this_thread::get_id()] = name;
}

void SetLogFile(std::string filename)
{
    Detail::logger.SetFile(std::move(filename));
}

void Disable()
{
    Detail::logger.Disable();
}

}
