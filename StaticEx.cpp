#include "stdafx.h"
#include "StaticEx.h"


CStaticEx::CStaticEx()
{
	m_nPos = 0;
	m_clrText = RGB(255,255,255);
	m_clrFront = RGB(0,200,0);
	m_clrBk = RGB(100,100,100);
	m_nType = 0;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(m_lf), &m_lf);
	m_lf.lfHeight = 16;
	m_hFont = CreateFontIndirect(&m_lf);
}


CStaticEx::~CStaticEx()
{
	DeleteObject(m_hFont);
}


void CStaticEx::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	DWORD dwStyle = GetStyle();
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_OWNERDRAW);
	CStatic::PreSubclassWindow();
}


void CStaticEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  添加您的代码以绘制指定项
	CString str;
	int cx;
	CRect rcItem(lpDrawItemStruct->rcItem),rcValid;
	HDC hdc = lpDrawItemStruct->hDC;
	SaveDC(hdc);
	::SetBkColor(hdc, m_clrBk);
	SetBkMode(hdc, TRANSPARENT);
	::SetTextColor(hdc,m_clrText);
	SelectObject(hdc, m_hFont);
	ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rcItem, NULL, 0, NULL);
	if (m_nType == 0)
	{
		cx = m_nPos*rcItem.Width() / 100;
		str.Format(TEXT("%d%%"), m_nPos);
		::SetBkColor(hdc, m_clrFront);
		CopyRect(&rcValid, &rcItem);
		rcValid.right = cx;
		ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rcValid, NULL, 0, NULL);
		if (m_nPos)
		{
			DrawText(hdc, str, -1, &rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	else
	{
		GetWindowText(str);
		DrawText(hdc, str, -1, &rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	RestoreDC(hdc, -1);
}

int CStaticEx::SetType(int nType)
{
	m_nType = nType ? 1 : 0;
	Invalidate(0);
	return m_nType;
}

int CStaticEx::SetFontSize(int nSize)
{
	DeleteObject(m_hFont);
	m_lf.lfHeight = nSize;
	m_hFont = CreateFontIndirect(&m_lf);
	Invalidate(0);
	return 0;
}

int CStaticEx::SetPos(int nPos)
{
	nPos = nPos < 0 ? 0 : nPos;
	nPos = nPos > 100 ? 100 : nPos;
	m_nPos = nPos;
	Invalidate(0);
	return m_nPos;
}


int CStaticEx::SetBkColor(COLORREF nColor)
{
	m_clrBk = nColor;
	Invalidate(0);
	return 0;
}


int CStaticEx::SetFrontColor(COLORREF nColor)
{
	m_clrFront = nColor;
	Invalidate(0);
	return 0;
}


int CStaticEx::SetTextColor(COLORREF nColor)
{
	m_clrText = nColor;
	Invalidate(0);
	return 0;
}
