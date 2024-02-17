#pragma once
//
//  SavedFrames.h
//  SavedFrames
//
//  Created by Younghwan Mun on 2016-08-22.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//

#include "MACFrame.h"
#include "ZTCFrame.h"
#include "NWKFrame.h"
#include <unordered_map>
#include "snifflib_global.h"

struct ZBCurSaved
{
	ZBInt32 idx;
	ZBTime  time;
};


class  SNIFFLIB_EXPORT SavedFrames
{
public:
    struct Saved
    {
        ZBInt32      idx;
        ZBTime       time;
        ZBTime       delta;
        ZBByte       channel;
        ZBByte       type;
        ZBBytes      bin;
    };
	/**
	* @brief SavedFrames constructor
	*/
	SavedFrames();
    /**
     * @brief instance() returns singleton class instance
     * @return instance
     */
    static SavedFrames* instance();

    /**
     * @brief clearAll() removes packets and frames
     */
    void clearAll();

    /**
     * @brief addRawPacket() stores packet binary
     * @param type
     * @param bin
     * @return idx
     */
    ZBInt32 addPacket(ZBByte type, const ZBBytes& bin);

    /**
     * @brief addFrame() stores frame binary
     * @param type
     * @param bin
     * @return idx
     */
    ZBInt32 addFrame(ZBByte type, const ZBBytes& bin);

    /**
     * @brief packet getter
     * @param idx
     * @return packet information
     */
    const Saved& packet(ZBInt32 idx);

    /**
     * @brief frame getter
     * @param idx
     * @return frame information
     */
    const Saved& frame(ZBInt32 idx);

    /**
     * @brief ztcFrame() returns ztc frame
     * @param idx
     * @return ZTCFrame
     */
    const ZTCFrame ztcFrame(ZBInt32 idx);

    /**
     * @brief ztcDataFrame() returns ztc data frame
     * @param idx
     * @return ZTCDataFrame
     */
    const ZTCDataFrame ztcDataFrame(ZBInt32 idx);

    /**
     * @brief macFrame() returns mac frame
     * @param idx
     * @return MACFrame
     */
    const MACFrame macFrame(ZBInt32 idx);

    /**
     * @brief macBeaconFrame() returns mac beacon frame
     * @param idx
     * @return MACBeaconFrame
     */
    const MACBeaconFrame macBeaconFrame(ZBInt32 idx);

    /**
     * @brief macDataFrame() returns mac data frame
     * @param idx
     * @return MACDataFrame
     */
    const MACDataFrame macDataFrame(ZBInt32 idx);

    /**
     * @brief macAckFrame() returns mac acknowledge frame
     * @param idx
     * @return MACAckFrame
     */
    const MACAckFrame macAckFrame(ZBInt32 idx);

    /**
     * @brief macCommandFrame() returns mac command frame
     * @param idx
     * @return MACCommandFrame
     */
    const MACCommandFrame macCommandFrame(ZBInt32 idx);

    /**
     * @brief nwkFrame() returns network frame
     * @param idx
     * @return NWKFrame
     */
    const NWKFrame nwkFrame(ZBInt32 idx);

    /**
     * @brief nwkDataFrame() returns network data frame
     * @param idx
     * @return NWKDataFrame
     */
    const NWKDataFrame nwkDataFrame(ZBInt32 idx);

    /**
     * @brief nwkCommandFrame() returns network command frame
     * @param idx
     * @return NWKCommandFrame
     */
    const NWKCommandFrame nwkCommandFrame(ZBInt32 idx);

    /**
     * @brief nwkLayerInfo() returns network layer informtion
     * @param idx
     * @return NWKFrame
     */
    const NWKLayerInfo nwkLayerInfo(ZBInt32 idx);

private:
    

    /**
     * @brief SavedFrames destructor
     */
    ~SavedFrames();

private:
     std::unordered_map<ZBInt32, Saved> packets;
     std::unordered_map<ZBInt32, Saved> frames;


	 ZBCurSaved savPacket;
	 ZBCurSaved savFrame ;
};
