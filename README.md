# WebrtcSDK_Net
.net版本的webrtc的sdk，为了兼容各种.net项目，使用.netstandard。编译器使用vs2017以上。

1.WebrtcCommon项目用于开发c++版本的webrtc动态库。WebrtcCommon.cpp文件中是开放的API。/n
2.SDKTest项目引用了WebrtcCommon项目，并以c++的方式调用开放API。方便调试API。/n
3.WebrtcSDK_NET最终将集成WebrtcCommon，并形成.net版本的SDK。/n


问题:
如果项目生成发生： fatal error LNK1104: cannot open file 'atls.lib'，请完全安装vs的c++ for windows                                           
