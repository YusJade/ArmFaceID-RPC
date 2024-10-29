#include "rpc_handler.h"

using namespace arm_face_id;

void RequestRpc(FaceRpc::AsyncService* service, grpc::ServerContext* ctx,
                RecognitionRequest& req,
                grpc::ServerAsyncResponseWriter<RecognitionResponse>& resp,
                ::grpc::CompletionQueue* new_call_cq,
                ::grpc::ServerCompletionQueue* notification_cq, void* tag) {
  service->RequestRpcRecognizeFace(ctx, &req, &resp, new_call_cq,
                                   notification_cq, tag);
}
