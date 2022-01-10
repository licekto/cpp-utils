#include <catch2/catch.hpp>

#include <CppUtils/ProgramOptions.hpp>

#include <optional>

namespace
{
CppUtils::ProgramOptions buildProgramOptions(bool& flagParam, int64_t& intParam, double& doubleParam, std::string& stringParam)
{
    CppUtils::ProgramOptions programOptions;
    programOptions.addDescription("Test program");
    programOptions.addParameter(CppUtils::ProgramOptions::ParameterT::Flag, 'f', "flag", "flag description", true, false, &flagParam);
    programOptions.addParameter(CppUtils::ProgramOptions::ParameterT::Int64, 'i', "integer", "integer description", true, true, &intParam);
    programOptions.addParameter(CppUtils::ProgramOptions::ParameterT::Double, 'd', "double", "double description", true, true, &doubleParam);
    programOptions.addParameter(CppUtils::ProgramOptions::ParameterT::String, 's', "string", "string description", true, true, &stringParam);
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

TEST_CASE("ProgramOptions valid mandatory")
{
    testScenario("-f", "-i", 123, "-d", 3.14, "-s", "abcd");
    testScenario("--flag", "-integer", -4165132, "--double", -1133.001, "--string", "@ACX#!12");
}

TEST_CASE("ProgramOptions valid optional")
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
    programOptions.addParameter(CppUtils::ProgramOptions::ParameterT::Flag, 'f', "flag", "flag description", false, false, &optFlagParam);
    programOptions.addParameter(CppUtils::ProgramOptions::ParameterT::Int64, 'i', "integer", "integer description", false, true, &optIntParam);
    programOptions.addParameter(CppUtils::ProgramOptions::ParameterT::Double, 'd', "double", "double description", false, true, &optDoubleParam);
    programOptions.addParameter(CppUtils::ProgramOptions::ParameterT::String, 's', "string", "string description", false, true, &optStringParam);
    programOptions.parse(argc, argv);

    REQUIRE_FALSE(optFlagParam.has_value());
    REQUIRE_FALSE(optIntParam.has_value());
    REQUIRE_FALSE(optDoubleParam.has_value());
    REQUIRE_FALSE(optStringParam.has_value());
}

TEST_CASE("ProgramOptions missing mandatory")
{
    const char* argv[] =
    {
        "program",
    };
    constexpr int argc = sizeof(argv)/sizeof(argv[0]);

    bool flagParam;

    CppUtils::ProgramOptions programOptions;
    programOptions.addDescription("Test program");
    programOptions.addParameter(CppUtils::ProgramOptions::ParameterT::Flag, 'f', "flag", "flag description", true, true, &flagParam);
    REQUIRE_THROWS_AS(programOptions.parse(argc, argv), CppUtils::ProgramOptions::MissingMandatoryParametersException);
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
    REQUIRE_THROWS_AS(programOptions.parse(argc, argv), CppUtils::ProgramOptions::UnknownParameterException);
}

TEST_CASE("ProgramOptions missing value")
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
    programOptions.addParameter(CppUtils::ProgramOptions::ParameterT::Int64, 'i', "integer", "integer description", true, true, &intParam);
    REQUIRE_THROWS_AS(programOptions.parse(argc, argv), CppUtils::ProgramOptions::MissingParameterValueException);
}
