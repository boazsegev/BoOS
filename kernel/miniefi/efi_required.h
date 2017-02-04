#pragma once
#ifndef H_EFI_REQUIRED_H
#define H_EFI_REQUIRED_H

/* ******************************************************
** EFI API compiling directive
****************************************************** */
#define EFIAPI __attribute__((ms_abi))

/* ******************************************************
** EFI Secpification Version
****************************************************** */
#define EFI_SYSTEM_TABLE_SIGNATURE 0x5453595320494249
#define EFI_2_60_SYSTEM_TABLE_REVISION ((2 << 16) | (60))
#define EFI_2_50_SYSTEM_TABLE_REVISION ((2 << 16) | (50))
#define EFI_2_40_SYSTEM_TABLE_REVISION ((2 << 16) | (40))
#define EFI_2_31_SYSTEM_TABLE_REVISION ((2 << 16) | (31))
#define EFI_2_30_SYSTEM_TABLE_REVISION ((2 << 16) | (30))
#define EFI_2_20_SYSTEM_TABLE_REVISION ((2 << 16) | (20))
#define EFI_2_10_SYSTEM_TABLE_REVISION ((2 << 16) | (10))
#define EFI_2_00_SYSTEM_TABLE_REVISION ((2 << 16) | (00))
#define EFI_1_10_SYSTEM_TABLE_REVISION ((1 << 16) | (10))
#define EFI_1_02_SYSTEM_TABLE_REVISION ((1 << 16) | (02))

#define EFI_SYSTEM_TABLE_REVISION EFI_2_60_SYSTEM_TABLE_REVISION
#define EFI_SPECIFICATION_VERSION EFI_SYSTEM_TABLE_REVISION

/* ******************************************************
** EFI General Types
****************************************************** */
#include "types.h"

typedef uint8_t BOOL;

// typedef uint128_t EFI_GUID;
typedef uint64_t EFI_STATUS;
typedef uint64_t EFI_LBA;
typedef uintptr_t EFI_TPL;
typedef uint8_t EFI_MAC_ADDRESS[32];
typedef uint8_t EFI_IPv4_ADDRESS[4];
typedef uint32_t EFI_IP_ADDRESS[4];
typedef uint8_t EFI_IPv6_ADDRESS[16];
typedef void *EFI_EVENT;
typedef void *EFI_HANDLE;

typedef struct {
  uint32_t head;
  uint16_t id1;
  uint16_t id2;
  uint8_t tail[8];
} __attribute__((aligned(8))) EFI_GUID;
_Static_assert(sizeof(EFI_GUID) == 16, "EFI_GUID != 128 bits");

/* ******************************************************
** EFI Table Header
****************************************************** */
typedef struct {
  uint64_t Signature;
  uint32_t Revision;
  uint32_t HeaderSize;
  uint32_t CRC32;
  uint32_t Reserved;
} EFI_TABLE_HEADER;

/* ******************************************************
** EFI Status Codes (returned values)
****************************************************** */

/* High Bit Clear */
#define EFI_SUCCESS 0
#define EFI_WARN_UNKNOWN_GLYPH 1
#define EFI_WARN_DELETE_FAILURE 2
#define EFI_WARN_WRITE_FAILURE 3
#define EFI_WARN_BUFFER_TOO_SMALL 4
#define EFI_WARN_STALE_DATA 5
#define EFI_WARN_FILE_SYSTEM 6

/* High Bit Set - 0x8000000000000000 */
#define EFI_LOAD_ERROR (0x8000000000000000 | 1)
#define EFI_INVALID_PARAMETER (0x8000000000000000 | 2)
#define EFI_UNSUPPORTED (0x8000000000000000 | 3)
#define EFI_BAD_BUFFER_SIZE (0x8000000000000000 | 4)
#define EFI_BUFFER_TOO_SMALL (0x8000000000000000 | 5)
#define EFI_NOT_READY (0x8000000000000000 | 6)
#define EFI_DEVICE_ERROR (0x8000000000000000 | 7)
#define EFI_WRITE_PROTECTED (0x8000000000000000 | 8)
#define EFI_OUT_OF_RESOURCES (0x8000000000000000 | 9)
#define EFI_VOLUME_CORRUPTED (0x8000000000000000 | 10)
#define EFI_VOLUME_FULL (0x8000000000000000 | 11)
#define EFI_NO_MEDIA (0x8000000000000000 | 12)
#define EFI_MEDIA_CHANGED (0x8000000000000000 | 13)
#define EFI_NOT_FOUND (0x8000000000000000 | 14)
#define EFI_ACCESS_DENIED (0x8000000000000000 | 15)
#define EFI_NO_RESPONSE (0x8000000000000000 | 16)
#define EFI_NO_MAPPING (0x8000000000000000 | 17)
#define EFI_TIMEOUT (0x8000000000000000 | 18)
#define EFI_NOT_STARTED (0x8000000000000000 | 19)
#define EFI_ALREADY_STARTED (0x8000000000000000 | 20)
#define EFI_ABORTED (0x8000000000000000 | 21)
#define EFI_ICMP_ERROR (0x8000000000000000 | 22)
#define EFI_TFTP_ERROR (0x8000000000000000 | 23)
#define EFI_PROTOCOL_ERROR (0x8000000000000000 | 24)
#define EFI_INCOMPATIBLE_VERSION (0x8000000000000000 | 25)
#define EFI_SECURITY_VIOLATION (0x8000000000000000 | 26)
#define EFI_CRC_ERROR (0x8000000000000000 | 27)
#define EFI_END_OF_MEDIA (0x8000000000000000 | 28)
#define EFI_END_OF_FILE (0x8000000000000000 | 31)
#define EFI_INVALID_LANGUAGE (0x8000000000000000 | 32)
#define EFI_COMPROMISED_DATA (0x8000000000000000 | 33)
#define EFI_IP_ADDRESS_CONFLICT (0x8000000000000000 | 34)
#define EFI_HTTP_ERROR (0x8000000000000000 | 35)

#endif
