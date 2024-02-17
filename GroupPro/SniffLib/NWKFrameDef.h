#pragma once
//
//  NWKFrameDef.h
//  NWKFrameDef (Zigbee)
//
//  Created by Younghwan Mun on 2016-08-22.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//

// Frame types
#define NWK_FRAME_DATA                      0x00
#define NWK_FRAME_COMMAND                   0x01

// Command frame identifier
#define NWK_FRAME_COMMAND_ROUTE_REQUEST     0x01
#define NWK_FRAME_COMMAND_ROUTE_REPLY       0x02
#define NWK_FRAME_COMMAND_NETWORK_STATUS       0x03
#define NWK_FRAME_COMMAND_LEAVE             0x04
#define NWK_FRAME_COMMAND_ROUTE_RECORD      0x05
#define NWK_FRAME_COMMAND_REJOIN_REQUEST    0x06
#define NWK_FRAME_COMMAND_REJOIN_RESPONSE   0x07
#define NWK_FRAME_COMMAND_LINK_STATUS       0x08
//      NWK_FRAME_COMMAND_RESERVED          0x09-0xFF

#define NWK_NETWORK_STATUS_ADDRESS_CONFLICT         0x0D
//      NWK_STATUS_RESERVED                 0x??-0FF


#define NWK_ROUTE_REQUEST_NOT_MANY_TO_ONE   0x00
//                                          ???? 0x03

// Protocol
#define NWK_PROTOCOL_ZIGBEE                 0x00

#define NWK_PROTOCOL_VERSION                0x02

// Stack Profile
#define NWK_STACK_PROFILE_PRO               0x02

// Discover Route
#define NWK_DISCOVER_ROUTE_SUPPRESS         0x00
#define NWK_DISCOVER_ROUTE_ENABLE           0x01
#define NWK_DISCOVER_ROUTE_FORCE            0x02
#define NWK_DISCOVER_ROUTE_RESERVED         0x03



