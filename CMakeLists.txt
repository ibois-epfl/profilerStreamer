
cmake_minimum_required (VERSION 4.2)
project (profilerStreamer)

set(CMAKE_GENERATOR_PLATFORM x64 CACHE STRING "Target platform")
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "This project requires a 64-bit build. Please set your generator/platform to x64.")
endif()

set (CMAKE_CXX_STANDARD 17)

set (BINDINGS_MODULE_NAME "profilerStreamerBindings")
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
download_dependency("open3d" "https://github.com/isl-org/Open3D/releases/download/v0.19.0/open3d-devel-windows-amd64-0.19.0-dbg.zip")
set(OPEN3D_ROOT_DIR ${PROJECT_SOURCE_DIR}/3rd_party/open3d/open3d-devel-windows-amd64-0.19.0)
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

# nanobind
if (CMAKE_VERSION VERSION_LESS 3.18)
  set(DEV_MODULE Development)
else()
  set(DEV_MODULE Development.Module)
endif()
find_package(Python 3.9 COMPONENTS Interpreter ${DEV_MODULE} REQUIRED)
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
add_subdirectory(${PROJECT_SOURCE_DIR}/3rd_party/nanobind)

# profilerStreamer
file(GLOB_RECURSE SOURCES "src/cpp/*.cc" "src/cpp/*.hh" "src/cpp/ProfilerStreamer.hh")
list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/src/cpp/bindings.cc")
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
    file(COPY "${OPEN3D_ROOT_DIR}/bin/tbb12_debug.dll" DESTINATION "${CMAKE_BINARY_DIR}")
else()
    target_link_libraries(${SHARED_LIB_NAME} PRIVATE OXApi-vc141-x64)
    target_link_libraries(${TEST_EXECUTABLE_NAME} PRIVATE Boost::system Boost::thread Boost::coroutine OXApi-vc141-x64 open62541::open62541 Threads::Threads Open3D::Open3D)
    file(COPY "${OXAPI_ROOT_DIR}/libs/OXApi-vc141-x64.dll" DESTINATION "${CMAKE_BINARY_DIR}")
    file(COPY "${OPEN3D_ROOT_DIR}/bin/Open3D.dll" DESTINATION "${CMAKE_BINARY_DIR}")
endif()

nanobind_add_module(${BINDINGS_MODULE_NAME} ${PROJECT_SOURCE_DIR}/src/cpp/bindings.cc)
target_link_libraries(${BINDINGS_MODULE_NAME} PRIVATE ${SHARED_LIB_NAME} Open3D::Open3D Boost::system Boost::thread Boost::coroutine open62541::open62541 Threads::Threads)
target_include_directories(${BINDINGS_MODULE_NAME} 
    PRIVATE ${OXAPI_ROOT_DIR}/include/
    PRIVATE ${OPEN3D_ROOT_DIR}/include/
    PRIVATE ${OPEN3D_ROOT_DIR}/include/open3d/3rdparty/
    PRIVATE ${PROJECT_SOURCE_DIR}/src/cpp/
)

set_target_properties(${BINDINGS_MODULE_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/src/py/bindings/)