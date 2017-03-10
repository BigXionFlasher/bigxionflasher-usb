/*******************************************************************************/
/*                          Interface zur Tiny-CAN API                         */
/* --------------------------------------------------------------------------- */
/*  Beschreibung    : Laden einer Treiber DLL                                  */
/*                                                                             */
/*  Version         : 1.20                                                     */
/*  Datei Name      : can_drv_win.c                                            */
/* --------------------------------------------------------------------------- */
/*  Datum           : 22.01.13                                                 */
/*  Copyright       : (C) 2011 - 2013 by MHS-Elektronik GmbH & Co. KG, Germany */
/*                      www.mhs-elektronik.de                                  */
/*  Autor           : Demlehner Klaus, info@mhs-elektronik.de                  */ 
/*******************************************************************************/
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include "can_drv.h"


int32_t CALLBACK DefCanInitDriver(char *options);
void CALLBACK DefCanDownDriver(void);
int32_t CALLBACK DefCanSetOptions(char *options);
int32_t CALLBACK DefCanDeviceOpen(uint32_t  index, char *parameter);
int32_t CALLBACK DefCanDeviceClose(uint32_t  index);
int32_t CALLBACK DefCanApplaySettings(uint32_t  index);

int32_t CALLBACK DefCanSetMode(uint32_t  index, unsigned char can_op_mode, uint16_t  can_command);
int32_t CALLBACK DefCanSet(uint32_t  index, uint16_t  obj_index, uint16_t  obj_sub_index, void *data, int32_t size);
int32_t CALLBACK DefCanGet(uint32_t  index, uint16_t  obj_index, uint16_t  obj_sub_index, void *data, int32_t size);

int32_t CALLBACK DefCanTransmit(uint32_t  index, struct TCanMsg *msg, int32_t count);
void CALLBACK DefCanTransmitClear(uint32_t  index);
uint32_t  CALLBACK DefCanTransmitGetCount(uint32_t  index);
int32_t CALLBACK DefCanTransmitSet(uint32_t  index, uint16_t  cmd,
  uint32_t  time);
int32_t CALLBACK DefCanReceive(uint32_t  index, struct TCanMsg *msg, int32_t count);
void CALLBACK DefCanReceiveClear(uint32_t  index);
uint32_t  CALLBACK DefCanReceiveGetCount(uint32_t  index);

int32_t CALLBACK DefCanSetSpeed(uint32_t  index, uint16_t  speed);
int32_t CALLBACK DefCanSetSpeedUser(uint32_t  index, uint32_t  value);
char * CALLBACK DefCanDrvInfo(void);
char * CALLBACK DefCanDrvHwInfo(uint32_t  index);
int32_t CALLBACK DefCanSetFilter(uint32_t  index, struct TMsgFilter *msg_filter);
int32_t CALLBACK DefCanGetDeviceStatus(uint32_t  index, struct TDeviceStatus *status);
void CALLBACK DefCanSetPnPEventCallback(void (CALLBACK *event)(uint32_t  index, int32_t status));
void CALLBACK DefCanSetStatusEventCallback(void (CALLBACK *event)
  (uint32_t  index, struct TDeviceStatus *device_status));
void CALLBACK DefCanSetRxEventCallback(void (CALLBACK *event)(uint32_t  index,
  struct TCanMsg *msg, int32_t count));

void CALLBACK DefCanSetEvents(uint16_t  events);
uint32_t CALLBACK DefCanEventStatus(void);

// EX-Funktionen
int32_t CALLBACK DefCanExGetDeviceCount(int flags);
int32_t CALLBACK DefCanExGetDeviceList(struct TCanDevicesList **devices_list, int flags);
int32_t CALLBACK DefCanExGetDeviceInfo(uint32_t index, struct TCanDeviceInfo *device_info,
  struct TInfoVar **hw_info, uint32_t *hw_info_size);
