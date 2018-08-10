
// QCBurnToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QCBurnTool.h"
#include "QCBurnToolDlg.h"
#include "afxdialogex.h"
#include "UsbInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CQCBurnToolDlg �Ի���



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


// CQCBurnToolDlg ��Ϣ�������

BOOL CQCBurnToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CQCBurnToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CQCBurnToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CQCBurnToolDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnTimer(nIDEvent);
}


void CQCBurnToolDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
