#ifndef H_EFI_BOOT_SERVICES_H
#define H_EFI_BOOT_SERVICES_H
#include "efi_mem.h"
#include "efi_time.h"

/* ******************************************************
** EFI Events and scheduling Types and Definitions
****************************************************** */
#define EVT_TIMER 0x80000000
#define EVT_RUNTIME 0x40000000
#define EVT_NOTIFY_WAIT 0x00000100
#define EVT_NOTIFY_SIGNAL 0x00000200
#define EVT_SIGNAL_EXIT_BOOT_SERVICES 0x00000201
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE 0x60000202

#define EFI_EVENT_GROUP_EXIT_BOOT_SERVICES                                     \
  {                                                                            \
    0x27abf055, 0xb1b8, 0x4c26, 0x80, 0x48, 0x74, 0x8f, 0x37, 0xba, 0xa2, 0xdf \
  }                                                                            \
  }
#define EFI_EVENT_GROUP_VIRTUAL_ADDRESS_CHANGE                                 \
  { 0x13fa7698, 0xc831, 0x49c7, 0x87, 0xea, 0x8f, 0x43, 0xfc, 0xc2, 0x51, 0x96 }
#define EFI_EVENT_GROUP_MEMORY_MAP_CHANGE                                      \
  { 0x78bee926, 0x692f, 0x48fd, 0x9e, 0xdb, 0x1, 0x42, 0x2e, 0xf0, 0xd7, 0xab }
#define EFI_EVENT_GROUP_READY_TO_BOOT                                          \
  { 0x7ce88fb3, 0x4bd7, 0x4679, 0x87, 0xa8, 0xa8, 0xd8, 0xde, 0xe5, 0xd, 0x2b }

/* an event handle */
typedef void *EFI_EVENT;
/* an event callback */
typedef void(EFIAPI *EFI_EVENT_NOTIFY)(EFI_EVENT Event, void *Context);
typedef enum { /* enum for event timers */
               TimerCancel,
               TimerPeriodic,
               TimerRelative
} EFI_TIMER_DELAY;

/* ******************************************************
** EFI Events and scheduling API
****************************************************** */

typedef EFI_STATUS(EFIAPI *EFI_CREATE_EVENT)(
    uint32_t Type, EFI_TPL NotifyTpl,
    EFI_EVENT_NOTIFY /* optional */ NotifyFunction,
    void * /* optional */ NotifyContext, EFI_EVENT *Event);

typedef EFI_STATUS(EFIAPI *EFI_CREATE_EVENT_EX)(
    uint32_t Type, EFI_TPL NotifyTpl, EFI_EVENT_NOTIFY NotifyFunction,
    const void /* optional */ *NotifyContext,
    const EFI_GUID /* optional */ *EventGroup, EFI_EVENT /* optional */ *Event);

typedef EFI_STATUS(EFIAPI *EFI_CLOSE_EVENT)(EFI_EVENT Event);

typedef EFI_STATUS(EFIAPI *EFI_SIGNAL_EVENT)(EFI_EVENT Event);

typedef EFI_STATUS(EFIAPI *EFI_WAIT_FOR_EVENT)(uintptr_t NumberOfEvents,
                                               EFI_EVENT *Event,
                                               uintptr_t *Index);

typedef EFI_STATUS(EFIAPI *EFI_CHECK_EVENT)(EFI_EVENT Event);
typedef EFI_STATUS(EFIAPI *EFI_SET_TIMER)(EFI_EVENT Event, EFI_TIMER_DELAY Type,
                                          uint64_t TriggerTime);

/* ******************************************************
** EFI Task Priority and Scheduling types and definitions
****************************************************** */
#define TPL_APPLICATION 4
#define TPL_CALLBACK 8
#define TPL_NOTIFY 16
#define TPL_HIGH_LEVEL 31

/* Defined in `efi_required.h`: */
/*    typedef uintptr_t EFI_TPL */

/* ******************************************************
** EFI Task Priority and Scheduling API
****************************************************** */
typedef EFI_TPL(EFIAPI *EFI_RAISE_TPL)(EFI_TPL NewTpl);

typedef void(EFIAPI *EFI_RESTORE_TPL)(EFI_TPL OldTpl);

/* ******************************************************
** EFI Protocol Installation Types
****************************************************** */
typedef enum { EFI_NATIVE_INTERFACE } EFI_INTERFACE_TYPE;
typedef enum {
  AllHandles,
  ByRegisterNotify,
  ByProtocol
} EFI_LOCATE_SEARCH_TYPE;

