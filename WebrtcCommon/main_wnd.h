/*
 *  Copyright 2012 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_EXAMPLES_PEERCONNECTION_CLIENT_MAIN_WND_H_
#define WEBRTC_EXAMPLES_PEERCONNECTION_CLIENT_MAIN_WND_H_
#pragma once

#include <map>
#include <memory>
#include <string>

#include "webrtc/api/mediastreaminterface.h"
#include "webrtc/api/video/video_frame.h"
#include "webrtc/base/win32.h"
#include "peer_connection_client.h"
#include "webrtc/media/base/mediachannel.h"
#include "webrtc/media/base/videocommon.h"

class MainWndCallback
{
public:
	virtual void StartLogin(const std::string& server, int port) = 0;
	virtual void DisconnectFromServer() = 0;
	virtual void ConnectToPeer(int peer_id) = 0;
	virtual void DisconnectFromCurrentPeer() = 0;
	virtual void UIThreadCallback(int msg_id, void* data) = 0;
	virtual void Close() = 0;
protected:
	virtual ~MainWndCallback() {}
};

// Pure virtual interface for the main window.
class MainWindow
{
public:
	virtual ~MainWindow() {}
 
	virtual void StartLocalRenderer(webrtc::VideoTrackInterface* local_video) = 0;
	virtual void StopLocalRenderer() = 0;
	virtual void StartRemoteRenderer(webrtc::VideoTrackInterface* remote_video) = 0;
	virtual void StopRemoteRenderer() = 0; 
};

#ifdef WIN32

class MainWnd : public MainWindow
{
public: 
	~MainWnd(); 
	MainWnd(HBITMAP* localImgControl, HBITMAP* remoteImgControl);
	void OnPaint();

	void StartLocalRenderer(webrtc::VideoTrackInterface* local_video);

	void StopLocalRenderer();

	void StartRemoteRenderer(webrtc::VideoTrackInterface* remote_video);

	void StopRemoteRenderer();

	 
	class VideoRenderer : public rtc::VideoSinkInterface<webrtc::VideoFrame>
	{
	public:
		VideoRenderer(HWND wnd, int width, int height, webrtc::VideoTrackInterface* track_to_render);
		virtual ~VideoRenderer();

		void Lock()
		{
			::EnterCriticalSection(&buffer_lock_);
		}

		void Unlock()
		{
			::LeaveCriticalSection(&buffer_lock_);
		}

		// VideoSinkInterface implementation
		void OnFrame(const webrtc::VideoFrame& frame) override;

		const BITMAPINFO& bmi() const { return bmi_; }
		const uint8_t* image() const { return image_.get(); }

	protected:
		void SetSize(int width, int height);

		enum
		{
			SET_SIZE,
			RENDER_FRAME,
		};

		HWND wnd_;
		BITMAPINFO bmi_;
		std::unique_ptr<uint8_t[]> image_;
		CRITICAL_SECTION buffer_lock_;
		rtc::scoped_refptr<webrtc::VideoTrackInterface> rendered_track_;
	};

	// A little helper class to make sure we always to proper locking and
	// unlocking when working with VideoRenderer buffers.
	template <typename T>
	class AutoLock
	{
	public:
		explicit AutoLock(T* obj) : obj_(obj) { obj_->Lock(); }
		~AutoLock() { obj_->Unlock(); }
	protected:
		T* obj_;
	};

protected:
	enum ChildWindowID
	{
		EDIT_ID = 1,
		BUTTON_ID,
		LABEL1_ID,
		LABEL2_ID,
		LISTBOX_ID,
	};
	 
private:
	std::unique_ptr<VideoRenderer> local_renderer_;
	std::unique_ptr<VideoRenderer> remote_renderer_;

	HBITMAP* localImgControl;
	HBITMAP* remoteImgControl;
};
#endif  // WIN32

#endif  // WEBRTC_EXAMPLES_PEERCONNECTION_CLIENT_MAIN_WND_H_
