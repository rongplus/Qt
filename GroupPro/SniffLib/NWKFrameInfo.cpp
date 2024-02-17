//
//  NWKFrameInfo.cpp
//  NWKFrameInfo (Zigbee)
//
//  Created by Younghwan Mun on 2016-08-22.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//
#include "NWKFrameInfo.h"
#include "NWKFrameDef.h"

ZBString NWKFrameInfo::type(ZBByte code)
{
    switch(code)
    {
        case NWK_FRAME_DATA    : return "Data";
        case NWK_FRAME_COMMAND : return "Command";

        default: return "Unknown";
    }
}

ZBString NWKFrameInfo::command(ZBByte code)
{
    switch(code)
    {
        case NWK_FRAME_COMMAND_ROUTE_REQUEST  : return "Route Request";
        case NWK_FRAME_COMMAND_ROUTE_REPLY    : return "Route Reply";
        case NWK_FRAME_COMMAND_NETWORK_STATUS : return "Network Status";
        case NWK_FRAME_COMMAND_LEAVE          : return "Leave";
        case NWK_FRAME_COMMAND_ROUTE_RECORD   : return "Route Record";
        case NWK_FRAME_COMMAND_REJOIN_REQUEST : return "Rejoin Request";
        case NWK_FRAME_COMMAND_REJOIN_RESPONSE: return "Rejoin Response";
        case NWK_FRAME_COMMAND_LINK_STATUS    : return "Link Status";

        default: return "Reserved";
    }
}

ZBString NWKFrameInfo::networkStatus(ZBByte code)
{
    switch(code)
    {
        case NWK_NETWORK_STATUS_ADDRESS_CONFLICT : return "Address Conflict";

        default: return "Reserved";
    }
}

ZBString NWKFrameInfo::manyToOne(ZBByte code)
{
    switch(code)
    {
        case NWK_ROUTE_REQUEST_NOT_MANY_TO_ONE: return "Not Many-to-One";

        default: return "Unknown";
    }
}

ZBString NWKFrameInfo::protocol(ZBByte code)
{
    switch(code)
    {
        case NWK_PROTOCOL_ZIGBEE: return "ZigBee";

        default: return "Reserved";
    }
}

ZBString NWKFrameInfo::profile(ZBByte code)
{
    switch(code)
    {
        case NWK_STACK_PROFILE_PRO: return "ZigBee Pro";

        default: return "Unknown";
    }
}

ZBString NWKFrameInfo::discoverRoute(ZBByte code)
{
    switch(code)
    {
        case NWK_DISCOVER_ROUTE_SUPPRESS: return "Suppress";
        case NWK_DISCOVER_ROUTE_ENABLE  : return "Enable";
        case NWK_DISCOVER_ROUTE_FORCE   : return "Force";
        case NWK_DISCOVER_ROUTE_RESERVED: return "Reserved";

        default: return "Unknown";
    }
}