void CALLBACK DefCanExDataFree(void **data);
int32_t CALLBACK DefCanExCreateDevice(uint32_t *index, char *options);
int32_t CALLBACK DefCanExDestroyDevice(uint32_t *index);
int32_t CALLBACK DefCanExCreateFifo(uint32_t index, uint32_t size, TMhsEvent *event_obj, uint32_t event, uint32_t channels);
int32_t CALLBACK DefCanExBindFifo(uint32_t fifo_index, uint32_t device_index, uint32_t bind);
TMhsEvent * CALLBACK DefCanExCreateEvent(void);
int32_t DRV_CALLBACK_TYPE DefCanExSetObjEvent(uint32_t index, uint32_t source, TMhsEvent *event_obj, uint32_t event);
void CALLBACK DefCanExSetEvent(TMhsEvent *event_obj, uint32_t event);
void CALLBACK DefCanExResetEvent(TMhsEvent *event_obj, uint32_t event);
uint32_t CALLBACK DefCanExWaitForEvent(TMhsEvent *event_obj, uint32_t timeout);
int32_t CALLBACK DefCanExInitDriver(char *options);
int32_t CALLBACK DefCanExSetOptions(uint32_t index, char *options);
int32_t CALLBACK DefCanExSetAsByte(uint32_t index, char *name, char value);
int32_t CALLBACK DefCanExSetAsWord(uint32_t index, char *name, int16_t value);
int32_t CALLBACK DefCanExSetAsLong(uint32_t index, char *name, int32_t value);
int32_t CALLBACK DefCanExSetAsUByte(uint32_t index, char *name, unsigned char value);
int32_t CALLBACK DefCanExSetAsUWord(uint32_t index, char *name, uint16_t value);
int32_t CALLBACK DefCanExSetAsULong(uint32_t index, char *name, uint32_t value);
int32_t CALLBACK DefCanExSetAsString(uint32_t index, char *name, char *value);
int32_t CALLBACK DefCanExGetAsByte(uint32_t index, char *name, char *value);
int32_t CALLBACK DefCanExGetAsWord(uint32_t index, char *name, int16_t *value);
int32_t CALLBACK DefCanExGetAsLong(uint32_t index, char *name, int32_t *value);
int32_t CALLBACK DefCanExGetAsUByte(uint32_t index, char *name, unsigned char *value);
int32_t CALLBACK DefCanExGetAsUWord(uint32_t index, char *name, uint16_t *value);
int32_t CALLBACK DefCanExGetAsULong(uint32_t index, char *name, uint32_t *value);
int32_t CALLBACK DefCanExGetAsString(uint32_t index, char *name, char **str);

TCanInitDriver CanInitDriver = DefCanInitDriver;
TCanDownDriver CanDownDriver = DefCanDownDriver;
TCanSetOptions CanSetOptions = DefCanSetOptions;
TCanDeviceOpen CanDeviceOpen = DefCanDeviceOpen;
TCanDeviceClose CanDeviceClose = DefCanDeviceClose;
TCanApplaySettings CanApplaySettings = DefCanApplaySettings;
TCanSetMode CanSetMode = DefCanSetMode;
TCanSet CanSet = DefCanSet;
TCanGet CanGet = DefCanGet;
TCanTransmit CanTransmit = DefCanTransmit;
TCanTransmitClear CanTransmitClear = DefCanTransmitClear;
TCanTransmitGetCount CanTransmitGetCount = DefCanTransmitGetCount;
TCanTransmitSet CanTransmitSet = DefCanTransmitSet;
TCanReceive CanReceive = DefCanReceive;
TCanReceiveClear CanReceiveClear = DefCanReceiveClear;
TCanReceiveGetCount CanReceiveGetCount = DefCanReceiveGetCount;
TCanSetSpeed CanSetSpeed = DefCanSetSpeed;
TCanSetSpeedUser CanSetSpeedUser = DefCanSetSpeedUser;
TCanDrvInfo CanDrvInfo = DefCanDrvInfo;
TCanDrvHwInfo CanDrvHwInfo = DefCanDrvHwInfo;
TCanSetFilter CanSetFilter = DefCanSetFilter;
TCanGetDeviceStatus CanGetDeviceStatus = DefCanGetDeviceStatus;
TCanSetPnPEventCallback CanSetPnPEventCallback = DefCanSetPnPEventCallback;
TCanSetStatusEventCallback CanSetStatusEventCallback = DefCanSetStatusEventCallback;
TCanSetRxEventCallback CanSetRxEventCallback = DefCanSetRxEventCallback;
TCanSetEvents CanSetEvents = DefCanSetEvents;
TCanEventStatus CanEventStatus = DefCanEventStatus;

