#pragma once
//
//  MACFrameInfo.h
//  MACFrameInfo (IEEE 802.15.4)
//
//  Created by Younghwan Mun on 2016-08-22.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//

#include "ZBType.h"
#include "snifflib_global.h"
class SNIFFLIB_EXPORT MACFrameInfo
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
     * @brief addrMode() returns addressing mode
     * @param code
     * @return address mode inforamtion
     */
    static ZBString addrMode(ZBByte code);
};
