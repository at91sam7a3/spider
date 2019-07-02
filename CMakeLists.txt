project(robot)
cmake_minimum_required(VERSION 2.8)



aux_source_directory(. SRC_LIST)
aux_source_directory(./proto SRC_LIST)
aux_source_directory(./body SRC_LIST)
aux_source_directory(./vision SRC_LIST)
aux_source_directory(./hardware/lx16lib LX16_SRC_LIST)

include(build/raspberry_crosscompile.cmake)
include(build/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(robot ${SRC_LIST})
add_library(lx16 ${LX16_SRC_LIST})
target_link_libraries(robot lx16 ${CONAN_LIBS}  wiringPi  opencv_videoio opencv_core opencv_highgui opencv_imgproc opencv_imgcodecs opencv_aruco realsense2 udev)

add_custom_target(useful_files DEPENDS ALL
    SOURCES
    command.proto
    conanfile.txt
)