// EX-Funktionen
TCanExGetDeviceCount CanExGetDeviceCount = DefCanExGetDeviceCount;
TCanExGetDeviceList CanExGetDeviceList = DefCanExGetDeviceList;
TCanExGetDeviceInfo CanExGetDeviceInfo = DefCanExGetDeviceInfo;
TCanExDataFree CanExDataFree = DefCanExDataFree;
TCanExCreateDevice CanExCreateDevice = DefCanExCreateDevice;
TCanExDestroyDevice CanExDestroyDevice = DefCanExDestroyDevice;
TCanExCreateFifo CanExCreateFifo = DefCanExCreateFifo;
TCanExBindFifo CanExBindFifo = DefCanExBindFifo;
TCanExCreateEvent CanExCreateEvent = DefCanExCreateEvent;
TCanExSetObjEvent CanExSetObjEvent = DefCanExSetObjEvent;
TCanExSetEvent CanExSetEvent = DefCanExSetEvent;
TCanExResetEvent CanExResetEvent = DefCanExResetEvent;
TCanExWaitForEvent CanExWaitForEvent = DefCanExWaitForEvent;
TCanExInitDriver CanExInitDriver = DefCanExInitDriver;
TCanExSetOptions CanExSetOptions = DefCanExSetOptions;
TCanExSetAsByte CanExSetAsByte = DefCanExSetAsByte;
TCanExSetAsWord CanExSetAsWord = DefCanExSetAsWord;
TCanExSetAsLong CanExSetAsLong = DefCanExSetAsLong;
TCanExSetAsUByte CanExSetAsUByte = DefCanExSetAsUByte;
TCanExSetAsUWord CanExSetAsUWord = DefCanExSetAsUWord;
TCanExSetAsULong CanExSetAsULong = DefCanExSetAsULong;
TCanExSetAsString CanExSetAsString = DefCanExSetAsString;
TCanExGetAsByte CanExGetAsByte = DefCanExGetAsByte;
TCanExGetAsWord CanExGetAsWord = DefCanExGetAsWord;
TCanExGetAsLong CanExGetAsLong = DefCanExGetAsLong;
TCanExGetAsUByte CanExGetAsUByte = DefCanExGetAsUByte;
TCanExGetAsUWord CanExGetAsUWord = DefCanExGetAsUWord;
TCanExGetAsULong CanExGetAsULong = DefCanExGetAsULong;
TCanExGetAsString CanExGetAsString = DefCanExGetAsString;


HINSTANCE DriverDLL = 0;               // Handle to DLL
char *DriverFileName = NULL;


#define MhsSaveFree(d) do { \
  if ((d)) \
    { \
    free((d)); \
    (d) = NULL; \
    } \
  } while(0)

static char *GetApiDriverWithPath(const char *driver_file);

