//
//  Helper.cpp
//  Helper
//
//  Created by Younghwan Mun on 2016-08-18.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//

#include "Helper.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QFontDatabase>
#include <QDebug>

void delay(int ms)
{
    QTime dieTime = QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void append(ZBBytes& dstBytes, const QByteArray& srcBytes)
{
    const ZBByte* bytesPtr = reinterpret_cast<const ZBByte*>(srcBytes.data());
    for (int i = 0; i < srcBytes.length(); i++)
        dstBytes.push_back(bytesPtr[i]);
}

QByteArray toByteArray(const ZBBytes& bytes)
{
    if (bytes.empty())
        return QByteArray();

    return QByteArray((const char*) bytes.data(), bytes.size());
}

ZBBytes toBytes(const QByteArray& bytes)
{
    if (bytes.isEmpty())
        return ZBBytes();

    const ZBByte* begin = reinterpret_cast<const ZBByte*>(bytes.data());
    const ZBByte* end = begin + bytes.length();
    return ZBBytes(begin, end);
}

QString toHex(const QByteArray& bytes, bool hasSpace)
{
    if (bytes.isEmpty())
        return "";

    QString hex = bytes.toHex().toUpper();
    if (hasSpace)
        for (int i = 2; i < hex.size(); i+=3)
            hex.insert(i, " ");
    return hex;
}

QString toHex(const ZBBytes& bytes, bool hasSpace)
{
    return toHex(toByteArray(bytes), hasSpace);
}

QString toHex(ZBByte byte)
{
    return QString("0x%1").arg(QString::number(uint(byte), 16).toUpper(), 2, '0');
}

QString toHex(ZBInt16 value)
{
    return QString("0x%1").arg(QString::number(uint(value), 16).toUpper(), 4, '0');
}

QString toHex24(ZBInt32 value)
{
    return QString("0x%1").arg(QString::number(value, 16).toUpper(), 6, '0');
}

QString toHex(ZBInt64 value)
{
    QString str;
    for (int i = 0; i < 8; i++, value >>= 8)
        str.push_front(QString(":%1").arg(QString::number(value & 0xff, 16), 2, '0'));
    str.remove(0, 1);
    return str;
}

QString toAscii(ZBBytes bytes)
{
    QString str;
    foreach (ZBByte b , bytes)
        str.append(QChar(b >= 20 ? b : '.'));
    return str;
}

QString toBit(ZBByte byte, ZBByte cnt)
{
    return QString("%1").arg(QString::number(uint(byte), 2), cnt, '0');
}

QString toBool(ZBByte byte)
{
    return byte == 0 ? "False" : "True";
}

QString toNum(ZBInt32 value)
{
    return QString::number(value);
}

QString toNumHex(ZBByte byte)
{
    return QString("%1 (%2)").arg(uint(byte)).arg(toHex(byte));
}

QString toString(QSerialPort::SerialPortError error)
{
    switch (error)
    {
    case QSerialPort::NoError                  : return "No Error";
    case QSerialPort::DeviceNotFoundError      : return "Device not found";
    case QSerialPort::PermissionError          : return "No enough permission";
    case QSerialPort::OpenError	               : return "Already opened the device";
    case QSerialPort::NotOpenError	           : return "Successfully opened the device";
    case QSerialPort::ParityError	           : return "Parity error detected by the hardware while reading data";
    case QSerialPort::FramingError	           : return "Framing error detected by the hardware while reading data";
    case QSerialPort::BreakConditionError      : return "Break condition detected by the hardware on the input line";
    case QSerialPort::WriteError	           : return "I/O error occurred while writing the data";
    case QSerialPort::ReadError                : return "I/O Error";
    case QSerialPort::ResourceError	           : return "Resource not available";
    case QSerialPort::UnsupportedOperationError: return "Unsupported Operation";
    case QSerialPort::TimeoutError	           : return "Timeout Error";
    case QSerialPort::UnknownError             :
    default:                                     return "Unknown Error";
    }
}

QString toString(ZBTime time, QString format)
{
    QTime qTime(time.sec/3600%24, time.sec/60%60, time.sec%60, time.msec/1000%10000);
    return qTime.toString(format);
}

ZBTime currentTime()
{
	qint64  time = QDateTime::currentMSecsSinceEpoch();
	
    ZBTime zbt;
    zbt.sec = ZBInt32(time /1000);
    zbt.msec = ZBInt32(time % 1000);

    return zbt;
}

QFont fixedFont()
{
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setStretch(QFont::SemiExpanded);
    return font;
}

