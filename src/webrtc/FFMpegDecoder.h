/*
 *  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 *
 */
//
// Created by anba8005 on 28/02/2020.
//

#ifndef NDI_WEBRTC_PEER_WORKER_FFMPEGDECODER_H
#define NDI_WEBRTC_PEER_WORKER_FFMPEGDECODER_H

#define OWT_ENABLE_H265

extern "C" {
#include "libavcodec/avcodec.h"
}

#include "media/base/codec.h"
#include "modules/video_coding/include/video_codec_interface.h"
#include "common_video/include/i420_buffer_pool.h"
#include "rtc_base/system/rtc_export.h"

struct AVCodecContextDeleter {
    void operator()(AVCodecContext *ptr) const { avcodec_free_context(&ptr); }
};

struct AVFrameDeleter {
    void operator()(AVFrame *ptr) const { av_frame_free(&ptr); }
};

class RTC_EXPORT FFMpegDecoder : public webrtc::VideoDecoder {
public:
    explicit FFMpegDecoder();

    ~FFMpegDecoder() override;

    int32_t InitDecode(const webrtc::VideoCodec *codec_settings, int32_t number_of_cores) override;

    int32_t Release() override;

    int32_t RegisterDecodeCompleteCallback(webrtc::DecodedImageCallback *callback) override;

    // |missing_frames|, |fragmentation| and |render_time_ms| are ignored.
    int32_t
    Decode(const webrtc::EncodedImage &input_image, bool /*missing_frames*/, int64_t render_time_ms = -1) override;

    const char *ImplementationName() const override;

    static bool IsSupported();

    static std::unique_ptr<FFMpegDecoder> Create();

private:
    bool IsInitialized() const;

    webrtc::I420BufferPool pool_;
    std::unique_ptr<AVCodecContext, AVCodecContextDeleter> av_context_;
    std::unique_ptr<AVFrame, AVFrameDeleter> av_frame_;

    webrtc::DecodedImageCallback *decoded_image_callback_;

};

#endif //NDI_WEBRTC_PEER_WORKER_FFMPEGDECODER_H