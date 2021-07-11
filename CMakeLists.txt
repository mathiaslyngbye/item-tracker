cmake_minimum_required(VERSION 3.10)
project(garment_tracker)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(OpenCV REQUIRED)

set(DEPENDENCIES ${OpenCV_LIBS})

add_executable(garment_tracker main.cpp virtual_camera.cpp display.cpp segment.cpp track.cpp)
target_link_libraries(garment_tracker stdc++fs ${DEPENDENCIES})
target_compile_features(garment_tracker PRIVATE cxx_std_17)
