add_executable(rpcppstub
        StubGenerator.h
        StubGenerator.cpp
        ServiceStubGenerator.h
        ServiceStubGenerator.cpp
        ClientStubGenerator.h
        ClientStubGenerator.cpp
        main.cpp
        )

target_link_libraries(rpcppstub jackson)
install(TARGETS rpcppstub DESTINATION bin)