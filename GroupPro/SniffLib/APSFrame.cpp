//
//  APSFrame.cpp
//  APSFrame (Zigbee)
//
//  Created by Younghwan Mun on 2016-09-07.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//
#include "APSFrame.h"

//
// APSFrame
//
APSFrame::APSFrame(const NWKFrame& frame) : APSFrame(NWKDataFrame(frame))
{

}

APSFrame::APSFrame(const NWKDataFrame& frame) : APSFrame(frame.payload())
{

}

APSFrame::APSFrame(const ZBBytes& bin)
{
    if (bin.empty())
        return;

    ZBByte off = 0;

    off += 1; // frame control
    off += 1; // destination Endpoint
    off += 2; // cluster Identifier
    off += 2; // profile Identifier
    off += 1; // source Endpoint
    off += 1; // counter

    _hdr = sub(bin, 0, off);
    _payload = sub(bin, off);
    _protocol = NWKFrameInfo::protocol(NWK_PROTOCOL_ZIGBEE);
    _layer = "APS";
    _type = APSFrameInfo::type(frameControl().frmType);
}

APSFrame::FrameControl APSFrame::frameControl() const
{
    FrameControl ctl;
    ctl.spec = _hdr[0];
    ctl.ackFormatInfo = APSFrameInfo::ackFormat(ctl.ackFormat);
    ctl.deliveryModeInfo = APSFrameInfo::deliveryMode(ctl.deliveryMode);
    return ctl;
}

ZBByte APSFrame::dstEndpoint() const
{
    return _hdr[1];
}

ZBInt16 APSFrame::clusterId() const
{
    return toi16(sub(_hdr, 2, 2));
}

ZBInt16 APSFrame::profileId() const
{
    return toi16(sub(_hdr, 4, 2));
}

ZBByte APSFrame::srcEndpoint() const
{
    return _hdr[6];
}

ZBByte APSFrame::counter() const
{
    return _hdr[7];
}

ZBString APSFrame::cluster() const
{
    return APSFrameInfo::cluster(clusterId());
}

ZBString APSFrame::profile() const
{
    return APSFrameInfo::profile(profileId());
}

//
// APSDataFrame
//
APSDataFrame::APSDataFrame(const APSFrame& other)
    : APSFrame(!other.empty() && other.frameControl().frmType == APS_FRAME_DATA ? other.bin() : ZBBytes())
{

}
