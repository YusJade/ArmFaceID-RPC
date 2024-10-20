#pragma once

#include "proto.grpc.pb.h"
#include "proto.pb.h"
#include <grpcpp/completion_queue.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/support/async_unary_call.h>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>
namespace arm_face_id {
class RpcServer {
public:
  ~RpcServer() {
    server_->Shutdown();
    cq_->Shutdown();
  }

  RpcServer(std::string server_addr);

  void Run();

private:
  class CallData {
  public:
    CallData(FaceRpc::AsyncService *service, grpc::ServerCompletionQueue *cq);

    void Proceed();

  private:
    enum CallStatus { CREATE, PROCESS, FINISH };
    CallStatus status_;

    FaceRpc::AsyncService *service_;
    grpc::ServerCompletionQueue *cq_;
    grpc::ServerContext ctx_;

    RecognitionRequest request_;
    RecognitionResponse reply_;

    grpc::ServerAsyncResponseWriter<RecognitionResponse> responder_;
  };

  std::unique_ptr<grpc::ServerCompletionQueue> cq_;
  FaceRpc::AsyncService service_;
  std::unique_ptr<grpc::Server> server_;
};
}; // namespace arm_face_id