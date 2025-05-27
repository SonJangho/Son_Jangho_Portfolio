#pragma once

#include "Page_Tile.h"
#include "Page_Object.h"


// CSheet

class CSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSheet)

public:
	CSheet();
	CSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSheet();
public:
	_int Get_TileIndex() {
		return m_Page_Tile.Get_TileIndex(); }
	_int Get_ObjectIndex() {
		return m_Page_Object.Get_ObjectIndex();
	}
private:
	CPage_Tile				m_Page_Tile;
	CPage_Object			m_Page_Object;

protected:
	DECLARE_MESSAGE_MAP()
};


