//
//  ZTCFrame.cpp
//  ZTCFrame
//
//  Created by Younghwan Mun on 2016-08-08.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//
#include "ZTCFrame.h"

//
// ZTCFrame
//
ZTCFrame::ZTCFrame(const ZBBytes& bin)
{
    if (bin.empty())
        return;

    ZBByte offset = 0;
    offset += 2; // opCode
    offset += 1; // payload length

    _hdr = sub(bin, 0, offset);
    _payload = sub(bin, offset);
    _layer = "ZTC";
}

ZTCFrame::ZTCFrame(ZBInt16 code, const ZBBytes& payload)
{
    *this = ZTCFrame(ZBBytes({ZBByte(code >> 8), ZBByte(code & 0xFF)}) + ZBByte(payload.size()) + payload);
}

ZBInt16 ZTCFrame::opCode() const
{
    return ZBInt16(_hdr[0]) << 8 | ZBInt16(_hdr[1]);
}

ZBByte ZTCFrame::status() const
{
    return _payload[0];
}

ZTCFrameInfo ZTCFrame::info() const
{
    ZBInt16 code = opCode();
    ZTCFrameInfo info = ZTCFrameInfo::get(code);
    switch(code)
    {
    case MAC_SetPIBAttribute_Request: info.setAttribute(_payload[0]); break;
    case MAC_SetPIBAttribute_Confirm: info.setAttribute(_payload[1]); info.setStatus(status()); break;

    case ZTC_ModeSelect_Confirm: info.setStatus(status()); break;
    }
    return info;
}

//
// ZTCDataFrame
//
ZTCDataFrame::ZTCDataFrame(const ZTCFrame& other)
    : ZTCFrame(!other.empty() && other.opCode() == MAC_Purge ? other.bin() : ZBBytes())
{
    if (empty())
        return;

    ZBByte offset = 0;
    offset += 1;        // link quality
    offset += 4;        // timestamp
    offset += 1;        // payload length


    _hdr = _hdr + sub(_payload, 0, offset);
    _payload = sub(_payload, offset);
    _type = "Data";

}

ZBByte ZTCDataFrame::linkQuality() const
{
    return _hdr[3];
}

ZBTime ZTCDataFrame::timeStamp() const
{
    ZBInt32 ts = toi32(sub(_hdr, 4, 4)) * MICROS_PER_SYMBOL;
    return {ts/1000000, ts%1000000};
}
