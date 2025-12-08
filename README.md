# cpp_config

A lightweight and strongly‑typed C++ configuration library designed for
simple `.cfg` / `.ini` style configuration files.\
It provides a clean API, type‑safe access, parameter validation, file
loading, and meaningful error handling.

------------------------------------------------------------------------

## ✨ Features

-   Strongly typed configuration keys (via enum)
-   Default values for all parameters
-   Automatic type conversion (`int`, `bool`, `float`, `double`,
    `std::string`)
-   Validated choice parameters (`ConfigChoice`)
-   Load from `.cfg` file
-   Save initial config file if missing
-   Clear exception types
-   Zero external dependencies (except optional GTest for tests)

------------------------------------------------------------------------

## 📁 Project Structure

    ├── include/
    │   ├── Config.h
    │   ├── ConfigChoice.h
    │   ├── ConfigParameter.h
    │   ├── ConfigExceptions.h
    │
    ├── src/
    │   ├── Config.cpp
    │   ├── ConfigChoice.cpp
    │   ├── ConfigParameter.cpp
    │
    ├── tests/
    │   ├── CMakeLists.txt
    │   └── test_config.cpp
    │
    ├── CMakeLists.txt
    └── README.md

------------------------------------------------------------------------

## 🚀 Getting Started

### 1. Add to your CMake project

``` cmake
add_subdirectory(cpp_config)
target_link_libraries(MyApp PRIVATE cpp_config)
```

------------------------------------------------------------------------

## 🧩 Basic Usage

### 1. Define a strongly‑typed parameter enum

``` cpp
enum class MyParams {
    Port,
    Host,
    Difficulty
};
```

### 2. Declare configuration type

``` cpp
using MyConfig = cpp_config::Config<MyParams>;
```

### 3. Provide file name specialization

``` cpp
namespace cpp_config {
template<>
const std::string Config<MyParams>::_confFileName = "my_config.cfg";
}
```

### 4. Register parameters

``` cpp
auto& cfg = MyConfig::instance();

cfg.addParam(
    MyParams::Port,
    std::make_shared<cpp_config::ConfigParameter>(
        "port", "TCP server port", "8080")
);

cfg.addParam(
    MyParams::Host,
    std::make_shared<cpp_config::ConfigParameter>(
        "host", "Server hostname", "localhost")
);

cfg.addParam(
    MyParams::Difficulty,
    std::make_shared<cpp_config::ConfigChoice>(
        "difficulty",
        "Game difficulty",
        "easy",
        std::vector<std::string>{"easy","medium","hard"})
);
```

### 5. Load configuration from file

``` cpp
cfg.loadFromFile();
```

Example config file:

    # Sample configuration
    port=9000
    host=192.168.1.10
    difficulty=hard

### 6. Read values

``` cpp
int port = cfg.value<int>(MyParams::Port);
std::string host = cfg.value<std::string>(MyParams::Host);
```

------------------------------------------------------------------------

## 🔧 Supported Types

`ConfigParameter::as<T>()` supports:

  Type                 Supported
  -------------------- ------------------------------------
  `std::string`        ✔
  `bool`               ✔ (`true/false/1/0`)
  `int`, `long long`   ✔
  `float`, `double`    ✔
  custom types         ✖ (throws `std::invalid_argument`)

------------------------------------------------------------------------

## 🎚 Using ConfigChoice

``` cpp
ConfigChoice mode("mode", "Operating mode", "auto",
                  {"auto", "manual", "off"});
```

Usage:

``` cpp
mode.set("manual");   // OK
mode.set("invalid");  // throws ConfigurationError
```

------------------------------------------------------------------------

## 📄 Configuration File Format

-   `key=value` pairs
-   Unknown keys are ignored
-   Invalid lines are ignored
-   Empty lines and comments (`#`) are ignored
-   On missing file → default file is created automatically

------------------------------------------------------------------------

## 🧪 Unit Tests (GoogleTest)

Tests are located in `/tests`.

Run:

``` bash
cd build
ctest
```

or directly:

``` bash
./test_config
```

The test suite covers:

-   ConfigParameter:
    -   type conversion
    -   invalid conversions
    -   move/copy semantics
-   ConfigChoice:
    -   value validation
    -   formatting of choice list
-   Config`<Enum>`{=html}:
    -   parameter registration
    -   exception on duplicates
    -   loading from file
    -   ignoring invalid lines
    -   writing config file

------------------------------------------------------------------------

## 🧱 Exceptions

  Exception                      When Raised
  ------------------------------ ----------------------------------------
  `ParameterAlreadyRegistered`   Duplicate key in `addParam()`
  `ConfigurationError`           Invalid value in `ConfigChoice::set()`
  `std::invalid_argument`        Unsupported type in `as<T>()`
  `std::out_of_range`            Missing key when calling `value<T>()`

------------------------------------------------------------------------

## 🧰 Singleton Notes

`Config<Enum>` is a singleton:

``` cpp
auto& cfg = MyConfig::instance();
```

Reset for tests:

``` cpp
MyConfig::instance().clear();
```

------------------------------------------------------------------------

## 🎯 Why cpp_config?

-   zero dependencies\
-   strongly typed configuration\
-   safe conversions\
-   easy to extend\
-   works in embedded and large projects\
-   clean and minimalistic API

------------------------------------------------------------------------

## 🤝 Contributions

Pull requests are welcome!\
If you'd like new features (TOML, INI parser, schema validation), open
an issue.

------------------------------------------------------------------------
