cmake_minimum_required (VERSION 3.14)
project (profilerStreamer)

set (CMAKE_CXX_STANDARD 17)

include (CMakeFunctions.cmake)
update_submodules()

# OXAPI
set(OXAPI_ROOT_DIR ${PROJECT_SOURCE_DIR}/3rd_party/BaumerSDK/Cpp/LibOxApi_V2_0_2/LibOxApi_V2_0_2)
include_directories( ${OXAPI_ROOT_DIR}/include/ )
link_directories(${OXAPI_ROOT_DIR}/libs/)

# boost
add_subdirectory( ${PROJECT_SOURCE_DIR}/3rd_party/boost)

# Threads
find_package(Threads)

add_executable(profilerStreamer src/cpp/test_exec.cc)
target_include_directories(profilerStreamer PRIVATE ${OXAPI_ROOT_DIR}/include/)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(profilerStreamer PRIVATE Boost::system Boost::thread Boost::coroutine OXApi_debug-vc141-x64 Threads::Threads)
    file(COPY "${OXAPI_ROOT_DIR}/libs/OXApi_debug-vc141-x64.dll" DESTINATION "${CMAKE_BINARY_DIR}")
else()
    target_link_libraries(profilerStreamer PRIVATE Boost::system Boost::thread Boost::coroutine OXApi-vc141-x64 Threads::Threads)
    file(COPY "${OXAPI_ROOT_DIR}/libs/OXApi-vc141-x64.dll" DESTINATION "${CMAKE_BINARY_DIR}")
endif()