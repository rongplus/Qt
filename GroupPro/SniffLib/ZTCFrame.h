#pragma once
//
//  ZTCFrame.h
//  ZTCFrame
//
//  Created by Younghwan Mun on 2016-08-08.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//

/*
 * ZigBee Frame format
 *
 * Byte:     0   1   2         (3..n+3)
 *         +---+---+---+------- . . . -------+
 *         !   !   !   !                     !
 *         +---+---+---+------- . . . -------+
 * Length:   1   1   1          n
 *
 *
 * Byte 0: Opcode group
 * Byte 1: Opcode
 * Byte 2: Length of data field (excluding header)
 * Byte 3 to (n+3): Data, including time stamps where applicable
 */

#include "ZBType.h"
#include "ZTCFrameDef.h"
#include "ZTCFrameInfo.h"
#include "snifflib_global.h"
/**
 * @brief Generic ZTC frame format
 *
 * OpCode         [2 bytes] : OpCode Group + OpCode
 * Payload Length [1 byte ]
 * Payload        [n bytes]
 * FCS            [1 byte ]
 */
class SNIFFLIB_EXPORT ZTCFrame : public ZBFrame
{
public:
    /**
     * @brief bin constructor
     * @param bytes
     */
    ZTCFrame(const ZBBytes& bin = {});

    /**
     * @brief ZTCFrame constructor
     * @param code
     * @param payload
     */
    ZTCFrame(ZBInt16 opCode, const ZBBytes& payload = ZBBytes());

    /**
     * @brief opCode() returns Opcode group and Opcode
     * @return code value
     */
    ZBInt16 opCode() const;

    /**
     * @brief status() returns status value
     * @return status
     */
    ZBByte status() const;

    /**
     * @brief info() returns frame information
     * @return frame information
     */
    ZTCFrameInfo info() const;
};

/**
 * @brief ZTC Data (Promiscuous mode) frame format
 *
 * OpCode         [2 byte ] = 86 03
 * Payload Length [1 byte ] : Generic ZTC Frame Payload Length
 * Link Quality   [1 byte ]
 * Timestamp      [4 bytes]
 * Payload Length [1 byte ]
 * Payload        [n bytes]
 */
class SNIFFLIB_EXPORT ZTCDataFrame : public ZTCFrame
{
public:
    /**
     * @brief ZTCDataFrame constructor
     * @param other
     */
    ZTCDataFrame(const ZTCFrame& other = {});

public:
    /**
     * @brief linkQuality() returns link quality
     * @return link quality value
     */
    ZBByte linkQuality() const;

    /**
     * @brief timeStamp() returns timestamp
     * @return timestamp value
     */
    ZBTime timeStamp() const;
};
