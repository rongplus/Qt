#pragma once
//
//  NWKFrame.h
//  NWKFrame (Zigbee)
//
//  Created by Younghwan Mun on 2016-08-26.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//

#include "ZBType.h"
#include "NWKFrameDef.h"
#include "NWKFrameInfo.h"
#include "MACFrame.h"
#include "snifflib_global.h"
/**
 * @brief Generic network frame format
 *
 * Frame Control Field      [2   bytes]
 * Destination Address      [2   bytes]
 * Source Address           [2   bytes]
 * Radius                   [1   byte ]
 * Sequence Number          [1   byte ]
 * Destination IEEE Address [0/8 bytes]
 * Source IEEE Address      [0/8 bytes]
 * Multicast Control        [0/1 byte ]
 * Source Route Subframe    [n   bytes]
 * Frame Payload            [n   bytes]
 */
class SNIFFLIB_EXPORT NWKFrame : public ZBFrame
{
public:
    /**
     * @brief Frame control field format
     *
     * Frame Type                  [2 bits]
     * Protocol Version            [4 bits]
     * Discover Route              [2 bits]
     * Multicast Flag              [1 bit ]
     * Security                    [1 bit ]
     * Source Route                [1 bit ]
     * Destination IEEE Address    [1 bit ]
     * Source IEEE Address         [1 bit ]
     * Reserved                    [3 bits]
     */
    union FrameControl
    {
        ZBInt16 spec;
        struct
        {
            ZBByte type          : 2; // 0-1
            ZBByte protocolVer   : 4; // 2-5
            ZBByte discoverRoute : 2; // 6-7
            ZBByte multicast     : 1; // 8
            ZBByte security      : 1; // 9
            ZBByte srcRoute      : 1; // 10
            ZBByte dstIEEEAddr16 : 1; // 11
            ZBByte srcIEEEAddr64 : 1; // 12
            ZBByte devInitiator  : 1; // 13
            ZBByte reserved      : 2; // 14-15
        };
    };

    /**
     * @brief RouteSubFrame fields format
     *
     * Relay Count [1 byte ]
     * Relay Index [1 byte ]
     * Relay List  [n bytes]
     */
    struct RouteSubFrame
    {
        ZBBytes  spec;  // raw bytes
        ZBByte   cnt;
        ZBByte   idx;
        ZBInt16s list;        
    };

    /**
     * @brief NWKFrame constructor
     * @param frame
     */
    NWKFrame(const MACDataFrame& frame = {});

    /**
     * @brief frameControl() returns frame control information
     * @return frame control
     */
    const FrameControl& frameControl() const;

    /**
     * @brief dstAddr() returns destination address
     * @return destination address
     */
    ZBInt16 dstAddr() const;

    /**
     * @brief srcAddr() returns source address
     * @return source address
     */
    ZBInt16 srcAddr() const;

	/**
	* @brief srcAddr() returns source address
	* @return source address
	*/
	ZBInt16 srcAddr16() const;

    /**
     * @brief radius() returns radius
     * @return radius
     */
    ZBByte radius() const;

    /**
     * @brief sequence() returns sequence number
     * @return sequence number
     */
    ZBByte sequence() const;

    /**
     * @brief dstIEEEAddr() returns destination IEEE address
     * @return destination mac address
     */
    ZBInt64 dstIEEEAddr() const;

    /**
     * @brief srcIEEEAddr() returns source IEEE address
     * @return source mac address
     */
    ZBInt64 srcIEEEAddr() const;

    /**
     * @brief multicast() returns multicast control
     * @return multicast control
     */
    ZBByte multicast() const;

    /**
     * @brief srcRouteSubFrame() returns source route subframe
     * @return source route subframe
     */
    const RouteSubFrame& srcRouteSubFrame() const;

    /**
     * @brief discoverRoute() returns discover route information
     * @return discover route information
     */
    const ZBString discoverRoute() const;

protected:
    /**
     * @brief NWKFrame constructor
     * @param bin
     */
    NWKFrame(const ZBBytes& bin);

private:
    FrameControl  _ctl;
    ZBInt64       _src64;
    ZBInt64       _dst16;
	ZBInt16       _src16;
    ZBByte        _multi;
    RouteSubFrame _sub;
};

