cmake_minimum_required (VERSION 3.14)
project (profilerStreamer)

set (CMAKE_CXX_STANDARD 17)

set (SHARED_LIB_NAME "profilerStreamer")
set (TEST_EXECUTABLE_NAME "profilerStreamerTest")

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

# Open3d
set(OPEN3D_ROOT_DIR ${PROJECT_SOURCE_DIR}/3rd_party/submodule-open3d)
set(Open3D_DIR ${OPEN3D_ROOT_DIR}/CMake)
find_package(Open3D 0.19.0 REQUIRED)

# OXAPI
set(OXAPI_ROOT_DIR ${PROJECT_SOURCE_DIR}/3rd_party/BaumerSDK/Cpp/LibOxApi_V2_0_2/LibOxApi_V2_0_2)
include_directories( ${OXAPI_ROOT_DIR}/include/ )
link_directories(${OXAPI_ROOT_DIR}/libs/)

# boost
add_subdirectory( ${PROJECT_SOURCE_DIR}/3rd_party/boost)

# open62541
add_subdirectory( ${PROJECT_SOURCE_DIR}/3rd_party/open62541)

# Threads
find_package(Threads)

# profilerStreamer
file(GLOB_RECURSE SOURCES "src/cpp/*.cc" "src/cpp/*.hh" "src/cpp/ProfilerStreamer.hh")
add_library(${SHARED_LIB_NAME} SHARED ${SOURCES})
target_link_libraries(${SHARED_LIB_NAME} PRIVATE Open3D::Open3D Boost::system Boost::thread Boost::coroutine open62541::open62541 Threads::Threads)
if(WIN32)
    set_target_properties(${SHARED_LIB_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# executable
add_executable(${TEST_EXECUTABLE_NAME} src/cpp/test_exec.cc)
target_link_libraries(${TEST_EXECUTABLE_NAME} PRIVATE ${SHARED_LIB_NAME})
target_include_directories(${TEST_EXECUTABLE_NAME} 
    PRIVATE ${OXAPI_ROOT_DIR}/include/
    PRIVATE ${OPEN3D_ROOT_DIR}/include/
    PRIVATE ${OPEN3D_ROOT_DIR}/include/open3d/3rdparty/
    PRIVATE ${PROJECT_SOURCE_DIR}/src/cpp/
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(${SHARED_LIB_NAME} PRIVATE OXApi_debug-vc141-x64)
    target_link_libraries(${TEST_EXECUTABLE_NAME} PRIVATE ${SHARED_LIB_NAME} Boost::system Boost::thread Boost::coroutine OXApi_debug-vc141-x64 open62541::open62541 Threads::Threads Open3D::Open3D)
    file(COPY "${OXAPI_ROOT_DIR}/libs/OXApi_debug-vc141-x64.dll" DESTINATION "${CMAKE_BINARY_DIR}")
    file(COPY "${OPEN3D_ROOT_DIR}/bin/Open3D.dll" DESTINATION "${CMAKE_BINARY_DIR}")
else()
    target_link_libraries(${SHARED_LIB_NAME} PRIVATE OXApi-vc141-x64)
    target_link_libraries(${TEST_EXECUTABLE_NAME} PRIVATE Boost::system Boost::thread Boost::coroutine OXApi-vc141-x64 open62541::open62541 Threads::Threads Open3D::Open3D)
    file(COPY "${OXAPI_ROOT_DIR}/libs/OXApi-vc141-x64.dll" DESTINATION "${CMAKE_BINARY_DIR}")
    file(COPY "${OPEN3D_ROOT_DIR}/bin/Open3D.dll" DESTINATION "${CMAKE_BINARY_DIR}")
endif()