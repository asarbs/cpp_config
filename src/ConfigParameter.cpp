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

#include "ConfigParameter.h"

namespace cpp_config {

    ConfigParameter::ConfigParameter() : _name("name"), _description(""), _value("") {
    }

    ConfigParameter::ConfigParameter(const std::string &name, const std::string &description, const std::string &value) : _name(name), _description(description), _value(value) {
        if (_name.find(' ') != std::string::npos) {
            throw std::invalid_argument("ConfigParameter name cannot contain spaces");
        }
    }
    ConfigParameter::~ConfigParameter() {
    }

    ConfigParameter::ConfigParameter(const ConfigParameter &rhs) : _name(rhs._name), _description(rhs._description), _value(rhs._value) {
    }

    ConfigParameter::ConfigParameter(const ConfigParameter &&rhs) : _name(std::move(rhs._name)), _description(std::move(rhs._description)), _value(std::move(rhs._value)) {
    }

    ConfigParameter &ConfigParameter::operator=(const ConfigParameter &rhs) {
        if (this != &rhs) {
            _name        = rhs._name;
            _description = rhs._description;
            _value       = rhs._value;
        }
        return *this;
    }

    ConfigParameter &ConfigParameter::operator=(const ConfigParameter &&rhs) {
        if (this != &rhs) {
            _name        = std::move(rhs._name);
            _description = std::move(rhs._description);
            _value       = std::move(rhs._value);
        }
        return *this;
    }

    const std::string ConfigParameter::name() const {
        return _name;
    }
    const std::string ConfigParameter::description() const {
        return _description;
    }
    const std::string ConfigParameter::value() const {
        return _value;
    }

    void ConfigParameter::set(const std::string &val) {
        _value = val;
    }

}  // namespace cpp_config
