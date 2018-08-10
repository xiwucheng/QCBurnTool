#pragma once


// CBurnDlg �Ի���

class CBurnDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBurnDlg)

public:
	CBurnDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBurnDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BURNDLG };
#endif
protected:
	CStaticEx m_Dev[8];
	CStatic   m_Info[8];
	CStaticEx m_sOK, m_sNG, m_sTotal;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
