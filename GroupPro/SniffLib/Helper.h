#pragma once
//
//  Helper.h
//  Helper
//
//  Created by Younghwan Mun on 2016-08-18.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//

#include "ZBType.h"
#include <QSerialPort>
#include <QByteArray>
#include <QString>
#include <QFont>
#include "snifflib_global.h"
/**
 * @brief delay() pauses designed moment
 * @param ms
 */
SNIFFLIB_EXPORT void delay(int ms);

/**
 * @brief append() append QByteArray to ZTCBytes
 * @param dstBytes
 * @param srcBytes
 */
SNIFFLIB_EXPORT void append(ZBBytes& dstBytes, const QByteArray& srcBytes);

/**
 * @brief toByteArray() converts ZTCBytes to QByteArray
 * @param bytes
 * @return QByteArray
 */
SNIFFLIB_EXPORT QByteArray toByteArray(const ZBBytes& bytes);

/**
 * @brief toBytes() converts QByteArray to ZTCBytes
 * @param bytes
 * @return QByteArray
 */
SNIFFLIB_EXPORT ZBBytes toBytes(const QByteArray& bytes);

/**
 * @brief toHex() converts bytes to hex formatted string with space
 * @param bytes
 * @param hasSpace
 * @return QString
 */
SNIFFLIB_EXPORT QString toHex(const QByteArray& bytes, bool hasSpace = true);

/**
 * @brief toHex() converts bytes to hex formatted string with space
 * @param bytes
 * @param hasSpace
 * @return QString
 */
SNIFFLIB_EXPORT QString toHex(const ZBBytes& bytes, bool hasSpace = true);

/**
 * @brief toHex() converts byte to hex formatted string
 * @param byte
 * @return QString
 */
SNIFFLIB_EXPORT QString toHex(ZBByte byte);

/**
 * @brief toHex() converts value to hex formatted string
 * @param num
 * @return QString
 */
SNIFFLIB_EXPORT QString toHex(ZBInt16 value);

/**
 * @brief toHex() converts value to hex formatted string
 * @param num
 * @return QString
 */
SNIFFLIB_EXPORT QString toHex24(ZBInt32 value);

/**
 * @brief toHex() converts value to hex formatted string
 * @param num
 * @return QString
 */
SNIFFLIB_EXPORT QString toHex(ZBInt64 value);

/**
 * @brief toAscii convers bytes to ascii formatted string
 * @param bytes
 * @return QString
 */
SNIFFLIB_EXPORT QString toAscii(ZBBytes bytes);

/**
 * @brief toBit() converts byte to bit formatted string
 * @param byte
 * @param cnt
 * @return QString
 */
SNIFFLIB_EXPORT QString toBit(ZBByte byte, ZBByte cnt = 1);

/**
 * @brief toBool() converts byte to bool(true, false) formatted string
 * @param byte
 * @return QString
 */
SNIFFLIB_EXPORT QString toBool(ZBByte byte);

/**
 * @brief toNum converts value to number formatted string
 * @param value
 * @return QString
 */
SNIFFLIB_EXPORT QString toNum(ZBInt32 value);

/**
 * @brief toNumHex converts byte to number and hex formatted string ("0 (0x00)")
 * @param byte
 * @return QString
 */
SNIFFLIB_EXPORT QString toNumHex(ZBByte byte);

/**
 * @brief toString() converts error code to error string
 * @param error
 * @return error string
 */
SNIFFLIB_EXPORT QString toString(QSerialPort::SerialPortError error);

/**
 * @brief toString() converts time(msec) to formatted string
 * @param time
 * @param format
 * @return
 */
SNIFFLIB_EXPORT QString toString(ZBTime time, QString format = "HH:mm:ss.zzz");

/**
 * @brief currentTime() return current time
 * @return
 */
SNIFFLIB_EXPORT ZBTime currentTime();

/**
 * @brief fixedFont() returns fixed font
 * @return fixed font
 */
SNIFFLIB_EXPORT QFont fixedFont();
