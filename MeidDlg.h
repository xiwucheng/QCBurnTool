#pragma once


// CMeidDlg 对话框

class CMeidDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMeidDlg)

public:
	CMeidDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMeidDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MEIDDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
