//
//  ZTCFrameInfo.cpp
//  ZTCFrameInfo
//
//  Created by Younghwan Mun on 2016-08-16.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//
#include "ZTCFrameInfo.h"
#include "ZTCFrameDef.h"
#include <unordered_map>

ZTCFrameInfo ZTCFrameInfo::get(ZBInt16 code)
{
    static std::unordered_map<ZBInt16, ZTCFrameInfo> table;
    if (table.empty())
    {
        // BlackBox Messages
        table[BlackBox_ReadSAS                                 ] = {"BlackBox", "ReadSAS", "Request"};
        table[BlackBox_ReadSAS_Confirm                         ] = {"BlackBox", "ReadSAS", "Confirm"};
        table[BlackBox_WriteSAS                                ] = {"BlackBox", "WriteSAS", "Request"};
        table[BlackBox_WriteSAS_Confirm                        ] = {"BlackBox", "WriteSAS", "Confirm"};

        // ZTC Messages
        table[ZTC_EZModeCommissioning_START_Request            ] = {"ZTC", "EZModeCommissioning_START", "Request"};
        table[ZTC_EZModeCommissioning_START_Confirm            ] = {"ZTC", "EZModeCommissioning_START", "Confirm"};
        table[ZTC_AddToAddressMapPermanent_Request             ] = {"ZTC", "AddToAddressMapPermanent", "Request"};
        table[ZTC_AddToAddressMapPermanent_Confirm             ] = {"ZTC", "AddToAddressMapPermanent", "Confirm"};
        table[ZTC_BeeStackSave_Request                         ] = {"ZTC", "BeeStackSave", "Request"};
        table[ZTC_BeeStackSave_Confirm                         ] = {"ZTC", "BeeStackSave", "Confirm"};
        table[ZTC_CPUReset_Request                             ] = {"ZTC", "CPUReset", "Request"};

        table[ZTC_Debug_Event                                  ] = {"ZTC", "Debug_Event", ""};
        table[ZTC_Error_Event                                  ] = {"ZTC", "Error_Event", ""};
        table[ZTC_Event                                        ] = {"ZTC", "Event", ""};
        table[ZTC_GetChannel_Request                           ] = {"ZTC", "GetChannel", "Request"};
        table[ZTC_GetChannel_Confirm                           ] = {"ZTC", "GetChannel", "Confirm"};
        table[ZTC_GetMode_Request                              ] = {"ZTC", "GetMode", "Request"};
        table[ZTC_GetMode_Confirm                              ] = {"ZTC", "GetMode", "Confirm"};
        table[ZTC_GetNumOfMsgs_Request                         ] = {"ZTC", "GetNumOfMsgs", "Request"};
        table[ZTC_GetNumOfMsgs_Confirm                         ] = {"ZTC", "GetNumOfMsgs", "Confirm"};
        table[ZTC_GetPanID_Request                             ] = {"ZTC", "GetPanID", "Request"};
        table[ZTC_GetPanID_Confirm                             ] = {"ZTC", "GetPanID", "Confirm"};
        table[ZTC_GetSeed_Request                              ] = {"ZTC", "GetSeed", "Request"};
        table[ZTC_GetSeed_Confirm                              ] = {"ZTC", "GetSeed", "Confirm"};
        table[ZTC_ModeSelect_Request                           ] = {"ZTC", "ModeSelect", "Request"};
        table[ZTC_ModeSelect_Confirm                           ] = {"ZTC", "ModeSelect", "Confirm"};
        table[ZTC_NVGetDataSetDesc_Request                     ] = {"ZTC", "NVGetDataSetDesc", "Request"};
        table[ZTC_NVGetDataSetDesc_Confirm                     ] = {"ZTC", "NVGetDataSetDesc", "Confirm"};
        table[ZTC_NVGetPageHeaders_Request                     ] = {"ZTC", "NVGetPageHeaders", "Request"};
        table[ZTC_NVGetPageHeaders_Confirm                     ] = {"ZTC", "NVGetPageHeaders", "Confirm"};
        table[ZTC_OTASupportAbortOTAUpgrade_Request            ] = {"ZTC", "OTASupportAbortOTAUpgrade", "Request"};
        table[ZTC_OTASupportAbortOTAUpgrade_Confirm            ] = {"ZTC", "OTASupportAbortOTAUpgrade", "Confirm"};
        table[ZTC_OTASupportSetLocalFileVersionPolicies_Request] = {"ZTC", "OTASupportSetLocalFileVersionPolicies", "Request"};
        table[ZTC_OTASupportSetLocalFileVersionPolicies_Confirm] = {"ZTC", "OTASupportSetLocalFileVersionPolicies", "Confirm"};
        table[ZTC_OTASupportCancelImage_Request                ] = {"ZTC", "OTASupportCancelImage", "Request"};
        table[ZTC_OTASupportCancelImage_Confirm                ] = {"ZTC", "OTASupportCancelImage", "Confirm"};
        table[ZTC_OTASupportCommitImage_Request                ] = {"ZTC", "OTASupportCommitImage", "Request"};
        table[ZTC_OTASupportCommitImage_Confirm                ] = {"ZTC", "OTASupportCommitImage", "Confirm"};
        table[ZTC_OTASupportImageChunk_Request                 ] = {"ZTC", "OTASupportImageChunk", "Request"};
        table[ZTC_OTASupportImageNotify_Request                ] = {"ZTC", "OTASupportImageNotify", "Request"};
        table[ZTC_OTASupportImageNotify_Confirm                ] = {"ZTC", "OTASupportImageNotify", "Confirm"};
        table[ZTC_OTASupportPushImageChunk_Request             ] = {"ZTC", "OTASupportPushImageChunk", "Request"};
        table[ZTC_OTASupportPushImageChunk_Confirm             ] = {"ZTC", "OTASupportPushImageChunk", "Confirm"};
        table[ZTC_OTASupportQueryImage_Request                 ] = {"ZTC", "OTASupportQueryImage", "Request"};
        table[ZTC_OTASupportQueryImageRsp_Request              ] = {"ZTC", "OTASupportQueryImageRsp", "Request"};
        table[ZTC_OTASupportQueryImageRsp_Confirm              ] = {"ZTC", "OTASupportQueryImageRsp", "Confirm"};
        table[ZTC_OTASupportSetMode_Request                    ] = {"ZTC", "OTASupportSetMode", "Request"};
        table[ZTC_OTASupportSetMode_Confirm                    ] = {"ZTC", "OTASupportSetMode", "Confirm"};
        table[ZTC_OTASupportStartImage_Request                 ] = {"ZTC", "OTASupportStartImage", "Request"};
        table[ZTC_OTASupportStartImage_Confirm                 ] = {"ZTC", "OTASupportStartImage", "Confirm"};
        table[ZTC_Ping_Request                                 ] = {"ZTC", "Ping", "Request"};
        table[ZTC_Ping_Confirm                                 ] = {"ZTC", "Ping", "Confirm"};
        table[ZTC_ReadExtAddr_Request                          ] = {"ZTC", "ReadExtAddr", "Request"};
        table[ZTC_ReadExtAddr_Confirm                          ] = {"ZTC", "ReadExtAddr", "Confirm"};
        table[ZTC_ReadMemoryBlock_Request                      ] = {"ZTC", "ReadMemoryBlock", "Request"};
        table[ZTC_ReadMemoryBlock_Confirm                      ] = {"ZTC", "ReadMemoryBlock", "Confirm"};
        table[ZTC_RemoveFromAddressMap_Request                 ] = {"ZTC", "RemoveFromAddressMap", "Request"};
        table[ZTC_RemoveFromAddressMap_Confirm                 ] = {"ZTC", "RemoveFromAddressMap", "Confirm"};
        table[ZTC_S08RadioRegisterRead_Request                 ] = {"ZTC", "S08RadioRegisterRead", "Request"};
        table[ZTC_S08RadioRegisterRead_Confirm                 ] = {"ZTC", "S08RadioRegisterRead", "Confirm"};
        table[ZTC_S08RadioRegisterWrite_Request                ] = {"ZTC", "S08RadioRegisterWrite", "Request"};
        table[ZTC_S08RadioRegisterWrite_Confirm                ] = {"ZTC", "S08RadioRegisterWrite", "Confirm"};
        table[ZTC_SetApsDeviceKey_Request                      ] = {"ZTC", "SetApsDeviceKey", "Request"};
        table[ZTC_SetApsDeviceKey_Confirm                      ] = {"ZTC", "SetApsDeviceKey", "Confirm"};
        table[ZTC_SetChannel_Request                           ] = {"ZTC", "SetChannel", "Request"};
        table[ZTC_SetChannel_Confirm                           ] = {"ZTC", "SetChannel", "Confirm"};
        table[ZTC_SetHighLowRamConcentrator_Request            ] = {"ZTC", "SetHighLowRamConcentrator", "Request"};
        table[ZTC_SetHighLowRamConcentrator_Confirm            ] = {"ZTC", "SetHighLowRamConcentrator", "Confirm"};
        table[ZTC_SetPanID_Request                             ] = {"ZTC", "SetPanID", "Request"};
        table[ZTC_SetPanID_Confirm                             ] = {"ZTC", "SetPanID", "Confirm"};
        table[ZTC_SetPollRate_Request                          ] = {"ZTC", "SetPollRate", "Request"};
        table[ZTC_SetPollRate_Confirm                          ] = {"ZTC", "SetPollRate", "Confirm"};
        table[ZTC_WriteExtAddr_Request                         ] = {"ZTC", "WriteExtAddr", "Request"};
        table[ZTC_WriteExtAddr_Confirm                         ] = {"ZTC", "WriteExtAddr", "Confirm"};
        table[ZTC_WriteMemoryBlock_Confirm                     ] = {"ZTC", "WriteMemoryBlock", "Confirm"};
        table[ZTC_WriteRAMMemoryBlock_Request                  ] = {"ZTC", "WriteRAMMemoryBlock", "Request"};
        table[ZTC_AFReset_Request                              ] = {"ZTC", "AFReset", "Request"};
        table[ZTC_APSReset_Request                             ] = {"ZTC", "APSReset", "Request"};

        // Mac Messages
        table[MAC_Associate_Request                            ] = {"MAC", "Associate", "Request"};
        table[MAC_Associate_Confirm                            ] = {"MAC", "Associate", "Confirm"};
        table[MAC_Associate_Indication                         ] = {"MAC", "Associate", "Indication"};
        table[MAC_Associate_Response                           ] = {"MAC", "Associate", "Response"};
        table[MAC_BeaconNotify_Indication                      ] = {"MAC", "BeaconNotify", "Indication"};
        table[MAC_CommStatus_Indication                        ] = {"MAC", "CommStatus", "Indication"};
        table[MAC_Data_Request                                 ] = {"MAC", "Data", "Request"};
        table[MAC_Data_Confirm                                 ] = {"MAC", "Data", "Confirm"};
        table[MAC_Data_Indication                              ] = {"MAC", "Data", "Indication"};
        table[MAC_Disassociate_Request                         ] = {"MAC", "Disassociate", "Request"};
        table[MAC_Disassociate_Confirm                         ] = {"MAC", "Disassociate", "Confirm"};
        table[MAC_Disassociate_Indication                      ] = {"MAC", "Disassociate", "Indication"};
        table[MAC_GetPIBAttribute_Request                      ] = {"MAC", "GetPIBAttribute", "Request"};
        table[MAC_GetPIBAttribute_Confirm                      ] = {"MAC", "GetPIBAttribute", "Confirm"};
        table[MAC_GTS_Request                                  ] = {"MAC", "GTS", "Request"};
        table[MAC_GTS_Confirm                                  ] = {"MAC", "GTS", "Confirm"};
        table[MAC_GTS_Indication                               ] = {"MAC", "GTS", "Indication"};
        table[MAC_InputError_Confirm                           ] = {"MAC", "InputError", "Confirm"};
        table[MAC_Orphan_Indication                            ] = {"MAC", "Orphan", "Indication"};
        table[MAC_Orphan_Response                              ] = {"MAC", "Orphan", "Response"};
        table[MAC_Poll_Request                                 ] = {"MAC", "Poll", "Request"};
        table[MAC_Poll_Confirm                                 ] = {"MAC", "Poll", "Confirm"};
        table[MAC_PollNotify_Indication                        ] = {"MAC", "PollNotify", "Indication"};
        table[MAC_Purge_Request                                ] = {"MAC", "Purge", "Request"};
        table[MAC_Purge_Confirm                                ] = {"MAC", "Purge", "Confirm"};
        table[MAC_Reset_Request                                ] = {"MAC", "Reset", "Request"};
        table[MAC_Reset_Confirm                                ] = {"MAC", "Reset", "Confirm"};
        table[MAC_RxEnable_Request                             ] = {"MAC", "RxEnable", "Request"};
        table[MAC_RxEnable_Confirm                             ] = {"MAC", "RxEnable", "Confirm"};
        table[MAC_Scan_Request                                 ] = {"MAC", "Scan", "Request"};
        table[MAC_Scan_Confirm                                 ] = {"MAC", "Scan", "Confirm"};
        table[MAC_SetPIBAttribute_Request                      ] = {"MAC", "SetPIBAttribute", "Request"};
        table[MAC_SetPIBAttribute_Confirm                      ] = {"MAC", "SetPIBAttribute", "Confirm"};
        table[MAC_Start_Request                                ] = {"MAC", "Start", "Request"};
        table[MAC_Start_Confirm                                ] = {"MAC", "Start", "Confirm"};
        table[MAC_Sync_Request                                 ] = {"MAC", "Sync", "Request"};
        table[MAC_SyncLoss_Indication                          ] = {"MAC", "SyncLoss", "Indication"};

        // added by mun
        table[MAC_Purge                                        ] = {"MAC", "Purge", "Data"};
    }    
    return table.find(code) != table.end() ? table[code] : ZTCFrameInfo("Unknown");
}

