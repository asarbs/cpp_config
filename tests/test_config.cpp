/*
 * World VTT / cpp_config – tests
 */

#include <gtest/gtest.h>

#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <cstdio>     // std::remove

#include "Config.h"
#include "ConfigParameter.h"
#include "ConfigChoice.h"
#include "ConfigExceptions.h"

using namespace cpp_config;

// =============================
//  Testowy enum dla Config<>
// =============================
enum class TestParam
{
    Port,
    Host,
    Difficulty,
};

// Skrót na konfigurację testową
using TestConfig = Config<TestParam>;

// -----------------------------------------
//  Definicja statycznego _confFileName
//  (inaczej będzie undefined symbol)
// -----------------------------------------
namespace cpp_config {
template<>
const std::string Config<TestParam>::_confFileName = "test_config.cfg";
}  // namespace cpp_config

// Pomocnicza funkcja – sprzątanie singletona między testami
static void ResetConfig()
{
    auto& cfg = TestConfig::instance();
    cfg.clear();
}

// ===================================================
//  TESTY: ConfigParameter
// ===================================================

TEST(ConfigParameterTest, DefaultConstructor)
{
    ConfigParameter p;

    EXPECT_EQ(p.name(), "name");
    EXPECT_EQ(p.description(), "");
    EXPECT_EQ(p.value(), "");
}

TEST(ConfigParameterTest, CustomConstructorStoresFields)
{
    ConfigParameter p("port", "Port number", "8080");

    EXPECT_EQ(p.name(), "port");
    EXPECT_EQ(p.description(), "Port number");
    EXPECT_EQ(p.value(), "8080");
}

TEST(ConfigParameterTest, NameCannotContainSpaces)
{
    // W konstruktorze jest sprawdzenie i std::invalid_argument
    EXPECT_THROW(
        ConfigParameter("bad name", "desc", "val"),
        std::invalid_argument
    );
}

TEST(ConfigParameterTest, CopyAndMoveSemantics)
{
    ConfigParameter p1("host", "Host name", "localhost");

    ConfigParameter p2(p1);  // copy ctor
    EXPECT_EQ(p2.name(), "host");
    EXPECT_EQ(p2.description(), "Host name");
    EXPECT_EQ(p2.value(), "localhost");

    ConfigParameter p3(std::move(p1));  // move ctor
    EXPECT_EQ(p3.name(), "host");
    EXPECT_EQ(p3.description(), "Host name");
    EXPECT_EQ(p3.value(), "localhost");

    ConfigParameter p4;
    p4 = p2;  // copy assign
    EXPECT_EQ(p4.name(), "host");

    ConfigParameter p5;
    p5 = std::move(p2);  // move assign
    EXPECT_EQ(p5.name(), "host");
}

TEST(ConfigParameterTest, SetChangesValue)
{
    ConfigParameter p("key", "desc", "old");
    p.set("new");
    EXPECT_EQ(p.value(), "new");
}

TEST(ConfigParameterTest, AsString)
{
    ConfigParameter p("k", "d", "hello");

    EXPECT_EQ(p.as<std::string>(), "hello");
}

TEST(ConfigParameterTest, AsBoolTrueFalse)
{
    ConfigParameter pTrue("t", "d", "true");
    ConfigParameter pFalse("f", "d", "false");

    EXPECT_TRUE(pTrue.as<bool>());
    EXPECT_FALSE(pFalse.as<bool>());
}

TEST(ConfigParameterTest, AsIntegral)
{
    ConfigParameter p("num", "d", "42");

    EXPECT_EQ(p.as<int>(), 42);
    EXPECT_EQ(p.as<long long>(), 42);
}

TEST(ConfigParameterTest, AsFloatingPoint)
{
    ConfigParameter p("pi", "d", "3.14");

    EXPECT_DOUBLE_EQ(p.as<double>(), 3.14);
    EXPECT_FLOAT_EQ(p.as<float>(), 3.14f);
}

TEST(ConfigParameterTest, AsIntegralInvalidThrows)
{
    ConfigParameter p("bad", "d", "abc");

    // std::stoll rzuca std::invalid_argument / std::out_of_range
    EXPECT_THROW(p.as<int>(), std::exception);
}

TEST(ConfigParameterTest, AsUnsupportedTypeThrows)
{
    ConfigParameter p("x", "d", "10");

    // Nieobsługiwany typ w as<T>() -> std::invalid_argument
    EXPECT_THROW(p.as<std::vector<int>>(), std::invalid_argument);
}

// ===================================================
//  TESTY: ConfigChoice
// ===================================================

TEST(ConfigChoiceTest, ConstructorAndDescription)
{
    std::vector<std::string> choices{"easy", "medium", "hard"};
    ConfigChoice c("difficulty", "Game difficulty", "easy", choices);

    EXPECT_EQ(c.name(), "difficulty");
    EXPECT_EQ(c.value(), "easy");

    // description() = bazowy opis + "; /option1/option2/..."
    std::string desc = c.description();
    EXPECT_NE(desc.find("Game difficulty"), std::string::npos);
    EXPECT_NE(desc.find("/easy/"), std::string::npos);
    EXPECT_NE(desc.find("/medium/"), std::string::npos);
    EXPECT_NE(desc.find("/hard/"), std::string::npos);
}

