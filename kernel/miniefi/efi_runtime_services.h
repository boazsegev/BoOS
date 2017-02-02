#ifndef H_EFI_RUNTIME_SERVICES_H
#define H_EFI_RUNTIME_SERVICES_H

#include "efi_config_table.h"
#include "efi_mem.h"
#include "efi_time.h"

#define EFI_RUNTIME_SERVICES_SIGNATURE 0x56524553544e5552
#define EFI_RUNTIME_SERVICES_REVISION EFI_SPECIFICATION_VERSION

/* ******************************************************
** EFI Runtime Variable Attributes
****************************************************** */
#define EFI_VARIABLE_NON_VOLATILE 0x00000001
#define EFI_VARIABLE_BOOTSERVICE_ACCESS 0x00000002
#define EFI_VARIABLE_RUNTIME_ACCESS 0x00000004
#define EFI_VARIABLE_HARDWARE_ERROR_RECORD 0x00000008
/* This attribute is identified by the mnemonic 'HR' elsewhere in this
 * specification. */
#define EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS 0x00000010
#define EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS 0x00000020
#define EFI_VARIABLE_APPEND_WRITE 0x00000040

#define EFI_HARDWARE_ERROR_VARIABLE                                            \
  {                                                                            \
    0x414E6BDD, 0xE47B, 0x47cc, {                                              \
      0xB2, 0x44, 0xBB, 0x61, 0x02, 0x0C, 0xF5, 0x16                           \
    }                                                                          \
  }
/* ******************************************************
** EFI_VARIABLE_AUTHENTICATION descriptors
****************************************************** */
#define EFI_CERT_TYPE_RSA2048_SHA256_GUID                                      \
  {                                                                            \
    0xa7717414, 0xc616, 0x4977, {                                              \
      0x94, 0x20, 0x84, 0x47, 0x12, 0xa7, 0x35, 0xbf                           \
    }                                                                          \
  }
#define EFI_CERT_TYPE_PKCS7_GUID                                               \
  {                                                                            \
    0x4aafd29d, 0x68df, 0x49ee, {                                              \
      0x8a, 0xa9, 0x34, 0x7d, 0x37, 0x56, 0x65, 0xa7                           \
    }                                                                          \
  }
typedef struct _EFI_CERT_BLOCK_RSA_2048_SHA256 {
  EFI_GUID HashType;
  uint8_t PublicKey[256];
  uint8_t Signature[256];
} EFI_CERT_BLOCK_RSA_2048_SHA256;

typedef struct _WIN_CERTIFICATE {
  uint32_t dwLength;
  uint16_t wRevision;
  uint16_t wCertificateType;
  uint8_t *bCertificate;
} WIN_CERTIFICATE;

typedef struct _WIN_CERTIFICATE_UEFI_GUID {
  WIN_CERTIFICATE Hdr;
  EFI_GUID CertType;
  uint8_t *CertData;
} WIN_CERTIFICATE_UEFI_GUID;

typedef struct _WIN_CERTIFICATE_EFI_PKCS1_15 {
  WIN_CERTIFICATE Hdr;
  EFI_GUID HashAlgorithm;
  uint8_t *Signature;
} WIN_CERTIFICATE_EFI_PKCS1_15;

#define WIN_CERT_TYPE_PKCS_SIGNED_DATA 0x0002
#define WIN_CERT_TYPE_EFI_PKCS115 0x0EF0
#define WIN_CERT_TYPE_EFI_GUID 0x0EF1

/* A counter-based authentication method descriptor template */
typedef struct {
  uint64_t MonotonicCount;
  WIN_CERTIFICATE_UEFI_GUID AuthInfo;
} EFI_VARIABLE_AUTHENTICATION;
/* A time-based authentication method descriptor template */
typedef struct {
  EFI_TIME TimeStamp;
  WIN_CERTIFICATE_UEFI_GUID AuthInfo;
} EFI_VARIABLE_AUTHENTICATION_2;

/* ******************************************************
** EFI Runtime Service Variable Function Types
****************************************************** */
typedef EFI_STATUS(EFIAPI *EFI_GET_VARIABLE)(
    uint16_t *VariableName, EFI_GUID *VendorGuid,
    uint32_t *Attributes /* optional */, uintptr_t DataSize,
    void *Data /* optional */);

typedef EFI_STATUS(EFIAPI *EFI_GET_NEXT_VARIABLE_NAME)(
    uintptr_t *VariableNameSize, int16_t *VariableName, EFI_GUID *VendorGuid);

typedef EFI_STATUS(EFIAPI *EFI_SET_VARIABLE)(uint16_t *VariableName,
                                             EFI_GUID *VendorGuid,
                                             uint32_t Attributes,
                                             uintptr_t DataSize, void *Data);

typedef EFI_STATUS(EFIAPI *EFI_QUERY_VARIABLE_INFO)(
    uint32_t Attributes, uint64_t *MaximumVariableStorageSize,
    uint64_t *RemainingVariableStorageSize, uint64_t *MaximumVariableSize);

/* ******************************************************
** EFI Runtime Service Misc Types and definitions
****************************************************** */

