//
//  MACFrameInfo.cpp
//  MACFrameInfo (IEEE 802.15.4)
//
//  Created by Younghwan Mun on 2016-08-22.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//
#include "MACFrameInfo.h"
#include "MACFrameDef.h"
#include <unordered_map>

ZBString MACFrameInfo::type(ZBByte code)
{
    static std::unordered_map<ZBByte, ZBString> table;
    if (table.empty())
    {
        table[MAC_FRAME_BEACON     ] = "Beacon";
        table[MAC_FRAME_DATA       ] = "Data";
        table[MAC_FRAME_ACKNOWLEDGE] = "Acknowledge";
        table[MAC_FRAME_COMMAND    ] = "Command";
    }
    return table.find(code) != table.end() ? table[code] : "Reserved";
}

ZBString MACFrameInfo::command(ZBByte code)
{
    static std::unordered_map<ZBByte, ZBString> table;
    if (table.empty())
    {
        table[MAC_FRAME_COMMAND_ASSOCIATION_REQUEST         ] = "Association Request";
        table[MAC_FRAME_COMMAND_ASSOCIATION_RESPONSE        ] = "Association Response";
        table[MAC_FRAME_COMMAND_DISASSOCIATION_NOTIFICATION ] = "Disassociation Notification";
        table[MAC_FRAME_COMMAND_DATA_REQUEST                ] = "Data Request";
        table[MAC_FRAME_COMMAND_PAN_ID_CONFLICT_NOTIFICATION] = "PAN ID Conflict Notification";
        table[MAC_FRAME_COMMAND_ORPHAN_NOTIFICATION         ] = "Orphan Notification";
        table[MAC_FRAME_COMMAND_BEACON_REQUEST              ] = "Beacon Request";
        table[MAC_FRAME_COMMAND_COORDINATOR                 ] = "Coordinator Realignment";
        table[MAC_FRAME_COMMAND_GTS_REQUEST                 ] = "GTS Request";
    }
    return table.find(code) != table.end() ? table[code] : "Reserved";
}

ZBString MACFrameInfo::addrMode(ZBByte code)
{
    static std::unordered_map<ZBByte, ZBString> table;
    if (table.empty())
    {
        table[MAC_ADDRESS_MODE_INDIRECT] = "None";
        table[MAC_ADDRESS_MODE_GROUP   ] = "Group";
        table[MAC_ADDRESS_MODE_16_BIT  ] = "Short/16-bit";
        table[MAC_ADDRESS_MODE_64_BIT  ] = "Extended/64-bit";
    }
    return table.find(code) != table.end() ? table[code] : "Unknown";
}
