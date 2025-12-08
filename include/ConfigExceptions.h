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

#ifndef CONFIGEXCEPTIONS_H
#define CONFIGEXCEPTIONS_H

#pragma once

#include <stdexcept>
#include <string>

namespace cpp_config {

    class ConfigurationError : public std::runtime_error {
        public:
            explicit ConfigurationError(const std::string &what_arg) : std::runtime_error(what_arg) {
            }
            explicit ConfigurationError(const char *what_arg) : std::runtime_error(what_arg) {
            }
    };

};  // namespace cpp_config

#endif