#define EFI_DEVICE_PATH_PROTOCOL_GUID                                          \
  {                                                                            \
    0x09576e91, 0x6d3f, 0x11d2, {                                              \
      0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b                           \
    }                                                                          \
  }
typedef struct _EFI_DEVICE_PATH_PROTOCOL {
  uint8_t Type;
  uint8_t SubType;
  uint8_t Length[2];
} EFI_DEVICE_PATH_PROTOCOL;

#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL 0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL 0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL 0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER 0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE 0x00000020

typedef struct {
  EFI_HANDLE AgentHandle;
  EFI_HANDLE ControllerHandle;
  uint32_t Attributes;
  uint32_t OpenCount;
} EFI_OPEN_PROTOCOL_INFORMATION_ENTRY;

/* ******************************************************
** EFI Protocol Installation API
****************************************************** */

typedef EFI_STATUS(EFIAPI *EFI_INSTALL_PROTOCOL_INTERFACE)(
    EFI_HANDLE *Handle, EFI_GUID *Protocol, EFI_INTERFACE_TYPE InterfaceType,
    void *Interface);

typedef EFI_STATUS(EFIAPI *EFI_UNINSTALL_PROTOCOL_INTERFACE)(EFI_HANDLE Handle,
                                                             EFI_GUID *Protocol,
                                                             void *Interface);

typedef EFI_STATUS(EFIAPI *EFI_REINSTALL_PROTOCOL_INTERFACE)(
    EFI_HANDLE Handle, EFI_GUID *Protocol, void *OldInterface,
    void *NewInterface);

typedef EFI_STATUS(EFIAPI *EFI_REGISTER_PROTOCOL_NOTIFY)(EFI_GUID *Protocol,
                                                         EFI_EVENT Event,
                                                         void **Registration);

typedef EFI_STATUS(EFIAPI *EFI_LOCATE_HANDLE)(EFI_LOCATE_SEARCH_TYPE SearchType,
                                              EFI_GUID /* optional */ *Protocol,
                                              void /* optional */ *SearchKey,
                                              uintptr_t *BufferSize,
                                              EFI_HANDLE *Buffer);

typedef EFI_STATUS(EFIAPI *EFI_HANDLE_PROTOCOL)(EFI_HANDLE Handle,
                                                EFI_GUID *Protocol,
                                                void **Interface);
typedef EFI_STATUS(EFIAPI *EFI_LOCATE_DEVICE_PATH)(
    EFI_GUID *Protocol, EFI_DEVICE_PATH_PROTOCOL **DevicePath,
    EFI_HANDLE *Device);

typedef EFI_STATUS(EFIAPI *EFI_OPEN_PROTOCOL)(
    EFI_HANDLE Handle, EFI_GUID *Protocol, void /* optional */ **Interface,
    EFI_HANDLE AgentHandle, EFI_HANDLE ControllerHandle, uint32_t Attributes);

typedef EFI_STATUS(EFIAPI *EFI_CLOSE_PROTOCOL)(EFI_HANDLE Handle,
                                               EFI_GUID *Protocol,
                                               EFI_HANDLE AgentHandle,
                                               EFI_HANDLE ControllerHandle);

typedef EFI_STATUS(EFIAPI *EFI_OPEN_PROTOCOL_INFORMATION)(
    EFI_HANDLE Handle, EFI_GUID *Protocol,
    EFI_OPEN_PROTOCOL_INFORMATION_ENTRY **EntryBuffer, uintptr_t *EntryCount);

typedef EFI_STATUS(EFIAPI *EFI_CONNECT_CONTROLLER)(
    EFI_HANDLE ControllerHandle, EFI_HANDLE /* optional */ *DriverImageHandle,
    EFI_DEVICE_PATH_PROTOCOL /* optional */ *RemainingDevicePath,
    uint8_t Recursive);

typedef EFI_STATUS(EFIAPI *EFI_DISCONNECT_CONTROLLER)(
    EFI_HANDLE ControllerHandle, EFI_HANDLE /* optional */ DriverImageHandle,
    EFI_HANDLE /* optional */ ChildHandle);

typedef EFI_STATUS(EFIAPI *EFI_PROTOCOLS_PER_HANDLE)(
    EFI_HANDLE Handle, EFI_GUID ***ProtocolBuffer,
    uintptr_t ProtocolBufferCount);

