#include <catch2/catch.hpp>

#include <CppUtils/ProgramOptions.hpp>

#include <optional>

namespace
{
CppUtils::ProgramOptions buildProgramOptions(bool& flagParam, int64_t& intParam, double& doubleParam, std::string& stringParam)
{
    CppUtils::ProgramOptions programOptions;
    programOptions.addDescription("Test program");
    programOptions.addParameter<bool>(CppUtils::ProgramOptions::ParameterType::Flag, 'f', "flag", "flag description", false, &flagParam);
    programOptions.addParameter<int64_t>(CppUtils::ProgramOptions::ParameterType::Int64, 'i', "integer", "integer description", true, &intParam);
    programOptions.addParameter<double>(CppUtils::ProgramOptions::ParameterType::Double, 'd', "double", "double description", true, &doubleParam);
    programOptions.addParameter<std::string>(CppUtils::ProgramOptions::ParameterType::String, 's', "string", "string description", true, &stringParam);
    return programOptions;
}

void testScenario(const char* flagOpt, const char* intOpt, const int64_t intVal, const char* doubleOpt, const double doubleVal, const char* strOpt, const char* strVal)
{
    const std::string intValStr = std::to_string(intVal);
    const std::string doubleValStr = std::to_string(doubleVal);

    const char* argv[] =
    {
        "program",
        flagOpt,
        intOpt,
        intValStr.c_str(),
        doubleOpt,
        doubleValStr.c_str(),
        strOpt,
        strVal
    };
    constexpr int argc = sizeof(argv)/sizeof(argv[0]);

    bool flagParam = false;
    int64_t intParam = 0;
    double doubleParam = 0;
    std::string stringParam;

    auto programOptions = buildProgramOptions(flagParam, intParam, doubleParam, stringParam);
    programOptions.parse(argc, argv);

    REQUIRE(flagParam);
    REQUIRE(intVal == intParam);
    REQUIRE(doubleVal == doubleParam);
    REQUIRE(std::string(strVal) == stringParam);
}
}

TEST_CASE("Valid mandatory", "[ProgramOptions]")
{
    testScenario("-f", "-i", 123, "-d", 3.14, "-s", "abcd");
    testScenario("--flag", "-integer", -4165132, "--double", -1133.001, "--string", "@ACX#!12");
}

TEST_CASE("Valid optional", "[ProgramOptions]")
{
    const char* argv[] =
    {
        "program",
    };
    constexpr int argc = sizeof(argv)/sizeof(argv[0]);

    std::optional<bool> optFlagParam;
    std::optional<int64_t> optIntParam;
    std::optional<double> optDoubleParam;
    std::optional<std::string> optStringParam;

    CppUtils::ProgramOptions programOptions;
    programOptions.addDescription("Test program");
    programOptions.addOptionalParameter<bool>(CppUtils::ProgramOptions::ParameterType::Flag, 'f', "flag", "flag description", false, &optFlagParam);
    programOptions.addOptionalParameter<int64_t>(CppUtils::ProgramOptions::ParameterType::Int64, 'i', "integer", "integer description", true, &optIntParam);
    programOptions.addOptionalParameter<double>(CppUtils::ProgramOptions::ParameterType::Double, 'd', "double", "double description", true, &optDoubleParam);
    programOptions.addOptionalParameter<std::string>(CppUtils::ProgramOptions::ParameterType::String, 's', "string", "string description", true, &optStringParam);
    programOptions.parse(argc, argv);

    REQUIRE_FALSE(optFlagParam.has_value());
    REQUIRE_FALSE(optIntParam.has_value());
    REQUIRE_FALSE(optDoubleParam.has_value());
    REQUIRE_FALSE(optStringParam.has_value());
}

TEST_CASE("Missing mandatory", "[ProgramOptions]")
{
    const char* argv[] =
    {
        "program",
    };
    constexpr int argc = sizeof(argv)/sizeof(argv[0]);

    bool flagParam;

    CppUtils::ProgramOptions programOptions;
    programOptions.addDescription("Test program");
    programOptions.addParameter(CppUtils::ProgramOptions::ParameterType::Flag, 'f', "flag", "flag description", true, &flagParam);
    REQUIRE_THROWS_AS(programOptions.parse(argc, argv), CppUtils::MissingMandatoryParametersException);
}

