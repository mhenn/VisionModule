CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project(VisionClient)

set(PROJECT_NAME "motionclient")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


file(GLOB_RECURSE SOURCE_FILES
  src/*.cpp)


set (VERSION_MAJOR 0)
set (VERSION_MINOR 2)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++0x -DDEBUG -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++0x -Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -Wall")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

find_package(Boost)

INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}/../../libvideo/
)
ADD_EXECUTABLE( ${PROJECT_NAME} 
   ${SOURCE_FILES}
)

target_link_libraries(${PROJECT_NAME}
   "boost_program_options"
   "boost_system"
   "boost_thread"
   "v4l2"
   "pthread"
   "jpeg")


