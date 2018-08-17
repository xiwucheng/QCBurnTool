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
	m_lf.lfHeight = -14;
	m_hFont = CreateFontIndirect(&m_lf);
}


CStaticEx::~CStaticEx()
{
	DeleteObject(m_hFont);
}


void CStaticEx::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	DWORD dwStyle = GetStyle();
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_OWNERDRAW);
	CStatic::PreSubclassWindow();
}


void CStaticEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  ������Ĵ����Ի���ָ����
	CString str;
	float ccx;
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
		cx = (LOBYTE(m_nPos)*100+HIBYTE(m_nPos))*rcItem.Width() / 10000;
		str.Format(TEXT("%d.%02d%%"), LOBYTE(m_nPos),HIBYTE(m_nPos));
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

WORD CStaticEx::SetPos(WORD nPos)
{
	WORD pos = LOBYTE(nPos) * 100 + HIBYTE(nPos);
	if (pos > 10000)
		m_nPos = MAKEWORD(100, 0);
	else
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
