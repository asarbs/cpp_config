# World VTT
#
# Copyright (C) 2025, Asar Miniatures
# All rights reserved.
#
# This file is part of the [Project Name] project. It may be used, modified,
# and distributed under the terms specified by the copyright holder.


enable_testing()

add_executable(test_config
    test_config.cpp
)

target_link_libraries(test_config
    PRIVATE
        cpp_config
        gtest
        gtest_main
)

add_test(NAME TestConfig COMMAND test_config)