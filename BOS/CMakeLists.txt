# Define a Zephyr library for the project
zephyr_library()

# Add source files to the Zephyr library
zephyr_library_sources(
  src/BOS.c
  src/BOS_RTOS.c
  src/BOS_Messaging.c
  src/BOS_MsgParser.c
  src/BOS_Shell.c
)

# Specify include directories for the project
zephyr_include_directories(include)

# Specify include directories for the library
zephyr_library_include_directories(include)

# Include directories for system call headers
# - Prepares for potential future system calls by including the 'include' directory
zephyr_syscall_include_directories(include)
