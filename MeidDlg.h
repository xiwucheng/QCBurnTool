#pragma once


// CMeidDlg �Ի���

class CMeidDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMeidDlg)

public:
	CMeidDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMeidDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MEIDDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
