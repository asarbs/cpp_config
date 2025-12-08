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

#ifndef CONFIGPARAMETER_H
#define CONFIGPARAMETER_H

#pragma once

#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace cpp_config {

    class ConfigParameter {
        public:
            ConfigParameter();
            ConfigParameter(const std::string &name, const std::string &description, const std::string &value);
            virtual ~ConfigParameter();
            ConfigParameter(const ConfigParameter &rhs);
            ConfigParameter(const ConfigParameter &&rhs);
            ConfigParameter &operator=(const ConfigParameter &rhs);
            ConfigParameter &operator=(const ConfigParameter &&rhs);

            const std::string name() const;  // cppcheck-suppress returnByReference
            virtual const std::string description() const;
            const std::string value() const;  // cppcheck-suppress returnByReference
            virtual void set(const std::string &val);
            template <typename T>
            T as() const {
                if constexpr (std::is_same_v<T, std::string>) {
                    return _value;
                } else if constexpr (std::is_same_v<T, bool>) {
                    return "true" == _value;
                } else if constexpr (std::is_integral_v<T>) {
                    return static_cast<T>(std::stoll(_value));
                } else if constexpr (std::is_floating_point_v<T>) {
                    return static_cast<T>(std::stod(_value));
                } else {
                    throw std::invalid_argument("Unsupported type.");
                }
            }

        protected:
            //
        private:
            std::string _name;
            std::string _description;
            std::string _value;
    };
}  // namespace cpp_config
#endif
