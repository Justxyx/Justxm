cmake_minimum_required(VERSION 3.10.2)
project(justxm)

set(CMAKE_CXX_STANDARD 11)

add_executable(justxm main.cpp log/Log.cpp log/Log.h test/LogTest.h log/FormatItem.h util/Util.cpp util/Util.h)
