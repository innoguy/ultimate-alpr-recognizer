cmake_minimum_required(VERSION 3.10)
project(RecognizerModular)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Include ultimateALPR SDK headers
include_directories(${CMAKE_SOURCE_DIR})
include_directories(./ultimateALPR-SDK/c++)  # SDK header files

# Include OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Include nlohmann/json
find_package(nlohmann_json REQUIRED)
include_directories(${nlohmann_json_INCLUDE_DIRS})

# Path to the ultimateALPR SDK precompiled binary library
set(ULTALPR_LIB_PATH "./ultimateALPR-SDK/binaries/linux/x86_64")
link_directories(${ULTALPR_LIB_PATH})

# Source files for the recognizer
set(SOURCES
    recognizer.cpp
    alpr_engine.cpp
    input_handler.cpp
)

# Define the executable
add_executable(recognizer ${SOURCES})

# Link the ALPR SDK and OpenCV libraries
target_link_libraries(recognizer
    ultimate_alpr-sdk
    ${OpenCV_LIBS}
	nlohmann_json::nlohmann_json
)

