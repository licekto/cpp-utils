#pragma once

#include <iostream>
#include <vector>

namespace CppUtils
{
class ProgramOptions
{
public:
    enum class ParameterType { String, Double, Int64, Flag };

    class UnknownParameterException : public std::exception
    {
    public:
        explicit UnknownParameterException(const std::string& name);
        const char* what() const noexcept override;

    private:
        std::string message;
    };

    class MissingParameterValueException : public std::exception
    {
    public:
        explicit MissingParameterValueException(const std::string& name);
        const char* what() const noexcept override;

    private:
        std::string message;
    };

    class MissingMandatoryParametersException : public std::exception
    {
    public:
        explicit MissingMandatoryParametersException(const std::vector<std::string>& missing);
        const char* what() const noexcept override;

    private:
        std::string message;
    };

    friend std::ostream& operator<<(std::ostream& os, const ProgramOptions& programOptions);
    ProgramOptions& addDescription(std::string&& _description);
    ProgramOptions& addParameter(const ParameterType type, const char shortName, std::string&& longName, std::string&& description,
                                 const bool isMandatory, const bool hasValue, void* value = nullptr);
    void parse(int argc, const char** argv);

private:
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
