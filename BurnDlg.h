#pragma once

#define WM_DEVICENOTIFY	(WM_USER+101)
#define WM_BURNPROGRESS (WM_USER+102)
#define BIT(x) (1 << x)
// CBurnDlg 对话框

typedef struct
{
	int usbroot;//USB root number
	int port;//COMxxx
	int usb;//USB port number
	int hub;//HUB number
	int id;//Register id number
	int valid;//tell main thread a valid device attached
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
	DWORD     m_dwThreadId[8];
	char      m_strFwPath[MAX_PATH];
	CStaticEx m_sOK, m_sNG, m_sTotal;
	HDEVNOTIFY m_hDevNotify;
	BOOL      m_bIsRunning;
	UINT      m_nCanStop;
	DevPort   m_DevPort[8];
	int       m_nTotal, m_nSuccess, m_nFailure;
	CCriticalSection m_cs;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnBnClickedStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);
	afx_msg LRESULT OnDeviceNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBurnProgress(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
private:
	static UINT DevMonitor(LPVOID lpv);

public:
	int EnumDevices();
	BOOL IsDeviceRegistered(int hubport, int usbport);
	BOOL IsAllowClose();
};
