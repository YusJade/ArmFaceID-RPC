#include "rpc_client.h"
#include "rpc_server.h"

#include <absl/flags/flag.h>
#include <absl/flags/parse.h>
#include <absl/flags/usage.h>

#include <chrono>
#include <grpc++/channel.h>
#include <grpc++/grpc++.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <spdlog/spdlog.h>
#include <string>
#include <thread>

ABSL_FLAG(std::string, server_addr, "localhost:8081", "服务器地址");

int main(int argc, char **argv) {

  std::string server_addr = absl::GetFlag(FLAGS_server_addr);
  arm_face_id::RpcClient rpc_client(
      grpc::CreateChannel(server_addr, grpc::InsecureChannelCredentials()));

  std::thread client_thread(&arm_face_id::RpcClient::ProcessAsyncReply,
                            &rpc_client);

  arm_face_id::RpcServer rpc_server(server_addr);
  std::thread server_thread(&arm_face_id::RpcServer::Run, &rpc_server);

  server_thread.detach();
  client_thread.detach();

  for (int i = 0; i < 10; i++) {
    spdlog::info("客户端发送一条请求");
    rpc_client.RecognizeFace();
    std::this_thread::sleep_for(std::chrono::seconds(3));
  }

  return 0;
}