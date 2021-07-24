project(rpcpp_example)
aux_source_directory(./client DIR_LIB_SRCS)
aux_source_directory(./server DIR_LIB_SRCS)
aux_source_directory(./client/connectors/linuxtcpsocketclient.h DIR_LIB_SRCS)
aux_source_directory(./client/connectors/linuxtcpsocketclient.cpp DIR_LIB_SRCS)
aux_source_directory(./client/connectors/tcpsocketclient.h DIR_LIB_SRCS)
aux_source_directory(./client/connectors/tcpsocketclient.cpp DIR_LIB_SRCS)
aux_source_directory(./server/connectors DIR_LIB_SRCS)
aux_source_directory(./common DIR_LIB_SRCS)
aux_source_directory(./ DIR_LIB_SRCS)
# 生成链接库
add_library (rpcpp_example STATIC ${DIR_LIB_SRCS}
    server/server.h
)

# add_library(rpcpp_example STATIC
#         client/client.cpp
#         client/client.h
#         client/rpcprotocolclient.h
#         client/rpcprotocolclient.cpp
#         client/iclientconnector.h
#         client/connectors/linuxtcpsocketclient.cpp
#         client/connectors/linuxtcpsocketclient.h
#         client/connectors/tcpsocketclient.h
#         client/connectors/tcpsocketclient.cpp
#         client/connectors/windowstcpsocketclient.h
#         client/connectors/windowstcpsocketclient.cpp
#         common/errors.h
#         common/rpcexception.h
#         common/noncopyable.h
#         common/poller.h
#         common/streamreader.h
#         common/streamwriter.h
#         common/errors.cpp
#         common/rpcexception.cpp
#         common/poller.cpp
#         common/streamreader.cpp
#         common/streamwriter.cpp
#         server/
#         )
find_package(jsoncpp CONFIG REQUIRED)

target_link_libraries(
    rpcpp_example
    -ljsoncpp
    -lpthread
)
set_target_properties(rpcpp_example PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS rpcpp_example DESTINATION lib)

# set(HEADERS
#     server/server.h
#     client/client.h)
# install(FILES ${HEADERS} DESTINATION include)

# add_subdirectory(stub)