// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: fl_defs.proto

#ifndef PROTOBUF_fl_5fdefs_2eproto__INCLUDED
#define PROTOBUF_fl_5fdefs_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3003000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3003000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)

namespace protobuf_fl_5fdefs_2eproto {
// Internal implementation detail -- do not call these.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[];
  static const ::google::protobuf::uint32 offsets[];
  static void InitDefaultsImpl();
  static void Shutdown();
};
void AddDescriptors();
void InitDefaults();
}  // namespace protobuf_fl_5fdefs_2eproto

enum StandardType {
  STD_UL = 0,
  STD_ULC = 1,
  StandardType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  StandardType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool StandardType_IsValid(int value);
const StandardType StandardType_MIN = STD_UL;
const StandardType StandardType_MAX = STD_ULC;
const int StandardType_ARRAYSIZE = StandardType_MAX + 1;

const ::google::protobuf::EnumDescriptor* StandardType_descriptor();
inline const ::std::string& StandardType_Name(StandardType value) {
  return ::google::protobuf::internal::NameOfEnum(
    StandardType_descriptor(), value);
}
inline bool StandardType_Parse(
    const ::std::string& name, StandardType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<StandardType>(
    StandardType_descriptor(), name, value);
}
enum DeviceBaseModel {
  DEV_UNDEFINED = 0,
  DEV_MIRCOM = 1,
  DEV_COOPER = 2,
  DEV_SYSTEM_SENSOR = 3,
  DEV_GENTEX = 4,
  DEV_POTTER = 5,
  DEV_MPD_65PK_MIRCOM_SERIES_65 = 6,
  DEV_MPD_65P_MIRCOM_SERIES_65 = 7,
  DEV_TD_135_FIXED_TO_135_DEGREE_F_RESSETTABLE = 8,
  DEV_TD_200_FIXED_TO_200_DEGREE_F_RESSETTABLE = 9,
  DEV_MHD_65_135_MIRCOM_SERIES_65 = 10,
  DEV_MS_401U_SINGLE_STAGE_PULL_STATION = 11,
  DEV_GL_004K_GLASS_ROD = 12,
  DEV_MS_705U_SINGLE_ACTION_PULL_STATION = 13,
  DEV_MS_710U_DUAL_ACTION_PULL_STATION = 14,
  DEV_BB_700_SERIES_700 = 15,
  DeviceBaseModel_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  DeviceBaseModel_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool DeviceBaseModel_IsValid(int value);
const DeviceBaseModel DeviceBaseModel_MIN = DEV_UNDEFINED;
const DeviceBaseModel DeviceBaseModel_MAX = DEV_BB_700_SERIES_700;
const int DeviceBaseModel_ARRAYSIZE = DeviceBaseModel_MAX + 1;

const ::google::protobuf::EnumDescriptor* DeviceBaseModel_descriptor();
inline const ::std::string& DeviceBaseModel_Name(DeviceBaseModel value) {
  return ::google::protobuf::internal::NameOfEnum(
    DeviceBaseModel_descriptor(), value);
}
inline bool DeviceBaseModel_Parse(
    const ::std::string& name, DeviceBaseModel* value) {
  return ::google::protobuf::internal::ParseNamedEnum<DeviceBaseModel>(
    DeviceBaseModel_descriptor(), name, value);
}
enum SignalingType {
  SIG_UNDEFINED = 0,
  SIG_CONTINUES = 1,
  SIG_TEMPORAL = 2,
  SIG_MARCH_TIME = 3,
  SIG_CALIFORNIA = 4,
  SignalingType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  SignalingType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool SignalingType_IsValid(int value);
const SignalingType SignalingType_MIN = SIG_UNDEFINED;
const SignalingType SignalingType_MAX = SIG_CALIFORNIA;
const int SignalingType_ARRAYSIZE = SignalingType_MAX + 1;

const ::google::protobuf::EnumDescriptor* SignalingType_descriptor();
inline const ::std::string& SignalingType_Name(SignalingType value) {
  return ::google::protobuf::internal::NameOfEnum(
    SignalingType_descriptor(), value);
}
inline bool SignalingType_Parse(
    const ::std::string& name, SignalingType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<SignalingType>(
    SignalingType_descriptor(), name, value);
}
enum LocalRelay {
  LOCAL_RELAY_0 = 0,
  LOCAL_RELAY_1 = 1,
  LOCAL_RELAY_2 = 2,
  LOCAL_RELAY_3 = 3,
  LocalRelay_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  LocalRelay_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool LocalRelay_IsValid(int value);
const LocalRelay LocalRelay_MIN = LOCAL_RELAY_0;
const LocalRelay LocalRelay_MAX = LOCAL_RELAY_3;
const int LocalRelay_ARRAYSIZE = LocalRelay_MAX + 1;

const ::google::protobuf::EnumDescriptor* LocalRelay_descriptor();
inline const ::std::string& LocalRelay_Name(LocalRelay value) {
  return ::google::protobuf::internal::NameOfEnum(
    LocalRelay_descriptor(), value);
}
inline bool LocalRelay_Parse(
    const ::std::string& name, LocalRelay* value) {
  return ::google::protobuf::internal::ParseNamedEnum<LocalRelay>(
    LocalRelay_descriptor(), name, value);
}
enum InputType {
  INPUT_0 = 0,
  INPUT_1 = 1,
  INPUT_2 = 2,
  INPUT_3 = 3,
  InputType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  InputType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool InputType_IsValid(int value);
const InputType InputType_MIN = INPUT_0;
const InputType InputType_MAX = INPUT_3;
const int InputType_ARRAYSIZE = InputType_MAX + 1;

const ::google::protobuf::EnumDescriptor* InputType_descriptor();
inline const ::std::string& InputType_Name(InputType value) {
  return ::google::protobuf::internal::NameOfEnum(
    InputType_descriptor(), value);
}
inline bool InputType_Parse(
    const ::std::string& name, InputType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<InputType>(
    InputType_descriptor(), name, value);
}
enum SystemStatusType {
  SYS_STAT_INITDONE = 0,
  SYS_STAT_COMMON_ALM = 1,
  SYS_STAT_COMMON_SUPV = 2,
  SYS_STAT_COMMON_TRB = 3,
  SYS_STAT_AUTO_SS_TIMING = 4,
  SYS_STAT_EVAC_ACTIVE = 5,
  SYS_STAT_ALARM_ACK = 6,
  SYS_STAT_FIRE_DRILL = 7,
  SYS_STAT_SIG_SIL = 8,
  SYS_STAT_BUZZER_SIL = 9,
  SYS_STAT_CONFIG_MODE = 10,
  SYS_STAT_SYSRESET = 11,
  SYS_STAT_WALKTEST = 12,
  SYS_AC_ON = 13,
  SystemStatusType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  SystemStatusType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool SystemStatusType_IsValid(int value);
const SystemStatusType SystemStatusType_MIN = SYS_STAT_INITDONE;
const SystemStatusType SystemStatusType_MAX = SYS_AC_ON;
const int SystemStatusType_ARRAYSIZE = SystemStatusType_MAX + 1;

const ::google::protobuf::EnumDescriptor* SystemStatusType_descriptor();
inline const ::std::string& SystemStatusType_Name(SystemStatusType value) {
  return ::google::protobuf::internal::NameOfEnum(
    SystemStatusType_descriptor(), value);
}
inline bool SystemStatusType_Parse(
    const ::std::string& name, SystemStatusType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<SystemStatusType>(
    SystemStatusType_descriptor(), name, value);
}
enum LocalTroubleType {
  TRB_NONE = 0,
  TRB_POWERUP = 1,
  TRB_SYSUPDATE = 2,
  TRB_SYSRESET = 3,
  TRB_UNIT_TRB = 5,
  TRB_BAD_LOGENTRY = 6,
  TRB_EVLOG_INIT = 7,
  TRB_BATTERY_NOT_PRESENT = 8,
  TRB_BATTERY_FAULT = 9,
  TRB_CHRGR_FAULT = 11,
  TRB_BOOSTER_FAULT = 12,
  TRB_AC_SUPPLY = 14,
  TRB_GND_FLT = 15,
  TRB_SIG_SIL = 16,
  TRB_CFG_NOTPROG = 17,
  TRB_TRBCKT_ALARM = 18,
  TRB_SERVICE_TOOL = 20,
  TRB_IPT_CKT = 21,
  TRB_OPT_CKT = 22,
  LocalTroubleType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  LocalTroubleType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool LocalTroubleType_IsValid(int value);
const LocalTroubleType LocalTroubleType_MIN = TRB_NONE;
const LocalTroubleType LocalTroubleType_MAX = TRB_OPT_CKT;
const int LocalTroubleType_ARRAYSIZE = LocalTroubleType_MAX + 1;

const ::google::protobuf::EnumDescriptor* LocalTroubleType_descriptor();
inline const ::std::string& LocalTroubleType_Name(LocalTroubleType value) {
  return ::google::protobuf::internal::NameOfEnum(
    LocalTroubleType_descriptor(), value);
}
inline bool LocalTroubleType_Parse(
    const ::std::string& name, LocalTroubleType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<LocalTroubleType>(
    LocalTroubleType_descriptor(), name, value);
}
enum RemoteTroubleType {
  REM_NONE = 0,
  REM_TRB_SYSUPDATE = 2,
  REM_TRB_EVLOG_INIT = 6,
  REM_TRB_BAD_LOGENTRY = 7,
  REM_TRB_STUCK_SWI = 8,
  REM_TRB_BATTERY_NOT_PRESENT = 11,
  REM_TRB_BATTERY_FAULT = 12,
  REM_TRB_CHRGR_FAULT = 14,
  REM_TRB_BOOSTER_FAULT = 15,
  REM_TRB_AC_SUPPLY = 17,
  REM_TRB_IPT_CKT_OPEN = 23,
  REM_TRB_OPT_CKT_OPEN = 25,
  REM_TRB_OPT_CKT_SHORT = 27,
  RemoteTroubleType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  RemoteTroubleType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool RemoteTroubleType_IsValid(int value);
const RemoteTroubleType RemoteTroubleType_MIN = REM_NONE;
const RemoteTroubleType RemoteTroubleType_MAX = REM_TRB_OPT_CKT_SHORT;
const int RemoteTroubleType_ARRAYSIZE = RemoteTroubleType_MAX + 1;

const ::google::protobuf::EnumDescriptor* RemoteTroubleType_descriptor();
inline const ::std::string& RemoteTroubleType_Name(RemoteTroubleType value) {
  return ::google::protobuf::internal::NameOfEnum(
    RemoteTroubleType_descriptor(), value);
}
inline bool RemoteTroubleType_Parse(
    const ::std::string& name, RemoteTroubleType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<RemoteTroubleType>(
    RemoteTroubleType_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)


#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::StandardType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::StandardType>() {
  return ::StandardType_descriptor();
}
template <> struct is_proto_enum< ::DeviceBaseModel> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::DeviceBaseModel>() {
  return ::DeviceBaseModel_descriptor();
}
template <> struct is_proto_enum< ::SignalingType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::SignalingType>() {
  return ::SignalingType_descriptor();
}
template <> struct is_proto_enum< ::LocalRelay> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::LocalRelay>() {
  return ::LocalRelay_descriptor();
}
template <> struct is_proto_enum< ::InputType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::InputType>() {
  return ::InputType_descriptor();
}
template <> struct is_proto_enum< ::SystemStatusType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::SystemStatusType>() {
  return ::SystemStatusType_descriptor();
}
template <> struct is_proto_enum< ::LocalTroubleType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::LocalTroubleType>() {
  return ::LocalTroubleType_descriptor();
}
template <> struct is_proto_enum< ::RemoteTroubleType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::RemoteTroubleType>() {
  return ::RemoteTroubleType_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_fl_5fdefs_2eproto__INCLUDED
