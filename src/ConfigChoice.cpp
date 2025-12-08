/*
 * World VTT
 *
 * Copyright (C) 2025, Asar Miniatures
 * All rights reserved.
 *
 * This file is part of the [Project Name] project. It may be used, modified,
 * and distributed under the terms specified by the copyright holder.
 *
 */

#include "ConfigChoice.h"

#include <algorithm>
#include <utility>

#include "ConfigExceptions.h"

namespace cpp_config {
    ConfigChoice::ConfigChoice() : ConfigParameter() {
    }
    ConfigChoice::ConfigChoice(const std::string &name, const std::string &description, const std::string &value, const std::vector<std::string> &choice)
        : ConfigParameter(name, description, value), _choice(choice) {
    }
    ConfigChoice::~ConfigChoice() {
    }
    ConfigChoice::ConfigChoice(const ConfigChoice &rhs) : ConfigParameter(rhs), _choice(rhs._choice) {
    }
    ConfigChoice::ConfigChoice(const ConfigChoice &&rhs) : ConfigParameter(std::move(rhs)), _choice(std::move(rhs._choice)) {
    }
    ConfigChoice &ConfigChoice::operator=(const ConfigChoice &rhs) {
        if (this != &rhs) {
            ConfigParameter::operator=(rhs);
            _choice = rhs._choice;
        }
        return *this;
    }
    ConfigChoice &ConfigChoice::operator=(const ConfigChoice &&rhs) {
        if (this != &rhs) {
            ConfigParameter::operator=(std::move(rhs));
            _choice = std::move(rhs._choice);
        }
        return *this;
    }
    void ConfigChoice::set(const std::string &val) {
        auto f = std::find(_choice.begin(), _choice.end(), val);
        if (f == _choice.end()) {
            std::stringstream ss;
            ss << "Value `" << val << "` is not on allowed list";

            throw cpp_config::ConfigurationError(ss.str());
        }
        ConfigParameter::set(val);
    }

    const std::string ConfigChoice::description() const {
        std::stringstream ss;
        ss << ConfigParameter::description() << "; /";
        for (const auto &c : _choice) {
            ss << c << "/";
        }
        return ss.str();
    }
}  // namespace cpp_config
