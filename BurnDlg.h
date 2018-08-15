#pragma once

#define BIT(x) (1 << x)
// CBurnDlg 对话框
string ws2s(const wstring &ws);
wstring s2ws(const string &s);

typedef struct
{
	int port;//COMxxx
	int usb;//USB port number
	int hub;//HUB number
	int id;
	int valid;
}DevPort, *PDevPort;

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
	CDragEdit m_fwPath;
	CStaticEx m_Dev[8];
	CStatic   m_Info[8];
	CShellCmd m_cSCmd[8];
	HANDLE    m_hDevThread[8];
	string    m_strCmd[8];
	CStaticEx m_sOK, m_sNG, m_sTotal;
	HDEVNOTIFY m_hDevNotify;
	BOOL      m_bIsRunning;
	UINT      m_nCanStop;
	DevPort   m_DevPort[8];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);
	afx_msg void OnDestroy();
private:
	static UINT Dev1Monitor(LPVOID lpv);
	static UINT Dev2Monitor(LPVOID lpv);
	static UINT Dev3Monitor(LPVOID lpv);
	static UINT Dev4Monitor(LPVOID lpv);
	static UINT Dev5Monitor(LPVOID lpv);
	static UINT Dev6Monitor(LPVOID lpv);
	static UINT Dev7Monitor(LPVOID lpv);
	static UINT Dev8Monitor(LPVOID lpv);

public:
	int EnumDevices();
};
