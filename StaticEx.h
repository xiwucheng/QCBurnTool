#pragma once
#include "afxwin.h"
class CStaticEx :public CStatic
{
public:
	CStaticEx();
	virtual ~CStaticEx();
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	int SetPos(int nPos);
	int SetBkColor(COLORREF nColor);
	int SetFrontColor(COLORREF nColor);
	int SetTextColor(COLORREF nColor);
	int SetType(int nType);
private:
	COLORREF m_clrText;
	COLORREF m_clrBk, m_clrFront;
	int m_nPos,m_nType;
};

