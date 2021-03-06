#pragma once
#include <iostream>
#include <iterator>
#include <string>
#include <list>
using namespace std;

class CShellCmd
{
public:
	CShellCmd();
	virtual ~CShellCmd();
public :
	int ExecuteCmd(LPSTR lpszCmd, BOOL bReturn = 0);
	void CleanResult();
	list<string> GetResult();
	BOOL IsCmdDone();
	void Terminate();
protected:
	PROCESS_INFORMATION mpi;
	HANDLE m_hReadPipe, m_hWritePipe, m_hResult, m_hCmd;
	BOOL m_bWantResult;
	list<string> m_retStr,m_dupStr;
	string m_lpCmd;
	CCriticalSection m_cs;
	HANDLE m_hSyncEvent;
private:
	static UINT CmdThread(LPVOID lpv);
	static UINT ResultThread(LPVOID lpv);
};