/***************************************************************/
/*  Treiber DLL laden                                          */
/***************************************************************/
int32_t LoadDriver(const char *file_name)
{
int error;

error = 0;
UnloadDriver();
if (!(DriverFileName = GetApiDriverWithPath(file_name)))
  return(-100);
DriverDLL = LoadLibrary(DriverFileName);
if (DriverDLL)
  {
  CanInitDriver = (TCanInitDriver)GetProcAddress(DriverDLL, (LPCSTR)"CanInitDriver");
  CanDownDriver = (TCanDownDriver)GetProcAddress(DriverDLL, (LPCSTR)"CanDownDriver");
  CanSetOptions = (TCanSetOptions)GetProcAddress(DriverDLL, (LPCSTR)"CanSetOptions");
  CanDeviceOpen = (TCanDeviceOpen)GetProcAddress(DriverDLL, (LPCSTR)"CanDeviceOpen");
  CanDeviceClose = (TCanDeviceClose)GetProcAddress(DriverDLL, (LPCSTR)"CanDeviceClose");
  CanApplaySettings = (TCanApplaySettings)GetProcAddress(DriverDLL, (LPCSTR)"CanApplaySettings");
  CanSetMode = (TCanSetMode)GetProcAddress(DriverDLL, (LPCSTR)"CanSetMode");
  CanSet = (TCanSet)GetProcAddress(DriverDLL, (LPCSTR)"CanSet");
  CanGet = (TCanGet)GetProcAddress(DriverDLL, (LPCSTR)"CanGet");
  CanTransmit = (TCanTransmit)GetProcAddress(DriverDLL, (LPCSTR)"CanTransmit");
  CanTransmitClear = (TCanTransmitClear)GetProcAddress(DriverDLL, (LPCSTR)"CanTransmitClear");
  CanTransmitGetCount = (TCanTransmitGetCount)GetProcAddress(DriverDLL, (LPCSTR)"CanTransmitGetCount");
  CanTransmitSet = (TCanTransmitSet)GetProcAddress(DriverDLL, (LPCSTR)"CanTransmitSet");
  CanReceive = (TCanReceive)GetProcAddress(DriverDLL, (LPCSTR)"CanReceive");
  CanReceiveClear = (TCanReceiveClear)GetProcAddress(DriverDLL, (LPCSTR)"CanReceiveClear");
  CanReceiveGetCount = (TCanReceiveGetCount)GetProcAddress(DriverDLL, (LPCSTR)"CanReceiveGetCount");
  CanSetSpeed = (TCanSetSpeed)GetProcAddress(DriverDLL, (LPCSTR)"CanSetSpeed");
  CanSetSpeedUser = (TCanSetSpeedUser)GetProcAddress(DriverDLL, (LPCSTR)"CanSetSpeedUser");
  CanDrvInfo = (TCanDrvInfo)GetProcAddress(DriverDLL, (LPCSTR)"CanDrvInfo");
  CanDrvHwInfo = (TCanDrvHwInfo)GetProcAddress(DriverDLL, (LPCSTR)"CanDrvHwInfo");
  CanSetFilter = (TCanSetFilter)GetProcAddress(DriverDLL, (LPCSTR)"CanSetFilter");
  CanGetDeviceStatus = (TCanGetDeviceStatus)GetProcAddress(DriverDLL, (LPCSTR)"CanGetDeviceStatus");
  CanSetPnPEventCallback = (TCanSetPnPEventCallback)GetProcAddress(DriverDLL, (LPCSTR)"CanSetPnPEventCallback");
  CanSetStatusEventCallback = (TCanSetStatusEventCallback)GetProcAddress(DriverDLL, (LPCSTR)"CanSetStatusEventCallback");
  CanSetRxEventCallback = (TCanSetRxEventCallback)GetProcAddress(DriverDLL, (LPCSTR)"CanSetRxEventCallback");
  CanSetEvents = (TCanSetEvents)GetProcAddress(DriverDLL, (LPCSTR)"CanSetEvents");
  CanEventStatus = (TCanEventStatus)GetProcAddress(DriverDLL, (LPCSTR)"CanEventStatus");
  // EX-Funktionen
  CanExGetDeviceCount = (TCanExGetDeviceCount)GetProcAddress(DriverDLL, (LPCSTR)"CanExGetDeviceCount");
  CanExGetDeviceList = (TCanExGetDeviceList)GetProcAddress(DriverDLL, (LPCSTR)"CanExGetDeviceList");
  CanExGetDeviceInfo = (TCanExGetDeviceInfo)GetProcAddress(DriverDLL, (LPCSTR)"CanExGetDeviceInfo");
  CanExDataFree = (TCanExDataFree)GetProcAddress(DriverDLL, (LPCSTR)"CanExDataFree");
  CanExCreateDevice = (TCanExCreateDevice)GetProcAddress(DriverDLL, (LPCSTR)"CanExCreateDevice");
  CanExDestroyDevice = (TCanExDestroyDevice)GetProcAddress(DriverDLL, (LPCSTR)"CanExDestroyDevice");
  CanExCreateFifo = (TCanExCreateFifo)GetProcAddress(DriverDLL, (LPCSTR)"CanExCreateFifo");
  CanExBindFifo = (TCanExBindFifo)GetProcAddress(DriverDLL, (LPCSTR)"CanExBindFifo");
  CanExCreateEvent = (TCanExCreateEvent)GetProcAddress(DriverDLL, (LPCSTR)"CanExCreateEvent");
  CanExSetObjEvent = (TCanExSetObjEvent)GetProcAddress(DriverDLL, (LPCSTR)"CanExSetObjEvent");
  CanExSetEvent = (TCanExSetEvent)GetProcAddress(DriverDLL, (LPCSTR)"CanExSetEvent");
  CanExResetEvent = (TCanExResetEvent)GetProcAddress(DriverDLL, (LPCSTR)"CanExResetEvent");
  CanExWaitForEvent = (TCanExWaitForEvent)GetProcAddress(DriverDLL, (LPCSTR)"CanExWaitForEvent");
  CanExInitDriver = (TCanExInitDriver)GetProcAddress(DriverDLL, (LPCSTR)"CanExInitDriver");
  CanExSetOptions = (TCanExSetOptions)GetProcAddress(DriverDLL, (LPCSTR)"CanExSetOptions");
  CanExSetAsByte = (TCanExSetAsByte)GetProcAddress(DriverDLL, (LPCSTR)"CanExSetAsByte");
  CanExSetAsWord = (TCanExSetAsWord)GetProcAddress(DriverDLL, (LPCSTR)"CanExSetAsWord");
  CanExSetAsLong = (TCanExSetAsLong)GetProcAddress(DriverDLL, (LPCSTR)"CanExSetAsLong");
  CanExSetAsUByte = (TCanExSetAsUByte)GetProcAddress(DriverDLL, (LPCSTR)"CanExSetAsUByte");
  CanExSetAsUWord = (TCanExSetAsUWord)GetProcAddress(DriverDLL, (LPCSTR)"CanExSetAsUWord");
  CanExSetAsULong = (TCanExSetAsULong)GetProcAddress(DriverDLL, (LPCSTR)"CanExSetAsULong");
  CanExSetAsString = (TCanExSetAsString)GetProcAddress(DriverDLL, (LPCSTR)"CanExSetAsString");
  CanExGetAsByte = (TCanExGetAsByte)GetProcAddress(DriverDLL, (LPCSTR)"CanExGetAsByte");
  CanExGetAsWord = (TCanExGetAsWord)GetProcAddress(DriverDLL, (LPCSTR)"CanExGetAsWord");
  CanExGetAsLong = (TCanExGetAsLong)GetProcAddress(DriverDLL, (LPCSTR)"CanExGetAsLong");
  CanExGetAsUByte = (TCanExGetAsUByte)GetProcAddress(DriverDLL, (LPCSTR)"CanExGetAsUByte");
  CanExGetAsUWord = (TCanExGetAsUWord)GetProcAddress(DriverDLL, (LPCSTR)"CanExGetAsUWord");
  CanExGetAsULong = (TCanExGetAsULong)GetProcAddress(DriverDLL, (LPCSTR)"CanExGetAsULong");
  CanExGetAsString = (TCanExGetAsString)GetProcAddress(DriverDLL, (LPCSTR)"CanExGetAsString");

  if (!CanInitDriver)
    error = -1;
  else if (!CanDownDriver)
    error = -2;
  else if (!CanSetOptions)
    error = -3;
  else if (!CanDeviceOpen)
    error = -4;
  else if (!CanDeviceClose)
    error = -5;
  else if (!CanApplaySettings)
    error = -6;
  else if (!CanSetMode)
    error = -7;
  else if (!CanSet)
    error = -8;
  else if (!CanGet)
    error = -9;
  else if (!CanTransmit)
    error = -10;
  else if (!CanTransmitClear)
    error = -11;
  else if (!CanTransmitGetCount)
    error = -12;
  else if (!CanTransmitSet)
    error = -13;
  else if (!CanReceive)
    error = -14;
  else if (!CanReceiveClear)
    error = -15;
  else if (!CanReceiveGetCount)
    error = -16;
  else if (!CanSetSpeed)
    error = -17;
  else if (!CanSetSpeedUser)
    error = -18;
  else if (!CanDrvInfo)
    error = -19;
  else if (!CanDrvHwInfo)
    error = -20;
  else if (!CanSetFilter)
    error = -21;
  else if (!CanGetDeviceStatus)
    error = -22;
  else if (!CanSetPnPEventCallback)
    error = -23;
  else if (!CanSetStatusEventCallback)
    error = -24;
  else if (!CanSetRxEventCallback)
    error = -25;
  else if (!CanSetEvents)
    error = -26;
  else if (!CanEventStatus)
    error = -27;
  // EX-Fuktionen
  else if (!CanExGetDeviceCount)
    error = -28;
  else if (!CanExGetDeviceList)
    error = -29;
  else if (!CanExGetDeviceInfo)
    error = -30;
  else if (!CanExDataFree)
    error = -31;
  else if (!CanExCreateDevice)
    error = -32;
  else if (!CanExDestroyDevice)
    error = -33;
  else if (!CanExCreateFifo)
    error = -34;
  else if (!CanExBindFifo)
    error = -35;
  else if (!CanExCreateEvent)
    error = -36;
  else if (!CanExSetObjEvent)
    error = -37;
  else if (!CanExSetEvent)
    error = -38;
  else if (!CanExResetEvent)
    error = -39;
  else if (!CanExWaitForEvent)
    error = -40;
  else if (!CanExInitDriver)
    error = -41;
  else if (!CanExSetOptions)
    error = -42;
  else if (!CanExSetAsByte)
    error = -43;
  else if (!CanExSetAsWord)
    error = -44;
  else if (!CanExSetAsLong)
    error = -45;
  else if (!CanExSetAsUByte)
    error = -46;
  else if (!CanExSetAsUWord)
    error = -47;
  else if (!CanExSetAsULong)
    error = -48;
  else if (!CanExSetAsString)
    error = -49;
  else if (!CanExGetAsByte)
    error = -50;
  else if (!CanExGetAsWord)
    error = -51;
  else if (!CanExGetAsLong)
    error = -52;
  else if (!CanExGetAsUByte)
    error = -53;
  else if (!CanExGetAsUWord)
    error = -54;
  else if (!CanExGetAsULong)
    error = -55;
  else if (!CanExGetAsString)
    error = -56;
  }
else
  error = -100;
if (error)
  UnloadDriver();
return(error);
}


