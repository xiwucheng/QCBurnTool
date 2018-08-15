// BurnDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QCBurnTool.h"
#include "BurnDlg.h"
#include "afxdialogex.h"

#include <SetupAPI.h>
#include <INITGUID.h>
#include <Dbt.h>
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "setupapi.lib")



DEFINE_GUID(GUID_DEVINTERFACE_USB_DEVICE, 0xA5DCBF10L, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED);
#define GUID_CLASS_USB_DEVICE		GUID_DEVINTERFACE_USB_DEVICE  

DEFINE_GUID(GUID_DEVINTERFACE_USB_HUB, 0xf18a0e88, 0xc30c, 0x11d0, 0x88, 0x15, 0x00, 0xa0, 0xc9, 0x06, 0xbe, 0xd8);
#define GUID_CLASS_USB_HUB		GUID_DEVINTERFACE_USB_HUB

// CBurnDlg 对话框

IMPLEMENT_DYNAMIC(CBurnDlg, CDialogEx)

CBurnDlg::CBurnDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BURNDLG, pParent)
{
	m_bIsRunning = FALSE;
	m_nCanStop = 0;
	memset(m_DevPort, 0, sizeof(m_DevPort));
}

CBurnDlg::~CBurnDlg()
{
}

void CBurnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBurnDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BROWSE, &CBurnDlg::OnBnClickedBrowse)
	ON_WM_DROPFILES()
	ON_WM_DEVICECHANGE()
	ON_BN_CLICKED(IDC_START, &CBurnDlg::OnBnClickedStart)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CBurnDlg 消息处理程序


BOOL CBurnDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	for (int i = 0; i < 8; i++)
	{
		m_Dev[i].SubclassDlgItem(IDC_DEV1 + i,this);
		m_Dev[i].SetBkColor(RGB(100, 100, 100));
		m_Dev[i].SetTextColor(RGB(255, 255, 255));
		m_Dev[i].SetFrontColor(RGB(0, 224, 0));
		//m_Dev[i].SetPos(88);
		m_Info[i].SubclassDlgItem(IDC_INFO1 + i,this);
		//m_Info[i].SetWindowText(TEXT("Hello"));
	}
	m_sOK.SubclassDlgItem(IDC_SUCCESS, this);
	m_sOK.SetType(1);
	m_sOK.SetFontSize(20);
	m_sOK.SetBkColor(RGB(0, 200, 0));
	m_sNG.SubclassDlgItem(IDC_FAILURE, this);
	m_sNG.SetType(1);
	m_sNG.SetFontSize(20);
	m_sNG.SetBkColor(RGB(200, 0, 0));
	m_sTotal.SubclassDlgItem(IDC_TOTAL, this);
	m_sTotal.SetType(1);
	m_sTotal.SetFontSize(20);
	m_sTotal.SetBkColor(RGB(200, 100, 0));

	//m_sOK.SetWindowText(TEXT("9999"));
	m_fwPath.SubclassDlgItem(IDC_FWPATH,this);
	//SetDlgItemText(IDC_FWPATH, TEXT("可通过直接拖拽固件包所在目录到此处来快速选择！"));
	//SetTimer(1, 1000, 0);
	//CEdit* p = (CEdit*)GetDlgItem(IDC_FWPATH);
	//p->ShowBalloonTip(TEXT("固件包路径提示"), TEXT("可通过浏览按钮或直接拖拽固件包所在目录来快速选择！"), TTI_INFO);
	DEV_BROADCAST_DEVICEINTERFACE   dbi;
	ZeroMemory(&dbi, sizeof(dbi));
	dbi.dbcc_size = sizeof(dbi);
	dbi.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	dbi.dbcc_reserved = 0;
	dbi.dbcc_classguid = GUID_CLASS_USB_DEVICE;
	WCHAR *com = L"Communications Port (COM21)";
	WCHAR *p = wcsstr(com, L"COM");
	EnumDevices();
	m_hDevNotify = RegisterDeviceNotification(m_hWnd, &dbi, DEVICE_NOTIFY_WINDOW_HANDLE);//DEVICE_NOTIFY_ALL_INTERFACE_CLASSES
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CBurnDlg::OnBnClickedBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	CFolderPickerDialog fd(NULL, 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		str = fd.GetPathName();
		SetDlgItemText(IDC_FWPATH,str);
	}
}


void CBurnDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnDropFiles(hDropInfo);
}


void CBurnDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_cSCmd.ExecuteCmd("cmd.exe /c dir c:\\windows\\system32 /og",1);
	//Sleep(50);
	//list<string> result = m_cSCmd.GetResult();
	//OutputDebugString(L"GetResult\n");
	if (m_bIsRunning)
	{
		if (m_nCanStop == 0)
		{
			m_bIsRunning = FALSE;
			WaitForMultipleObjects(8, m_hDevThread, TRUE, INFINITE);
			OutputDebugString(L"WaitForMultipleObjects returned\n");
			SetDlgItemText(IDC_START, TEXT("启动"));//to be start
			GetDlgItem(IDC_FWPATH)->EnableWindow();
			GetDlgItem(IDC_BROWSE)->EnableWindow();
		}
		else
		{
			MessageBox(TEXT("无法停止当前操作，请等待操作完成后停止！"), TEXT("非法操作"), MB_ICONERROR);
		}
	}
	else
	{
		//Start DevMonitor
		m_bIsRunning = TRUE;
		SetDlgItemText(IDC_START, TEXT("停止"));//to be stop
		GetDlgItem(IDC_FWPATH)->EnableWindow(0);
		GetDlgItem(IDC_BROWSE)->EnableWindow(0);
		m_hDevThread[0] = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Dev1Monitor, this, 0, 0);
		m_hDevThread[1] = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Dev2Monitor, this, 0, 0);
		m_hDevThread[2] = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Dev3Monitor, this, 0, 0);
		m_hDevThread[3] = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Dev4Monitor, this, 0, 0);
		m_hDevThread[4] = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Dev5Monitor, this, 0, 0);
		m_hDevThread[5] = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Dev6Monitor, this, 0, 0);
		m_hDevThread[6] = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Dev7Monitor, this, 0, 0);
		m_hDevThread[7] = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Dev8Monitor, this, 0, 0);
	}
}


void CBurnDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		KillTimer(1);
	}

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CBurnDlg::OnDeviceChange(UINT nEventType, DWORD_PTR dwData)
{
	switch (nEventType)
	{
	case DBT_DEVICEARRIVAL:
	case DBT_DEVICEREMOVECOMPLETE:
	{
		PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)dwData;
		PDEV_BROADCAST_DEVICEINTERFACE pDevInf;
		if (pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
		{
			pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)pHdr;
			if (memcmp(&pDevInf->dbcc_classguid, &GUID_CLASS_USB_DEVICE, sizeof(GUID)) == 0 &&
				wcsstr(pDevInf->dbcc_name,TEXT("VID_05C6&PID_9008")))
			{
				if (nEventType == DBT_DEVICEARRIVAL)
				{
					OutputDebugString(TEXT("MSM8953 EDL Device  ----  Inserted\n"));
				}
				else if (nEventType == DBT_DEVICEREMOVECOMPLETE)
				{
					OutputDebugString(TEXT("MSM8953 EDL Device  ----  Removed\n"));
				}
				EnumDevices();
			}
		}
		break;
	}
	}
	return TRUE;
}



void CBurnDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	UnregisterDeviceNotification(m_hDevNotify);
}

UINT CBurnDlg::Dev1Monitor(LPVOID lpv)
{
	CBurnDlg* p = (CBurnDlg*)lpv;
	while (p->m_bIsRunning)
	{
		Sleep(100);
	}
	return 0L;
}

