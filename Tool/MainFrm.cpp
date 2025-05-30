
// MainFrm.cpp : CMainFrame 클래스의 구현
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
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
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
	//	TRACE0("도구 모음을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("상태 표시줄을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	//// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
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
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

// CMainFrame 진단

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


// CMainFrame 메시지 처리기



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_MainSplitter.CreateStatic(this, 1, 2);	

	// 분할된 윈도우에 뷰를 셋팅한다.
	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CToolView), CSize(1280, 768), pContext);
	m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CForm), CSize(400, 768), pContext);
	

	// 분할된 윈도우에 셋팅되어있던 뷰의 주소르 ㄹ얻어온다.
	m_pForm = (CForm*)m_MainSplitter.GetPane(0, 0);
	m_pMainView = (CToolView*)m_MainSplitter.GetPane(0, 1);

	return TRUE;

	// return CFrameWnd::OnCreateClient(lpcs, pContext);

	
}


void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
