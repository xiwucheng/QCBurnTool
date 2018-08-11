#include "stdafx.h"
#include "ShellCmd.h"


CShellCmd::CShellCmd()
{
}


CShellCmd::~CShellCmd()
{
	WaitForSingleObject(m_hCmd, INFINITE);
	WaitForSingleObject(m_hResult, INFINITE);
}

int CShellCmd::ExecuteCmd(LPSTR lpszCmd, BOOL bReturn)
{
	m_lpCmd = lpszCmd;
	m_bWantResult = bReturn;
	m_retStr.clear();
	m_hCmd = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CmdThread, this, 0, 0);
	if (m_bWantResult)
	{
		Sleep(500);
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
		return 1;
	}
	si.cb = sizeof STARTUPINFO;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.hStdOutput = si.hStdError = p->m_hWritePipe;
	p->CleanResult();
	retval = CreateProcessA(NULL, (LPSTR)p->m_lpCmd.c_str(), &sa, &sa, p->m_bWantResult, 0, NULL, 0, &si, &p->pi);
	if (!retval)
	{
		return 2;
	}
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
	OutputDebugString(L"GetResult\n");
	Sleep(10000);
	m_cs.Unlock();
	return m_dupStr;
}

#define BUFFSIZE	8192
UINT CShellCmd::ResultThread(LPVOID lpv)
{
	CShellCmd* p = (CShellCmd*)lpv;
	char buf[BUFFSIZE] = { 0 }, tmp[1024] = { 0 }, *szToken, *szNextToken;
	DWORD dwRead,dwWantRd;
	int len_tmp;
	while (WAIT_OBJECT_0 != WaitForSingleObject(p->pi.hThread, 1))
	{
		dwWantRd = GetFileSize(p->m_hReadPipe, 0);
		memset(buf, 0, BUFFSIZE);
		len_tmp = strlen(tmp);
		if (len_tmp)
		{
			strcpy_s(buf, BUFFSIZE, tmp);
		}
		if (dwWantRd != 0 && ReadFile(p->m_hReadPipe, buf+len_tmp, min(BUFFSIZE-len_tmp,dwWantRd), &dwRead, 0))
		{
			szToken = strtok_s(buf,"\r\n",&szNextToken);
			while (szToken && szNextToken)
			{
				p->m_cs.Lock();
				p->m_retStr.push_back(szToken);
				//OutputDebugString(L"Read Result\n");
				p->m_cs.Unlock();
				szToken = strtok_s(NULL, "\r\n",&szNextToken);
			}
			if (szToken)
			{
				strcpy_s(tmp, 1024, szToken);
			}
		}
	}

	dwWantRd = GetFileSize(p->m_hReadPipe, 0);
	memset(buf, 0, BUFFSIZE);
	len_tmp = strlen(tmp);
	if (len_tmp)
	{
		strcpy_s(buf, BUFFSIZE, tmp);
	}
	while (dwWantRd != 0 && ReadFile(p->m_hReadPipe, buf + len_tmp, min(BUFFSIZE - len_tmp, dwWantRd), &dwRead, 0))
	{
		szToken = strtok_s(buf, "\r\n", &szNextToken);
		while (szToken && szNextToken)
		{
			p->m_cs.Lock();
			p->m_retStr.push_back(szToken);
			p->m_cs.Unlock();
			szToken = strtok_s(NULL, "\r\n", &szNextToken);
		}
		if (szToken)
		{
			strcpy_s(tmp, 1024, szToken);
		}
		dwWantRd = GetFileSize(p->m_hReadPipe, 0);
		Sleep(1);
	}
	CloseHandle(p->pi.hThread);
	CloseHandle(p->pi.hProcess);
	return 0L;
}