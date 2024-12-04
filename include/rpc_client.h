#pragma once

#include <memory>

#include <grpcpp/client_context.h>
#include <grpcpp/completion_queue.h>
#include <grpcpp/impl/channel_interface.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/support/status.h>

#include "observer/core.h"

#include "face.grpc.pb.h"
#include "face.pb.h"

namespace arm_face_id {
class RpcClient : public treasure_chest::pattern::Subject {
 public:
  explicit RpcClient(std::shared_ptr<grpc::ChannelInterface> channel);

  void RecognizeFace(std::string img_bytes);

  void ProcessAsyncReply();

 private:
  struct AsyncClientCall {
    RecognitionResponse reply;
    grpc::ClientContext context;
    grpc::Status status;
    std::unique_ptr<grpc::ClientAsyncResponseReader<RecognitionResponse>>
        response_reader;
  };

  std::unique_ptr<FaceRpc::Stub> stub_;
  grpc::CompletionQueue cq_;
};
}  // namespace arm_face_id