cmake_minimum_required (VERSION 3.5.1)

project (oxapiexamples)

set (CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Release)

#For the shared library:
#set ( PROJECT_LINK_LIBS OXApi.so )
#if using libOXAPI.a file use following line
set ( PROJECT_LINK_LIBS OXApi )

link_directories( ~/OxApi/usr/lib/ )
include_directories( ~/OxApi/usr/include )
include_directories(${Boost_INCLUDE_DIRS})

find_package(Threads)
find_package(Boost 1.70 REQUIRED COMPONENTS system thread coroutine)

add_executable(oxapiexamples src/oxapiexamples.cpp)

target_compile_options(oxapiexamples PRIVATE -Wdate-time -D_FORTIFY_SOURCE=2)
target_compile_options(oxapiexamples PRIVATE -fstack-protector-strong -Wformat -Werror=format-security)

target_link_libraries(oxapiexamples ${PROJECT_LINK_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${BOOST_BEAST_FILES}  ${Boost_LIBRARIES} )
