//
//  MACFrame.cpp
//  MACFrame (IEEE 802.15.4)
//
//  Created by Younghwan Mun on 2016-08-22.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//
#include "MACFrame.h"

//
// MACFrame
//
MACFrame::MACFrame(const ZTCDataFrame& frame) : MACFrame(frame.payload())
{

}

MACFrame::MACFrame(const ZBBytes& bin)
{
    if (bin.empty())
        return;

    ZBByte offset = 0;

    // frame control
    _ctl.spec = toi16(sub(bin, 0, 2));
    offset += 2;

    // sequence number
    offset += 1;

    // addressing
    switch (_ctl.dstMode)
    {
    case MAC_ADDRESS_MODE_INDIRECT:
        break;

    case MAC_ADDRESS_MODE_GROUP:
    case MAC_ADDRESS_MODE_16_BIT:
        _addr.dstPanID = toi16(sub(bin, offset, 2));
        offset += 2;
        _addr.dstAddr16 = toi16(sub(bin, offset, 2));
        offset += 2;
        break;

    case MAC_ADDRESS_MODE_64_BIT:
        _addr.dstPanID = toi16(sub(bin, offset, 2));
        offset += 2;
        _addr.dstAddr64 = toi64(sub(bin, offset, 8));
        offset += 8;
        break;
    }

    switch (_ctl.srcMode)
    {
    case MAC_ADDRESS_MODE_INDIRECT:
        break;

    case MAC_ADDRESS_MODE_GROUP:
    case MAC_ADDRESS_MODE_16_BIT:
        if (!_ctl.intraPAN)
        {
            _addr.srcPanID = toi16(sub(bin, offset, 2));
            offset += 2;
        }
        _addr.srcAddr16 = toi16(sub(bin, offset, 2));
        offset += 2;
        break;

    case MAC_ADDRESS_MODE_64_BIT:
        if (!_ctl.intraPAN)
        {
            _addr.srcPanID = toi16(sub(bin, offset, 2));
            offset += 2;
        }
        _addr.srcAddr64 = toi64(sub(bin, offset, 8));
        offset += 8;
        break;
    }

    _hdr = sub(bin, 0, offset);
    _payload = sub(bin, offset);
    _protocol = "IEEE 802.15.4";
    _layer = "MAC";
    _type = MACFrameInfo::type(frameControl().type);
}

const MACFrame::FrameControl& MACFrame::frameControl() const
{
    return _ctl;
}

ZBByte MACFrame::sequence() const
{
    return _hdr[2];
}

const MACFrame::Addressing& MACFrame::addressing() const
{   
     return _addr;
}

ZBString MACFrame::srcAddrMode() const
{
    return MACFrameInfo::addrMode(_ctl.srcMode);
}

ZBString MACFrame::dstAddrMode() const
{
    return MACFrameInfo::addrMode(_ctl.dstMode);
}

//
// MACBeaconFrame
//
MACBeaconFrame::MACBeaconFrame(const MACFrame& other)
    : MACFrame(!other.empty() && other.frameControl().type == MAC_FRAME_BEACON ? other.bin() : ZBBytes())
{
    if (empty() || _payload.size() < 4)
        return;

    ZBByte offset = 0;

    // super frame
    _superFrm.spec = toi16(sub(_payload, 0, 2));
    offset += 2;

    // gts
    _gts.spec = _payload[offset];
    offset += 1;
#if ZB_DEBUG
    Q_ASSERT_X (_gts.descCnt == 0, "MACBeaconFrame", QString("GTS: %1").arg(uint(_gts.descCnt)).toStdString().c_str());
#endif
    _gts.descriptors = sub(_payload, offset, _gts.descCnt);
    offset += _gts.descriptors.size();

    // pending address
    _pendAddr.spec = _payload[offset];
    offset += 1;
#if ZB_DEBUG
    Q_ASSERT_X (_pendAddr.addr16Cnt == 0, "MACBeaconFrame", QString("PendingAddress 16: %1").arg(uint(_pendAddr.addr16Cnt)).toStdString().c_str());
    Q_ASSERT_X (_pendAddr.addr16Cnt == 0, "MACBeaconFrame", QString("PendingAddress 16: %1").arg(uint(_pendAddr.addr16Cnt)).toStdString().c_str());
#endif
    for (ZBByte i = 0; i < _pendAddr.addr16Cnt; i++)
    {
       _pendAddr.addr16s.push_back(toi16(sub(_payload, offset, 2)));
       offset += 2;
    }
    for (ZBByte i = 0; i < _pendAddr.addr64Cnt; i++)
    {
        _pendAddr.addr64s.push_back(toi64(sub(_payload, offset, 8)));
        offset += 8;
    }

    _hdr = _hdr + sub(_payload, 0, offset);
    _payload = sub(_payload, offset);
    _type = MACFrameInfo::type(frameControl().type);
}

const MACBeaconFrame::SuperFrame& MACBeaconFrame::superFrame() const
{
    return _superFrm;
}

const MACBeaconFrame::GTS& MACBeaconFrame::gts() const
{
    return _gts;
}

const MACBeaconFrame::PendingAddress& MACBeaconFrame::pendingAddress() const
{
    return _pendAddr;
}

//
// MACDataFrame
//
MACDataFrame::MACDataFrame(const MACFrame& other)
    : MACFrame(!other.empty() && other.frameControl().type == MAC_FRAME_DATA ? other.bin() : ZBBytes())
{

}

//
// MACAckFrame
//
MACAckFrame::MACAckFrame(const MACFrame& other)
    : MACFrame(!other.empty() && other.frameControl().type == MAC_FRAME_ACKNOWLEDGE ? other.bin() : ZBBytes())
{

}

//
// MACCommandFrame
//
MACCommandFrame::MACCommandFrame(const MACFrame& other)
    : MACFrame(!other.empty() && other.frameControl().type == MAC_FRAME_COMMAND ? other.bin() : ZBBytes())
{
    if (empty())
        return;

    ZBByte offset = 0;

    // command
    offset += 1;

    _hdr = _hdr + sub(_payload, 0, offset);
    _payload = sub(_payload, offset);
}

ZBByte MACCommandFrame::command() const
{
    return *(_hdr.end()-1);
}

ZBString MACCommandFrame::commandInfo() const
{
    return MACFrameInfo::command(command());
}
