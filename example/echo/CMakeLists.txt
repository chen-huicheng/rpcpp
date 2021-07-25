# add_custom_command(
#         OUTPUT raw_header
#         COMMAND rpcppstub
#         ARGS -o -i ${CMAKE_CURRENT_SOURCE_DIR}/echo.json
#         MAIN_DEPENDENCY echo.json
#         DEPENDS rpcppstub
#         COMMENT "Generating Server/Client Stub..."
#         VERBATIM
# )

# set(stub_dir ${PROJECT_BINARY_DIR}/example/echo)

# add_custom_command(
#         OUTPUT echoServiceStub.h echoClientStub.h
#         COMMAND ${CMAKE_STUB_FORMATTER}
#         ARGS -i ${stub_dir}/echoServiceStub.h ${stub_dir}/echoClientStub.h
#         DEPENDS raw_header
#         COMMENT "clang format Stub..."
#         VERBATIM
# )

add_executable(echo_server_ echoService.cpp echoServiceStub.h)
target_link_libraries(echo_server_ rpcpp)
install(TARGETS echo_server_ DESTINATION bin)

add_executable(echo_client_ echoClient.cpp echoClientStub.h)
target_link_libraries(echo_client_ rpcpp)
install(TARGETS echo_client_ DESTINATION bin)