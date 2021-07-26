add_executable(echo_server_ echoService.cpp echoServiceStub.h)
target_link_libraries(echo_server_ rpcpp)
install(TARGETS echo_server_ DESTINATION bin)

add_executable(echo_client_ echoClient.cpp echoClientStub.h)
target_link_libraries(echo_client_ rpcpp)
install(TARGETS echo_client_ DESTINATION bin)