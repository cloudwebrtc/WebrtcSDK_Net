/*
 *  Copyright 2012 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#include "stdafx.h"


#include "main_wnd.h"

#include <math.h>

#include "libyuv/convert_argb.h" 
#include "webrtc/api/video/i420_buffer.h"
#include "defaults.h"
#include "webrtc/base/arraysize.h"
#include "webrtc/base/checks.h"
#include "webrtc/base/logging.h"
 
using rtc::sprintfn;

MainWnd::~MainWnd() { 
}

MainWnd::MainWnd(HBITMAP* _localImgControl, HBITMAP* _remoteImgControl)
{
	localImgControl = _localImgControl;
	remoteImgControl = _remoteImgControl;
}
 
void MainWnd::OnPaint()
{
	VideoRenderer* local_renderer = local_renderer_.get();
	VideoRenderer* remote_renderer = remote_renderer_.get();
	if (remote_renderer && local_renderer)
	{
		AutoLock<VideoRenderer> local_lock(local_renderer);
		AutoLock<VideoRenderer> remote_lock(remote_renderer);

		const BITMAPINFO& remotebmi = remote_renderer->bmi();
		int remoteheight = abs(remotebmi.bmiHeader.biHeight);
		int remotewidth = remotebmi.bmiHeader.biWidth;
		const uint8_t* remoteimage = remote_renderer->image();
		if (remoteimage != NULL) {

			StretchDIBits((HDC)* remoteImgControl, 0, 0, 200, 200, 0, 0, remotewidth, remoteheight, remoteimage, &remotebmi, DIB_RGB_COLORS, SRCCOPY);
		}

		const BITMAPINFO& localbmi = local_renderer->bmi();
		int localheight = abs(localbmi.bmiHeader.biHeight);
		int localwidth = localbmi.bmiHeader.biWidth;
		const uint8_t* localimage = local_renderer->image();
		if (remoteimage != NULL) {

			StretchDIBits((HDC)* localImgControl, 0, 0, 200, 200, 0, 0, localwidth, localheight, localimage, &localbmi, DIB_RGB_COLORS, SRCCOPY);
		}
	}
}

void MainWnd::StartLocalRenderer(webrtc::VideoTrackInterface* local_video) {
	auto x = new VideoRenderer((HWND)(*localImgControl), 1, 1, local_video);

	local_renderer_.reset(x);
}

void MainWnd::StopLocalRenderer() {
	local_renderer_.reset();
}

void MainWnd::StartRemoteRenderer(webrtc::VideoTrackInterface* remote_video) {
	remote_renderer_.reset(new VideoRenderer((HWND)* remoteImgControl, 1, 1, remote_video));
}

void MainWnd::StopRemoteRenderer() {
	remote_renderer_.reset();
}

//
// MainWnd::VideoRenderer
//

MainWnd::VideoRenderer::VideoRenderer(
	HWND wnd, int width, int height,
	webrtc::VideoTrackInterface* track_to_render)
	: wnd_(wnd), rendered_track_(track_to_render)
{
	::InitializeCriticalSection(&buffer_lock_);
	ZeroMemory(&bmi_, sizeof(bmi_));
	bmi_.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi_.bmiHeader.biPlanes = 1;
	bmi_.bmiHeader.biBitCount = 32;
	bmi_.bmiHeader.biCompression = BI_RGB;
	bmi_.bmiHeader.biWidth = width;
	bmi_.bmiHeader.biHeight = -height;
	bmi_.bmiHeader.biSizeImage = width * height *
		(bmi_.bmiHeader.biBitCount >> 3);
	rendered_track_->AddOrUpdateSink(this, rtc::VideoSinkWants());
}

MainWnd::VideoRenderer::~VideoRenderer()
{
	rendered_track_->RemoveSink(this);
	::DeleteCriticalSection(&buffer_lock_);
}

void MainWnd::VideoRenderer::SetSize(int width, int height)
{
	AutoLock<VideoRenderer> lock(this);

	if (width == bmi_.bmiHeader.biWidth && height == bmi_.bmiHeader.biHeight)
	{
		return;
	}

	bmi_.bmiHeader.biWidth = width;
	bmi_.bmiHeader.biHeight = -height;
	bmi_.bmiHeader.biSizeImage = width * height * (bmi_.bmiHeader.biBitCount >> 3);
	image_.reset(new uint8_t[bmi_.bmiHeader.biSizeImage]);
}

void MainWnd::VideoRenderer::OnFrame(const webrtc::VideoFrame & video_frame)
{
	AutoLock<VideoRenderer> lock(this);

	rtc::scoped_refptr<webrtc::VideoFrameBuffer> buffer(video_frame.video_frame_buffer());
	if (video_frame.rotation() != webrtc::kVideoRotation_0)
	{
		buffer = webrtc::I420Buffer::Rotate(*buffer, video_frame.rotation());
	}

	SetSize(buffer->width(), buffer->height());

	RTC_DCHECK(image_.get() != NULL);
	libyuv::I420ToARGB(buffer->DataY(), buffer->StrideY(),
		buffer->DataU(), buffer->StrideU(),
		buffer->DataV(), buffer->StrideV(),
		image_.get(),
		bmi_.bmiHeader.biWidth *
		bmi_.bmiHeader.biBitCount / 8,
		buffer->width(), buffer->height());

	InvalidateRect(wnd_, NULL, TRUE);
}
