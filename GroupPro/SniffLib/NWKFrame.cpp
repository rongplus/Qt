//
//  NWKFrame.cpp
//  NWKFrame (Zigbee)
//
//  Created by Younghwan Mun on 2016-08-22.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//
#include "NWKFrame.h"

//
// NWKFrame
//
NWKFrame::NWKFrame(const MACDataFrame& frame) : NWKFrame(frame.payload())
{

}

NWKFrame::NWKFrame(const ZBBytes& bin)
{   
    if (bin.empty())
        return;
	if (bin.size() < 10)
		return;

    ZBByte offset = 0;

    // frame control
    _ctl.spec = toi16(sub(bin, 0, 2));
    offset += 2;
    if (_ctl.protocolVer != NWK_PROTOCOL_VERSION)
        return;

    // destination address
    offset += 2;

    // source address
	
	{
		_src16 = toi16(sub(bin, offset, 2));
		offset += 2;
	}

    // radius
    offset += 1;

    // sequence number
    offset += 1;

    if (_ctl.dstIEEEAddr16)
    {
        _dst16 = toi16(sub(bin, offset, 2));
        offset += 2;
    }

    if (_ctl.srcIEEEAddr64)
    {
        _src64 = toi64(sub(bin, offset, 8));
        offset += 8;
    }

    if (_ctl.multicast)
        _multi = bin[offset++];

    if (_ctl.srcRoute)
    {
        _sub.spec.push_back(_sub.cnt = bin[offset++]);
        _sub.spec.push_back(_sub.idx = bin[offset++]);
        for (ZBByte i = 0; i < _sub.cnt; i++)
        {
            ZBBytes l = sub(bin, offset, 2);
            offset += 2;

            _sub.spec = _sub.spec + l;
            _sub.list.push_back(toi16(l));
        }
    }

    _hdr = sub(bin, 0, offset);
    _payload = sub(bin, offset);
    _protocol = NWKFrameInfo::protocol(NWK_PROTOCOL_ZIGBEE);
    _layer = "NWK";    
    _type = NWKFrameInfo::type(frameControl().type);
}

const NWKFrame::FrameControl& NWKFrame::frameControl() const
{
    return _ctl;
}

ZBInt16 NWKFrame::dstAddr() const
{
    return toi16(sub(_hdr, 2, 2));
}

ZBInt16 NWKFrame::srcAddr() const
{
    return toi16(sub(_hdr, 4, 2));
}

ZBInt16 NWKFrame::srcAddr16() const
{
	return _src16;
}

ZBByte NWKFrame::radius() const
{
    return _hdr[6];
}

ZBByte NWKFrame::sequence() const
{
    return _hdr[7];
}

ZBInt64 NWKFrame::dstIEEEAddr() const
{
    return _dst16;
}

ZBInt64 NWKFrame::srcIEEEAddr() const
{
    return _src64;
}

ZBByte NWKFrame::multicast() const
{
    return _multi;
}

const NWKFrame::RouteSubFrame& NWKFrame::srcRouteSubFrame() const
{
    return _sub;
}

const ZBString NWKFrame::discoverRoute() const
{
    return NWKFrameInfo::discoverRoute(_ctl.discoverRoute);
}

//
// NWKDataFrame
//
NWKDataFrame::NWKDataFrame(const NWKFrame& other)
    : NWKFrame(!other.empty() && other.frameControl().type == NWK_FRAME_DATA ? other.bin() : ZBBytes())
{

}

//
// NWKCommandFrame
//
NWKCommandFrame::NWKCommandFrame(const NWKFrame& other)
    : NWKFrame(!other.empty() && other.frameControl().type == NWK_FRAME_COMMAND ? other.bin() : ZBBytes())
{  
    if (empty())
        return;

    ZBByte offset = 0;

    // command
    offset += 1;

    _hdr = _hdr + sub(_payload, 0, offset);
    _payload = sub(_payload, offset);
}

NWKCommandFrame::~NWKCommandFrame()
{

}
ZBByte NWKCommandFrame::command() const
{
    return *(_hdr.end()-1);
}

