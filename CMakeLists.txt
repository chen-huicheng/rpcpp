cmake_minimum_required(VERSION 3.10.2)
PROJECT(rpcpp_test)

set(project_name rpcpp_test)
set(project_root ${PROJECT_SOURCE_DIR})
set(project_src ${project_root})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 11)  #设置使用c11标准编译程序
add_definitions(-std=c++11) #设置使用c11标准编译程序

set(CMAKE_BUILD_TYPE "Debug") #Debug Release
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O -Wall -g -ggdb -fpermissive")#在debug下不优化,即采用最低级别的O0
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")#在release下采用最高级别优化


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# 需要 sudo apt-get install libjsoncpp-dev 安装后使用
find_package(jsoncpp CONFIG REQUIRED)

include_directories(
    /usr/include/
    ${project_root}/
    ${PROJECT_BINARY_DIR}
)

# link_directories(
#     /usr/lib/x86_64-linux-gnu/
# )

# aux_source_directory(${project_root}/ src)
# aux_source_directory(${project_src}/client src)
# aux_source_directory(${project_src}/server src)
# aux_source_directory(${project_src}/common src)
# aux_source_directory(${project_root}/example src)

# add_executable(${project_name} ${src})

# target_link_libraries(
#     ${project_name}
#     -ljsoncpp
#     -lpthread
# )
add_subdirectory(rpcpp)
add_subdirectory(example)

# target_link_libraries(main PRIVATE jsoncpp_object jsoncpp_static)