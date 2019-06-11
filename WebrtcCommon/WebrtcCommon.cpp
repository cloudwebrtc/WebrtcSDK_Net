// WebrtcCommon.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#include "webrtc/pc/webrtcsession.h" 
#include "webrtc/modules/audio_device/include/audio_device.h" 
#include "webrtc/api/peerconnectioninterface.h" 
#include "webrtc/pc/peerconnection.h"

#include "peer_connection_client.h"
#include "conductor.h"
#include "main_wnd.h"
#include "flagdefs.h" 

using namespace webrtc; 
using namespace rtc;

 

EX_API bool Test()
{ 
	return true;
}
 
 


EX_API bool StartConnect(HBITMAP* localImgControl, HBITMAP* remoteImgControl)
{   
	MainWnd wnd(localImgControl, remoteImgControl);
	PeerConnectionClient client;
	scoped_refptr<Conductor> conductor(new rtc::RefCountedObject<Conductor>(&client, &wnd));
	return 1;
}
