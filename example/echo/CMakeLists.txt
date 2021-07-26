add_executable(echoService echoService.cpp echoServiceStub.h)
target_link_libraries(echoService rpcpp)
install(TARGETS echoService DESTINATION bin)

add_executable(echoClient echoClient.cpp echoClientStub.h)
target_link_libraries(echoClient rpcpp)
install(TARGETS echoClient DESTINATION bin)