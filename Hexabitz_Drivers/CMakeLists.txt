# Define a Zephyr library for the project
zephyr_library()

# Add source files to the Zephyr library
zephyr_library_sources(
  UART/UART.c
  ADC/ADC.c
  Timer/Timer.c
)

# Specify include directories for the project
zephyr_include_directories(
    UART
    ADC
    Timer
    )

# Specify include directories for the library
zephyr_library_include_directories(
    UART
    ADC
    Timer
    )