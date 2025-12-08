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

#ifndef CONFIGCHOICE_H
#define CONFIGCHOICE_H

#pragma once

#include <string>
#include <vector>

#include "ConfigParameter.h"

namespace cpp_config {
    class ConfigChoice : public ConfigParameter {
        public:
            ConfigChoice();
            ConfigChoice(const std::string &name, const std::string &description, const std::string &value, const std::vector<std::string> &choice);
            ~ConfigChoice() override;
            ConfigChoice(const ConfigChoice &rhs);
            ConfigChoice(const ConfigChoice &&rhs);
            ConfigChoice &operator=(const ConfigChoice &rhs);
            ConfigChoice &operator=(const ConfigChoice &&rhs);
            void set(const std::string &val) override;
            const std::string description() const override;

        protected:
            //
        private:
            std::vector<std::string> _choice;
    };
}  // namespace cpp_config

#endif
