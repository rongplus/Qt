#pragma once
//
//  SavedPreference.h
//  SavedPreference
//
//  Created by Younghwan Mun on 2016-08-11.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//

#include <QSerialPort>
#include "snifflib_global.h"
class SNIFFLIB_EXPORT SavedPreference
{
public:
    struct DeviceInfo
    {
        QString name;
        QString description;
        QString manufacturer;
        QString serialNumber;
        QString location;
        QString vendorID;
        QString productID;

        qint32 channel;
        qint32 baudRate;
        QSerialPort::DataBits dataBits;
        QSerialPort::Parity parity;
        QSerialPort::StopBits stopBits;
        QSerialPort::FlowControl flowControl;

        QString sChannel;
        QString sBaudRate;
        QString sDataBits;
        QString sParity;
        QString sStopBits;
        QString sFlowControl;
    };

    /**
     * @brief instance() returns singleton class instance
     * @return instance
     */
    static SavedPreference* instance();

    /**
     * @brief serialInfo() returns serial port configuration
     * @return serial port configuration
     */
    const DeviceInfo& serialInfo() const;

    /**
     * @brief setSerialInfo stores serial port configuration
     * @param val
     */
    void setSerialInfo(const DeviceInfo& val);

private:
     SavedPreference();
    ~SavedPreference();

     DeviceInfo serialPortInfo;
};
