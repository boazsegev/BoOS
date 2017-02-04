#pragma once
#ifndef H_EFI_TIME_H
#define H_EFI_TIME_H

#include "efi_required.h"

/* ******************************************************
** EFI Time Data Structures
****************************************************** */

typedef struct {
  uint16_t Year;  // 1900 – 9999
  uint8_t Month;  // 1–12
  uint8_t Day;    // 1–31
  uint8_t Hour;   // 0–23
  uint8_t Minute; // 0–59
  uint8_t Second; // 0–59
  uint8_t Pad1;
  uint32_t Nanosecond; // 0 – 999,999,999
  int16_t TimeZone;    // -1440 to 1440 or 2047
  uint8_t Daylight;
  uint8_t Pad2;
} EFI_TIME;

typedef struct {
  uint32_t Resolution;
  uint32_t Accuracy;
  uint8_t SetsToZero;
} EFI_TIME_CAPABILITIES;

/* ******************************************************
** EFI Time Services
****************************************************** */
#define EFI_TIME_ADJUST_DAYLIGHT 0x01
#define EFI_TIME_IN_DAYLIGHT 0x02
#define EFI_UNSPECIFIED_TIMEZONE 0x07FF

/* ******************************************************
** EFI Time API
****************************************************** */

typedef EFI_STATUS(EFIAPI *EFI_GET_TIME)(
    EFI_TIME *Time, EFI_TIME_CAPABILITIES *Capabilities /* optional */);

typedef EFI_STATUS(EFIAPI *EFI_SET_TIME)(EFI_TIME *Time);

typedef EFI_STATUS(EFIAPI *EFI_GET_WAKEUP_TIME)(uint8_t *Enabled,
                                                uint8_t *Pending,
                                                EFI_TIME *Time);

typedef EFI_STATUS(EFIAPI *EFI_SET_WAKEUP_TIME)(uint8_t Enable, EFI_TIME *Time);

#endif
