# ArmFaceID-RPC

供 `ArmFaceID` 与 `ArmFaceID-client`使用的 `rpc`通信模块，使用`gRPC`实现。

<a href="https://github.com/YusJade/ArmFaceID?tab=readme-ov-file">
  <img src="https://github-readme-stats.vercel.app/api/pin/?username=YusJade&repo=ArmFaceID&theme=tokyonight" alt="YusJade/ArmFaceID" />
</a>

<a href="https://github.com/YusJade/ArmFaceID-client?tab=readme-ov-file">
  <img src="https://github-readme-stats.vercel.app/api/pin/?username=YusJade&repo=ArmFaceID-client&theme=tokyonight" alt="YusJade/ArmFaceID-client" />
</a>

## 计划实现功能

- [ ] 服务端抛弃超时请求：客户端发送识别请求时附带时间戳，服务端接受到请求时检查时间戳，若超过设定的阈值，则不进行识别，直接返回响应。
- [ ] 服务端使用异步实现，对 CallData 进行进一步封装。
- [ ] 客户端使用异步实现，对请求类型进行封装。