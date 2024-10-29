cmake_minimum_required(VERSION 3.10)

project(ArmFaceID-RPC)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE DEBUG)

find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_library(rpc src/rpc_client.cpp src/rpc_server.cpp src/rpc_handler.cpp)
target_link_libraries(rpc PUBLIC protobuf::libprotobuf 
                      gRPC::grpc gRPC::grpc++ spdlog::spdlog proto)
target_include_directories(rpc PUBLIC include)

# include_directories(include)

add_executable(test src/test.cpp)
target_link_libraries(test PUBLIC rpc)

add_subdirectory(proto)