typedef EFI_STATUS(EFIAPI *EFI_LOCATE_HANDLE_BUFFER)(
    EFI_LOCATE_SEARCH_TYPE SearchType, EFI_GUID /* optional */ *Protocol,
    void /* optional */ *SearchKey, uintptr_t *NoHandles, EFI_HANDLE **Buffer);

typedef EFI_STATUS(EFIAPI *EFI_LOCATE_PROTOCOL)(
    EFI_GUID *Protocol, void /* optional */ *Registration, void **Interface);

typedef EFI_STATUS(EFIAPI *EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES)(
    EFI_HANDLE *Handle, ...);

typedef EFI_STATUS(EFIAPI *EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES)(
    EFI_HANDLE Handle, ...);

/* ******************************************************
** EFI Image Loading types and definitions
****************************************************** */

#define EFI_HII_PACKAGE_LIST_PROTOCOL_GUID                                     \
  {                                                                            \
    0x6a1ee763, 0xd47a, 0x43b4, {                                              \
      0xaa, 0xbe, 0xef, 0x1d, 0xe2, 0xab, 0x56, 0xfc                           \
    }                                                                          \
  }
#define EFI_HII_PACKAGE_TYPE_ALL 0x00
#define EFI_HII_PACKAGE_TYPE_GUID 0x01
#define EFI_HII_PACKAGE_FORMS 0x02
#define EFI_HII_PACKAGE_STRINGS 0x04
#define EFI_HII_PACKAGE_FONTS 0x05
#define EFI_HII_PACKAGE_IMAGES 0x06
#define EFI_HII_PACKAGE_SIMPLE_FONTS 0x07
#define EFI_HII_PACKAGE_DEVICE_PATH 0x08
#define EFI_HII_PACKAGE_KEYBOARD_LAYOUT 0x09
#define EFI_HII_PACKAGE_ANIMATIONS 0x0A
#define EFI_HII_PACKAGE_END 0xDF
#define EFI_HII_PACKAGE_TYPE_SYSTEM_BEGIN 0xE0
#define EFI_HII_PACKAGE_TYPE_SYSTEM_END 0xFF

typedef struct {
  uint32_t Length : 24;
  uint32_t Type : 8;
  uint8_t *Data;
} EFI_HII_PACKAGE_HEADER;

typedef struct {
  EFI_GUID PackageListGuid;
  uint32_t PackagLength;
} EFI_HII_PACKAGE_LIST_HEADER;

typedef EFI_HII_PACKAGE_LIST_HEADER *EFI_HII_PACKAGE_LIST_PROTOCOL;

/* more starting on page 1960 of the spects, but it's font related */

/* ******************************************************
** EFI Image Loading API
****************************************************** */

typedef EFI_STATUS(EFIAPI *EFI_IMAGE_LOAD)(uint8_t BootPolicy,
                                           EFI_HANDLE ParentImageHandle,
                                           EFI_DEVICE_PATH_PROTOCOL *DevicePath,
                                           void /* optional */ *SourceBuffer,
                                           uintptr_t SourceSize,
                                           EFI_HANDLE *ImageHandle);

typedef EFI_STATUS(EFIAPI *EFI_IMAGE_START)(EFI_HANDLE ImageHandle,
                                            uintptr_t *ExitDataSize,
                                            uint16_t **ExitData);

typedef EFI_STATUS(EFIAPI *EFI_IMAGE_UNLOAD)(EFI_HANDLE ImageHandle);

/* ******************************************************
** EFI Boot Services misc API
****************************************************** */

typedef EFI_STATUS(EFIAPI *EFI_EXIT)(EFI_HANDLE ImageHandle,
                                     EFI_STATUS ExitStatus,
                                     uintptr_t ExitDataSize,
                                     uint16_t /* optional */ *ExitData);

typedef EFI_STATUS(EFIAPI *EFI_EXIT_BOOT_SERVICES)(EFI_HANDLE ImageHandle,
                                                   uintptr_t MapKey);

typedef EFI_STATUS(EFIAPI *EFI_SET_WATCHDOG_TIMER)(
    uintptr_t Timeout, uint64_t WatchdogCode, uintptr_t DataSize,
    uint16_t /* optional */ *WatchdogData);

typedef EFI_STATUS(EFIAPI *EFI_STALL)(uintptr_t Microseconds);

typedef void(EFIAPI *EFI_COPY_MEM)(void *Destination, void *Source,
                                   uintptr_t Length);

