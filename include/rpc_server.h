#pragma once

#include <memory>
#include <string>

#include <grpcpp/completion_queue.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/support/async_unary_call.h>
#include <spdlog/spdlog.h>

#include "rpc_handler.h"

#include "face.grpc.pb.h"

namespace arm_face_id {
class RpcServer {
 public:
  ~RpcServer() {
    server_->Shutdown();
    cq_->Shutdown();
  }

  RpcServer(std::string server_addr);

  void Run();

  template <typename Req, typename Resp>
  void RegisterRPCHandler(Call<Req, Resp> handler) {
    new RPCHandler<Req, Resp>(&service_, cq_.get(), handler);
  }

 private:
  std::unique_ptr<grpc::ServerCompletionQueue> cq_;
  FaceRpc::AsyncService service_;
  std::unique_ptr<grpc::Server> server_;
};
};  // namespace arm_face_id