/***************************************************************/
/*  Treiber DLL entladen                                       */
/***************************************************************/
void UnloadDriver(void)
{
MhsSaveFree(DriverFileName);
if ((DriverDLL) && (CanDownDriver))
  CanDownDriver();
CanInitDriver = DefCanInitDriver;
CanDownDriver = DefCanDownDriver;
CanSetOptions = DefCanSetOptions;
CanDeviceOpen = DefCanDeviceOpen;
CanDeviceClose = DefCanDeviceClose;
CanApplaySettings = DefCanApplaySettings;
CanSetMode = DefCanSetMode;
CanSet = DefCanSet;
CanGet = DefCanGet;
CanTransmit = DefCanTransmit;
CanTransmitClear = DefCanTransmitClear;
CanTransmitGetCount = DefCanTransmitGetCount;
CanTransmitSet = DefCanTransmitSet;
CanReceive = DefCanReceive;
CanReceiveClear = DefCanReceiveClear;
CanReceiveGetCount = DefCanReceiveGetCount;
CanSetSpeed = DefCanSetSpeed;
CanSetSpeedUser = DefCanSetSpeedUser;
CanDrvInfo = DefCanDrvInfo;
CanDrvHwInfo = DefCanDrvHwInfo;
CanSetFilter = DefCanSetFilter;
CanGetDeviceStatus = DefCanGetDeviceStatus;
CanSetPnPEventCallback = DefCanSetPnPEventCallback;
CanSetStatusEventCallback = DefCanSetStatusEventCallback;
CanSetRxEventCallback = DefCanSetRxEventCallback;
CanSetEvents = DefCanSetEvents;
CanEventStatus = DefCanEventStatus;
// EX-Funktionen
CanExGetDeviceCount = DefCanExGetDeviceCount;
CanExGetDeviceList = DefCanExGetDeviceList;
CanExGetDeviceInfo = DefCanExGetDeviceInfo;
CanExDataFree = DefCanExDataFree;
CanExCreateDevice = DefCanExCreateDevice;
CanExDestroyDevice = DefCanExDestroyDevice;
CanExCreateFifo = DefCanExCreateFifo;
CanExBindFifo = DefCanExBindFifo;
CanExCreateEvent = DefCanExCreateEvent;
CanExSetObjEvent = DefCanExSetObjEvent;
CanExSetEvent = DefCanExSetEvent;
CanExResetEvent = DefCanExResetEvent;
CanExWaitForEvent = DefCanExWaitForEvent;
CanExInitDriver = DefCanExInitDriver;
CanExSetOptions = DefCanExSetOptions;
CanExSetAsByte = DefCanExSetAsByte;
CanExSetAsWord = DefCanExSetAsWord;
CanExSetAsLong = DefCanExSetAsLong;
CanExSetAsUByte = DefCanExSetAsUByte;
CanExSetAsUWord = DefCanExSetAsUWord;
CanExSetAsULong = DefCanExSetAsULong;
CanExSetAsString = DefCanExSetAsString;
CanExGetAsByte = DefCanExGetAsByte;
CanExGetAsWord = DefCanExGetAsWord;
CanExGetAsLong = DefCanExGetAsLong;
CanExGetAsUByte = DefCanExGetAsUByte;
CanExGetAsUWord = DefCanExGetAsUWord;
CanExGetAsULong = DefCanExGetAsULong;
CanExGetAsString = DefCanExGetAsString;

if (DriverDLL)
  {
  FreeLibrary(DriverDLL);
  DriverDLL = NULL;
  }
}


