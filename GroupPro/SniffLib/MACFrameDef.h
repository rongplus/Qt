#pragma once
//
//  MACFrameDef.h
//  MACFrameDef (IEEE 802.15.4)
//
//  Created by Younghwan Mun on 2016-08-22.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//

// Frame types
#define MAC_FRAME_BEACON                                0x00 // 000
#define MAC_FRAME_DATA                                  0x01 // 001
#define MAC_FRAME_ACKNOWLEDGE                           0x02 // 010
#define MAC_FRAME_COMMAND                               0x03 // 011
//      MAC_FRAME_RESERVED                                   // 100-111

// Command frame identifier
#define MAC_FRAME_COMMAND_ASSOCIATION_REQUEST           0x01
#define MAC_FRAME_COMMAND_ASSOCIATION_RESPONSE          0x02
#define MAC_FRAME_COMMAND_DISASSOCIATION_NOTIFICATION   0x03
#define MAC_FRAME_COMMAND_DATA_REQUEST                  0x04
#define MAC_FRAME_COMMAND_PAN_ID_CONFLICT_NOTIFICATION  0x05
#define MAC_FRAME_COMMAND_ORPHAN_NOTIFICATION           0x06
#define MAC_FRAME_COMMAND_BEACON_REQUEST                0x07
#define MAC_FRAME_COMMAND_COORDINATOR                   0x08
#define MAC_FRAME_COMMAND_GTS_REQUEST                   0x09
//      MAC_FRAME_COMMAND_RESERVED                      0x0a-0xFF

#define MAC_ADDRESS_MODE_INDIRECT                       0x00
#define MAC_ADDRESS_MODE_GROUP                          0x01
#define MAC_ADDRESS_MODE_16_BIT                         0x02
#define MAC_ADDRESS_MODE_64_BIT                         0x03

