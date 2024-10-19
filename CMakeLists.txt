cmake_minimum_required(VERSION 3.10)

project(ArmFaceID-RPC)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

add_library(rpc src/rpc_client.cpp)

target_link_libraries(rpc PUBLIC protobuf::libprotobuf 
                      gRPC::grpc gRPC::grpc++ proto)


add_subdirectory(proto)