/***************************************************************/
/*  Default Call                                               */
/***************************************************************/
void DefDriverProcedure(int func)
{

}


/***************************************************************/
/*  Default Funktionen                                         */
/***************************************************************/
int32_t CALLBACK DefCanInitDriver(char *options)
{
DefDriverProcedure(1);
return(0);
}


void CALLBACK DefCanDownDriver(void)
{
// DefDriverProcedure(1);
}


int32_t CALLBACK DefCanSetOptions(char *options)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanDeviceOpen(uint32_t  index, char *parameter)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanDeviceClose(uint32_t  index)
{
return(0);
}


int32_t CALLBACK DefCanApplaySettings(uint32_t  index)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanSetMode(uint32_t index, unsigned char can_op_mode, uint16_t can_command)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanSet(uint32_t index, uint16_t obj_index, uint16_t obj_sub_index, void *data, int32_t size)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanGet(uint32_t index, uint16_t obj_index, uint16_t obj_sub_index, void *data, int32_t size)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanTransmit(uint32_t index, struct TCanMsg *msg, int32_t count)
{
DefDriverProcedure(1);
return(0);
}


void CALLBACK DefCanTransmitClear(uint32_t index)
{
DefDriverProcedure(1);
}


uint32_t  CALLBACK DefCanTransmitGetCount(uint32_t index)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanTransmitSet(uint32_t index, uint16_t cmd,
  uint32_t time)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanReceive(uint32_t index, struct TCanMsg *msg, int32_t count)
{
DefDriverProcedure(1);
return(0);
}


void CALLBACK DefCanReceiveClear(uint32_t index)
{
DefDriverProcedure(1);
}


uint32_t  CALLBACK DefCanReceiveGetCount(uint32_t index)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanSetSpeed(uint32_t index, uint16_t speed)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanSetSpeedUser(uint32_t index, uint32_t value)
{
DefDriverProcedure(1);
return(0);
}