/**
 * @brief Network Data frame format
 *
 * Frame Control Field [2 bytes]
 * Routing Fields      [n bytes]
 * Data Payload        [n bytes]
 */
class SNIFFLIB_EXPORT NWKDataFrame : public NWKFrame
{
public:
    /**
     * @brief NWKDataFrame constructor
     * @param other
     */
    NWKDataFrame(const NWKFrame& other = {});
};

/**
 * @brief Network Command frame format
 *
 * Frame Control Field      [2 bytes]
 * Routing Fields           [n bytes]
 * Command Frame Identifier [1 byte ]
 * Command Payload          [n bytes]
 */
class SNIFFLIB_EXPORT NWKCommandFrame : public NWKFrame
{
public:
    /**
     * @brief Route request command fields format
     *
     * Command Options               [1   byte ]
     *   Reserved                    [3   bits ]
     *   Many to One Discovery       [2   bits ]
     *   Destination IEEE Address    [1   bit  ]
     *   Multicast Route Request     [1   bit  ]
     *   Reserved                    [1   bit  ]
     * Route Request Identifier      [1   byte ]
     * Route Destination Address     [2   bytes]
     * Path Cost                     [1   byte ]
     * Route Destination IEEE Address[0/8 bytes]
     */
    struct RouteRequest
    {
        union
        {
            ZBByte opt;
            struct
            {
                ZBByte rsvd         : 3; // 0-2
                ZBByte manyToOne    : 2; // 3-4
                ZBByte hasDstIEEE   : 1; // 5
                ZBByte multicastReq : 1; // 6
                ZBByte rsvd2        : 1; // 7
            };
        };
        ZBByte  reqId;
        ZBInt16 dstAddr16;
        ZBInt64 dstAddr64;
        ZBByte  pathCost;
        ZBString manyToOneInfo;
    };

    /**
     * @brief Route reply command fields format
     *
     * Command Options          [1 byte ]
     *   Reserved               [7 bits ]
     *   Route Repair           [1 bit  ]
     * Route Request Identifier [1 byte ]
     * Originator Address       [2 bytes]
     * Responder Address        [2 bytes]
     * Path Cost                [1 byte ]
     */
    struct RouteReply
    {
        union
        {
            ZBByte opt;
            struct
            {
                ZBByte rsvd   : 7; // 0-6
                ZBByte repair : 1; // 7
            };
        };
        ZBByte  reqId;
        ZBInt16 srcAddr;
        ZBInt16 dstAddr;
        ZBByte  pathCost;
    };

    /**
     * @brief Network Status command fields format
     *
     * Status Code              [1 byte ]
     * Destination Address      [2 bytes]
     */
    struct NetworkStatus
    {
        ZBByte  status;
        ZBInt16 dstAddr16;
        ZBString statusInfo;
    };

    /**
     * @brief Leave command fields format
     *
     * Command Options          [1 byte ]
     *   Reserved               [6 bits ]
     *   Request/Indication     [1 bit  ]
     *   Remove Children        [1 bit  ]
     */
    struct Leave
    {
        union
        {
            ZBByte opt;
            struct
            {
                ZBByte rsvd           : 5; // 0-4
                ZBByte rejoin         : 1; // 5
                ZBByte request        : 1; // 6
                ZBByte removeChildren : 1; // 7
            };
        };
    };