ZBString NWKCommandFrame::commandInfo() const
{
    return NWKFrameInfo::command(command());
}

NWKCommandFrame::RouteRequest NWKCommandFrame::routeRequest() const
{

	RouteRequest cmd;
	if (_payload.size() >= 13)
	{
		cmd.opt = _payload[0];
		cmd.reqId = _payload[1];
		cmd.dstAddr16 = toi16(sub(_payload, 2, 2));
		cmd.pathCost = _payload[4];
		cmd.dstAddr64 = toi64(sub(_payload, 5, 8));
		cmd.manyToOneInfo = NWKFrameInfo::manyToOne(cmd.manyToOne);
	}
    return cmd;
}

NWKCommandFrame::RouteReply NWKCommandFrame::routeReply() const
{
    RouteReply cmd;
    cmd.opt = _payload[0];
    cmd.reqId = _payload[1];
    cmd.srcAddr = toi16(sub(_payload, 2, 2));
    cmd.dstAddr = toi16(sub(_payload, 4, 2));
    cmd.pathCost = _payload[6];
    return cmd;
}

NWKCommandFrame::NetworkStatus NWKCommandFrame::networkStatus() const
{
    NetworkStatus cmd;
    cmd.status = _payload[0];
    cmd.dstAddr16 = toi16(sub(_payload, 1, 2));
    cmd.statusInfo = NWKFrameInfo::networkStatus(cmd.status);
    return cmd;
}

NWKCommandFrame::Leave NWKCommandFrame::leave() const
{
    Leave cmd;
    cmd.opt = _payload[0];
    return cmd;
}

NWKCommandFrame::LinkStatus NWKCommandFrame::linkStatus() const
{
    ZBByte offset = 0;

    LinkStatus cmd;
    cmd.opt = _payload[offset++];
    for (ZBByte i = 0; i < cmd.linkCnt; i++)
    {
        Link link;
        link.bin = sub(_payload, offset, 3);
        link.neighbor = toi16(link.bin);
        link.status = link.bin[2];
        cmd.links.push_back(link);
        offset += 3;
    }
    return cmd;
}

//
// NWKLayerInfo
//
NWKLayerInfo::NWKLayerInfo(const MACBeaconFrame& frame)
{
    const ZBBytes& bin = frame.payload();
    if (bin.empty() || bin[0] != NWK_PROTOCOL_ZIGBEE)
        return;

    ZBByte offset = 14;

    _hdr = sub(bin, 0, offset);
    _payload = sub(bin, offset);
    _protocol = NWKFrameInfo::protocol(protocolID());
    _layer = "NWK";
    _type = "Beacon";
}

ZBByte NWKLayerInfo::protocolID() const
{
    return _hdr[0];
}

ZBByte NWKLayerInfo::stackProfile() const
{
    return _hdr[1] & 0x0f;
}

ZBByte NWKLayerInfo::protocolVer() const
{
    return (_hdr[1] >> 4) & 0x0f;
}

ZBByte NWKLayerInfo::rsvd() const
{
    return _hdr[2] & 0x03;
}

ZBByte NWKLayerInfo::routerCapacity() const
{
    return (_hdr[2] >> 2) & 0x01;
}

ZBByte NWKLayerInfo::deviceDepth() const
{
    return (_hdr[2] >> 3) & 0x0f;
}

ZBByte NWKLayerInfo::endDeviceCapacity() const
{
    return (_hdr[2] >> 7) & 0x01;
}

ZBInt64 NWKLayerInfo::extendedPanID() const
{
    return toi64(sub(_hdr, 3, 8));
}

ZBInt32 NWKLayerInfo::transmitOffset() const
{
    return _hdr[11] | ZBInt32(_hdr[12]) << 8 | ZBInt32(_hdr[13]) << 16;
}

ZBByte NWKLayerInfo::updateID() const
{
	return 0;// _hdr[14];
}

ZBString NWKLayerInfo::profile() const
{
    return NWKFrameInfo::profile(stackProfile());
}
