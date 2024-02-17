//
//  SavedFrames.cpp
//  SavedFrames
//
//  Created by Younghwan Mun on 2016-08-22.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//
#include "SavedFrames.h"
#include "SavedPreference.h"
#include "Helper.h"





static const SavedFrames::Saved empty =
{
    0, {0,0}, {0,0}, 0, 0, {}
};

SavedFrames* SavedFrames::instance()
{
    static SavedFrames inst;
    return &inst;
}

SavedFrames::SavedFrames()
{
	savPacket = { 1,{ 0, 0 } };
	savFrame = { 1,{ 0, 0 } };
}

SavedFrames::~SavedFrames()
{

}

void SavedFrames::clearAll()
{
    savPacket = {1, {0, 0}};
    savFrame.idx = 1;
	savFrame.time.msec = 0;
	savFrame.time.sec = 0;

    packets.clear();
    frames.clear();
}

ZBInt32 SavedFrames::addPacket(ZBByte type, const ZBBytes& bin)
{
    ZBInt32& idx = savPacket.idx;
    ZBTime  time = currentTime();
    ZBTime delta = time - (idx == 1 ? time : savPacket.time);
    savPacket.time = time;
    packets[idx] = {idx, time, delta, (ZBByte) SavedPreference::instance()->serialInfo().channel, type, bin};
    return idx++;
}

ZBInt32 SavedFrames::addFrame(ZBByte type, const ZBBytes& bin)
{
    ZBInt32& idx = savFrame.idx;
    ZBTime  time = currentTime();
    ZBTime delta = time - (idx == 1 ? time : savFrame.time);
    savFrame.time = time;
    frames[idx] = {idx, time, delta, (ZBByte) SavedPreference::instance()->serialInfo().channel, type, bin};
    return idx++;
}

const SavedFrames::Saved& SavedFrames::packet(ZBInt32 idx)
{
    auto iter = packets.find(idx);
    return iter != packets.end() ? iter->second : empty;
}

const SavedFrames::Saved& SavedFrames::frame(ZBInt32 idx)
{
    auto iter = frames.find(idx);
    return iter != frames.end() ? iter->second : empty;
}

const ZTCFrame SavedFrames::ztcFrame(ZBInt32 idx)
{
    return ZTCFrame(frame(idx).bin);
}

const ZTCDataFrame SavedFrames::ztcDataFrame(ZBInt32 idx)
{
    return ztcFrame(idx);
}

const MACFrame SavedFrames::macFrame(ZBInt32 idx)
{
    return ztcDataFrame(idx);
}

const MACBeaconFrame SavedFrames::macBeaconFrame(ZBInt32 idx)
{
    return macFrame(idx);
}

const MACDataFrame SavedFrames::macDataFrame(ZBInt32 idx)
{
     return macFrame(idx);
}

const MACAckFrame SavedFrames::macAckFrame(ZBInt32 idx)
{
    return macFrame(idx);
}

const MACCommandFrame SavedFrames::macCommandFrame(ZBInt32 idx)
{
    return macFrame(idx);
}

const NWKFrame SavedFrames::nwkFrame(ZBInt32 idx)
{
    return macDataFrame(idx);
}

const NWKDataFrame SavedFrames::nwkDataFrame(ZBInt32 idx)
{
    return nwkFrame(idx);
}

const NWKCommandFrame SavedFrames::nwkCommandFrame(ZBInt32 idx)
{
    return nwkFrame(idx);
}

const NWKLayerInfo SavedFrames::nwkLayerInfo(ZBInt32 idx)
{
    return macBeaconFrame(idx);
}
