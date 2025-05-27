#pragma once
#include "afxwin.h"

#include "Resource_Manager.h"

// CPage_Object ��ȭ �����Դϴ�.

class CPage_Object : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Object)

public:
	CPage_Object();
	virtual ~CPage_Object();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_OBJECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo();
	afx_msg void OnStnClickedPicture();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
public:
	_int Get_ObjectIndex() {
		return m_Combo.GetCurSel();
	}
private:
	CStatic m_Picture;
	CComboBox m_Combo;
	CTexture_Bmp*	m_pBmp = nullptr;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
};