char * CALLBACK DefCanDrvInfo(void)
{
DefDriverProcedure(1);
return(NULL);
}


char * CALLBACK DefCanDrvHwInfo(uint32_t index)
{
DefDriverProcedure(1);
return(NULL);
}


int32_t CALLBACK DefCanSetFilter(uint32_t index, struct TMsgFilter *msg_filter)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanGetDeviceStatus(uint32_t index, struct TDeviceStatus *status)
{
status->CanStatus = CAN_STATUS_UNBEKANNT;
status->FifoStatus = FIFO_STATUS_UNBEKANNT;
status->DrvStatus = DRV_NOT_LOAD;
return(0);
}


void CALLBACK DefCanSetPnPEventCallback(void (CALLBACK *event)(uint32_t index, int32_t status))
{
DefDriverProcedure(1);
}


void CALLBACK DefCanSetStatusEventCallback(void (CALLBACK *event)
  (uint32_t index, struct TDeviceStatus *device_status))
{
DefDriverProcedure(1);
}


void CALLBACK DefCanSetRxEventCallback(void (CALLBACK *event)(uint32_t index,
  struct TCanMsg *msg, int32_t count))
{
DefDriverProcedure(1);
}


void CALLBACK DefCanSetEvents(uint16_t  events)
{
DefDriverProcedure(1);
}


uint32_t CALLBACK DefCanEventStatus(void)
{
DefDriverProcedure(1);
return(1);
}


// EX-Funktionen
int32_t CALLBACK DefCanExGetDeviceCount(int flags)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExGetDeviceList(struct TCanDevicesList **devices_list, int flags)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExGetDeviceInfo(uint32_t index, struct TCanDeviceInfo *device_info,
  struct TInfoVar **hw_info, uint32_t *hw_info_size)
{
DefDriverProcedure(1);
return(0);
}


void CALLBACK DefCanExDataFree(void **data)
{
DefDriverProcedure(1);
}


int32_t CALLBACK DefCanExCreateDevice(uint32_t *index, char *options)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExDestroyDevice(uint32_t *index)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExCreateFifo(uint32_t index, uint32_t size, TMhsEvent *event_obj, uint32_t event, uint32_t channels)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExBindFifo(uint32_t fifo_index, uint32_t device_index, uint32_t bind)
{
DefDriverProcedure(1);
return(0);
}


TMhsEvent * CALLBACK DefCanExCreateEvent(void)
{
DefDriverProcedure(1);
return(NULL);
}


int32_t DRV_CALLBACK_TYPE DefCanExSetObjEvent(uint32_t index, uint32_t source, TMhsEvent *event_obj, uint32_t event)
{
DefDriverProcedure(1);
return(0);
}


void CALLBACK DefCanExSetEvent(TMhsEvent *event_obj, uint32_t event)
{
DefDriverProcedure(1);
}


void CALLBACK DefCanExResetEvent(TMhsEvent *event_obj, uint32_t event)
{
DefDriverProcedure(1);
}


uint32_t CALLBACK DefCanExWaitForEvent(TMhsEvent *event_obj, uint32_t timeout)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExInitDriver(char *options)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExSetOptions(uint32_t index, char *options)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExSetAsByte(uint32_t index, char *name, char value)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExSetAsWord(uint32_t index, char *name, int16_t value)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExSetAsLong(uint32_t index, char *name, int32_t value)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExSetAsUByte(uint32_t index, char *name, unsigned char value)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExSetAsUWord(uint32_t index, char *name, uint16_t value)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExSetAsULong(uint32_t index, char *name, uint32_t value)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExSetAsString(uint32_t index, char *name, char *value)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExGetAsByte(uint32_t index, char *name, char *value)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExGetAsWord(uint32_t index, char *name, int16_t *value)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExGetAsLong(uint32_t index, char *name, int32_t *value)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExGetAsUByte(uint32_t index, char *name, unsigned char *value)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExGetAsUWord(uint32_t index, char *name, uint16_t *value)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExGetAsULong(uint32_t index, char *name, uint32_t *value)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanExGetAsString(uint32_t index, char *name, char **str)
{
DefDriverProcedure(1);
return(0);
}

/***************************************************************/
/*                API Treiber Path ermitteln                   */
/***************************************************************/
#define MAX_PATH_LEN 255

#define DIR_SEPARATOR '\\'
#define DIR_SEPARATOR_STR "\\"

/**************************************************************************/
/*                        C O N S T A N T E N                             */
/**************************************************************************/
static const char REG_TINY_CAN_API[] = {"Software\\Tiny-CAN\\API"};
static const char REG_TINY_CAN_API_PATH_ENTRY[] = {"PATH"};

static const char API_DRIVER_DLL[] = {"mhstcan.dll"};

