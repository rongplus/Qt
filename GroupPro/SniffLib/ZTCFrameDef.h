#pragma once
//
//  ZTCFrameDef.h
//  ZTCFrameDef
//
//  Created by Younghwan Mun on 2016-08-12.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//

// Sync: Frame delimiter indicating packet start
#define ZTC_STX                                                 0x02

// Message Status
#define ZTC_Success                                             0x00
#define ZTC_Failed                                              0x01
#define ZTC_Timeout                                             0x07
#define ZTC_InvalidRequest                                      0xFF

// ZTC Messages
#define BlackBox_ReadSAS                                        0x5002
#define BlackBox_ReadSAS_Confirm                                0x5003
#define BlackBox_WriteSAS                                       0x5001
#define BlackBox_WriteSAS_Confirm                               0x5004

#define ZTC_EZModeCommissioning_START_Request                   0xA351
#define ZTC_EZModeCommissioning_START_Confirm                   0xA451
#define ZTC_AddToAddressMapPermanent_Request                    0xA3C0
#define ZTC_AddToAddressMapPermanent_Confirm                    0xA4C0
#define ZTC_BeeStackSave_Request                                0xA3E4
#define ZTC_BeeStackSave_Confirm                                0xA4E4
#define ZTC_CPUReset_Request                                    0xA308
#define ZTC_Debug_Event                                         0xA4FF
#define ZTC_Error_Event                                         0xA4FE
#define ZTC_Event                                               0xA404
#define ZTC_GetChannel_Request                                  0xA312
#define ZTC_GetChannel_Confirm                                  0xA412
#define ZTC_GetMode_Request                                     0xA302
#define ZTC_GetMode_Confirm                                     0xA402
#define ZTC_GetNumOfMsgs_Request                                0xA324
#define ZTC_GetNumOfMsgs_Confirm                                0xA424
#define ZTC_GetPanID_Request                                    0xA314
#define ZTC_GetPanID_Confirm                                    0xA414
#define ZTC_GetSeed_Request                                     0xA3D0
#define ZTC_GetSeed_Confirm                                     0xA4D0
#define ZTC_ModeSelect_Request                                  0xA300
#define ZTC_ModeSelect_Confirm                                  0xA400
#define ZTC_NVGetDataSetDesc_Request                            0xA3E5
#define ZTC_NVGetDataSetDesc_Confirm                            0xA4E5
#define ZTC_NVGetPageHeaders_Request                            0xA3E6
#define ZTC_NVGetPageHeaders_Confirm                            0xA4E6
#define ZTC_OTASupportAbortOTAUpgrade_Request                   0xA32E
#define ZTC_OTASupportAbortOTAUpgrade_Confirm                   0xA42E
#define ZTC_OTASupportSetLocalFileVersionPolicies_Request       0xA32D
#define ZTC_OTASupportSetLocalFileVersionPolicies_Confirm       0xA42D
#define ZTC_OTASupportCancelImage_Request                       0xA32C
#define ZTC_OTASupportCancelImage_Confirm                       0xA42C
#define ZTC_OTASupportCommitImage_Request                       0xA32B
#define ZTC_OTASupportCommitImage_Confirm                       0xA42B
#define ZTC_OTASupportImageChunk_Request                        0xA32F
#define ZTC_OTASupportImageNotify_Request                       0xA3C4
#define ZTC_OTASupportImageNotify_Confirm                       0xA4C4
#define ZTC_OTASupportPushImageChunk_Request                    0xA32A
#define ZTC_OTASupportPushImageChunk_Confirm                    0xA42A
#define ZTC_OTASupportQueryImage_Request                        0xA3C2
#define ZTC_OTASupportQueryImageRsp_Request                     0xA3C3
#define ZTC_OTASupportQueryImageRsp_Confirm                     0xA4C3
#define ZTC_OTASupportSetMode_Request                           0xA328
#define ZTC_OTASupportSetMode_Confirm                           0xA428
#define ZTC_OTASupportStartImage_Request                        0xA329
#define ZTC_OTASupportStartImage_Confirm                        0xA429
#define ZTC_Ping_Request                                        0xA338
#define ZTC_Ping_Confirm                                        0xA438
#define ZTC_ReadExtAddr_Request                                 0xA3D2
#define ZTC_ReadExtAddr_Confirm                                 0xA4D2
#define ZTC_ReadMemoryBlock_Request                             0xA331
#define ZTC_ReadMemoryBlock_Confirm                             0xA431
#define ZTC_RemoveFromAddressMap_Request                        0xA3C1
#define ZTC_RemoveFromAddressMap_Confirm                        0xA4C1
#define ZTC_S08RadioRegisterRead_Request                        0xA33A
#define ZTC_S08RadioRegisterRead_Confirm                        0xA43A
#define ZTC_S08RadioRegisterWrite_Request                       0xA339
#define ZTC_S08RadioRegisterWrite_Confirm                       0xA439
#define ZTC_SetApsDeviceKey_Request                             0xA33D
#define ZTC_SetApsDeviceKey_Confirm                             0xA43D
#define ZTC_SetChannel_Request                                  0xA313
#define ZTC_SetChannel_Confirm                                  0xA413
#define ZTC_SetHighLowRamConcentrator_Request                   0xA350
#define ZTC_SetHighLowRamConcentrator_Confirm                   0xA450
#define ZTC_SetPanID_Request                                    0xA315
#define ZTC_SetPanID_Confirm                                    0xA415
#define ZTC_SetPollRate_Request                                 0xA343
#define ZTC_SetPollRate_Confirm                                 0xA443
#define ZTC_WriteExtAddr_Request                                0xA3DB
#define ZTC_WriteExtAddr_Confirm                                0xA4DB
#define ZTC_WriteMemoryBlock_Confirm                            0xA430
#define ZTC_WriteRAMMemoryBlock_Request                         0xA330
#define ZTC_AFReset_Request                                     0xA305
#define ZTC_APSReset_Request                                    0xA306

