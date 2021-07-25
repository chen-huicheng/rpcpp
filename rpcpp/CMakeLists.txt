project(rpcpp)
aux_source_directory(./client/ DIR_LIB_SRCS)
aux_source_directory(./server/ DIR_LIB_SRCS)
# aux_source_directory(./client/connectors/LinuxTcpSocketClient.h DIR_LIB_SRCS)
# aux_source_directory(./client/connectors/LinuxTcpSocketClient.cpp DIR_LIB_SRCS)
# aux_source_directory(./client/connectors/TcpSocketClient.h DIR_LIB_SRCS)
# aux_source_directory(./client/connectors/TcpSocketClient.cpp DIR_LIB_SRCS)
aux_source_directory(./client/connectors DIR_LIB_SRCS)
aux_source_directory(./server/connectors DIR_LIB_SRCS)
aux_source_directory(./common/ DIR_LIB_SRCS)
aux_source_directory(./ DIR_LIB_SRCS)
# 生成链接库
add_library (rpcpp STATIC ${DIR_LIB_SRCS})

find_package(jsoncpp CONFIG REQUIRED)

target_link_libraries(
    rpcpp
    -ljsoncpp
    -lpthread
)
# set_target_properties(rpcpp PROPERTIES LINKER_LANGUAGE CXX)
# install(TARGETS rpcpp DESTINATION lib)

set(HEADERS
    server/RpcServer.h
    client/RpcClient.h)
install(FILES ${HEADERS} DESTINATION include)

add_subdirectory(stub)