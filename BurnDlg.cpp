// BurnDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QCBurnTool.h"
#include "BurnDlg.h"
#include "afxdialogex.h"


// CBurnDlg �Ի���

IMPLEMENT_DYNAMIC(CBurnDlg, CDialogEx)

CBurnDlg::CBurnDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BURNDLG, pParent)
{

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
	ON_BN_CLICKED(IDC_START, &CBurnDlg::OnBnClickedStart)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CBurnDlg ��Ϣ�������


BOOL CBurnDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	for (int i = 0; i < 8; i++)
	{
		m_Dev[i].SubclassDlgItem(IDC_DEV1 + i,this);
		m_Dev[i].SetBkColor(RGB(100, 100, 100));
		m_Dev[i].SetTextColor(RGB(255, 255, 255));
		m_Dev[i].SetFrontColor(RGB(0, 224, 0));
		m_Info[i].SubclassDlgItem(IDC_INFO1 + i,this);
		//m_Info[i].SetWindowText(TEXT("Hello"));
	}
	m_sOK.SubclassDlgItem(IDC_SUCCESS, this);
	m_sOK.SetType(1);
	m_sOK.SetBkColor(RGB(0, 200, 0));
	m_sNG.SubclassDlgItem(IDC_FAILURE, this);
	m_sNG.SetType(1);
	m_sNG.SetBkColor(RGB(200, 0, 0));
	m_sTotal.SubclassDlgItem(IDC_TOTAL, this);
	m_sTotal.SetType(1);
	m_sTotal.SetBkColor(RGB(200, 100, 0));

	typedef BOOL(WINAPI* ChangeWindowMessageFilterFn)(UINT, DWORD);
	HMODULE hUserMod = LoadLibrary(TEXT("user32.dll"));
	if (hUserMod)
	{
		ChangeWindowMessageFilterFn pfnChangeWindowMessageFilter = (ChangeWindowMessageFilterFn)GetProcAddress(hUserMod, "ChangeWindowMessageFilter");
		if (pfnChangeWindowMessageFilter)
		{
			pfnChangeWindowMessageFilter(WM_DROPFILES, 1); // 1-MSGFLT_ADD, 2-MSGFLT_REMOVE
			pfnChangeWindowMessageFilter(0x0049, 1); // 1-MSGFLT_ADD, 2-MSGFLT_REMOVE
		}

		FreeLibrary(hUserMod);
	}

	SetDlgItemText(IDC_FWPATH, TEXT("��ͨ��ֱ����ק�̼�������Ŀ¼���˴�������ѡ��"));
	//SetTimer(1, 1000, 0);
	//CEdit* p = (CEdit*)GetDlgItem(IDC_FWPATH);
	//p->ShowBalloonTip(TEXT("�̼���·����ʾ"), TEXT("��ͨ�������ť��ֱ����ק�̼�������Ŀ¼������ѡ��"), TTI_INFO);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CBurnDlg::OnBnClickedBrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	// TODO: Add your message handler code here and/or call default
	TCHAR szPath[2048] = { 0 }, *p;
	int len = DragQueryFile(hDropInfo, 0, szPath, sizeof(szPath));
	if (len)
	{
		WIN32_FIND_DATA wfd;
		HANDLE hFind = FindFirstFile(szPath, &wfd);
		if (hFind != INVALID_HANDLE_VALUE && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			SetDlgItemText(IDC_FWPATH,szPath);
		}
		else if (hFind != INVALID_HANDLE_VALUE)
		{
			p = _tcsrchr(szPath,'\\');
			if (p)
			{
				*p = 0;
				SetDlgItemText(IDC_FWPATH, szPath);
			}
		}
		FindClose(hFind);
	}

	CDialogEx::OnDropFiles(hDropInfo);
}


void CBurnDlg::OnBnClickedStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CBurnDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1)
	{
		KillTimer(1);
		CEdit* p = (CEdit*)GetDlgItem(IDC_FWPATH);
		p->ShowBalloonTip(TEXT("�̼���·����ʾ"), TEXT("��ͨ�������ť��ֱ����ק�̼�������Ŀ¼������ѡ��"), TTI_INFO);
	}

	CDialogEx::OnTimer(nIDEvent);
}
