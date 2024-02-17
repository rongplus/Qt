#pragma once
//
//  NWKFrameInfo.h
//  NWKFrameInfo (Zigbee)
//
//  Created by Younghwan Mun on 2016-08-22.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//

#include "ZBType.h"

#include "snifflib_global.h"
class SNIFFLIB_EXPORT NWKFrameInfo
{
public:
    /**
     * @brief type() returns frame type in frame control field
     * @param code
     * @return frame type in frame control field
     */
    static ZBString type(ZBByte code);

    /**
     * @brief command() returns command frame identifier
     * @param code
     * @return command frame identifier
     */
    static ZBString command(ZBByte code);

    /**
     * @brief routeError() returns route error of command frame
     * @param code
     * @return route error of command frame
     */
    static ZBString networkStatus(ZBByte code);

    /**
     * @brief manyToOne() returns many-to-one discovery infomation
     * @param code
     * @return string
     */
    static ZBString manyToOne(ZBByte code);

    /**
     * @brief protocol returns network protocol
     * @param code
     * @return network protocol
     */
    static ZBString protocol(ZBByte code);

    /**
     * @brief profile() returns stack profile information
     * @param code
     * @return stack profile information
     */
    static ZBString profile(ZBByte code);

    /**
     * @brief discoverRoute() returns discover route
     * @param code
     * @return discover route
     */
    static ZBString discoverRoute(ZBByte code);
};
