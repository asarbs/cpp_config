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

#ifndef CONFIG_H
#define CONFIG_H

#pragma once

#include <exception>
#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "ConfigChoice.h"
#include "ConfigParameter.h"

namespace cpp_config {
    template <class ParamsDict>
    class Config {
        public:
            class ParameterAlreadyRegistered : std::exception {};
            class ParameterNotRegistered : std::exception {};
            class ConfigurationFileError : std::exception {};

            static Config<ParamsDict> &instance() {  // cppcheck-suppress unusedFunction
                static Config<ParamsDict> instance;
                return instance;
            }
            void addParam(const ParamsDict &key,
                          std::shared_ptr<ConfigParameter> param) {  // cppcheck-suppress unusedFunction
                if (_paramCollection.find(key) != _paramCollection.end()) {
                    throw ParameterAlreadyRegistered();
                }

                _paramCollection[key]        = param;
                _params2enums[param->name()] = key;
            }

            const std::shared_ptr<ConfigParameter> get(const ParamsDict &key) const {
                return _paramCollection.at(key);
            }

            template <typename T>
            T value(const ParamsDict &key) const {
                return get(key)->template as<T>();
            }

            void saveToFile() {  // cppcheck-suppress unusedFunction
                std::ofstream out(_confFileName);
                out.close();
            }

            void loadFromFile() {
                std::ifstream in(_confFileName);
                if (!in.is_open()) {
                    saveToFile();
                    // throw ConfigurationFileError();
                }
                std::string line;
                while (std::getline(in, line)) {
                    if (line.empty() || line[0] == '#') {
                        continue;
                    }
                    std::pair<std::string, std::string> parameter = splitParam(line, '=');

                    if (parameter.first.empty()) {
                        continue;
                    }

                    auto it = _params2enums.find(parameter.first);
                    if (it == _params2enums.end()) {
                        continue;
                    }
                    ParamsDict key = _params2enums[parameter.first];
                    _paramCollection.at(key)->set(parameter.second);
                }
                in.close();
            }

            void clear() {
                _paramCollection.clear();
            }

        protected:
            //
        private:
            Config() {
            }
            ~Config() {
            }
            std::map<ParamsDict, std::shared_ptr<ConfigParameter>> _paramCollection;
            std::map<std::string, ParamsDict> _params2enums;
            static const std::string _confFileName;

            std::pair<std::string, std::string> splitParam(const std::string &in, char delimiter) {
                size_t delimiterPos = in.find_first_of(delimiter);
                return {in.substr(0, delimiterPos), in.substr(delimiterPos + 1)};
            }
    };

};  // namespace cpp_config
#endif
