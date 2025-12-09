# World VTT
#
# Copyright (C) 2025, Asar Miniatures
# All rights reserved.
#
# This file is part of the [Project Name] project. It may be used, modified,
# and distributed under the terms specified by the copyright holder.


set(TARGET_NAME cpp_config)

add_library(${TARGET_NAME} STATIC
    Config.cpp
    ConfigChoice.cpp
    ConfigParameter.cpp
)

target_include_directories(${TARGET_NAME} PUBLIC
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)


target_link_libraries(${TARGET_NAME}
    PUBLIC
        Boost::system
)