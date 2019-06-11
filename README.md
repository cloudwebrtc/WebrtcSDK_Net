# WebrtcSDK_Net
.net版本的webrtc的sdk，为了兼容各种.net项目，使用.netstandard。
编译器使用vs2017，请完全安装vs中c++的windows开发模块。

1.WebrtcCommon项目用于开发c++版本的webrtc动态库。WebrtcCommon.cpp文件中是开放的API。/n
2.SDKTest项目引用了WebrtcCommon项目，并以c++的方式调用开放API。方便调试API。/n
3.WebrtcSDK_NET最终将集成WebrtcCommon，并形成.net版本的SDK。/n


