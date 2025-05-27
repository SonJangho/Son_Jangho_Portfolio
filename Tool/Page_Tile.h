#pragma once
#include "afxwin.h"

#include "Resource_Manager.h"

// CPage_Tile ��ȭ �����Դϴ�.

class CPage_Tile : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Tile)

public:
	CPage_Tile();
	virtual ~CPage_Tile();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_TILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
private:
	int m_iNumTileX;
	int m_iNumTileY;
public:
	_int Get_TileIndex() {
		return m_Combo.GetCurSel(); }

	afx_msg void OnBnClickedButton1();
private:
	CComboBox		m_Combo;
	CStatic			m_Picture;
	CTexture_Bmp*	m_pBmp = nullptr;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();

	afx_msg void OnCbnSelchangeCombo();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnStnClickedPicture();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnCbnSelchangeCombo1();
private:
	CComboBox m_TileCombo;
};
