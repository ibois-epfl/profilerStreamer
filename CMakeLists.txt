cmake_minimum_required (VERSION 3.14)
project (profilerStreamer)

set (CMAKE_CXX_STANDARD 17)

# to align with open62541' s CMakeLists.txt
if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W1") # Reduce warnings level
    add_compile_options(
        $<$<CONFIG:>:/MD>
        $<$<CONFIG:Debug>:/MDd>
        $<$<CONFIG:Release>:/MD>
    )
else()
    add_compile_options(-Wno-unused-variable -Wno-unused-function)
    add_compile_options(-Wno-gnu-zero-variadic-macro-arguments) # silence warning for the check library
endif()

include (CMakeFunctions.cmake)
update_submodules()

# OXAPI
set(OXAPI_ROOT_DIR ${PROJECT_SOURCE_DIR}/3rd_party/BaumerSDK/Cpp/LibOxApi_V2_0_2/LibOxApi_V2_0_2)
include_directories( ${OXAPI_ROOT_DIR}/include/ )
link_directories(${OXAPI_ROOT_DIR}/libs/)

set(OPEN3D_ROOT_DIR ${PROJECT_SOURCE_DIR}/3rd_party/submodule-open3d)
set(Open3D_DIR ${OPEN3D_ROOT_DIR}/CMake)
find_package(Open3D 0.19.0 REQUIRED)

# boost
add_subdirectory( ${PROJECT_SOURCE_DIR}/3rd_party/boost)

# open62541
add_subdirectory( ${PROJECT_SOURCE_DIR}/3rd_party/open62541)

# Threads
find_package(Threads)

add_executable(profilerStreamer src/cpp/test_exec.cc)
target_include_directories(profilerStreamer PRIVATE ${OXAPI_ROOT_DIR}/include/)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(profilerStreamer PRIVATE Boost::system Boost::thread Boost::coroutine OXApi_debug-vc141-x64 open62541 Threads::Threads Open3D::Open3D)
    file(COPY "${OXAPI_ROOT_DIR}/libs/OXApi_debug-vc141-x64.dll" DESTINATION "${CMAKE_BINARY_DIR}")
    file(COPY "${OPEN3D_ROOT_DIR}/bin/Open3D.dll" DESTINATION "${CMAKE_BINARY_DIR}")
else()
    target_link_libraries(profilerStreamer PRIVATE Boost::system Boost::thread Boost::coroutine OXApi-vc141-x64 open62541 Threads::Threads Open3D::Open3D)
    file(COPY "${OXAPI_ROOT_DIR}/libs/OXApi-vc141-x64.dll" DESTINATION "${CMAKE_BINARY_DIR}")
    file(COPY "${OPEN3D_ROOT_DIR}/bin/Open3D.dll" DESTINATION "${CMAKE_BINARY_DIR}")
endif()