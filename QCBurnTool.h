
// QCBurnTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CQCBurnToolApp: 
// �йش����ʵ�֣������ QCBurnTool.cpp
//

class CQCBurnToolApp : public CWinApp
{
public:
	CQCBurnToolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CQCBurnToolApp theApp;