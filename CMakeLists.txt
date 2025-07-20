# Copyright (c) 2021 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0
#
# This CMake file is picked by the Zephyr build system because it is defined
# as the module CMake entry point (see zephyr/module.yml).


# This is needed so that custom driver classes using system calls are taken into
# account
# zephyr_syscall_include_directories(include)

# zephyr_include_directories(include)

add_subdirectory(BOS)
add_subdirectory(Hexabitz_Modules)
add_subdirectory(Hexabitz_Drivers)

# Add custom DTS bindings path
# list(APPEND DT_EXTRA_BINDINGS_DIR
#     ${CMAKE_CURRENT_SOURCE_DIR}/dts/bindings/hexabitz
# )

# zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dts/bindings)