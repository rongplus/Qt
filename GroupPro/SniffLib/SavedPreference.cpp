//
//  SavedPreference.cpp
//  SavedPreference
//
//  Created by Younghwan Mun on 2016-08-11.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//
#include "SavedPreference.h"

SavedPreference* SavedPreference::instance()
{
    static SavedPreference inst;
    return &inst;
}

SavedPreference::SavedPreference()
{

}

SavedPreference::~SavedPreference()
{

}

const SavedPreference::DeviceInfo& SavedPreference::serialInfo() const
{
    return serialPortInfo;
}

void SavedPreference::setSerialInfo(const DeviceInfo& val)
{
    serialPortInfo = val;
}
