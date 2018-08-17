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
	ON_MESSAGE(WM_DEVICENOTIFY, OnDeviceNotify)
	ON_MESSAGE(WM_BURNPROGRESS, OnBurnProgress)
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
		//m_Dev[i].SetPos(MAKEWORD(11,55));
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
	//EnumDevices();
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
		CString strPath;
		m_fwPath.GetWindowText(strPath);
		if (strPath.GetLength() == 0)
		{
			MessageBox(TEXT("固件路径不能为空！"), TEXT("错误"), MB_ICONERROR);
			return;
		}

		WideCharToMultiByte(CP_ACP,0, strPath, -1,m_strFwPath,MAX_PATH,0,0);
		strPath += TEXT("\\prog_emmc_firehose_8953_ddr.mbn");
		WIN32_FIND_DATA wfd = { 0 };
		HANDLE hFind = FindFirstFile(strPath, &wfd);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			MessageBox(TEXT("固件路径不正确，找不到对应的文件！"), TEXT("错误"), MB_ICONERROR);
			return;
		}
		FindClose(hFind);

		m_bIsRunning = TRUE;
		SetDlgItemText(IDC_START, TEXT("停止"));//to be stop
		GetDlgItem(IDC_FWPATH)->EnableWindow(0);
		GetDlgItem(IDC_BROWSE)->EnableWindow(0);
		m_cs.Lock();
		for (int i = 0; i < 8; i++)
		{
			m_hDevThread[i] = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)DevMonitor, this, 0, &m_dwThreadId[i]);
		}
		m_cs.Unlock();
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
					EnumDevices();
				}
				else if (nEventType == DBT_DEVICEREMOVECOMPLETE)
				{
					OutputDebugString(TEXT("MSM8953 EDL Device  ----  Removed\n"));
					wchar_t buff[128] = { 0 };
					wcscpy_s(buff, 128, pDevInf->dbcc_name);
					wchar_t* subStr = wcsrchr(buff, '#');
					int usbroot, usb;
					if (subStr)
					{
						*subStr = 0;
						subStr -= 4;
						swscanf_s(subStr, TEXT("&%d&%d"), &usbroot, &usb);
						for (int i = 0; i < 8; i++)
						{
							if (m_DevPort[i].usbroot == usbroot && m_DevPort[i].usb == usb)
							{
								m_DevPort[i].valid = 0;
								PostMessage(WM_DEVICENOTIFY, 0, i);
								break;
							}
						}
					}
				}
			}
		}
		break;
	}
	}
	return TRUE;
}

LRESULT CBurnDlg::OnDeviceNotify(WPARAM wParam, LPARAM lParam)
{
	CString strInfo;
	if (wParam == 0)//设备的插入与拔出
	{
		if (m_DevPort[lParam].id)
		{
			if (m_DevPort[lParam].valid)
			{
				m_Dev[lParam].SetType(0);
				m_Dev[lParam].SetPos(0);
				m_Dev[lParam].SetBkColor(RGB(100, 100, 100));
			}
			strInfo.Format(TEXT("ROOT(%04d)#HUB(%04d)#USB(%04d)#COM%d     %s"),
				m_DevPort[lParam].usbroot,
				m_DevPort[lParam].hub, 
				m_DevPort[lParam].usb, 
				m_DevPort[lParam].port,
				m_DevPort[lParam].valid ? TEXT("已连接") : TEXT("未连接"));
			m_Info[lParam].SetWindowText(strInfo);
		}
	}
	else if (wParam == 1 || wParam == 2)//烧录出错
	{
		m_Dev[lParam].SetType(1);
		m_Dev[lParam].SetBkColor(RGB(200, 0, 0));
		m_Dev[lParam].SetWindowText(TEXT("烧录失败"));
	}
	else if (wParam == 3)//烧录完成
	{
		m_Dev[lParam].SetType(1);
		m_Dev[lParam].SetBkColor(RGB(0, 200, 0));
		m_Dev[lParam].SetWindowText(TEXT("烧录成功"));
	}
	return 0;
}

LRESULT CBurnDlg::OnBurnProgress(WPARAM wParam, LPARAM lParam)
{
	int i = (int)wParam;
	int percent = (int)lParam;
	if (i > 7)
	{
		return 0;
	}
	m_Dev[i].SetPos(percent);
	return 1;
}

void CBurnDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	UnregisterDeviceNotification(m_hDevNotify);
}

