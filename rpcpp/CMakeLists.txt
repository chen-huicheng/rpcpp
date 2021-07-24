project(rpcpp_example)
aux_source_directory(./client DIR_LIB_SRCS)
aux_source_directory(./server DIR_LIB_SRCS)
aux_source_directory(./client/connectors/LinuxTcpSocketClient.h DIR_LIB_SRCS)
aux_source_directory(./client/connectors/LinuxTcpSocketClient.cpp DIR_LIB_SRCS)
aux_source_directory(./client/connectors/TcpSocketClient.h DIR_LIB_SRCS)
aux_source_directory(./client/connectors/TcpSocketClient.cpp DIR_LIB_SRCS)
aux_source_directory(./server/connectors DIR_LIB_SRCS)
aux_source_directory(./common DIR_LIB_SRCS)
aux_source_directory(./ DIR_LIB_SRCS)
# 生成链接库
add_library (rpcpp_example STATIC ${DIR_LIB_SRCS}
    # server/server.h
)

find_package(jsoncpp CONFIG REQUIRED)

target_link_libraries(
    rpcpp_example
    -ljsoncpp
    -lpthread
)
set_target_properties(rpcpp_example PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS rpcpp_example DESTINATION lib)

set(HEADERS
    server/Server.h
    client/Client.h)
install(FILES ${HEADERS} DESTINATION include)

add_subdirectory(stub)