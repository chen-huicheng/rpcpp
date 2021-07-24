include_directories(../..)
aux_source_directory(. src)
# aux_source_directory( src)
add_executable(rpcppstub
        ${src}
        )

target_link_libraries(rpcppstub jackson)
install(TARGETS rpcppstub DESTINATION bin)