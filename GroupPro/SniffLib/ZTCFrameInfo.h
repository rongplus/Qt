#pragma once
//
//  ZTCFrameInfo.h
//  ZTCFrameInfo
//
//  Created by Younghwan Mun on 2016-08-12.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//

#include "ZBType.h"
#include "snifflib_global.h"
class  SNIFFLIB_EXPORT ZTCFrameInfo
{
public:
    /**
     * @brief get() returns frame information (layer, service, type)
     * @param code
     * @return frame information
     */
    static ZTCFrameInfo get(ZBInt16 code);

    /**
     * @brief ZTCFrameInfo constructor
     * @param layer
     * @param service
     * @param type
     */
    ZTCFrameInfo(const ZBString& layer = "", const ZBString& service = "", const ZBString& type = "");

    /**
     * @brief setAttribute() sets attribute information
     * @param code
     */
    void setAttribute(ZBByte code);

    /**
     * @brief setStatus() sets status information
     * @param code
     */
    void setStatus(ZBByte code);

    /**
     * @brief layer() returns string
     * @return layer string
     */
    const ZBString& layer() const;

    /**
     * @brief message returns string
     * @return message string
     */
    const ZBString& message() const;

    /**
     * @brief type() returns string
     * @return type string
     */
    const ZBString& type() const;

    /**
     * @brief attribute() returns string
     * @return attribute string
     */
    const ZBString& attribute() const;

    /**
     * @brief status() returns string
     * @return status string
     */
    const ZBString& status() const;

private:
    ZBString _layer;
    ZBString _message;
    ZBString _type;
    ZBString _attribute;
    ZBString _status;
};
