#include "stdafx.h"
#include "ShellCmd.h"


CShellCmd::CShellCmd()
{
	m_hCmd = NULL;
	m_hResult = NULL;
	m_bWantResult = FALSE;
	memset(&mpi, 0,sizeof(mpi));
	m_hSyncEvent = CreateEvent(NULL, 1, 0, 0);
}


CShellCmd::~CShellCmd()
{
	if (m_hCmd)
	{
		WaitForSingleObject(m_hCmd, INFINITE);
		CloseHandle(m_hCmd);
		m_hCmd = NULL;
	}
	if (m_hResult)
	{
		WaitForSingleObject(m_hResult, INFINITE);
		CloseHandle(m_hResult);
		m_hResult = NULL;
	}
	m_bWantResult = FALSE;
	CloseHandle(m_hSyncEvent);
}

BOOL CShellCmd::IsCmdDone()
{
	if (!m_bWantResult)
	{
		if (mpi.hProcess)
		{
			return (WAIT_OBJECT_0 == WaitForSingleObject(mpi.hProcess, 0));
		}
		return TRUE;
	}
	
	if (!m_hResult)
	{
		return TRUE;
	}
	return (WAIT_OBJECT_0 == WaitForSingleObject(m_hResult, 0));
}

void CShellCmd::Terminate()
{
	STARTUPINFOA si = { 0 };
	SECURITY_ATTRIBUTES sa = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	char cmdline[MAX_PATH] = { 0 };
	si.cb = sizeof STARTUPINFO;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW;
	if (WAIT_OBJECT_0 != WaitForSingleObject(mpi.hProcess, 0))
	{
		sprintf_s(cmdline, "taskkill /pid %d", mpi.dwProcessId);
		if (CreateProcessA(0, cmdline, &sa, &sa, 0, 0, 0, 0, &si, &pi))
		{
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
	}
	if (m_hCmd)
	{
		CloseHandle(m_hCmd);
		m_hCmd = NULL;
	}
	if (m_hResult)
	{
		CloseHandle(m_hResult);
		m_hResult = NULL;
	}
	m_bWantResult = FALSE;
	memset(&pi, 0, sizeof(pi));
}

int CShellCmd::ExecuteCmd(LPSTR lpszCmd, BOOL bReturn)
{
	m_lpCmd = lpszCmd;
	m_bWantResult = bReturn;
	m_retStr.clear();
	m_hCmd = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CmdThread, this, 0, 0);
	if (m_bWantResult)
	{
		m_hResult = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ResultThread, this, 0, 0);
	}
	return 0;
}

UINT CShellCmd::CmdThread(LPVOID lpv)
{
	CShellCmd* p = (CShellCmd*)lpv;
	BOOL retval;
	int result = -1;
	STARTUPINFOA si = { 0 };
	SECURITY_ATTRIBUTES sa = { 0 };
	DWORD retcode = -1;
	CString strMsg;

	sa.bInheritHandle = p->m_bWantResult;
	sa.nLength = sizeof SECURITY_ATTRIBUTES;
	sa.lpSecurityDescriptor = NULL;
	retval = CreatePipe(&p->m_hReadPipe, &p->m_hWritePipe, &sa, 0);
	if (!retval)
	{
		SetEvent(p->m_hSyncEvent);
		return 1;
	}
	si.cb = sizeof STARTUPINFO;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.hStdOutput = si.hStdError = p->m_hWritePipe;
	p->CleanResult();
	memset(&p->mpi, 0, sizeof(PROCESS_INFORMATION));
	retval = CreateProcessA(NULL, (LPSTR)p->m_lpCmd.c_str(), &sa, &sa, p->m_bWantResult, 0, NULL, 0, &si, &p->mpi);
	if (!retval)
	{
		SetEvent(p->m_hSyncEvent);
		return 2;
	}
	OutputDebugString(L"INFO2: CmdThread Run Over\n");
	SetEvent(p->m_hSyncEvent);
	return 0;
}

void CShellCmd::CleanResult()
{
	m_retStr.clear();
	m_dupStr.clear();
}

list<string> CShellCmd::GetResult()
{
	m_dupStr.clear();
	m_cs.Lock();
	copy(m_retStr.begin(), m_retStr.end(), back_inserter(m_dupStr));
	m_retStr.clear();
	m_cs.Unlock();
	return m_dupStr;
}

#define BUFFSIZE	8192
UINT CShellCmd::ResultThread(LPVOID lpv)
{
	CShellCmd* p = (CShellCmd*)lpv;
	char buf[BUFFSIZE] = { 0 }, tmp[1024] = { 0 };
	DWORD dwRead;
	LONG lWantRd;
	char c;
	int idx;

	while (WAIT_OBJECT_0 != WaitForSingleObject(p->m_hSyncEvent, 100));
	ResetEvent(p->m_hSyncEvent);
	OutputDebugString(L"INFO2: ResultThread begin...\n");

	while (WAIT_OBJECT_0 != WaitForSingleObject(p->mpi.hThread, 1))
	{
		lWantRd = (LONG)GetFileSize(p->m_hReadPipe, 0);
		memset(buf, 0, BUFFSIZE);
		idx = 0;
		while (lWantRd-- > 0)
		{
			ReadFile(p->m_hReadPipe, &c, 1, &dwRead, 0);
			if (c == '\r' || c == '\n' || c == 0)
			{
				if (strlen(buf))
				{
					p->m_cs.Lock();
					p->m_retStr.push_back(buf);
					OutputDebugStringA(buf);
					//OutputDebugString(L"\n");
					p->m_cs.Unlock();
				}
				memset(buf, 0, BUFFSIZE);
				idx = 0;
				continue;
			}
			buf[idx] = c;
			idx++;
		}
	}
	//进程已结束，再读一次
	lWantRd = (LONG)GetFileSize(p->m_hReadPipe, 0);
	memset(buf, 0, BUFFSIZE);
	idx = 0;
	while (lWantRd-- > 0)
	{
		ReadFile(p->m_hReadPipe, &c, 1, &dwRead, 0);
		if (c == '\r' || c == '\n' || c == 0)
		{
			if (strlen(buf))
			{
				p->m_cs.Lock();
				p->m_retStr.push_back(buf);
				OutputDebugStringA(buf);
				//OutputDebugString(L"\n");
				p->m_cs.Unlock();
			}
			memset(buf, 0, BUFFSIZE);
			idx = 0;
			continue;
		}
		buf[idx] = c;
		idx++;
	}
	return 0L;
}