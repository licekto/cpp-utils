#include "ProgramOptions.hpp"

#include <algorithm>
#include <optional>

#include <CppUtils/StringUtils.hpp>

namespace CppUtils
{

namespace
{
std::string_view stripDashes(const std::string_view param)
{
    const auto nonDashPos = param.find_first_not_of("-");
    return param.substr(nonDashPos, param.size() - nonDashPos);
}

template <typename T>
void setTargetValue(void* target, const T& value, const bool isMandatory)
{
    if (isMandatory)
    {
        *static_cast<T*>(target) = value;
    }
    else
    {
        *static_cast<std::optional<T>*>(target) = value;
    }
}
}

UnknownParameterException::UnknownParameterException(const std::string& name)
    : ProgramOptionsException("Parameter '" + name + "' is unknown")
{}

MissingParameterValueException::MissingParameterValueException(const std::string& name)
    : ProgramOptionsException("Missing value for the '" + name + "' parameter")
{}

MissingMandatoryParametersException::MissingMandatoryParametersException(const std::vector<std::string>& missing)
    : ProgramOptionsException("Missing mandatory '" + Combinator(",").Combine(missing) + "' parameter")
{}

WrongParameterTypeException::WrongParameterTypeException(const std::string& expected, const std::string& actual)
    : ProgramOptionsException("Wrong parameter type, expected: '" + expected + "', actual: " + actual)
{}

MissingVariableException::MissingVariableException(const std::string& paramName)
    : ProgramOptionsException("Missing variable for the '" + paramName + "' parameter")
{}

std::ostream& operator<<(std::ostream& os, const ProgramOptions& programOptions)
{
    os << programOptions.programName << " - " << programOptions.description << "\nOptions:\n";
    for (const auto& parameter : programOptions.parameters)
    {
        os << parameter << "\n";
    }
    return os;
}

ProgramOptions& ProgramOptions::addDescription(std::string&& _description)
{
    description = _description;
    return *this;
}

void ProgramOptions::parse(int argc, const char** argv)
{
    programName = argv[0];

    const size_t argCount = argc;
    for (size_t i = 1; i < argCount; ++i)
    {
        auto* param = getParameter(stripDashes(argv[i]));
        if (param == nullptr)
        {
            throw UnknownParameterException(argv[i]);
        }

        if (param->hasValue)
        {
            ++i;
            if (i >= argCount)
            {
                throw MissingParameterValueException(param->longName);
            }
        }
        param->setValue(argv[i]);
        param->populated = true;
    }

    checkMandatoryPresent();
}

ProgramOptions::Parameter::Parameter(const ParameterType _type, const char _shortName, std::string&& _longName, std::string&& _description,
                                     const bool _isMandatory, const bool _hasValue, void* _value)
    : type(_type)
    , shortName(_shortName)
    , longName(std::move(_longName))
    , description(std::move(_description))
    , isMandatory(_isMandatory)
    , hasValue(_hasValue)
    , value(_value)
{}

void ProgramOptions::Parameter::setValue(const std::string_view valueStr)
{
    switch (type)
    {
        case ParameterType::Double :
        {
            setTargetValue(value, atof(valueStr.data()), isMandatory);
            break;
        }
        case ParameterType::Flag :
        {
            setTargetValue(value, true, isMandatory);
            break;
        }
        case ParameterType::Int64 :
        {
            setTargetValue(value, atol(valueStr.data()), isMandatory);
            break;
        }
        case ParameterType::String :
        {
            setTargetValue(value, std::string(valueStr), isMandatory);
            break;
        }
    }
}

std::ostream& operator<<(std::ostream& os, const ProgramOptions::Parameter& parameter)
{
    return os << "\t-" << parameter.shortName << ", --" << parameter.longName
              << (parameter.isMandatory ? " (mandatory)" : "") << "\n"
              << "\t\t" << parameter.description;
}

ProgramOptions::Parameter* ProgramOptions::getParameter(const std::string_view name)
{
    const auto it = std::find_if(std::begin(parameters), std::end(parameters),
                                 [&name](const auto& param){ return param.longName == name || param.shortName == name[0]; });

    return it == std::end(parameters) ? nullptr : &*it;
}

void ProgramOptions::checkMandatoryPresent() const
{
    std::vector<std::string> missing;
    for (const auto& parameter : parameters)
    {
        if (parameter.isMandatory && !parameter.populated)
        {
            missing.push_back(parameter.longName);
        }
    }

    if (!missing.empty())
    {
        throw MissingMandatoryParametersException(missing);
    }
}
}
