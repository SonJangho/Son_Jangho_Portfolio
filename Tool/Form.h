#pragma once



// CForm 폼 뷰입니다.
#include "Defines.h"

class CSheet;
class CForm : public CFormView
{
	DECLARE_DYNCREATE(CForm)

protected:
	CForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	_int Get_TileIndex();
	_int Get_ObjectIndex();
private:
	CSheet*				m_pSheet = nullptr;
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
};


