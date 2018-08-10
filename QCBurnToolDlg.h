
// QCBurnToolDlg.h : 头文件
//

#pragma once
#include "BurnDlg.h"
#include "MeidDlg.h"
// CQCBurnToolDlg 对话框
class CQCBurnToolDlg : public CDialogEx
{
// 构造
public:
	CQCBurnToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QCBURNTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
public:
	CTabCtrl* m_pTab;
	CBurnDlg m_brnDlg;
	CMeidDlg m_mdDlg;
	CImageList m_ImgList;
	HICON  m_hIconFw, m_hIconSn;

protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
