#pragma once

#include "resource.h"
#include <stdio.h>
#include <windows.h>
#include <SetupAPI.h>
#include <iostream>
#include <Usbiodef.h>
#include <Usbioctl.h>
#include <devguid.h>
#include <winioctl.h>
#include <INITGUID.h>
#include <Dbt.h>
#include <map>
#include <wchar.h>
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "setupapi.lib")

DEFINE_GUID(GUID_DEVINTERFACE_USB_DEVICE, 0xA5DCBF10L, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED);
#define GUID_CLASS_USB_DEVICE		GUID_DEVINTERFACE_USB_DEVICE  

DEFINE_GUID(GUID_DEVINTERFACE_USB_HUB, 0xf18a0e88, 0xc30c, 0x11d0, 0x88, 0x15, 0x00, 0xa0, 0xc9, 0x06, 0xbe, 0xd8);
#define GUID_CLASS_USB_HUB		GUID_DEVINTERFACE_USB_HUB

void getDevices();