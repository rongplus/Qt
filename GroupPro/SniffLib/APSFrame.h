#pragma once
//
//  APSFrame.h
//  APSFrame (Zigbee)
//
//  Created by Younghwan Mun on 2016-09-07.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//

#include "ZBType.h"
#include "APSFrameDef.h"
#include "APSFrameInfo.h"
#include "NWKFrame.h"

/**
 * @brief Generic APS frame format
 *
 * Frame Control Field  [1 byte ]
 * Destination Endpoint [1 byte ]
 * Cluster Identifier   [2 bytes]
 * Profile Identifier   [2 bytes]
 * Source Endpoint      [1 byte ]
 * Counter              [1 byte ]
 * Payload              [n bytes]
 */
class SNIFFLIB_EXPORT APSFrame : public ZBFrame
{
public:
    /**
     * @brief Frame control field format
     *
     * Frame Type             [2 bits ]
     * Delivery Mode          [2 bits ]
     * Acknowledge Format     [1 bits ]
     * Security Enabled       [1 bit  ]
     * Acknowledge Request    [1 bit  ]
     * Extended Header Present[1 bit  ]
     */
    struct FrameControl
    {
        union
        {
            ZBInt16 spec;
            struct
            {
                ZBByte frmType      : 2; // 0-1
                ZBByte deliveryMode : 2; // 2-3
                ZBByte ackFormat    : 1; // 4
                ZBByte security     : 1; // 5
                ZBByte ackReq       : 1; // 6
                ZBByte extHdr       : 1; // 7
            };
        };
        ZBString deliveryModeInfo;
        ZBString ackFormatInfo;
    };

    /**
     * @brief APSFrame constructor
     * @param frame
     */
    APSFrame(const NWKFrame& frame);
    APSFrame(const NWKDataFrame& frame);

    /**
     * @brief frameControl() returns frame control
     * @return frame control
     */
    FrameControl frameControl() const;

    /**
     * @brief dstEndpoint() getter
     * @return destination endpoint
     */
    ZBByte dstEndpoint() const;

    /**
     * @brief clusterId() getter
     * @return cluster identifier
     */
    ZBInt16 clusterId() const;

    /**
     * @brief clusterId() getter
     * @return profile identifier
     */
    ZBInt16 profileId() const;

    /**
     * @brief clusterId() getter
     * @return source endpoint
     */
    ZBByte srcEndpoint() const;

    /**
     * @brief counter() getter
     * @return conter
     */
    ZBByte counter() const;

    /**
     * @brief cluster() returns string correspond to idenfier
     * @return string
     */
    ZBString cluster() const;

    /**
     * @brief profile() returns string correspond to idenfier
     * @return string
     */
    ZBString profile() const;

protected:
    /**
     * @brief APSFrame constructor
     * @param bin
     */
    APSFrame(const ZBBytes& bin);
};

/**
 * @brief APS Data frame format
 *
 * Frame Control Field  [1 byte ]
 * Destination Endpoint [1 byte ]
 * Cluster Identifier   [2 bytes]
 * Profile Identifier   [2 bytes]
 * Source Endpoint      [1 byte ]
 * Counter              [1 byte ]
 * Payload              [n bytes]
 */
class APSDataFrame : public APSFrame
{
public:
    /**
     * @brief APSDataFrame constructor
     * @param other
     */
    APSDataFrame(const APSFrame& other);
};
