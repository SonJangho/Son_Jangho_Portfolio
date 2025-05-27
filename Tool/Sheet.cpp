// Sheet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Sheet.h"


// CSheet

IMPLEMENT_DYNAMIC(CSheet, CPropertySheet)

CSheet::CSheet()
{
	AddPage(&m_Page_Tile);
	AddPage(&m_Page_Object);

}

CSheet::CSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CSheet::CSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

CSheet::~CSheet()
{
	m_Page_Object.DestroyWindow();
	m_Page_Tile.DestroyWindow();

	
}


BEGIN_MESSAGE_MAP(CSheet, CPropertySheet)
END_MESSAGE_MAP()


// CSheet 메시지 처리기입니다.
