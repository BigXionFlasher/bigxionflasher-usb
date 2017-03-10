#ifndef __CAN_DRV_EX_H__
#define __CAN_DRV_EX_H__


#ifdef __cplusplus
  extern "C" {
#endif

/***************************************************************/
/* Define Makros                                               */
/***************************************************************/
#define CAN_FEATURE_LOM          0x0001  // Silent Mode (LOM = Listen only Mode)
#define CAN_FEATURE_ARD          0x0002  // Automatic Retransmission disable
#define CAN_FEATURE_TX_ACK       0x0004  // TX ACK (Gesendete Nachrichten bestätigen)
#define CAN_FEATURE_HW_TIMESTAMP 0x8000  // Hardware Time Stamp

// (V)alue (T)ype
#define VT_BYTE         0x01
#define VT_UBYTE        0x02
#define VT_WORD         0x03
#define VT_UWORD        0x04
#define VT_LONG         0x05
#define VT_ULONG        0x06

#define VT_BYTE_ARRAY   0x07
#define VT_UBYTE_ARRAY  0x08
#define VT_WORD_ARRAY   0x09
#define VT_UWORD_ARRAY  0x0A
#define VT_LONG_ARRAY   0x0B
#define VT_ULONG_ARRAY  0x0C

#define VT_BYTE_RANGE_ARRAY   0x0D
#define VT_UBYTE_RANGE_ARRAY  0x0E
#define VT_WORD_RANGE_ARRAY   0x0F
#define VT_UWORD_RANGE_ARRAY  0x10
#define VT_LONG_RANGE_ARRAY   0x11
#define VT_ULONG_RANGE_ARRAY  0x12

#define VT_HBYTE  0x40
#define VT_HWORD  0x41
#define VT_HLONG  0x42 

#define VT_STREAM 0x80
#define VT_STRING 0x81
#define VT_POINTER 0x82

// MHS (EV)ent (S)ource
#define MHS_EVS_STATUS  1
#define MHS_EVS_PNP     2
#define MHS_EVS_OBJECT  3

#define MHS_TERMINATE 0x80000000

/***************************************************************/
/*  Typen                                                      */
/***************************************************************/
struct TModulFeatures
  {
  uint32_t CanClock;           // Clock-Frequenz des CAN-Controllers, muss nicht mit
                               // der Clock-Frequenz des Mikrocontrollers übereinstimmen
  uint32_t Flags;              // Unterstützte Features des Moduls:
                               //  Bit  0 -> Silent Mode (LOM = Listen only Mode)
                               //       1 -> Automatic Retransmission disable
                               //       2 -> TX ACK (Gesendete Nachrichten bestätigen)
                               //      15 -> Hardware Time Stamp
  uint32_t CanChannelsCount;   // Anzahl der CAN Schnittstellen, reserviert für 
                               // zukünftige Module mit mehr als einer Schnittstelle                               
  uint32_t HwRxFilterCount;    // Anzahl der zur Verfügung stehenden Receive-Filter
  uint32_t HwTxPufferCount;    // Anzahl der zur Verfügung stehenden Transmit Puffer mit Timer
  };

struct TCanDevicesList
  {
  uint32_t TCanIdx;            // Ist das Device geöffnet ist der Wert auf dem Device-Index 
                               // gesetzt, ansonsten ist der Wert auf "INDEX_INVALID" gesetzt.
  uint32_t HwId;               // Ein 32 Bit Schlüssel der die Hardware eindeutig Identifiziert.
                               // Manche Module müssen erst geöffnet werden damit dieser Wert
                               // gesetzt wird
  char DeviceName[255];        // Nur Linux: entspricht den Device Namen des USB-Devices,
                               //            z.B. /dev/ttyUSB0
  char SerialNumber[16];       // Seriennummer des Moduls
  char Description[64];                 // Modul Bezeichnung, z.B. "Tiny-CAN IV-XL",
                                        // muss in den USB-Controller programmiert sein,
                                        // was zur Zeit nur bei den Modulen Tiny-CAN II-XL, 
                                        // IV-XL u. M1 der Fall ist.
  struct TModulFeatures ModulFeatures;  // Unterstützte Features des Moduls, nur gültig
                                        // wenn HwId > 0 
  };


struct TCanDeviceInfo
  {
  uint32_t HwId;               // Ein 32 Bit Schlüssel der die Hardware eindeutig Identifiziert.
  uint32_t FirmwareVersion;    // Version der Firmware des Tiny-CAN Moduls
  uint32_t FirmwareInfo;       // Informationen zum Stand der Firmware Version
                               //   0 = Unbekannt
                               //   1 = Firmware veraltet, Device kann nicht geöffnet werden
                               //   2 = Firmware veraltet, Funktionsumfang eingeschränkt
                               //   3 = Firmware veraltet, keine Einschränkungen
                               //   4 = Firmware auf Stand
                               //   5 = Firmware neuer als Erwartet
  char SerialNumber[16];       // Seriennummer des Moduls
  char Description[64];        // Modul Bezeichnung, z.B. "Tiny-CAN IV-XL"
  struct TModulFeatures ModulFeatures; // Unterstützte Features des Moduls
  };


struct TInfoVar
  {
  uint32_t Key;      // Variablen Schlüssel
  uint32_t Type;     // Variablen Type
  uint32_t Size;     // (Max)Größe der Variable in Byte
  char Data[255];    // Wert der Variable
  };


typedef struct _TMhsEvent TMhsEvent;

struct _TMhsEvent
  {
  volatile uint32_t Events;
  volatile uint32_t EventsMask;
  volatile int32_t Waiting;
#ifdef __WIN32__
// ****** Windows
  HANDLE Event;
  CRITICAL_SECTION EventLock;
#else
// ****** Linux

#endif
  };

/***************************************************************/
/*  Funktionstypen                                             */
/***************************************************************/
typedef int32_t (DRV_CALLBACK_TYPE *TCanExGetDeviceCount)(int flags);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExGetDeviceList)(struct TCanDevicesList **devices_list, int flags);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExGetDeviceInfo)(uint32_t index, struct TCanDeviceInfo *device_info,
  struct TInfoVar **hw_info, uint32_t *hw_info_size);