typedef enum {
  EfiResetCold,
  EfiResetWarm,
  EfiResetShutdown,
  EfiResetPlatformSpecific
} EFI_RESET_TYPE;

typedef struct {
  uint64_t Length;
  union {
    EFI_PHYSICAL_ADDRESS DataBlock;
    EFI_PHYSICAL_ADDRESS ContinuationPointer;
  } Union;
} EFI_CAPSULE_BLOCK_DESCRIPTOR;

typedef struct {
  EFI_GUID CapsuleGuid;
  uint32_t HeaderSize;
  uint32_t Flags;
  uint32_t CapsuleImageSize;
} EFI_CAPSULE_HEADER;

#define CAPSULE_FLAGS_PERSIST_ACROSS_RESET 0x00010000
#define CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE 0x00020000
#define CAPSULE_FLAGS_INITIATE_RESET 0x00040000

/* ******************************************************
** EFI Runtime Service Misc Function types
****************************************************** */

typedef void(EFIAPI *EFI_RESET_SYSTEM)(EFI_RESET_TYPE ResetType,
                                       EFI_STATUS ResetStatus,
                                       uintptr_t DataSize,
                                       void * /* optional */ ResetData);
typedef EFI_STATUS(EFIAPI *EFI_GET_NEXT_HIGH_MONO_COUNT)(uint32_t *HighCount);

typedef EFI_STATUS(EFIAPI *EFI_UPDATE_CAPSULE)(
    EFI_CAPSULE_HEADER **CapsuleHeaderArray, uintptr_t CapsuleCount,
    EFI_PHYSICAL_ADDRESS /* optional */ ScatterGatherList);

typedef EFI_STATUS(EFIAPI *EFI_QUERY_CAPSULE_CAPABILITIES)(
    EFI_CAPSULE_HEADER **CapsuleHeaderArray, uintptr_t CapsuleCount,
    uint64_t *MaximumCapsuleSize, EFI_RESET_TYPE *ResetType);

typedef struct {
  uint32_t VariableTotalSize;
  uint32_t Reserved; /* padding */
  EFI_GUID CapsuleGuid;
  EFI_TIME CapsuleProcessed;
  EFI_STATUS CapsuleStatus;
} EFI_CAPSULE_RESULT_VARIABLE_HEADER;

typedef struct {
  uint16_t Version;
  uint8_t PayloadIndex;
  uint8_t UpdateImageIndex;
  EFI_GUID UpdateImageTypeId;
  uint16_t *CapsuleFileName;
  uint16_t *CapsuleTarget;
} EFI_CAPSULE_RESULT_VARIABLE_FMP;

#define EFI_OS_INDICATIONS_BOOT_TO_FW_UI 0x0000000000000001
#define EFI_OS_INDICATIONS_TIMESTAMP_REVOCATION 0x0000000000000002
#define EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED 0x0000000000000004
#define EFI_OS_INDICATIONS_FMP_CAPSULE_SUPPORTED 0x0000000000000008
#define EFI_OS_INDICATIONS_CAPSULE_RESULT_VAR_SUPPORTED 0x0000000000000010
#define EFI_OS_INDICATIONS_START_OS_RECOVERY 0x0000000000000020
#define EFI_OS_INDICATIONS_START_PLATFORM_RECOVERY 0x0000000000000040

/* {39B68C46-F7FB-441B-B6EC-16B0F69821F3} */
#define EFI_CAPSULE_REPORT_GUID                                                \
  {0x39b68c46,                                                                 \
   0xf7fb,                                                                     \
   0x441b,                                                                     \
   {0xb6, 0xec, 0x16, 0xb0, 0xf6, 0x98, 0x21, 0xf3}};
/* ******************************************************
** EFI Runtime Services
****************************************************** */
typedef struct {
  EFI_TABLE_HEADER Hdr;
  /* Time Services */
  EFI_GET_TIME GetTime;
  EFI_SET_TIME SetTime;
  EFI_GET_WAKEUP_TIME GetWakeupTime;
  EFI_SET_WAKEUP_TIME SetWakeupTime;
  /* Virtual Memory Services */
  EFI_SET_VIRTUAL_ADDRESS_MAP SetVirtualAddressMap;
  EFI_CONVERT_POINTER ConvertPointer;
  /* Variable Services */
  EFI_GET_VARIABLE GetVariable;
  EFI_GET_NEXT_VARIABLE_NAME GetNextVariableName;
  EFI_SET_VARIABLE SetVariable;
  /* Miscellaneous Services */
  EFI_GET_NEXT_HIGH_MONO_COUNT GetNextHighMonotonicCount;
  EFI_RESET_SYSTEM ResetSystem;
  /* UEFI 2.0 Capsule Services */
  EFI_UPDATE_CAPSULE UpdateCapsule;
  EFI_QUERY_CAPSULE_CAPABILITIES QueryCapsuleCapabilities;
  /* Miscellaneous UEFI 2.0 Services */
  EFI_QUERY_VARIABLE_INFO QueryVariableInfo;
} EFI_RUNTIME_SERVICES;

#endif
