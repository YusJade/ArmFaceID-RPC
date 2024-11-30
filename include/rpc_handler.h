#include <functional>

#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/support/status.h>
#include <spdlog/spdlog.h>

#include "face.grpc.pb.h"
#include "face.pb.h"

namespace arm_face_id {

/**
 * @brief 由模板类 RpcHandler
 * 调用，表明自己已做好处理目标请求的准备，等待请求的到来
 *
 * @tparam Req 目标的请求消息类型
 * @tparam Resp 返回的响应消息类型
 * @param service
 * @param ctx
 * @param req
 * @param resp
 * @param new_call_cq
 * @param notification_cq
 * @param tag
 */
template <typename Req, typename Resp>
void RequestRpc(FaceRpc::AsyncService* service, grpc::ServerContext* ctx,
                Req& req, grpc::ServerAsyncResponseWriter<Resp>& resp,
                ::grpc::CompletionQueue* new_call_cq,
                ::grpc::ServerCompletionQueue* notification_cq, void* tag) {}

template <>
inline void RequestRpc(
    FaceRpc::AsyncService* service, grpc::ServerContext* ctx,
    RecognitionRequest& req,
    grpc::ServerAsyncResponseWriter<RecognitionResponse>& resp,
    ::grpc::CompletionQueue* new_call_cq,
    ::grpc::ServerCompletionQueue* notification_cq, void* tag) {
  service->RequestRpcRecognizeFace(ctx, &req, &resp, new_call_cq,
                                   notification_cq, tag);
}

template <>
inline void RequestRpc(
    FaceRpc::AsyncService* service, grpc::ServerContext* ctx,
    RegistrationRequest& req,
    grpc::ServerAsyncResponseWriter<RegistrationResponse>& resp,
    ::grpc::CompletionQueue* new_call_cq,
    ::grpc::ServerCompletionQueue* notification_cq, void* tag) {
  service->RequestRpcRegisterFace(ctx, &req, &resp, new_call_cq,
                                  notification_cq, tag);
}

class RPCHandlerBase {
 public:
  virtual void Proceed(){};
};

template <typename Req, typename Resp>
using Call = std::function<grpc::Status(Req&, Resp&)>;

template <typename Req, typename Resp>
class RPCHandler : public RPCHandlerBase {
 private:
  friend class RpcServer;

 public:
  RPCHandler(FaceRpc::AsyncService* service, grpc::ServerCompletionQueue* cq,
             Call<Req, Resp> handler)
      : service_(service),
        cq_(cq),
        responder_(&ctx_),
        status_(CREATE),
        on_process_func_(handler) {
    Proceed();
  }

  void Proceed() override {
    if (status_ == CREATE) {
      status_ = PROCESS;
      RequestRpc(service_, &ctx_, request_, responder_, cq_, cq_, this);
    } else if (status_ == PROCESS) {
      new RPCHandler(service_, cq_, on_process_func_);

      auto resp_begin_time_point = std::chrono::high_resolution_clock::now();
      SPDLOG_INFO("Recieved a grpc request<{}>.", typeid(Req).name());

      auto grpc_status = on_process_func_(request_, reply_);
      status_ = FINISH;
      responder_.Finish(reply_, grpc_status, this);

      auto resp_finish_time_point = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> took_time =
          resp_finish_time_point - resp_begin_time_point;
      SPDLOG_INFO("Produced a grpc response<{}>, cost {}s", typeid(Resp).name(),
                  took_time.count());
    } else {
      delete this;
    }
  }

 private:
  enum CallStatus { CREATE, PROCESS, FINISH };
  CallStatus status_;

  arm_face_id::FaceRpc::AsyncService* service_;
  grpc::ServerCompletionQueue* cq_;
  grpc::ServerContext ctx_;

  Call<Req, Resp> on_process_func_;
  Req request_;
  Resp reply_;
  grpc::ServerAsyncResponseWriter<Resp> responder_;
};

}  // namespace arm_face_id