//
//  APSFrameInfo.cpp
//  APSFrameInfo (Zigbee)
//
//  Created by Younghwan Mun on 2016-08-22.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//
#include "APSFrameInfo.h"
#include "APSFrameDef.h"

ZBString APSFrameInfo::type(ZBByte code)
{
    switch(code)
    {
        case APS_FRAME_DATA: return "Data";

        default: return "Unknown";
    }
}

ZBString APSFrameInfo::deliveryMode(ZBByte code)
{
    switch(code)
    {
        case APS_DELIVERY_NORMAL_UNICAST: return "Normal Unicast";

        default: return "Unknown";
    }
}

ZBString APSFrameInfo::ackFormat(ZBByte code)
{
    switch(code)
    {
        case APS_ACK_FORMAT_DATA_FRAME: return "Data Frame";

        default: return "Unknown";
    }
}

ZBString APSFrameInfo::cluster(ZBByte code)
{
    switch(code)
    {
        case APS_CLUSTER_POWER_CONFIGURATION: return "Power Configuration";

        default: return "Unknown";
    }
}

ZBString APSFrameInfo::profile(ZBByte code)
{
    switch(code)
    {
        case APS_PROFILE_FREESCALE: return "Freescale";

        default: return "Unknown";
    }
}
