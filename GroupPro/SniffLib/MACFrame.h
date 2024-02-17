#pragma once
//
//  MACFrame.h
//  MACFrame (IEEE 802.15.4)
//
//  Created by Younghwan Mun on 2016-08-22.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//

#include "ZBType.h"
#include "MACFrameDef.h"
#include "MACFrameInfo.h"
#include "ZTCFrame.h"
#include "snifflib_global.h"
/**
 * @brief Generic MAC frame format
 *
 * Frame Control Field [2      bytes]
 * Sequence Number     [1      byte ]
 * Addressing Fields   [0/4/10 bytes]
 * Frame Payload       [n      bytes]
 * FCS                 [2      bytes] : Unsupported
 */
class SNIFFLIB_EXPORT MACFrame : public ZBFrame
{
public:
    /**
     * @brief Frame control field format
     *
     * Frame Type                  [3 bits]
     * Security Enabled            [1 bit ]
     * Frame Pending               [1 bit ]
     * Acknowledge Request         [1 bit ]
     * Intra-PAN                   [1 bit ]
     * Reserved                    [3 bits]
     * Destination Addressing Mode [2 bits]
     * Frame Version               [2 bits]
     * Source Addressing Mode      [2 bits]
     */
    union FrameControl
    {
        ZBInt16 spec;
        struct
        {
            ZBByte type         : 3; // 0-2
            ZBByte security     : 1; // 3
            ZBByte pending      : 1; // 4
            ZBByte ackRequest   : 1; // 5
            ZBByte intraPAN     : 1; // 6
            ZBByte reserved     : 1; // 7-9 (Caution Bitpack)
            ZBByte reserved2    : 2;
            ZBByte dstMode      : 2; // 10-11
            ZBByte version      : 2; // 12-13
            ZBByte srcMode      : 2; // 14-15
        };
    };

    /**
     * @brief Addressing fields format
     *
     * Destination PAN ID  [0/2   bytes]
     * Destination Address [0/2/8 bytes]
     * Source PAN ID       [0/2   bytes]
     * Source Address      [0/2/8 bytes]
     */
    struct Addressing
    {
        ZBInt16 dstPanID;
        union
        {
            ZBInt16 dstAddr16;
            ZBInt64 dstAddr64;
        };
        ZBInt16 srcPanID;
        union
        {
            ZBInt16 srcAddr16;
            ZBInt64 srcAddr64;
        };
    };

    /**
     * @brief MACFrame constructor
     * @param frame
     */
    MACFrame(const ZTCDataFrame& frame = {});

    /**
     * @brief frameControl() returns frame control information
     * @return frame control
     */
    const FrameControl& frameControl() const;

    /**
     * @brief sequence() returns sequence number
     * @return sequence number
     */
    ZBByte sequence() const;

    /**
     * @brief addressing() returns addressing fields
     * @return addressing fields
     */
    const Addressing& addressing() const;

    /**
     * @brief srcAddrMode() returns source addressing mode information
     * @return addressing mode information
     */
    ZBString srcAddrMode() const;

    /**
     * @brief dstAddrMode() returns destination addressing mode information
     * @return addressing mode information
     */
    ZBString dstAddrMode() const;


    /**
     * @brief MACFrame constructor
     * @param bin
     */
    MACFrame(const ZBBytes& bin);

private:
    FrameControl _ctl;
    Addressing   _addr;
};

/**
 * @brief MAC Beacon frame format
 *
 * Frame Control Field      [2    bytes]
 * Sequence Number          [1    byte ]
 * Addressing Fields        [4/10 bytes]
 * Superframe Specification [2    bytes]
 * GTS Fields               [n    bytes]
 * Pending Address Fields   [n    bytes]
 * Beacon Payload           [n    bytes]
 * FCS                      [2    bytes] : Unsupported
 */
