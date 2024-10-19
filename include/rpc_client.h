#pragma once

#include "proto.grpc.pb.h"

#include <memory>

namespace arm_face_id {
class RpcClient {

private:
  std::unique_ptr<FaceRpc::Stub> rpc_stub_;
};
} // namespace arm_face_id