#include "stdafx.h"
#include "DragEdit.h"


CDragEdit::CDragEdit()
{
}


CDragEdit::~CDragEdit()
{
}
BEGIN_MESSAGE_MAP(CDragEdit, CEdit)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


void CDragEdit::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

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
	// TODO: 在此添加专用代码和/或调用基类
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
	CEdit::PreSubclassWindow();
}
