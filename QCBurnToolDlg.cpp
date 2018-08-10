
// QCBurnToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QCBurnTool.h"
#include "QCBurnToolDlg.h"
#include "afxdialogex.h"
#include "UsbInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CQCBurnToolDlg 对话框



CQCBurnToolDlg::CQCBurnToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_QCBURNTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQCBurnToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQCBurnToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CQCBurnToolDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CQCBurnToolDlg 消息处理程序

BOOL CQCBurnToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_hIconFw = AfxGetApp()->LoadIcon(IDR_FW);
	m_hIconSn = AfxGetApp()->LoadIcon(IDR_SN);
	m_ImgList.Create(16, 16, ILC_COLOR32, 0, 0);
	m_ImgList.Add(m_hIconFw);
	m_ImgList.Add(m_hIconSn);
	CRect rc,rcArea;
	m_pTab = (CTabCtrl*)GetDlgItem(IDC_TAB1);
	m_pTab->SetImageList(&m_ImgList);
	m_pTab->InsertItem(0, TEXT("MSM8953"), 0);
	m_pTab->InsertItem(1, TEXT("MEID"), 1);
	m_pTab->GetClientRect(&rc);
	m_brnDlg.Create(IDD_BURNDLG, this);
	m_mdDlg.Create(IDD_MEIDDLG, this);
	GetClientRect(&rcArea);
	rcArea.top = rc.bottom;
	rcArea.bottom -= 24;
	m_brnDlg.MoveWindow(&rcArea);
	m_mdDlg.MoveWindow(&rcArea);
	m_brnDlg.ShowWindow(SW_SHOW);
	getDevices();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CQCBurnToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CQCBurnToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CQCBurnToolDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnTimer(nIDEvent);
}


void CQCBurnToolDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int sel = m_pTab->GetCurSel();
	if (sel == 0)
	{
		m_brnDlg.ShowWindow(SW_SHOW);
		m_mdDlg.ShowWindow(SW_HIDE);
	}
	else if (sel == 1)
	{
		m_mdDlg.ShowWindow(SW_SHOW);
		m_brnDlg.ShowWindow(SW_HIDE);
	}
}
