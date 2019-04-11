// MeidDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QCBurnTool.h"
#include "MeidDlg.h"
#include "afxdialogex.h"


// CMeidDlg 对话框

IMPLEMENT_DYNAMIC(CMeidDlg, CDialogEx)

CMeidDlg::CMeidDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MEIDDLG, pParent)
{

}

CMeidDlg::~CMeidDlg()
{
}

void CMeidDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMeidDlg, CDialogEx)
END_MESSAGE_MAP()


// CMeidDlg 消息处理程序


BOOL CMeidDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
