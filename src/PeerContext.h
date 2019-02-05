//
// Created by anba8005 on 12/20/18.
//

#ifndef GYVAITV_WEBRTC_PEERCONTEXT_H
#define GYVAITV_WEBRTC_PEERCONTEXT_H

#include "Signaling.h"

#include <string>
#include <memory>

#include "pc/peerconnectionfactory.h"
#include "api/peerconnectioninterface.h"
#include "PeerFactoryContext.h"
#include "api/test/fakeconstraints.h"
#include "NDIWriter.h"
#include "NDIReader.h"

using namespace std;

const string COMMAND_SET_LOCAL_DESCRIPTION = "setLocalDescription";
const string COMMAND_SET_REMOTE_DESCRIPTION = "setRemoteDescription";
const string COMMAND_ADD_ICE_CANDIDATE = "addIceCandidate";
const string COMMAND_CREATE_OFFER = "createOffer";
const string COMMAND_CREATE_ANSWER = "createAnswer";
const string COMMAND_CREATE_DATA_CHANNEL = "createDataChannel";

class PeerContext : public webrtc::PeerConnectionObserver {
public:
    PeerContext(shared_ptr<Signaling> signaling);

    virtual ~PeerContext();

    //

    void start();

    void end();

    void processMessages();

    //

    void setLocalDescription(const string &type, const string &sdp, int64_t correlation);

    void setRemoteDescription(const string &type, const string &sdp, int64_t correlation);

    void addIceCandidate(const string &mid, int mlineindex, const string &sdp, int64_t correlation);

    void createAnswer(int64_t correlation);

    void createOffer(int64_t correlation);

    void createDataChannel(const string &name, int64_t correlation);

protected:

    shared_ptr<PeerFactoryContext> context;
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> pc;
    rtc::scoped_refptr<webrtc::DataChannelInterface> dc;

    shared_ptr<Signaling> signaling;

    // inherited from PeerConnectionObserver

    void OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) override;

    void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state) override;

    void OnRenegotiationNeeded() override;

    void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state) override;

    void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state) override;

    void OnIceCandidate(const webrtc::IceCandidateInterface *candidate) override;

    void OnAddTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
                    const vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>> &streams) override;

    void OnRemoveTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver) override;

private:

    unique_ptr<webrtc::SessionDescriptionInterface> createSessionDescription(const string &type_str, const string &sdp);

    unique_ptr<NDIWriter> writer;

    unique_ptr<NDIReader> reader;

    void addTracks();

    size_t totalTracks;
};


#endif //GYVAITV_WEBRTC_PEERCONTEXT_H