    /**
     * @brief Link Status command fields format
     *
     * Command Options           [1 byte ]
     *   Link Count              [5 bits ]
     *   First Frame             [1 bit  ]
     *   Last Frame              [1 bit  ]
     *   Reserved                [1 bit  ]
     * Link List                 [n bytes]
     *   Neighbor Network Address[2 bytes]
     *   Status                  [1 byte ]
     *     Incoming Cost         [3  bits]
     *     Reserved              [1  bit ]
     *     Outgoing Cost         [3  bits]
     *     Reserved              [1  bit ]
     */
    struct Link
    {
        ZBBytes bin;             // raw bytes
        ZBInt16 neighbor;
        union
        {
            ZBByte status;
            struct
            {
                ZBByte incoming : 3; // 0-2
                ZBByte rsvd1    : 1; // 3
                ZBByte outgoing : 3; // 4-6
                ZBByte rsvd2    : 1; // 7
            };
        };
    };
    typedef std::vector<Link> Links;

    struct LinkStatus
    {
        union
        {
            ZBByte opt;
            struct
            {
                ZBByte linkCnt  : 5; // 0-4
                ZBByte firstFrm : 1; // 5
                ZBByte lastFrm  : 1; // 6
                ZBByte rsvd     : 1; // 7
            };
        };
        Links links;
    };

    /**
     * @brief MACCommandFrame constructor
     * @param other
     */
    NWKCommandFrame(const NWKFrame& other = {});
	~NWKCommandFrame();

    /**
     * @brief command() returns command frame identifier
     * @return command frame identifier
     */
    ZBByte command() const;

    /**
     * @brief commandInfo() returns command frame identifier information
     * @return command frame identifier information
     */
    ZBString commandInfo() const;

    /**
     * @brief routeRequest() returns route request command information
     * @return command information
     */
    RouteRequest routeRequest() const;

    /**
     * @brief routeReply() returns route reply command information
     * @return command information
     */
    RouteReply routeReply() const;

    /**
     * @brief networkStatus() returns network status command information
     * @return command information
     */
    NetworkStatus networkStatus() const;

    /**
     * @brief leave() returns leave command information
     * @return command information
     */
    Leave leave() const;

    /**
     * @brief linkStatus() returns link status command information
     * @return command information
     */
    LinkStatus linkStatus() const;
};

/**
 * @brief Network layer information format
 *
 * Protocol ID             [8   bits ] : 1 byte
 * Stack Profile           [4   bits ]
 * Network Protocol Version[4   bits ]
 * Reserved                [2   bits ]
 * Router Capacity         [1   bit  ]
 * Device Depth            [4   bits ]
 * End Device Capacity     [64  bits ]
 * Tansmit Offset          [24  bits ]
 * Network Update ID       [8   bits ]
 */
class SNIFFLIB_EXPORT NWKLayerInfo : public ZBFrame
{
public:
    /**
     * @brief NWKLayerInfo constructor
     * @param frame
     */
    NWKLayerInfo(const MACBeaconFrame& frame = {});

    /**
     * @brief protocolID() returns protocol identification
     * @return protocol identification
     */
    ZBByte protocolID() const;

    /**
     * @brief stackProfile returns stack profile
     * @return stack profile (4-bit)
     */
    ZBByte stackProfile() const;

    /**
     * @brief protocolVer() return network protocol version
     * @return network protocol version (4-bit)
     */
    ZBByte protocolVer() const;

    /**
     * @brief rsvd() return reserved value
     * @return reserved value (2-bit)
     */
    ZBByte rsvd() const;

    /**
     * @brief routerCapacity() returns whether router capacity has or not
     * @return 0 or 1
     */
    ZBByte routerCapacity() const;

    /**
     * @brief deviceDepth() returns device depth
     * @return device depth (4-bit)
     */
    ZBByte deviceDepth() const;

    /**
     * @brief endDeviceCapacity returns whether end device capacity has or not
     * @return 0 or 1
     */
    ZBByte endDeviceCapacity() const;

    /**
     * @brief extendedPanID() returns pan identification
     * @return pan identification
     */
    ZBInt64 extendedPanID() const;

    /**
     * @brief txOffset() returns transmit offset
     * @return transmit offset (24-bit)
     */
    ZBInt32 transmitOffset() const;

    /**
     * @brief updateID() returns network update identification
     * @return network update identification
     */
    ZBByte updateID() const;

    /**
     * @brief profile() return stack profile information
     * @return stack profile information
     */
    ZBString profile() const;
};