UINT CBurnDlg::Dev2Monitor(LPVOID lpv)
{
	CBurnDlg* p = (CBurnDlg*)lpv;
	while (p->m_bIsRunning)
	{
		Sleep(100);
	}
	return 0L;
}

UINT CBurnDlg::Dev3Monitor(LPVOID lpv)
{
	CBurnDlg* p = (CBurnDlg*)lpv;
	while (p->m_bIsRunning)
	{
		Sleep(100);
	}
	return 0L;
}

UINT CBurnDlg::Dev4Monitor(LPVOID lpv)
{
	CBurnDlg* p = (CBurnDlg*)lpv;
	while (p->m_bIsRunning)
	{
		Sleep(100);
	}
	return 0L;
}

UINT CBurnDlg::Dev5Monitor(LPVOID lpv)
{
	CBurnDlg* p = (CBurnDlg*)lpv;
	while (p->m_bIsRunning)
	{
		Sleep(100);
	}
	return 0L;
}

UINT CBurnDlg::Dev6Monitor(LPVOID lpv)
{
	CBurnDlg* p = (CBurnDlg*)lpv;
	while (p->m_bIsRunning)
	{
		Sleep(100);
	}
	return 0L;
}

UINT CBurnDlg::Dev7Monitor(LPVOID lpv)
{
	CBurnDlg* p = (CBurnDlg*)lpv;
	while (p->m_bIsRunning)
	{
		Sleep(100);
	}
	return 0L;
}

UINT CBurnDlg::Dev8Monitor(LPVOID lpv)
{
	CBurnDlg* p = (CBurnDlg*)lpv;
	while (p->m_bIsRunning)
	{
		Sleep(100);
	}
	return 0L;
}


string ws2s(const wstring &ws)
{
	size_t i;
	string curLocale = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "chs");
	const wchar_t* _source = ws.c_str();
	size_t _dsize = 2 * ws.size() + 1;
	char* _dest = new char[_dsize];
	memset(_dest, 0x0, _dsize);
	wcstombs_s(&i, _dest, _dsize, _source, _dsize);
	string result = _dest;
	delete[] _dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

wstring s2ws(const string &s)
{
	size_t i;
	string curLocale = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "chs");
	const char* _source = s.c_str();
	size_t _dsize = s.size() + 1;
	wchar_t* _dest = new wchar_t[_dsize];
	wmemset(_dest, 0x0, _dsize);
	mbstowcs_s(&i, _dest, _dsize, _source, _dsize);
	wstring result = _dest;
	delete[] _dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

int CBurnDlg::EnumDevices()
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
		WCHAR did[128] = { 0 };
		WCHAR locinfo[128] = { 0 };
		WCHAR friendName[128] = { 0 };
		WCHAR *subStr = 0;
		int com_port = 0;
		//get device description information
		SetupDiGetDeviceInstanceId(info, &DeviceInfoData, (PTSTR)did, buffersize, &req_bufsize);
		SetupDiGetDeviceRegistryProperty(info, &DeviceInfoData, SPDRP_LOCATION_INFORMATION, &DataT, (LPBYTE)locinfo, buffersize, &req_bufsize);
		SetupDiGetDeviceRegistryProperty(info, &DeviceInfoData, SPDRP_FRIENDLYNAME, &DataT, (LPBYTE)friendName, buffersize, &req_bufsize);
		req_bufsize = 0;

		subStr = wcsstr(friendName, TEXT("COM"));
		if (subStr)
		{
			swscanf(subStr, TEXT("COM%d"), &com_port);
		}

		for (int j = 0; j < 8; j++)
		{
			if (!m_DevPort[j].valid)
			{
				swscanf_s(locinfo, TEXT("Port_#%04d.Hub_#%04d"), &m_DevPort[j].usb, &m_DevPort[j].hub);
				m_DevPort[j].valid = 1;
				m_DevPort[j].port = com_port;
				break;
			}
		}
	}

	SetupDiDestroyDeviceInfoList(info);
	return 0;
}
