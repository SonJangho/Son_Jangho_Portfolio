// Form.cpp : ���� �����Դϴ�.
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


// CForm �����Դϴ�.

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


// CForm �޽��� ó�����Դϴ�.

// Create Tile
//void CForm::OnBnClickedButton2()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	UpdateData(TRUE);
//
//	// CMainFrame��ü�� �ּҸ� �޾ƿ´�.
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

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	// ���������� �ø������� ��Ʈ�� �����Ѵ�.
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	Safe_Delete(m_pSheet);
}
