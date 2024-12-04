#include "rpc_client.h"

#include <cassert>

#include <grpcpp/client_context.h>
#include <grpcpp/impl/channel_interface.h>
#include <spdlog/spdlog.h>

#include "face.grpc.pb.h"
#include "face.pb.h"

using namespace arm_face_id;

RpcClient::RpcClient(std::shared_ptr<grpc::ChannelInterface> channel)
    : stub_(FaceRpc::NewStub(channel)) {
  spdlog::info("RPC 客户端：已构造。");
}

void RpcClient::ProcessAsyncReply() {
  void *got_tag;
  bool ok = false;

  while (cq_.Next(&got_tag, &ok)) {
    AsyncClientCall *call = static_cast<AsyncClientCall *>(got_tag);

    assert(ok);

    if (call->status.ok()) {
      spdlog::info("RPC 客户端：成功接收到服务端的响应 :P");
      auto res = call->reply.res();
      Notify(res);
    } else {
      spdlog::warn("RPC 客户端：响应错误!: {}", call->status.error_message());
    }
  }
}

void RpcClient::RecognizeFace(std::string img_bytes) {
  // RPC请求数据封装
  RecognitionRequest request;
  request.set_image(img_bytes);

  // 异步客户端请求，存储请求响应的状态和数据的结构体等，在下方进行的定义
  AsyncClientCall *call = new AsyncClientCall;

  // 初始化response_reader
  // stub_->PrepareAsyncSayHello()创建一个RPC对象，但是不会立即启动RPC调用
  call->response_reader =
      stub_->PrepareAsyncRpcRecognizeFace(&call->context, request, &cq_);

  // StartCall()方法发起真正的RPC请求
  call->response_reader->StartCall();

  // Finish()方法前两个参数用于指定响应数据的存储位置，第三个参数指定了该次RPC异步请求的地址
  call->response_reader->Finish(&call->reply, &call->status, (void *)call);
}