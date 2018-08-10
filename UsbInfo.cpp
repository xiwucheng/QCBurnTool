#include "stdafx.h"
#include "UsbInfo.h"

void getDevices()
{
	//LPGUID lpGuid = (LPGUID)&GUID_CLASS_USB_HUB;//
	LPGUID lpGuid = (LPGUID)&GUID_CLASS_USB_DEVICE;//
	int INTERFACE_DETAIL_SIZE = 1024;
	int nCount = 0;
	HDEVINFO info = SetupDiGetClassDevs(lpGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);

	//enumerate device information  
	DWORD required_size = 0;
	int i;
	SP_DEVINFO_DATA DeviceInfoData = { sizeof(DeviceInfoData) };

	DWORD DataT;
	DWORD buffersize = 1024;
	DWORD req_bufsize = 0;

	for (i = 0; SetupDiEnumDeviceInfo(info, i, &DeviceInfoData); i++)
	{
		WCHAR did[1024];
		WCHAR locid[1024];
		WCHAR locdesc[1024];
		//get device description information
		if (!SetupDiGetDeviceInstanceId(info, &DeviceInfoData, (PTSTR)did, buffersize, &req_bufsize))
			continue;
		if (!SetupDiGetDeviceRegistryPropertyW(info, &DeviceInfoData, SPDRP_LOCATION_PATHS, &DataT, (LPBYTE)locid, buffersize, &req_bufsize))
			continue;

		if (!SetupDiGetDeviceRegistryPropertyW(info, &DeviceInfoData, SPDRP_LOCATION_INFORMATION, &DataT, (LPBYTE)locdesc, buffersize, &req_bufsize))
			continue;
		req_bufsize=0;
	}

	SetupDiDestroyDeviceInfoList(info);
}