# Define a Zephyr library for the project
zephyr_library()

# Add source files to the Zephyr library
zephyr_library_sources(
  src/BOS.c
)

# Specify include directories for the project
zephyr_include_directories(include)

# Specify include directories for the library
zephyr_library_include_directories(include)

# Include directories for system call headers
# - Prepares for potential future system calls by including the 'include' directory
zephyr_syscall_include_directories(include)


# if(CONFIG_BOARD_H01R0)
#   add_subdirectory(
#     ${ZEPHYR_CURRENT_MODULE_DIR}/Hexabitz_Modules/H01R0
#     ${CMAKE_CURRENT_BINARY_DIR}/Hexabitz_Modules/H01R0
#   )
# elseif(CONFIG_BOARD_H0BR4)
#   add_subdirectory(
#     ${ZEPHYR_CURRENT_MODULE_DIR}/Hexabitz_Modules/H0BR4
#     ${CMAKE_CURRENT_BINARY_DIR}/Hexabitz_Modules/H0BR4
#   )
# endif()


