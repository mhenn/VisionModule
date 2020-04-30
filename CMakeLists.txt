CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project(VisionModule)

set(PROJECT_NAME "visionmodule")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


file(GLOB_RECURSE SOURCE_FILES
     ${CMAKE_SOURCE_DIR}/libvideo/*.cpp
  ${CMAKE_SOURCE_DIR}/src/*.cpp)

  file(GLOB_RECURSE HEADER_FILES
     ${CMAKE_SOURCE_DIR}/src/*.h
     ${CMAKE_SOURCE_DIR}/libvideo/*.h)


set (VERSION_MAJOR 0)
set (VERSION_MINOR 2)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++0x -DDEBUG -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++0x -Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -Wall")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

find_package(Boost)


configure_file (
   "${PROJECT_SOURCE_DIR}/src/config.h.in"
   "${PROJECT_BINARY_DIR}/src/config.h"
)

ADD_EXECUTABLE( ${PROJECT_NAME}
   ${HEADER_FILES}
   ${SOURCE_FILES}
)

ADD_DEFINITIONS(
  -DENABLE_V4L2
  -DENABLE_FILE
)

INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}/libvideo/
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
   "boost_program_options"
   "boost_system"
   "boost_thread"
   "v4l2"
   "avcodec" "avformat" "avutil" "swscale" "z" "bz2"
   "pthread"
   "jpeg"
)

ADD_CUSTOM_COMMAND(
   TARGET  ${PROJECT_NAME}
  POST_BUILD
  COMMAND rm -fr ${CMAKE_BINARY_DIR}/www
  COMMENT Removing www directory
  COMMAND cp -aR ${CMAKE_SOURCE_DIR}/www ${CMAKE_BINARY_DIR}/www
  COMMENT Copying www directories
)

if(CMAKE_ALL)
   ADD_SUBDIRECTORY("submodules/visionclient")
   ADD_SUBDIRECTORY("submodules/motionclient")
   ADD_SUBDIRECTORY("submodules/motionserver")
endif(CMAKE_ALL)
