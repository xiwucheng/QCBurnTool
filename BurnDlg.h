#pragma once


// CBurnDlg 对话框

class CBurnDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBurnDlg)

public:
	CBurnDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBurnDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BURNDLG };
#endif
protected:
	CStaticEx m_Dev[8];
	CStatic   m_Info[8];
	CStaticEx m_sOK, m_sNG, m_sTotal;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
