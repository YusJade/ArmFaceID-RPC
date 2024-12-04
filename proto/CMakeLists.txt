project(proto)

if(CMAKE_CROSSCOMPILING)
    set(PROTO_FILES face.proto)
    generate_protobuf_and_grpc(${PROTO_FILES})
    set(PROTO_FILES face.grpc.pb.cc face.pb.cc)
    
    add_library(proto ${PROTO_FILES})
    
    target_link_libraries(proto
        PUBLIC
            protobuf::libprotobuf
            gRPC::grpc
            gRPC::grpc++
    )

    target_include_directories(proto PUBLIC
        ${PROTOBUF_INCLUDE_DIRS} 
        ${CMAKE_CURRENT_BINARY_DIR})
else()
    set(PROTO_FILES face.proto)

    add_library(proto ${PROTO_FILES})

    target_link_libraries(proto
        PUBLIC
            protobuf::libprotobuf
            gRPC::grpc
            gRPC::grpc++
    )

    target_include_directories(proto PUBLIC
        ${PROTOBUF_INCLUDE_DIRS} 
        ${CMAKE_CURRENT_BINARY_DIR})
        protobuf_generate(TARGET proto LANGUAGE cpp)
        protobuf_generate(TARGET proto LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")
endif()