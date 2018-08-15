#include "stdafx.h"
#include "DragEdit.h"


CDragEdit::CDragEdit()
{
	m_bTip = TRUE;
}


CDragEdit::~CDragEdit()
{
}
BEGIN_MESSAGE_MAP(CDragEdit, CEdit)
	ON_WM_DROPFILES()
	ON_WM_TIMER()
END_MESSAGE_MAP()


void CDragEdit::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	TCHAR szPath[2048] = { 0 }, *p;
	int len = DragQueryFile(hDropInfo, 0, szPath, sizeof(szPath));
	if (len)
	{
		WIN32_FIND_DATA wfd;
		HANDLE hFind = FindFirstFile(szPath, &wfd);
		if (hFind != INVALID_HANDLE_VALUE && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			SetWindowText(szPath);
		}
		else if (hFind != INVALID_HANDLE_VALUE)
		{
			p = _tcsrchr(szPath, '\\');
			if (p)
			{
				*p = 0;
				SetWindowText(szPath);
			}
		}
		FindClose(hFind);
	}
	CEdit::OnDropFiles(hDropInfo);
}


void CDragEdit::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	DragAcceptFiles();
	typedef BOOL(WINAPI* ChangeWindowMessageFilterFn)(UINT, DWORD);
	HMODULE hUserMod = LoadLibrary(TEXT("user32.dll"));
	if (hUserMod)
	{
		ChangeWindowMessageFilterFn pfnChangeWindowMessageFilter = (ChangeWindowMessageFilterFn)GetProcAddress(hUserMod, "ChangeWindowMessageFilter");
		if (pfnChangeWindowMessageFilter)
		{
			pfnChangeWindowMessageFilter(WM_DROPFILES, 1); // 1-MSGFLT_ADD, 2-MSGFLT_REMOVE
			pfnChangeWindowMessageFilter(0x0049, 1); // 1-MSGFLT_ADD, 2-MSGFLT_REMOVE
		}

		FreeLibrary(hUserMod);
	}
	SetTimer(1, 1000, 0);
	CEdit::PreSubclassWindow();
}

void CDragEdit::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1)
	{
		KillTimer(1);
		ShowBalloonTip(TEXT("�̼���·����ʾ"), TEXT("��ͨ��ֱ����ק�̼�������Ŀ¼���˴�������ѡ��"), TTI_INFO);
	}
	CEdit::OnTimer(nIDEvent);
}
