
// QCBurnToolDlg.h : ͷ�ļ�
//

#pragma once
#include "BurnDlg.h"
#include "MeidDlg.h"
// CQCBurnToolDlg �Ի���
class CQCBurnToolDlg : public CDialogEx
{
// ����
public:
	CQCBurnToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QCBURNTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
public:
	CTabCtrl* m_pTab;
	CBurnDlg m_brnDlg;
	CMeidDlg m_mdDlg;
	CImageList m_ImgList;
	HICON  m_hIconFw, m_hIconSn;

protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