// ZTC_ModeSelect Values
#define ZTC_DisableMode                                         0x00
#define ZTC_HookMode                                            0x01
#define ZTC_MonitorMode                                         0x02

// MAC Messages
#define MAC_Associate_Request                                   0x8500
#define MAC_Associate_Confirm                                   0x8401
#define MAC_Associate_Indication                                0x8400
#define MAC_Associate_Response                                  0x8501
#define MAC_BeaconNotify_Indication                             0x8404
#define MAC_CommStatus_Indication                               0x840C
#define MAC_Data_Request                                        0x8700
#define MAC_Data_Confirm                                        0x8600
#define MAC_Data_Indication                                     0x8601
#define MAC_Disassociate_Request                                0x8502
#define MAC_Disassociate_Confirm                                0x8403
#define MAC_Disassociate_Indication                             0x8402
#define MAC_GetPIBAttribute_Request                             0x8503
#define MAC_GetPIBAttribute_Confirm                             0x8405
#define MAC_GTS_Request                                         0x8504
#define MAC_GTS_Confirm                                         0x8407
#define MAC_GTS_Indication                                      0x8406
#define MAC_InputError_Confirm                                  0x8411
#define MAC_Orphan_Indication                                   0x8408
#define MAC_Orphan_Response                                     0x8505
#define MAC_Poll_Request                                        0x850C
#define MAC_Poll_Confirm                                        0x8410
#define MAC_PollNotify_Indication                               0x8414
#define MAC_Purge_Request                                       0x8701
#define MAC_Purge_Confirm                                       0x8602
#define MAC_Reset_Request                                       0x8506
#define MAC_Reset_Confirm                                       0x8409
#define MAC_RxEnable_Request                                    0x8507
#define MAC_RxEnable_Confirm                                    0x840A
#define MAC_Scan_Request                                        0x8508
#define MAC_Scan_Confirm                                        0x840B
#define MAC_SetPIBAttribute_Request                             0x8509
#define MAC_SetPIBAttribute_Confirm                             0x840D
#define MAC_Start_Request                                       0x850A
#define MAC_Start_Confirm                                       0x840E
#define MAC_Sync_Request                                        0x850B
#define MAC_SyncLoss_Indication                                 0x840F

// added by mun
#define MAC_Purge                                               0x8603

// MAC PIB attribute identifiers
#define MAC_AckWaitDuration                                     0x40
#define MAC_AssociationPermit                                   0x41
#define MAC_AutoRequest                                         0x42
#define MAC_BattLifeExt                                         0x43
#define MAC_BattLifeExtPeriods                                  0x44
#define MAC_BeaconPayload                                       0x45
#define MAC_BeaconPayloadLength                                 0x46
#define MAC_BeaconOrder                                         0x47
#define MAC_BeaconTxTime                                        0x48
#define MAC_BSN                                                 0x49
#define MAC_CoordExtendedAddress                                0x4A
#define MAC_CoordShortAddress                                   0x4B
#define MAC_DSN                                                 0x4C
#define MAC_GTSPermit                                           0x4D
#define MAC_LogicalChannel                                      0x21
#define MAC_MaxCSMABackoffs                                     0x4E
#define MAC_MinBE                                               0x4F
#define MAC_PANId                                               0x50
#define MAC_PromiscuousMode                                     0x51
#define MAC_Role                                                0x20
#define MAC_BeaconResponseDenied                                0x24
#define MAC_BeaconResponseLQIThreshold                          0x27
#define MAC_RxOnWhenIdle                                        0x52
#define MAC_ShortAddress                                        0x53
#define MAC_SuperframeOrder                                     0x54
#define MAC_TransactionPersistenceTime                          0x55
#define MAC_ACLEntryDescriptorSet                               0x70
#define MAC_ACLEntryDescriptorSetSize                           0x71
#define MAC_DefaultSecurity                                     0x72
#define MAC_DefaultSecurityMaterialLength                       0x73
#define MAC_DefaultSecurityMaterial                             0x74
#define MAC_DefaultSecuritySuite                                0x75
#define MAC_SecurityMode                                        0x76
#define MAC_AclEntryCurrent                                     0x77
#define MAC_AclEntryExtAddress                                  0x78
#define MAC_AclEntryShortAddress                                0x79
#define MAC_AclEntryPanId                                       0x7A
#define MAC_AclEntrySecurityMaterialLength                      0x7B
#define MAC_AclEntrySecurityMaterial                            0x7C
#define MAC_AclEntrySecuritySuite                               0x7D