class SNIFFLIB_EXPORT MACBeaconFrame : public MACFrame
{
public:
    /**
     * @brief SuperFrame Specification field format
     *
     * Beacon Interval        [4 bits]
     * Superframe Interval    [4 bits]
     * Final Cap Slot         [4 bits]
     * Battery Life Extension [1 bit ]
     * Reserved               [1 bit ]
     * PAN Coordinator        [1 bit ]
     * Association Permit     [1 bit ]
     */
    union SuperFrame
    {
        ZBInt16 spec;
        struct
        {
            ZBByte beaconInterval    : 4; // 0-3
            ZBByte superFrameInterval: 4; // 4-7
            ZBByte finalCapSlot      : 4; // 8-11
            ZBByte batteryExtension  : 1; // 12
            ZBByte reserved          : 1; // 13
            ZBByte panCoordinator    : 1; // 14
            ZBByte associationPermit : 1; // 15
        };
    };

    /**
     * @brief GTS fields format
     *
     * Descriptor Count [3 bits ]
     * Reserved         [4 bits ]
     * Permit           [1 bit  ]
     * Descriptors      [n bytes]
     */
    struct GTS
    {
        union
        {
            ZBByte spec;
            struct
            {
                ZBByte descCnt   : 3; // 0-2
                ZBByte reserved  : 1; // 3
                ZBByte reserved2 : 3; // 4-6
                ZBByte permit    : 1; // 7
            };
        };
        ZBBytes descriptors;
    };

    /**
     * @brief Pending Address fields format
     *
     * Short Address Count    [3 bits]
     * Reserved               [1 bit ]
     * Extended Address Count [3 bits]
     * Reserved               [1 bit ]
     * Short Address          [n bytes] : 16-bit addresses
     * Extended Address       [n bytes] : 64-bit addresses
     */
    struct PendingAddress
    {
        union
        {
            ZBByte spec;
            struct
            {
                ZBByte addr16Cnt : 3; // 0-2
                ZBByte reserved  : 1; // 3
                ZBByte addr64Cnt : 3; // 4-6
                ZBByte reserved2 : 1; // 7
            };
        };
        ZBInt16s addr16s;
        ZBInt64s addr64s;
    };

    /**
     * @brief MACBeaconFrame constructor
     * @param other
     */
    MACBeaconFrame(const MACFrame& other = {});

    /**
     * @brief superFrame() returns superframe specification
     * @return superframe specification
     */
    const SuperFrame& superFrame() const;

    /**
     * @brief gts() returns GTS fields
     * @return gts fields
     */
    const GTS& gts() const;

    /**
     * @brief pendingAddress() returns pending address fields
     * @return pending address fields
     */
    const PendingAddress& pendingAddress() const;

private:
    SuperFrame     _superFrm;
    GTS            _gts;
    PendingAddress _pendAddr;
};

/**
 * @brief MAC Data frame format
 *
 * Frame Control Field [2    bytes]
 * Sequence Number     [1    byte ]
 * Addressing Fields   [4/10 bytes]
 * Data Payload        [n    bytes]
 * FCS                 [2    bytes] : Unsupprted
 */
class SNIFFLIB_EXPORT MACDataFrame : public MACFrame
{
public:
    /**
     * @brief MACDataFrame constructor
     * @param other
     */
    MACDataFrame(const MACFrame& other = {});
};

/**
 * @brief MAC Acknowledgement frame format
 *
 * Frame Control Field [2 bytes]
 * Sequence Number     [1 byte ]
 * FCS                 [2 bytes] : Unsupported
 */
class SNIFFLIB_EXPORT MACAckFrame : public MACFrame
{
public:
    /**
     * @brief MACAckFrame constructor
     * @param other
     */
    MACAckFrame(const MACFrame& other = {});

    const Addressing& addressing() const = delete;
};

/**
 * @brief MAC Command frame format
 *
 * Frame Control Field      [2    bytes]
 * Sequence Number          [1    byte ]
 * Addressing Fields        [4/10 bytes]
 * Command Frame Identifier [1    byte ]
 * Command Payload          [n    bytes]
 * FCS                      [2    bytes] : Unsupported
 */
class SNIFFLIB_EXPORT MACCommandFrame : public MACFrame
{
public:
    /**
     * @brief MACCommandFrame constructor
     * @param other
     */
    MACCommandFrame(const MACFrame& other = {});

    /**
     * @brief command() returns command frame identifier
     * @return command frame identifier
     */
    ZBByte command() const;

    /**
     * @brief commandInfo() returns command frame identifier information
     * @return command frame identifier information
     */
    ZBString commandInfo() const;
};
