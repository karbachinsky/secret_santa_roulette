cmake_minimum_required(VERSION 2.8.4)
project(newyear_roulette)

set(BOOST_ROOT "/usr/local/boost")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

set(SOURCE_FILES main.cpp)

set(BOOST_ROOT "/usr/local/boost")
set(Boost_USE_STATIC_LIBS ON)

include_directories ("${PROJECT_SOURCE_DIR}/lib")

add_executable(newyear_roulette ${SOURCE_FILES})

include_directories(${LIB_DIR})

# Searching boost
find_package(Boost COMPONENTS system filesystem REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(newyear_roulette ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})


