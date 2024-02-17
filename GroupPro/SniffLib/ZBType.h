#pragma once
//
//  ZBType.h
//  ZBType
//
//  Created by Younghwan Mun on 2016-08-22.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//

#define ZB_DEBUG             1

#if ZB_DEBUG
#include <QDebug>
#include <QMessageBox>
#endif

#include "snifflib_global.h"

// Common
#define ZB_TX                0x00
#define ZB_RX                0x01
#define ZB_Enable            0x01
#define ZB_Disable           0x00
#define ZB_Null              0x00

#define MICROS_PER_SYMBOL    16 // symbol duration in us

#include <vector>
#include <string>

typedef unsigned char        ZBByte;
typedef unsigned short       ZBInt16;
typedef unsigned int         ZBInt32;
typedef unsigned long long   ZBInt64;
typedef std::vector<ZBByte>  ZBBytes;
typedef std::vector<ZBInt16> ZBInt16s;
typedef std::vector<ZBInt64> ZBInt64s;
typedef std::string          ZBString;

/**
 * @brief operator + combines two bytes
 * @param lhs
 * @param rhs
 * @return bytes
 */
SNIFFLIB_EXPORT ZBBytes operator+(ZBBytes lhs, const ZBBytes& rhs);

/**
 * @brief operator + combines a byte and a bytes
 * @param lhs
 * @param rhs
 * @return bytes
 */
SNIFFLIB_EXPORT ZBBytes operator+(ZBBytes lhs, ZBByte rhs);

/**
 * @brief operator + combines a byte and a bytes
 * @param lhs
 * @param rhs
 * @return bytes
 */
SNIFFLIB_EXPORT ZBBytes operator+(ZBByte lhs, ZBBytes rhs);

/**
 * @brief substr makes sub string
 * @param bytes
 * @param pos
 * @param len
 * @return string
 */
SNIFFLIB_EXPORT ZBBytes sub(const ZBBytes& bytes, int pos, int len = -1);

/**
 * @brief toi16 converts ZBBytes to ZBInt16
 * @param bytes
 * @return 16-bit value
 */
SNIFFLIB_EXPORT ZBInt16 toi16(const ZBBytes& bytes);

/**
 * @brief toi32 converts ZBBytes to ZBInt32
 * @param bytes
 * @return 32-bit value
 */
SNIFFLIB_EXPORT ZBInt32 toi32(const ZBBytes& bytes);

/**
 * @brief toi64 converts ZBBytes to ZBInt64
 * @param bytes
 * @return 64-bit value
 */
SNIFFLIB_EXPORT ZBInt64 toi64(const ZBBytes& bytes);

SNIFFLIB_EXPORT ZBBytes toa(ZBInt16 value);
SNIFFLIB_EXPORT ZBBytes toa(ZBInt32 value);

struct SNIFFLIB_EXPORT ZBTime
{
    ZBInt32 sec;
    ZBInt32 msec;

    /**
     * @brief ZBTime constructor
     * @param sec
     * @param msec
     */
    ZBTime(ZBInt32 sec = 0, ZBInt32 msec = 0);

    /**
     * @brief operator+ help operator
     * @param lhs
     * @param rhs
     * @return ZBTime
     */
    friend ZBTime operator+(ZBTime lhs, const ZBTime& rhs);

    /**
     * @brief operator- help operator
     * @param lhs
     * @param rhs
     * @return ZBTime
     */
    friend ZBTime operator-(ZBTime lhs, const ZBTime& rhs);
};

Q_DECLARE_METATYPE(ZBTime)

class SNIFFLIB_EXPORT ZBFrame
{
public:    

	~ZBFrame() {}
    /**
     * @brief protocol returns frame protocol
     * @return frame protocol
     */
    const ZBString& protocol() const;

    /**
     * @brief name() returns frame layer
     * @return frame layer
     */
    const ZBString& layer() const;

    /**
     * @brief name() returns frame type
     * @return frame type
     */
    const ZBString& type() const;

    /**
     * @brief name() returns frame name (protocol, layer, and type)
     * @return frame info
     */
    ZBString name() const;

    /**
     * @brief hdr() returns frame header
     * @return frame header
     */
    const ZBBytes& hdr() const;

    /**
     * @brief payload() returns frame payload
     * @return frame payload
     */
    const ZBBytes& payload() const;

    /**
     * @brief bin() returns frame header and payload
     * @return header and payload
     */
    ZBBytes bin() const;

    /**
     * @brief clear() removes all bytes
     */
    void clear();

    /**
     * @brief empty() checks frame bytes exist
     * @return true if bytes doesn't exist otherwise false
     */
    bool empty() const;

    /**
     * @brief size() returns frame size
     * @return frame size
     */
    ZBByte size() const;

protected:
    ZBString _protocol;
    ZBString _layer;
    ZBString _type;
    ZBBytes  _hdr;
    ZBBytes  _payload;
};

class SNIFFLIB_EXPORT ZBInfo
{
public:
    static ZBInt16 frequency(ZBByte channel); 
};