typedef void (DRV_CALLBACK_TYPE *TCanExDataFree)(void **data);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExCreateDevice)(uint32_t *index, char *options);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExDestroyDevice)(uint32_t *index);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExCreateFifo)(uint32_t index, uint32_t size, TMhsEvent *event_obj, 
                 uint32_t event, uint32_t channels);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExBindFifo)(uint32_t fifo_index, uint32_t device_index, 
                 uint32_t bind);
typedef TMhsEvent * (DRV_CALLBACK_TYPE *TCanExCreateEvent)(void);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExSetObjEvent)(uint32_t index, uint32_t source, TMhsEvent *event_obj, uint32_t event);
typedef void (DRV_CALLBACK_TYPE *TCanExSetEvent)(TMhsEvent *event_obj, uint32_t event);
typedef void (DRV_CALLBACK_TYPE *TCanExResetEvent)(TMhsEvent *event_obj, uint32_t event);
typedef uint32_t (DRV_CALLBACK_TYPE *TCanExWaitForEvent)(TMhsEvent *event_obj, uint32_t timeout);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExInitDriver)(char *options);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExSetOptions)(uint32_t index, char *options);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExSetAsByte)(uint32_t index, char *name, char value);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExSetAsWord)(uint32_t index, char *name, int16_t value);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExSetAsLong)(uint32_t index, char *name, int32_t value);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExSetAsUByte)(uint32_t index, char *name, unsigned char value);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExSetAsUWord)(uint32_t index, char *name, uint16_t value);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExSetAsULong)(uint32_t index, char *name, uint32_t value);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExSetAsString)(uint32_t index, char *name, char *value);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExGetAsByte)(uint32_t index, char *name, char *value);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExGetAsWord)(uint32_t index, char *name, int16_t *value);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExGetAsLong)(uint32_t index, char *name, int32_t *value);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExGetAsUByte)(uint32_t index, char *name, unsigned char *value);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExGetAsUWord)(uint32_t index, char *name, uint16_t *value);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExGetAsULong)(uint32_t index, char *name, uint32_t *value);
typedef int32_t (DRV_CALLBACK_TYPE *TCanExGetAsString)(uint32_t index, char *name, char **str);

/***************************************************************/
/*  Tiny-CAN API Funktionen                                    */
/***************************************************************/
extern TCanExGetDeviceCount CanExGetDeviceCount;
extern TCanExGetDeviceList CanExGetDeviceList;
extern TCanExGetDeviceInfo CanExGetDeviceInfo;
extern TCanExDataFree CanExDataFree;
extern TCanExCreateDevice CanExCreateDevice;
extern TCanExDestroyDevice CanExDestroyDevice;
extern TCanExCreateFifo CanExCreateFifo;
extern TCanExBindFifo CanExBindFifo;
extern TCanExCreateEvent CanExCreateEvent;
extern TCanExSetObjEvent CanExSetObjEvent;
extern TCanExSetEvent CanExSetEvent;
extern TCanExResetEvent CanExResetEvent;
extern TCanExWaitForEvent CanExWaitForEvent;
extern TCanExInitDriver CanExInitDriver;
extern TCanExSetOptions CanExSetOptions;
extern TCanExSetAsByte CanExSetAsByte;
extern TCanExSetAsWord CanExSetAsWord;
extern TCanExSetAsLong CanExSetAsLong;
extern TCanExSetAsUByte CanExSetAsUByte;
extern TCanExSetAsUWord CanExSetAsUWord;
extern TCanExSetAsULong CanExSetAsULong;
extern TCanExSetAsString CanExSetAsString;
extern TCanExGetAsByte CanExGetAsByte;
extern TCanExGetAsWord CanExGetAsWord;
extern TCanExGetAsLong CanExGetAsLong;
extern TCanExGetAsUByte CanExGetAsUByte;
extern TCanExGetAsUWord CanExGetAsUWord;
extern TCanExGetAsULong CanExGetAsULong;
extern TCanExGetAsString CanExGetAsString;

#ifdef __cplusplus
  }
#endif

#endif
