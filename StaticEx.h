#pragma once
#include "afxwin.h"
class CStaticEx :public CStatic
{
public:
	CStaticEx();
	virtual ~CStaticEx();
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	WORD SetPos(WORD nPos);
	int SetBkColor(COLORREF nColor);
	int SetFrontColor(COLORREF nColor);
	int SetTextColor(COLORREF nColor);
	int SetType(int nType);
	int SetFontSize(int nSize);
private:
	COLORREF m_clrText;
	COLORREF m_clrBk, m_clrFront;
	int  m_nType;
	WORD m_nPos;
	LOGFONT m_lf;
	HFONT m_hFont;
};

