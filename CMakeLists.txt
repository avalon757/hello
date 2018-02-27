cmake_minimum_required(VERSION 3.9)
project(hello)

set(BOOST_ROOT /home/wang/boost_1_66_0)
# see https://cmake.org/cmake/help/latest/module/FindBoost.html
find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

aux_source_directory(/home/wang/boost_1_66_0/libs/date_time/src/gregorian DATETIME_SRC)


set(CMAKE_CXX_STANDARD 11)

add_executable(hello main.cpp)