typedef void(EFIAPI *EFI_SET_MEM)(void *Destination, uintptr_t Length,
                                  uint8_t Value);

typedef EFI_STATUS(EFIAPI *EFI_GET_NEXT_MONOTONIC_COUNT)(uint64_t *Count);

typedef EFI_STATUS(EFIAPI *EFI_INSTALL_CONFIGURATION_TABLE)(EFI_GUID *Guid,
                                                            void *Table);

typedef EFI_STATUS(EFIAPI *EFI_CALCULATE_CRC32)(void *Data, uintptr_t DataSize,
                                                uint32_t *Crc32);

/* ******************************************************
** EFI Boot Services Definitions
****************************************************** */
#define EFI_BOOT_SERVICES_SIGNATURE 0x56524553544f4f42
#define EFI_BOOT_SERVICES_REVISION EFI_SPECIFICATION_VERSION

/* ******************************************************
** EFI Boot Services Table (struct)
****************************************************** */

typedef struct {
  EFI_TABLE_HEADER Hdr;
  /* Task Priority Services */
  EFI_RAISE_TPL RaiseTPL;
  EFI_RESTORE_TPL RestoreTPL;
  /* Memory Services */
  EFI_ALLOCATE_PAGES AllocatePages;
  EFI_FREE_PAGES FreePages;
  EFI_GET_MEMORY_MAP GetMemoryMap;
  EFI_ALLOCATE_POOL AllocatePool;
  EFI_FREE_POOL FreePool;
  /* Event & Timer Services */
  EFI_CREATE_EVENT CreateEvent;
  EFI_SET_TIMER SetTimer;
  EFI_WAIT_FOR_EVENT WaitForEvent;
  EFI_SIGNAL_EVENT SignalEvent;
  EFI_CLOSE_EVENT CloseEvent;
  EFI_CHECK_EVENT CheckEvent;
  /* Protocol Handler Services */
  EFI_INSTALL_PROTOCOL_INTERFACE InstallProtocolInterface;
  EFI_REINSTALL_PROTOCOL_INTERFACE ReinstallProtocolInterface;
  EFI_UNINSTALL_PROTOCOL_INTERFACE UninstallProtocolInterface;
  EFI_HANDLE_PROTOCOL HandleProtocol;
  void *Reserved;
  EFI_REGISTER_PROTOCOL_NOTIFY RegisterProtocolNotify;
  EFI_LOCATE_HANDLE LocateHandle;
  EFI_LOCATE_DEVICE_PATH LocateDevicePath;
  EFI_INSTALL_CONFIGURATION_TABLE InstallConfigurationTable;
  /* Image Handler Services */
  EFI_IMAGE_LOAD LoadImage;
  EFI_IMAGE_START StartImage;
  EFI_EXIT Exit;
  EFI_IMAGE_UNLOAD UnloadImage;
  EFI_EXIT_BOOT_SERVICES ExitBootServices;
  /* Miscellaneous Services */
  EFI_GET_NEXT_MONOTONIC_COUNT GetNextMonotonicCount;
  EFI_STALL Stall;
  EFI_SET_WATCHDOG_TIMER SetWatchdogTimer;
  /* DriverSupport Services */
  EFI_CONNECT_CONTROLLER ConnectController;
  EFI_DISCONNECT_CONTROLLER DisconnectController;
  /* Open and Close Services */
  EFI_OPEN_PROTOCOL OpenProtocol;
  EFI_CLOSE_PROTOCOL CloseProtocol;
  EFI_OPEN_PROTOCOL_INFORMATION OpenProtocolInformation;
  /* Library Services Services */
  EFI_PROTOCOLS_PER_HANDLE ProtocolsPerHandle;
  EFI_LOCATE_HANDLE_BUFFER LocateHandleBuffer;
  EFI_LOCATE_PROTOCOL LocateProtocol;
  EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES
  InstallMultipleProtocolInterfaces;
  EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES
  UninstallMultipleProtocolInterfaces;
  /* 32-bit CRC Services */
  EFI_CALCULATE_CRC32 CalculateCrc32;
  /* Miscellaneous Services */
  EFI_COPY_MEM CopyMem;
  EFI_SET_MEM SetMem;
  EFI_CREATE_EVENT_EX CreateEventEx;
} EFI_BOOT_SERVICES;

/* ******************************************************
** H_EFI_BOOT_SERVICES_H end
****************************************************** */
#endif