UINT CBurnDlg::DevMonitor(LPVOID lpv)
{
	CBurnDlg* p = (CBurnDlg*)lpv;
	UINT uID;
	wchar_t strThreadId[32] = { 0 };
	HANDLE hDevMon = 0;
	DWORD dwDevMonId = 0;

	p->m_cs.Lock();
	hDevMon = GetCurrentThread();
	dwDevMonId = GetCurrentThreadId();
	for (int i = 0; i < 8; i++)
	{
		if (dwDevMonId == p->m_dwThreadId[i])
		{
			uID = i;
			swprintf_s(strThreadId, 32, TEXT("ThreadId->%d is running!\n"), uID);
			OutputDebugString(strThreadId);
			break;
		}
	}
	p->m_cs.Unlock();

	while (p->m_bIsRunning)
	{
		if (p->m_DevPort[uID].valid)//设备已插入，开始烧录
		{
			p->m_cs.Lock();
			p->m_nCanStop |= BIT(0);
			p->m_cs.Unlock();
			char szCmd[512] = { 0 }, *szOut;
			list<string> result;
			string szTmp;
			int flag = 0;
			int per_int=0, per_float=0;
			int per_sum=0, per_sum_old=0;
			sprintf_s(szCmd, 512, "cmd.exe /c QSaharaServer.exe -p \\\\.\\COM%d -c 1 -s 13:%s\\prog_emmc_firehose_8953_ddr.mbn -x", p->m_DevPort[uID].port,p->m_strFwPath);
			p->m_cSCmd[uID].ExecuteCmd(szCmd, 1);
			while (!p->m_cSCmd[uID].IsCmdDone())//等待运行结束
			{
				Sleep(100);
			}
			result = p->m_cSCmd[uID].GetResult();
			for (list<string>::iterator it = result.begin(); it != result.end(); it++)
			{
				szTmp = *it;
				strcpy_s(szCmd, 512, szTmp.c_str());
				if (flag == 0)
				{
					if (strstr(szCmd, "File transferred successfully"))
					{
						flag = 1;
					}
				}
				else if (flag == 1)
				{
					if (strstr(szCmd, "Sahara protocol completed"))
					{
						flag = 2;
						break;
					}
				}
			}
			result.clear();
			if (flag != 2)//第1条命令出错，退出烧录
			{
				p->m_DevPort[uID].valid = 0;
				p->m_cs.Lock();
				p->m_nCanStop &= ~BIT(0);
				p->m_cs.Unlock();
				p->PostMessage(WM_DEVICENOTIFY, 1, uID);//通知主程序在第1条指令出错了
				continue;//等待下一次新设备插入
			}

			//-----------------------------------------------------------------------
			flag = 0;
			sprintf_s(szCmd, 512, "cmd.exe /c fh_loader.exe --port=\\\\.\\COM%d --sendxml=rawprogram_unsparse.xml,patch0.xml --search_path=\"%s\" --noprompt --showpercentagecomplete --zlpawarehost=1 --memoryname=eMMC --setactivepartition=0", p->m_DevPort[uID].port,p->m_strFwPath);
			p->m_cSCmd[uID].ExecuteCmd(szCmd, 1);
			while (!p->m_cSCmd[uID].IsCmdDone())//等待运行结束
			{
				if (p->m_DevPort[uID].valid == 0)
				{
					break;
				}
				result = p->m_cSCmd[uID].GetResult();
				for (list<string>::iterator it = result.begin(); it != result.end(); it++)
				{
					szTmp = *it;
					strcpy_s(szCmd, 512, szTmp.c_str());
					if (flag == 0)
					{
						szOut = strstr(szCmd, "{percent files transferred");
						if (szOut)
						{
							sscanf_s(szOut, "{percent files transferred %d.%02d}", &per_int, &per_float);
							per_sum = per_int * 100 + per_float;
							if (per_sum > per_sum_old)
							{
								per_sum_old = per_sum;
								p->PostMessage(WM_BURNPROGRESS, uID, MAKEWORD(per_int, per_float));
								OutputDebugStringA(szOut);
								OutputDebugStringA("\n");
								Sleep(500);
							}
						}
						if (per_int == 100)
						{
							flag = 1;
						}
					}
					else if (flag == 1)
					{
						szOut = strstr(szCmd, "All Finished Successfully");
						if (szOut)
						{
							flag = 2;
						}
					}
					else if (flag == 2)
					{
						break;
					}
				}
				result.clear();
				Sleep(10);
			}
			//运行已结束，获取结束后的信息
			result = p->m_cSCmd[uID].GetResult();
			for (list<string>::iterator it = result.begin(); it != result.end(); it++)
			{
				if (p->m_DevPort[uID].valid == 0)
				{
					break;
				}

				szTmp = *it;
				strcpy_s(szCmd, 512, szTmp.c_str());
				if (flag == 0)
				{
					szOut = strstr(szCmd, "{percent files transferred");
					if (szOut)
					{
						sscanf_s(szOut, "{percent files transferred %d.%02d}", &per_int, &per_float);
						per_sum = per_int * 100 + per_float;
						if (per_sum > per_sum_old)
						{
							per_sum_old = per_sum;
							p->PostMessage(WM_BURNPROGRESS, uID, MAKEWORD(per_int, per_float));
							OutputDebugStringA(szOut);
							OutputDebugStringA("\n");
							Sleep(200);
						}
					}
					if (per_int == 100)
					{
						flag = 1;
					}
				}
				else if (flag == 1)
				{
					szOut = strstr(szCmd, "All Finished Successfully");
					if (szOut)
					{
						flag = 2;
					}
				}
				else if (flag == 2)
				{
					break;
				}
			}
			result.clear();
			//Sleep(1000);
			if (p->m_DevPort[uID].valid == 0)
			{
				p->m_cSCmd[uID].Terminate();
			}


			if (flag != 2)//第2条命令出错，退出烧录
			{
				p->m_DevPort[uID].valid = 0;
				p->m_cs.Lock();
				p->m_nCanStop &= ~BIT(0);
				p->m_cs.Unlock();
				p->PostMessage(WM_DEVICENOTIFY, 2, uID);//通知主程序在第2条指令出错了
				continue;//等待下一次新设备插入
			}
			else
			{
				p->m_DevPort[uID].valid = 0;
				p->m_cs.Lock();
				p->m_nCanStop &= ~BIT(0);
				p->m_cs.Unlock();
				p->PostMessage(WM_DEVICENOTIFY, 3, uID);//通知主程序所有操作已正确完成
				continue;//等待下一次新设备插入
			}
		}
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
		WCHAR locpath[128] = { 0 };
		WCHAR locinfo[128] = { 0 };
		WCHAR friendName[128] = { 0 };
		WCHAR *subStr = 0;
		int com_port = 0;//串口号
		int pciroot,pci,usbroot = 0;
		int hub = 0;//Hub位置
		int usb = 0;//USB位置
		//get device description information
		SetupDiGetDeviceInstanceId(info, &DeviceInfoData, (PTSTR)did, buffersize, &req_bufsize);
		SetupDiGetDeviceRegistryProperty(info, &DeviceInfoData, SPDRP_LOCATION_PATHS, &DataT, (LPBYTE)locpath, buffersize, &req_bufsize);
		SetupDiGetDeviceRegistryProperty(info, &DeviceInfoData, SPDRP_LOCATION_INFORMATION, &DataT, (LPBYTE)locinfo, buffersize, &req_bufsize);
		SetupDiGetDeviceRegistryProperty(info, &DeviceInfoData, SPDRP_FRIENDLYNAME, &DataT, (LPBYTE)friendName, buffersize, &req_bufsize);
		req_bufsize = 0;

		if (wcsstr(did, TEXT("VID_05C6&PID_9008")) == 0)//不是MSM8953则跳过
		{
			continue;
		}

		subStr = wcsstr(friendName, TEXT("COM"));
		if (subStr)
		{
			swscanf_s(subStr, TEXT("COM%d"), &com_port);
		}
		else//没有成功转换成串口号则跳过
		{
			continue;
		}

		swscanf_s(locpath, TEXT("PCIROOT(%d)#PCI(%d)#USBROOT(%d)#USB(%d)"), &pciroot,&pci,&usbroot,&usb);
		swscanf_s(locinfo, TEXT("Port_#%04d.Hub_#%04d"), &usb, &hub);
		if (usb == 0 || hub == 0)//无效的Hub及USB位置则跳过
		{
			continue;
		}
		for (int j = 0; j < 8; j++)
		{
			//if (m_DevPort[j].id)
			if (!m_DevPort[j].id && !IsDeviceRegistered(hub,usb))//此设备未被注册过(id == 0)
			{
				m_DevPort[j].valid = 1;//1表示设备已连接，0表示设备已断开
				m_DevPort[j].id = j+1;//Id号下标从1开始
				m_DevPort[j].port = com_port;
				m_DevPort[j].hub = hub;
				m_DevPort[j].usb = usb;
				PostMessage(WM_DEVICENOTIFY, 0, j);
				break;
			}
			else if (m_DevPort[j].id && m_DevPort[j].hub == hub && m_DevPort[j].usb == usb)
			{
				m_DevPort[j].valid = 1;
				PostMessage(WM_DEVICENOTIFY, 0, j);
			}
		}
	}

	SetupDiDestroyDeviceInfoList(info);
	return 0;
}

BOOL CBurnDlg::IsDeviceRegistered(int hubport, int usbport)
{
	for (int i = 0; i < 8; i++)
	{
		if (m_DevPort[i].hub == hubport && m_DevPort[i].usb == usbport)
		{
			return TRUE;
		}
	}
	return FALSE;
}
