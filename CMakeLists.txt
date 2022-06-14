cmake_minimum_required(VERSION 3.10.2)
project(justxm)

set(CMAKE_CXX_STANDARD 11)

# boost
#添加头文件搜索路径
include_directories( /Users/xiaoyingxiong/install/boost/boost_1_76_0/)
#添加库文件搜索路径
link_directories(/Users/xiaoyingxiong/install/boost/boost_1_76_0/stage/lib/)


add_executable(justxm main.cpp log/Log.cpp log/Log.h test/LogTest.h log/FormatItem.h util/Util.cpp util/Util.h util/Singleton.h config/Config.cpp config/Config.h config/Specialization.h test/ConfigTest.h)
