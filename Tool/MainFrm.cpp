
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "Tool.h"

#include "ToolView.h"
#include "MiniView.h"
#include "Form.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("���� ������ ������ ���߽��ϴ�.\n");
	//	return -1;      // ������ ���߽��ϴ�.
	//}

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
	//	return -1;      // ������ ���߽��ϴ�.
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	//// TODO: ���� ������ ��ŷ�� �� ���� �Ϸ��� �� �� ���� �����Ͻʽÿ�.
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);


	return 0;
}

RECT CMainFrame::Get_ClientRect_FormView()
{
	RECT			rcClient = { 0 };

	if (nullptr == m_pForm)
		return rcClient;

	m_pForm->GetClientRect(&rcClient);

	return rcClient;
}

_int CMainFrame::Get_TileIndex()
{
	if (nullptr == m_pForm)
		return -1;

	return m_pForm->Get_TileIndex();
}

_int CMainFrame::Get_ObjectIndex()
{
	if (nullptr == m_pForm)
		return -1;

	return m_pForm->Get_ObjectIndex();
}

HRESULT CMainFrame::Save_TileInfo()
{	
	return m_pMainView->Save_TileInfo();	
}

HRESULT CMainFrame::Load_TileInfo()
{
	return m_pMainView->Load_TileInfo();
}

HRESULT CMainFrame::Save_ObjectInfo()
{
	return m_pMainView->Save_ObjectInfo();
}

HRESULT CMainFrame::Load_ObjectInfo()
{
	return m_pMainView->Load_ObjectInfo();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
HRESULT CMainFrame::Ready_Tiles(const _uint & iNumCntX, const _uint & iNumCntY)
{
	if (nullptr == m_pMainView)
		return E_FAIL;

	if (FAILED(m_pMainView->Ready_Tiles(iNumCntX, iNumCntY)))
		return E_FAIL;

	return NOERROR;
}
HRESULT CMainFrame::Ready_Object()
{
	if (nullptr == m_pMainView)
		return E_FAIL;

	if (FAILED(m_pMainView->Ready_Object()))
		return E_FAIL;

	return NOERROR;
}
#endif //_DEBUG


// CMainFrame �޽��� ó����



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_MainSplitter.CreateStatic(this, 1, 2);	

	// ���ҵ� �����쿡 �並 �����Ѵ�.
	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CToolView), CSize(1280, 768), pContext);
	m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CForm), CSize(400, 768), pContext);
	

	// ���ҵ� �����쿡 ���õǾ��ִ� ���� �ּҸ� �����´�.
	m_pForm = (CForm*)m_MainSplitter.GetPane(0, 0);
	m_pMainView = (CToolView*)m_MainSplitter.GetPane(0, 1);

	return TRUE;

	// return CFrameWnd::OnCreateClient(lpcs, pContext);

	
}


void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
