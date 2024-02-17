//
//  ZBType.h
//  ZBType
//
//  Created by Younghwan Mun on 2016-08-24.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//
#include "ZBType.h"
#include <unordered_map>

//
// ZBBytes
//
ZBBytes operator+(ZBBytes lhs, const ZBBytes& rhs)
{
    for (ZBByte b : rhs)
        lhs.push_back(b);
    return lhs;
}

ZBBytes operator+(ZBBytes lhs, ZBByte rhs)
{
    lhs.push_back(rhs);
    return lhs;
}

ZBBytes operator+(ZBByte lhs, ZBBytes rhs)
{
    rhs.insert(rhs.begin(), lhs);
    return rhs;
}

ZBBytes sub(const ZBBytes& bytes, int pos, int len)
{
#if ZB_DEBUG
    if (pos > (int) bytes.size() || (len != -1 && (pos+len) > (int) bytes.size()))
        qDebug() << QString("Memory Error: %1, %2, %3").arg(pos).arg(len).arg(bytes.size());
#endif

    if (len == 0 && pos == (int) bytes.size())
        return ZBBytes();
    else if (len == -1)
        return ZBBytes(bytes.begin()+pos, bytes.end());
    else
        return ZBBytes(bytes.begin()+pos, bytes.begin()+pos+len);
}

ZBInt16 toi16(const ZBBytes& bytes)
{
    return bytes[0] | ZBInt16(bytes[1]) << 8;
}

ZBInt32 toi32(const ZBBytes& bytes)
{
    return bytes[0] | ZBInt32(bytes[1]) << 8 | ZBInt32(bytes[2]) << 16 | ZBInt32(bytes[3]) << 24;
}

ZBInt64 toi64(const ZBBytes& bytes)
{
    ZBInt64 val = 0;
    for (int i = 7; i >= 0; i--)
    {
        val <<= 8;
        val  |= bytes[i];
    }
    return val;
}

ZBBytes toa(ZBInt16 value)
{
    ZBBytes bytes;
#if 0
    bytes.push_back(value&0xF);
    bytes.push_back((value>>8)&0xF);
#else
    bytes.push_back((value>>8)&0xF);
    bytes.push_back(value&0xF);
#endif
    return bytes;
}

ZBBytes toa(ZBInt32 value)
{
    ZBBytes bytes;
#if 0
    bytes.push_back(value&0xF);
    bytes.push_back((value>>8)&0xF);
    bytes.push_back((value>>16)&0xF);
    bytes.push_back((value>>24)&0xF);
#else
    bytes.push_back((value>>24)&0xF);
    bytes.push_back((value>>16)&0xF);
    bytes.push_back((value>>8)&0xF);
    bytes.push_back(value&0xF);
#endif
    return bytes;
}

//
// ZBTime
//
ZBTime::ZBTime(ZBInt32 sec, ZBInt32 msec)
    : sec(sec), msec(msec)
{

}

ZBTime operator+(ZBTime lhs, const ZBTime& rhs)
{
    lhs.sec  += rhs.sec;
    lhs.msec += rhs.msec;
    lhs.sec  += lhs.msec / 1000000;
    lhs.msec %= 1000000;
    return lhs;
}

ZBTime operator-(ZBTime lhs, const ZBTime& rhs)
{
    if (rhs.msec > lhs.msec)
    {
        lhs.sec--;
        lhs.msec += 1000000;
    }

    if (rhs.sec > lhs.sec)
    {
        lhs.sec = 0;
        lhs.msec = 0;
    }
    else
    {
        lhs.sec -= rhs.sec;
        lhs.msec -= rhs.msec;
    }
    return lhs;
}

//
// ZBFrame
//
const ZBString& ZBFrame::protocol() const
{
    return _protocol;
}

const ZBString& ZBFrame::layer() const
{
    return _layer;
}

const ZBString& ZBFrame::type() const
{
    return _type;
}

ZBString ZBFrame::name() const
{
    ZBString info = _protocol;
    info += info.empty() ? _layer : ZBString(" ") + _layer;
    info += info.empty() ? _type : ZBString(" ") + _type;
    return info;
}

const ZBBytes& ZBFrame::hdr() const
{
    return _hdr;
}

const ZBBytes& ZBFrame::payload() const
{
    return _payload;
}

ZBBytes ZBFrame::bin() const
{
    return _hdr + _payload;
}

void ZBFrame::clear()
{
    _hdr.clear();
    _payload.clear();
    _type.clear();
}

bool ZBFrame::empty() const
{
    return _hdr.empty() && _payload.empty();
}

ZBByte ZBFrame::size() const
{
    return ZBByte(_hdr.size()+_payload.size());
}


//
// ZBInfo
//
ZBInt16 ZBInfo::frequency(ZBByte channel)
{
    static std::unordered_map<ZBByte, ZBInt16> table;
    if (table.empty())
    {
        // ZigBee Channels,915 MHz ISM Band
        table[ 1] = 906;
        table[ 2] = 908;
        table[ 3] = 910;
        table[ 4] = 912;
        table[ 5] = 914;
        table[ 6] = 916;
        table[ 7] = 918;
        table[ 8] = 920;
        table[ 9] = 922;
        table[10] =	924;

        // ZigBee Channels,2.4 GHz ISM Band
        table[11] = 2405;
        table[12] = 2410;
        table[13] = 2415;
        table[14] = 2420;
        table[15] = 2425;
        table[16] = 2430;
        table[17] = 2435;
        table[18] = 2440;
        table[19] = 2445;
        table[20] = 2450;
        table[21] = 2455;
        table[22] = 2460;
        table[23] = 2465;
        table[24] = 2470;
        table[25] = 2475;
        table[26] = 2480;
    }
    return table.find(channel) != table.end() ? table[channel] : 0;
}