void ZTCFrameInfo::setAttribute(ZBByte code)
{
    static std::unordered_map<ZBByte, ZBString> table;
    if (table.empty())
    {
        // MAC PIB attribute identifiers
        table[MAC_AckWaitDuration                              ] = "AckWaitDuration";
        table[MAC_AssociationPermit                            ] = "AssociationPermit";
        table[MAC_AutoRequest                                  ] = "AutoRequest";
        table[MAC_BattLifeExt                                  ] = "BattLifeExt";
        table[MAC_BattLifeExtPeriods                           ] = "BattLifeExtPeriods";
        table[MAC_BeaconPayload                                ] = "BeaconPayload";
        table[MAC_BeaconPayloadLength                          ] = "BeaconPayloadLength";
        table[MAC_BeaconOrder                                  ] = "BeaconOrder";
        table[MAC_BeaconTxTime                                 ] = "BeaconTxTime";
        table[MAC_BSN                                          ] = "BSN";
        table[MAC_CoordExtendedAddress                         ] = "CoordExtendedAddress";
        table[MAC_CoordShortAddress                            ] = "CoordShortAddress";
        table[MAC_DSN                                          ] = "DSN";
        table[MAC_GTSPermit                                    ] = "GTSPermit";
        table[MAC_LogicalChannel                               ] = "LogicalChannel";
        table[MAC_MaxCSMABackoffs                              ] = "MaxCSMABackoffs";
        table[MAC_MinBE                                        ] = "MinBE";
        table[MAC_PANId                                        ] = "PANId";
        table[MAC_PromiscuousMode                              ] = "PromiscuousMode";
        table[MAC_Role                                         ] = "Role";
        table[MAC_BeaconResponseDenied                         ] = "BeaconResponseDenied";
        table[MAC_BeaconResponseLQIThreshold                   ] = "BeaconResponseLQIThreshold";
        table[MAC_RxOnWhenIdle                                 ] = "RxOnWhenIdle";
        table[MAC_ShortAddress                                 ] = "ShortAddress";
        table[MAC_SuperframeOrder                              ] = "SuperframeOrder";
        table[MAC_TransactionPersistenceTime                   ] = "TransactionPersistenceTime";
        table[MAC_ACLEntryDescriptorSet                        ] = "ACLEntryDescriptorSet";
        table[MAC_ACLEntryDescriptorSetSize                    ] = "ACLEntryDescriptorSetSize";
        table[MAC_DefaultSecurity                              ] = "DefaultSecurity";
        table[MAC_DefaultSecurityMaterialLength                ] = "DefaultSecurityMaterialLength";
        table[MAC_DefaultSecurityMaterial                      ] = "DefaultSecurityMaterial";
        table[MAC_DefaultSecuritySuite                         ] = "DefaultSecuritySuite";
        table[MAC_SecurityMode                                 ] = "SecurityMode";
        table[MAC_AclEntryCurrent                              ] = "AclEntryCurrent";
        table[MAC_AclEntryExtAddress                           ] = "AclEntryExtAddress";
        table[MAC_AclEntryShortAddress                         ] = "AclEntryShortAddress";
        table[MAC_AclEntryPanId                                ] = "AclEntryPanId";
        table[MAC_AclEntrySecurityMaterialLength               ] = "AclEntrySecurityMaterialLength";
        table[MAC_AclEntrySecurityMaterial                     ] = "AclEntrySecurityMaterial";
        table[MAC_AclEntrySecuritySuite                        ] = "AclEntrySecuritySuite";
    }
    _attribute = table.find(code) != table.end() ? table[code] : "Unknown";
}

void ZTCFrameInfo::setStatus(ZBByte code)
{
    static std::unordered_map<ZBByte, ZBString> table;
    if (table.empty())
    {
        table[ZTC_Success       ] = "Success";
        table[ZTC_Failed        ] = "Failed";
        table[ZTC_Timeout       ] = "Timeout";
        table[ZTC_InvalidRequest] = "Invalid Request";
    }
    _status = table.find(code) != table.end() ? table[code] : "Unknown";
}

ZTCFrameInfo::ZTCFrameInfo(const ZBString& layer, const ZBString& service, const ZBString& type) :
    _layer(layer), _message(service), _type(type)
{

}

const ZBString& ZTCFrameInfo::layer() const
{
    return _layer;
}
const ZBString& ZTCFrameInfo::message() const
{
    return _message;
}

const ZBString& ZTCFrameInfo::type() const
{
    return _type;
}

const ZBString& ZTCFrameInfo::attribute() const
{
    return _attribute;
}

const ZBString& ZTCFrameInfo::status() const
{
    return _status;
}