TEST(ConfigChoiceTest, SetAcceptsAllowedValue)
{
    std::vector<std::string> choices{"red", "green", "blue"};
    ConfigChoice c("color", "Color", "red", choices);

    EXPECT_NO_THROW(c.set("green"));
    EXPECT_EQ(c.value(), "green");
}

TEST(ConfigChoiceTest, SetRejectsNotAllowedValue)
{
    std::vector<std::string> choices{"red", "green", "blue"};
    ConfigChoice c("color", "Color", "red", choices);

    EXPECT_THROW(c.set("yellow"), ConfigurationError);
    EXPECT_EQ(c.value(), "red");  // wartość powinna pozostać bez zmian
}

// ===================================================
//  TESTY: Config<Enum> – logika rejestracji i odczytu
// ===================================================

TEST(ConfigTest, AddParamAndGetValue)
{
    ResetConfig();
    auto& cfg = TestConfig::instance();

    cfg.addParam(
        TestParam::Port,
        std::make_shared<ConfigParameter>("port", "TCP port", "8080")
    );

    // value<T>() korzysta z ConfigParameter::as<T>()
    int port = cfg.value<int>(TestParam::Port);
    EXPECT_EQ(port, 8080);

    // drugi raz ten sam klucz powinien rzucić ParameterAlreadyRegistered
    EXPECT_THROW(
        cfg.addParam(
            TestParam::Port,
            std::make_shared<ConfigParameter>("port", "TCP port", "9000")
        ),
        TestConfig::ParameterAlreadyRegistered
    );
}

TEST(ConfigTest, ValueThrowsForNotRegistered)
{
    ResetConfig();
    auto& cfg = TestConfig::instance();

    // get/at na niezarejestrowanym kluczu -> std::out_of_range
    EXPECT_THROW(
        cfg.value<int>(TestParam::Host),
        std::out_of_range
    );
}

// ===================================================
//  TESTY: Config<Enum> – loadFromFile()
// ===================================================

TEST(ConfigTest, LoadFromFileAssignsValuesFromFile)
{
    ResetConfig();
    auto& cfg = TestConfig::instance();

    // Rejestrujemy parametry z nazwami, które będą w pliku
    cfg.addParam(
        TestParam::Port,
        std::make_shared<ConfigParameter>("port", "TCP port", "0")
    );
    cfg.addParam(
        TestParam::Host,
        std::make_shared<ConfigParameter>("host", "Server host", "")
    );
    cfg.addParam(
        TestParam::Difficulty,
        std::make_shared<ConfigChoice>(
            "difficulty",
            "Game difficulty",
            "easy",
            std::vector<std::string>{"easy", "medium", "hard"}
        )
    );

    // Tworzymy plik konfiguracyjny, który będzie czytany w loadFromFile()
    {
        std::ofstream out("test_config.cfg");
        out << "# comment line should be ignored\n";
        out << "port=9000\n";
        out << "host=localhost\n";
        out << "\n";  // pusta linia
        out << "difficulty=hard\n";
    }

    // Wczytanie z pliku
    cfg.loadFromFile();

    EXPECT_EQ(cfg.value<int>(TestParam::Port), 9000);
    EXPECT_EQ(cfg.value<std::string>(TestParam::Host), "localhost");
    EXPECT_EQ(cfg.value<std::string>(TestParam::Difficulty), "hard");

    // Sprzątanie pliku
    std::remove("test_config.cfg");
}

TEST(ConfigTest, LoadFromFileIgnoresUnknownKeysAndBadLines)
{
    ResetConfig();
    auto& cfg = TestConfig::instance();

    cfg.addParam(
        TestParam::Port,
        std::make_shared<ConfigParameter>("port", "TCP port", "0")
    );

    {
        std::ofstream out("test_config.cfg");
        out << "unknown=123\n";     // brak odpowiadającego parametru – ma być zignorowany
        out << "port=1234\n";
        out << "bad_line_without_equal_sign\n"; // splitParam -> empty first -> continue
    }

    cfg.loadFromFile();

    EXPECT_EQ(cfg.value<int>(TestParam::Port), 1234);

    std::remove("test_config.cfg");
}

TEST(ConfigTest, SaveToFileCreatesFileIfNotExists)
{
    // saveToFile() w Config po prostu otwiera i zamyka plik _confFileName
    ResetConfig();
    auto& cfg = TestConfig::instance();

    // Na wszelki wypadek usuń jeśli istnieje
    std::remove("test_config.cfg");

    cfg.saveToFile();

    std::ifstream in("test_config.cfg");
    EXPECT_TRUE(in.is_open());
    in.close();

    std::remove("test_config.cfg");
}
