cmake_minimum_required(VERSION 3.10.2)
project(justxm)

set(CMAKE_CXX_STANDARD 11)

# boost
#添加头文件搜索路径
include_directories( /Users/xiaoyingxiong/install/boost/boost_1_76_0/)
#添加库文件搜索路径
link_directories(/Users/xiaoyingxiong/install/boost/boost_1_76_0/stage/lib/)

#  yaml-cpp
include_directories(/opt/homebrew/Cellar/yaml-cpp/0.6.3_1/include)
link_directories(/opt/homebrew/Cellar/yaml-cpp/0.6.3_1/lib)

add_executable(justxm main.cpp log/Log.cpp log/Log.h test/LogTest.h log/FormatItem.h util/Util.cpp util/Util.h util/Singleton.h config/Config.cpp config/Config.h config/Specialization.h test/ConfigTest.h thread/Threadx.cpp thread/Threadx.h test/Thread.h)


target_link_libraries(justxm yaml-cpp)
