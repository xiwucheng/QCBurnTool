#pragma once
#include "afxwin.h"
class CDragEdit :
	public CEdit
{
public:
	CDragEdit();
	virtual ~CDragEdit();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual void PreSubclassWindow();
private:
	BOOL m_bTip;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

