# Define a Zephyr library for the project
zephyr_library()

# Add source files to the Zephyr library
zephyr_library_sources(
  H01R0/src/H01R0.c
  H0BR4/src/H0BR4.c
)

# Specify include directories for the project
zephyr_include_directories(
    H01R0/include
    H0BR4/include
    )

# Specify include directories for the library
zephyr_library_include_directories(
    H01R0/include
    H0BR4/include
    )