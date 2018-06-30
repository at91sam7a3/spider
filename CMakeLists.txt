project(robot)
cmake_minimum_required(VERSION 2.8)

SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)

SET(CMAKE_C_COMPILER $ENV{HOME}/raspberrypi/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER $ENV{HOME}/raspberrypi/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)


# Where is the target environment
SET(CMAKE_FIND_ROOT_PATH $ENV{HOME}/raspberrypi/rootfs)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")
SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")

# Search for programs only in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath-link=/home/oleksii/raspberrypi/rootfs/lib/arm-linux-gnueabihf:/home/oleksii/raspberrypi/rootfs/usr/lib/arm-linux-gnueabihf:/home/oleksii/raspberrypi/rootfs/lib:/home/oleksii/raspberrypi/rootfs/usr/lib")

# Search for libraries and headers only in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
INCLUDE_DIRECTORIES($ENV{HOME}/raspberrypi/rootfs/usr/include)
INCLUDE_DIRECTORIES($ENV{HOME}/raspberrypi/rootfs/usr/local/include)

link_directories($ENV{HOME}/raspberrypi/rootfs/usr/lib/arm-linux-gnueabihf)
link_directories($ENV{HOME}/raspberrypi/rootfs/usr/local/lib)
link_directories($ENV{HOME}/raspberrypi/rootfs/usr/lib)
link_directories($ENV{HOME}/raspberrypi/rootfs/opt/vc/lib)

INCLUDE_DIRECTORIES($ENV{HOME}/raspberrypi/rootfs/opt/vc/include)
INCLUDE_DIRECTORIES($ENV{HOME}/raspberrypi/rootfs/opt/vc/include/interface/vcos/pthreads)
INCLUDE_DIRECTORIES($ENV{HOME}/raspberrypi/rootfs/opt/vc/include/interface/vmcs_host/linux)


aux_source_directory(. SRC_LIST)
#aux_source_directory(./body SRC_LIST)
#aux_source_directory(./drivers DRV_SRC_LIST)
set(CMAKE_CXX_FLAGS "-std=c++11  -L/home/oleksii/raspberrypi/rootfs/lib/arm-linux-gnueabihf -L/home/oleksii/raspberrypi/rootfs/usr/lib/arm-linux-gnueabihf -L/home/oleksii/raspberrypi/rootfs/lib -L/home/oleksii/raspberrypi/rootfs/usr/lib -L/home/oleksii/raspberrypi/rootfs/usr/local/lib" )

SET(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -Wl,-rpath-link=/home/oleksii/raspberrypi/rootfs/lib/arm-linux-gnueabihf:/home/oleksii/raspberrypi/rootfs/usr/lib/arm-linux-gnueabihf:/home/oleksii/raspberrypi/rootfs/lib:/home/oleksii/raspberrypi/rootfs/usr/lib:/home/oleksii/raspberrypi/rootfs/opt/vc/lib")


include(build/conanbuildinfo.cmake)
conan_basic_setup()


add_executable(robot ${SRC_LIST}  ${DRV_SRC_LIST})
target_link_libraries(robot ${CONAN_LIBS}  mosquittopp pthread opencv_core opencv_highgui raspicam boost_system boost_serialization opencv_imgproc wiringPi boost_filesystem raspicam_cv  aruco)