TEST_CASE("ProgramOptions unknown parameter")
{
    const char* argv[] =
    {
        "program",
        "--abc"
    };
    constexpr int argc = sizeof(argv)/sizeof(argv[0]);

    CppUtils::ProgramOptions programOptions;
    programOptions.addDescription("Test program");
    REQUIRE_THROWS_AS(programOptions.parse(argc, argv), CppUtils::UnknownParameterException);
}

TEST_CASE("Missing value", "[ProgramOptions]")
{
    const char* argv[] =
    {
        "program",
        "--integer"
    };
    constexpr int argc = sizeof(argv)/sizeof(argv[0]);

    int64_t intParam;

    CppUtils::ProgramOptions programOptions;
    programOptions.addDescription("Test program");
    programOptions.addParameter(CppUtils::ProgramOptions::ParameterType::Int64, 'i', "integer", "integer description", true, &intParam);
    REQUIRE_THROWS_AS(programOptions.parse(argc, argv), CppUtils::MissingParameterValueException);
}

TEST_CASE("Wrong type", "[ProgramOptions]")
{
    std::string stringParam;
    double doubleParam;
    std::optional<int64_t> intParam;
    std::optional<bool> flagParam;

    CppUtils::ProgramOptions programOptions;
    programOptions.addDescription("Test program");

    REQUIRE_THROWS_AS(programOptions.addParameter(CppUtils::ProgramOptions::ParameterType::String, 's', "string", "string description", true, &doubleParam),
                      CppUtils::WrongParameterTypeException);
    REQUIRE_THROWS_AS(programOptions.addParameter(CppUtils::ProgramOptions::ParameterType::Double, 'd', "double", "double description", true, &stringParam),
                      CppUtils::WrongParameterTypeException);
    REQUIRE_THROWS_AS(programOptions.addParameter(CppUtils::ProgramOptions::ParameterType::Int64, 'i', "integer", "integer description", true, &stringParam),
                      CppUtils::WrongParameterTypeException);
    REQUIRE_THROWS_AS(programOptions.addParameter(CppUtils::ProgramOptions::ParameterType::Flag, 'f', "flag", "flag description", true, &stringParam),
                      CppUtils::WrongParameterTypeException);

    REQUIRE_THROWS_AS(programOptions.addOptionalParameter(CppUtils::ProgramOptions::ParameterType::String, 's', "string", "string description", true, &intParam),
                      CppUtils::WrongParameterTypeException);
    REQUIRE_THROWS_AS(programOptions.addOptionalParameter(CppUtils::ProgramOptions::ParameterType::Double, 'd', "double", "double description", true, &intParam),
                      CppUtils::WrongParameterTypeException);
    REQUIRE_THROWS_AS(programOptions.addOptionalParameter(CppUtils::ProgramOptions::ParameterType::Int64, 'i', "integer", "integer description", true, &flagParam),
                      CppUtils::WrongParameterTypeException);
    REQUIRE_THROWS_AS(programOptions.addOptionalParameter(CppUtils::ProgramOptions::ParameterType::Flag, 'f', "flag", "flag description", true, &intParam),
                      CppUtils::WrongParameterTypeException);

    REQUIRE_THROWS_AS(programOptions.addOptionalParameter(CppUtils::ProgramOptions::ParameterType::Flag, 'f', "flag", "flag description", true, &intParam),
                      CppUtils::WrongParameterTypeException);
}

TEST_CASE("Missing variable", "[ProgramOptions]")
{
    CppUtils::ProgramOptions programOptions;
    programOptions.addDescription("Test program");

    REQUIRE_THROWS_AS(programOptions.addOptionalParameter<double>(CppUtils::ProgramOptions::ParameterType::Double, 'd', "double", "double description", true),
                      CppUtils::MissingVariableException);
}
