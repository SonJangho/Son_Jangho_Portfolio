// Form.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Form.h"
#include "MainFrm.h"
#include "Sheet.h"


// CForm

IMPLEMENT_DYNCREATE(CForm, CFormView)

CForm::CForm()
	: CFormView(IDD_FORM)

{

}

CForm::~CForm()
{
}

void CForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

}

_int CForm::Get_TileIndex()
{
	if (nullptr == m_pSheet)
		return -1;

	return m_pSheet->Get_TileIndex();
}

_int CForm::Get_ObjectIndex()
{
	if (nullptr == m_pSheet)
		return -1;

	return m_pSheet->Get_ObjectIndex();
}

BEGIN_MESSAGE_MAP(CForm, CFormView)	
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CForm 진단입니다.

#ifdef _DEBUG
void CForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CForm 메시지 처리기입니다.

// Create Tile
//void CForm::OnBnClickedButton2()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	UpdateData(TRUE);
//
//	// CMainFrame객체의 주소를 받아온다.
//	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();
//	if (nullptr == pMainFrm)
//		return;
//
//	if (FAILED(pMainFrm->Ready_Tiles(m_iNumCntX, m_iNumCntY)))
//		return;
//
//
//
//
//
//
//
//	
//}


void CForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// 페이지들을 올리기위한 시트를 생성한다.
	m_pSheet = new CSheet;

	m_pSheet->Create(this, WS_CHILD | WS_VISIBLE);

	m_pSheet->MoveWindow(0, 0, g_iFormCX, g_iFormCY);

	for (size_t i = 0; i < 2; ++i)
	{
		m_pSheet->SetActivePage(i);
	}

	m_pSheet->SetActivePage(0);
	










}


void CForm::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Safe_Delete(m_pSheet);
}
