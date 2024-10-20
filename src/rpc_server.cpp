#include "rpc_server.h"
#include <grpcpp/support/status.h>
#include <spdlog/spdlog.h>

using namespace arm_face_id;

RpcServer::RpcServer(std::string server_addr) {
  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_addr, grpc::InsecureServerCredentials());
  builder.RegisterService(&service_);
  cq_ = builder.AddCompletionQueue();
  server_ = builder.BuildAndStart();

  spdlog::info("RPC 服务端：监听于: {} 。", server_addr);
}

void RpcServer::Run() {
  spdlog::info("RPC 服务端：等待请求中。");
  new CallData(&service_, cq_.get());
  void *tag;
  bool ok;
  while (true) {
    cq_->Next(&tag, &ok);
    if (!ok)
      continue;
    static_cast<CallData *>(tag)->Proceed();
  }
  spdlog::info("RPC 服务端：退出进程。");
}

RpcServer::CallData::CallData(FaceRpc::AsyncService *service,
                              grpc::ServerCompletionQueue *cq)
    : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
  Proceed();
}

void RpcServer::CallData::Proceed() {
  if (status_ == CREATE) {
    status_ = PROCESS;
    service_->RequestRpcRecognizeFace(&ctx_, &request_, &responder_, cq_, cq_,
                                      this);
  } else if (status_ == PROCESS) {
    new CallData(service_, cq_);

    // 处理业务逻辑
    spdlog::info("RPC 服务端：处理一条请求。");

    status_ = FINISH;
    responder_.Finish(reply_, grpc::Status::OK, this);
  } else {
    delete this;
  }
}