//typedef char *(*TCanDrvInfoCB)(void);

/**************************************************************************/
/*                        F U N K T I O N E N                             */
/**************************************************************************/
static char *MhsStpCpy(char *dest, const char *src)
{
register char *d = dest;
register const char *s = src;

do
  *d++ = *s;
while (*s++);

return(d - 1);
}


static char *MhsStrDup(const char *str)
{
int len;
char *new_str;

if (str)
  {
  len = strlen(str) + 1;
  new_str = (char *)malloc(len);
  if (!new_str)
    return(NULL);
  memcpy(new_str, str, len);
  return(new_str);
  }
else
  return(NULL);
}


static char *MhsStrconcat(const char *string1, ...)
{
int l;
va_list args;
char *s, *concat, *ptr;

if (!string1)
  return(NULL);

l = 1 + strlen(string1);
va_start(args, string1);
s = va_arg(args, char*);
while (s)
  {
  l += strlen(s);
  s = va_arg(args, char*);
  }
va_end(args);

concat = (char *)malloc(l);
if (!concat)
  return(NULL);
ptr = concat;

ptr = MhsStpCpy(ptr, string1);
va_start (args, string1);
s = va_arg (args, char*);
while (s)
  {
  ptr = MhsStpCpy(ptr, s);
  s = va_arg(args, char*);
  }
va_end (args);

return(concat);
}


static char *MhsCreateFileName(const char *dir, const char *file_name)
{
if (!dir)
  return(MhsStrDup(file_name));
if (!file_name)
  return(NULL);
if (strchr(file_name, DIR_SEPARATOR))
  return(MhsStrDup(file_name));
else
  {
  if (dir[strlen(dir)-1] == DIR_SEPARATOR)
    return(MhsStrconcat(dir, file_name, NULL));
  else
    return(MhsStrconcat(dir, DIR_SEPARATOR_STR, file_name, NULL));
  }
}


static char *MhsRegGetString(HKEY key, const char *value_name)
{
DWORD type, data_size;
char *str;

type = 0;
data_size = 0;
if ((!key) || (!value_name))
  return(NULL);
// Länge des Strings ermitteln
if (RegQueryValueEx(key, value_name, 0, &type, NULL, &data_size) != ERROR_SUCCESS)
  return(NULL);
// Wert ist String ?
if (type != REG_SZ)
  return(NULL);
str = malloc(data_size+1);
if (!str)
  return(NULL);
// String lesen
if (RegQueryValueEx(key, value_name, 0, &type, (LPBYTE)str, &data_size) != ERROR_SUCCESS)
  {
  free(str);
  return(NULL);
  }
str[data_size] = '\0';
return(str);
}


static char *MhsRegReadStringEntry(const char *path, const char *entry)
{
HKEY key;
char *value;

value = NULL;
// HKEY_CURRENT_USER ?
if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, path, 0, KEY_QUERY_VALUE, &key) == ERROR_SUCCESS) // KEY_READ
  {
  value = MhsRegGetString(key, entry);
  RegCloseKey(key);
  }
return(value);
}


static int TestApi(const char *file_name)
{
HINSTANCE driver_dll;
TCanDrvInfo drv_info_cb;

drv_info_cb = NULL;
driver_dll = LoadLibrary(file_name);
if (driver_dll)
  {
  drv_info_cb = (TCanDrvInfo)GetProcAddress(driver_dll, (LPCSTR)"CanDrvInfo");
  FreeLibrary(driver_dll);
  }
if (drv_info_cb)
  return(0);
else
  return(-1);
}


static char *GetApiDriverWithPath(const char *driver_file)
{
char *path, *file_name;
char app_file[MAX_PATH_LEN];
char app_path[MAX_PATH_LEN];
char *ptr;

file_name = NULL;
if ((!driver_file ) || (!strlen(driver_file)))
  driver_file = API_DRIVER_DLL; 
if (strchr(driver_file, DIR_SEPARATOR))
  return(MhsStrDup(driver_file));  
if ((path = MhsRegReadStringEntry(REG_TINY_CAN_API, REG_TINY_CAN_API_PATH_ENTRY)))
  {
  file_name = MhsCreateFileName(path, driver_file);
  if (TestApi(file_name))
    MhsSaveFree(file_name);
  free(path);
  }
if (!file_name)
  {
  // Programm Path ermitteln
  GetModuleFileName(GetModuleHandle(NULL), app_file, MAX_PATH_LEN);
  GetFullPathName(app_file, sizeof(app_file), app_path, &ptr);
  app_path[ptr-app_path] = '\0';
  file_name = MhsCreateFileName(app_path, driver_file);
  if (TestApi(file_name))
    MhsSaveFree(file_name);  
  }
return(file_name);
}


