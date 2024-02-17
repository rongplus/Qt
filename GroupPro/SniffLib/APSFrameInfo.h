#pragma once
//
//  APSFrameInfo.h
//  APSFrameInfo (Zigbee)
//
//  Created by Younghwan Mun on 2016-09-07.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//

#include "ZBType.h"
#include "snifflib_global.h"
class SNIFFLIB_EXPORT APSFrameInfo
{
public:
    /**
     * @brief type() returns returns string correspond to frame type
     * @param code
     * @return string
     */
    static ZBString type(ZBByte code);

    /**
     * @brief type() returns returns string correspond to deliver mode
     * @param code
     * @return string
     */
    static ZBString deliveryMode(ZBByte code);

    /**
     * @brief type() returns returns string correspond to frame type
     * @param code
     * @return string
     */
    static ZBString ackFormat(ZBByte code);

    /**
     * @brief type() returns returns string correspond to cluster identifier
     * @param code
     * @return string
     */
    static ZBString cluster(ZBByte code);

    /**
     * @brief type() returns returns string correspond to profile identifier
     * @param code
     * @return string
     */
    static ZBString profile(ZBByte code);
};
