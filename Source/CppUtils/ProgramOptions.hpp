#pragma once

#include <iostream>
#include <optional>
#include <vector>

#include "Exceptions.hpp"

namespace CppUtils
{

class UnknownParameterException : public Exception
{
public:
    explicit UnknownParameterException(const std::string& name);
};

class MissingParameterValueException : public Exception
{
public:
    explicit MissingParameterValueException(const std::string& name);
};

class MissingMandatoryParametersException : public Exception
{
public:
    explicit MissingMandatoryParametersException(const std::vector<std::string>& missing);
};

class WrongParameterTypeException : public Exception
{
public:
    explicit WrongParameterTypeException(const std::string& expected, const std::string& actual);
};

class MissingVariableException : public Exception
{
public:
    explicit MissingVariableException(const std::string& paramName);
};

class ProgramOptions
{
public:
    enum class ParameterType { String, Double, Int64, Flag };

    friend std::ostream& operator<<(std::ostream& os, const ProgramOptions& programOptions);
    ProgramOptions& addDescription(std::string&& _description);

    template <typename T>
    ProgramOptions& addParameter(const ParameterType type, const char shortName, std::string&& longName,
                                 std::string&& description, const bool hasValue, T* value = nullptr)
    {
        checkParam<T>(type, value, longName);
        parameters.emplace_back(type, shortName, std::move(longName), std::move(description), true, hasValue, value);
        return *this;
    }

    template <typename T>
    ProgramOptions& addOptionalParameter(const ParameterType type, const char shortName, std::string&& longName,
                                         std::string&& description, const bool hasValue, std::optional<T>* value = nullptr)
    {
        checkParam<T>(type, value, longName);
        parameters.emplace_back(type, shortName, std::move(longName), std::move(description), false, hasValue, value);
        return *this;
    }

    void parse(int argc, const char** argv);

private:

    template <typename T, typename P>
    static void checkParam(const ParameterType type, P *ptr, const std::string paramName)
    {
        if (type == ParameterType::String && !std::is_same_v<T, std::string>)
        {
            throw WrongParameterTypeException("Int64_t", typeid(T).name());
        }
        if (type == ParameterType::Double && !std::is_same_v<T, double>)
        {
            throw WrongParameterTypeException("Int64_t", typeid(T).name());
        }
        if (type == ParameterType::Int64 && !std::is_same_v<T, int64_t>)
        {
            throw WrongParameterTypeException("Int64_t", typeid(T).name());
        }
        if (type == ParameterType::Flag && !std::is_same_v<T, bool>)
        {
            throw WrongParameterTypeException("Int64_t", typeid(T).name());
        }

        if (type != ParameterType::Flag && ptr == nullptr)
        {
            throw MissingVariableException(paramName);
        }
    }

    struct Parameter
    {
        const ParameterType type;
        const char shortName;
        std::string longName;
        std::string description;
        const bool isMandatory{false};
        const bool hasValue{false};
        void* value{nullptr};
        bool populated{false};

        Parameter(const ParameterType _type, const char _shortName, std::string&& _longName, std::string&& _description,
                  const bool _isMandatory, const bool _hasValue, void* _value);

        void setValue(const std::string_view valueStr);
    };

    friend std::ostream& operator<<(std::ostream& os, const Parameter& parameter);

    Parameter* getParameter(const std::string_view name);
    void checkMandatoryPresent() const;

    std::string programName{};
    std::string description{};
    std::vector<Parameter> parameters{};
};
}
