#include "rpc_server.h"

#include <absl/time/civil_time.h>
#include <absl/time/clock.h>
#include <absl/time/time.h>
#include <grpcpp/support/status.h>
#include <spdlog/spdlog.h>

using namespace arm_face_id;

RpcServer::RpcServer(std::string server_addr) {
  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_addr, grpc::InsecureServerCredentials());
  builder.RegisterService(&service_);
  cq_ = builder.AddCompletionQueue();
  server_ = builder.BuildAndStart();

  SPDLOG_INFO("gRPC service is listening on {}", server_addr);
}

void RpcServer::Run() {
  SPDLOG_INFO("gRPC service is waitting for request...");
  // new CallData(&service_, cq_.get());
  void *tag;
  bool ok;
  while (true) {
    cq_->Next(&tag, &ok);

    if (!ok) continue;

    static_cast<RPCHandlerBase *>(tag)->Proceed();
  }
  SPDLOG_WARN("gRPC service is closed!");
  cq_->Shutdown();
  server_->Shutdown();
}

// RpcServer::CallData::CallData(FaceRpc::AsyncService *service,
//                               grpc::ServerCompletionQueue *cq)
//     : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
//   Proceed();
// }

// void RpcServer::CallData::Proceed() {
//   if (status_ == CREATE) {
//     status_ = PROCESS;
//     service_->RequestRpcRecognizeFace(&ctx_, &request_, &responder_, cq_,
//     cq_,
//                                       this);
//   } else if (status_ == PROCESS) {
//     new CallData(service_, cq_);
//     absl::CivilSecond civil_second =
//         absl::ToCivilSecond(absl::Now(), absl::LocalTimeZone());
//     std::string formatted_str = absl::FormatCivilTime(civil_second);
//     // 处理业务逻辑
//     spdlog::info("RPC 服务端：处理一条请求。{}", formatted_str);

//     status_ = FINISH;
//     responder_.Finish(reply_, grpc::Status::OK, this);
//   } else {
//     delete this;
//   }
// }
