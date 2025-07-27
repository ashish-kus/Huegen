cmake_minimum_required(VERSION 3.10)
project(HuegenApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Try to find OpenCV first
find_package(OpenCV QUIET)

# Create executable
add_executable(heugen
    src/main.cpp
    src/color_selector.cpp
    src/color_utils.cpp
    src/kmeans_wrapper.cpp
    src/template_engine.cpp
)

# Set include directories
target_include_directories(heugen PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    /usr/include/opencv4
)

if(OpenCV_FOUND)
    # If OpenCV is found but gives linking issues, use manual linking
    message(STATUS "OpenCV found, using manual library linking to avoid HDF5 issues")
    target_link_libraries(heugen PRIVATE 
        opencv_core 
        opencv_imgcodecs 
        opencv_imgproc
    )
else()
    # Fallback manual linking
    message(STATUS "OpenCV not found via find_package, using manual linking")
    target_link_libraries(heugen PRIVATE 
        opencv_core 
        opencv_imgcodecs 
        opencv_imgproc
    )
endif()
