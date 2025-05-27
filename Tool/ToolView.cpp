
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "Resource_Manager.h"
#include "Tile.h"
#include "FarmObject.h"
#include "MainFrm.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* pDC)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	if (nullptr == m_pGraphic_Device)
		return;	

	m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.f, 0);

	m_pGraphic_Device->BeginScene();

	for (auto& pTile : m_vecTiles)
	{
		if (nullptr != pTile)
		{
			pTile->Update_Tile();
			pTile->Render_Tile();
		}
	}

	for (auto& pFarmObejct : m_vecObject)
	{
		if (nullptr != pFarmObejct)
		{
			pFarmObejct->Update_Object();
			pFarmObejct->Render_Object();
		}
	}

	m_pGraphic_Device->EndScene();

	m_pGraphic_Device->Present(nullptr, nullptr, 0, nullptr);
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

HRESULT CToolView::Ready_Tiles(const _uint & iCntX, const _uint & iCntY)
{
	for (auto& pTile : m_vecTiles)
		Safe_Release(pTile);
	m_vecTiles.clear();

	m_iNumTileX = iCntX;
	m_iNumTileY = iCntY;
	m_iNumTiles = iCntX * iCntY;

	m_vecTiles.reserve(m_iNumTiles);

	for (size_t i = 0; i < iCntY; ++i)
	{
		for (size_t j = 0; j < iCntX; ++j)
		{
			_vec3		vScale = _vec3(64.0f, 64.0f, 1.f);
			_vec3		vPosition = _vec3(j*64.f+32.f, i*64.f+32.f, 0.f);

			CTile*		pTile = CTile::Create(m_pGraphic_Device, &vPosition, &vScale);
			if (nullptr == pTile)
			{				
				for (auto& pTile : m_vecTiles)				
					Safe_Release(pTile);
				return E_FAIL;
			}

			m_vecTiles.push_back(pTile);
		}
	}

	//_vec3		vScale = _vec3(64.0f, 64.0f, 1.f);
	//_vec3		vPosition = _vec3(j*64.f + 32.f, i*64.f + 32.f, 0.f);

	//CTile*		pTile = CTile::Create(m_pGraphic_Device, &vPosition, &vScale);

	Invalidate(FALSE);

	SetScrollSizes(MM_TEXT, CSize(m_iNumTileX * 64.f, m_iNumTileY * 64.0f));

	Set_WindowSize();
	
	return	NOERROR;
}

HRESULT CToolView::Ready_Object()
{
	Invalidate(FALSE);

	SetScrollSizes(MM_TEXT, CSize(40 * 64.f, 30 * 64.0f));

	Set_WindowSize();

	return	NOERROR;
}

void CToolView::Set_WindowSize()
{
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();
	if (nullptr == pMainFrm)
		return;

	// 내 해상도에의해 자동으로 설정된 윈도우 전체의 크기를 얻어온다.
	RECT		rcWindow = { 0 };
	pMainFrm->GetWindowRect(&rcWindow);
	SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);

	// 현재 내 클라이언의 크기를 얻어온다.	
	RECT			rcMainView = { 0 };
	GetClientRect(&rcMainView);

	// 현재 내 미니뷰(폼뷰)의 크기를 얻어온다.
	RECT			rcFormView = { 0 };
	rcFormView = pMainFrm->Get_ClientRect_FormView();

	_uint iFrameX = rcWindow.right - (rcMainView.right + rcFormView.right);
	_uint iFrameY = rcWindow.bottom - (rcMainView.bottom);

	pMainFrm->MoveWindow(0, 0, g_iFormCX + g_iBackCX + iFrameX, g_iBackCY + iFrameY);

	//rcFormView = pMainFrm->Get_ClientRect_FormView();
	//GetClientRect(&rcMainView);
}

HRESULT CToolView::Save_TileInfo()
{
	CFileDialog		FileDlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("TileInfo Files (*.dat)|*.dat|"), this);

	if (IDOK == FileDlg.DoModal())
	{
		HANDLE			hFile = 0;
		_ulong			dwByte = 0;

		hFile = CreateFile(FileDlg.GetPathName(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return E_FAIL;

		WriteFile(hFile, &m_iNumTileX, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &m_iNumTileY, sizeof(_uint), &dwByte, nullptr);

		for (auto& pTile : m_vecTiles)
		{
			WriteFile(hFile, &pTile->Get_TileInfo(), sizeof(TILEINFO), &dwByte, nullptr);
		}
		
		CloseHandle(hFile);
	}

	return NOERROR;
}

HRESULT CToolView::Load_TileInfo()
{
	CFileDialog		FileDlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("TileInfo Files (*.dat)|*.dat|"), this);

	if (IDOK == FileDlg.DoModal())
	{
		HANDLE			hFile = 0;
		_ulong			dwByte = 0;

		hFile = CreateFile(FileDlg.GetPathName(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return E_FAIL;

		ReadFile(hFile, &m_iNumTileX, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &m_iNumTileY, sizeof(_uint), &dwByte, nullptr);

		while (true)
		{
			TILEINFO		TileInfo;
			
			ReadFile(hFile, &TileInfo, sizeof(TILEINFO), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CTile*		pTile = CTile::Create(m_pGraphic_Device, &TileInfo);
			if (nullptr == pTile)
				break;

			m_vecTiles.push_back(pTile);
		}

		CloseHandle(hFile);
	}

	SetScrollSizes(MM_TEXT, CSize(m_iNumTileX * 64.f, m_iNumTileY * 64.f));

	return NOERROR;
}

HRESULT CToolView::Save_ObjectInfo()
{
	CFileDialog		FileDlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("ObjectInfo Files (*.dat)|*.dat|"), this);

	if (IDOK == FileDlg.DoModal())
	{
		HANDLE			hFile = 0;
		_ulong			dwByte = 0;

		hFile = CreateFile(FileDlg.GetPathName(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return E_FAIL;

		for (auto& FarmObject : m_vecObject)
		{
			WriteFile(hFile, &FarmObject->Get_ObjectInfo(), sizeof(OBJECTINFO), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}

	return NOERROR;
}

HRESULT CToolView::Load_ObjectInfo()
{
	CFileDialog		FileDlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("ObjectInfo Files (*.dat)|*.dat|"), this);

	if (IDOK == FileDlg.DoModal())
	{
		HANDLE			hFile = 0;
		_ulong			dwByte = 0;

		hFile = CreateFile(FileDlg.GetPathName(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return E_FAIL;

		while (true)
		{
			OBJECTINFO		ObjectInfo;

			ReadFile(hFile, &ObjectInfo, sizeof(OBJECTINFO), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CFarmObject*		pFarmObject = CFarmObject::Create(m_pGraphic_Device, &ObjectInfo);
			if (nullptr == pFarmObject)
				break;

			m_vecObject.push_back(pFarmObject);
		}

		CloseHandle(hFile);
	}

	SetScrollSizes(MM_TEXT, CSize(m_iNumTileX * 64.f, m_iNumTileY * 64.f));

	return NOERROR;
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (FAILED(CGraphic_Device::GetInstance()->Ready_Graphic_Device(m_hWnd, CGraphic_Device::MODE_WIN, g_iBackCX, g_iBackCY)))
		return;

	m_pGraphic_Device = CGraphic_Device::GetInstance()->Get_GraphicDev();

	m_pGraphic_Device->AddRef();

	// 리소스들을 준비한다.
	CResource_Manager*		pResource_Manager = CResource_Manager::GetInstance();
	if (nullptr == pResource_Manager)
		return;

	pResource_Manager->AddRef();

	// Resource_Buffer_RcTex
	if (FAILED(pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return;


	//// Resource_Texture_Tiles_House
	//if (FAILED(pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Tiles_Bmp", CTexture_Bmp::Create(m_pGraphic_Device, L"Resource/Tile/bmp/HouseTile/HouseTile%d.bmp",18))))
	//	return;
	//if (FAILED(pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Tiles", CTexture::Create(m_pGraphic_Device, L"Resource/Tile/Png/HouseTile/HouseTile%d.png", 18))))
	//	return;

	//// Resource_Texture_Tiles_Farm
	//if (FAILED(pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Tiles_Bmp", CTexture_Bmp::Create(m_pGraphic_Device, L"Resource/Tile/bmp/FarmTile/FarmTile%d.bmp", 40))))
	//	return;
	//if (FAILED(pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Tiles", CTexture::Create(m_pGraphic_Device, L"Resource/Tile/Png/FarmTile/FarmTile%d.png", 40))))
	//	return;

	// Resource_Texture_Tiles_Town
	if (FAILED(pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Tiles_Bmp", CTexture_Bmp::Create(m_pGraphic_Device, L"Resource/Tile/bmp/TownTile/TownTile%d.bmp", 75))))
		return;
	if (FAILED(pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Tiles", CTexture::Create(m_pGraphic_Device, L"Resource/Tile/Png/TownTile/TownTile%d.png",75))))
		return;


	//// Resource_Texture_FarmObject
	//if (FAILED(pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Object_Bmp", CTexture_Bmp::Create(m_pGraphic_Device, L"Resource/Tile/bmp/FarmObject/FarmObject%d.bmp", 18))))
	//	return;
	//if (FAILED(pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Object", CTexture::Create(m_pGraphic_Device, L"Resource/Tile/Png/FarmObject/FarmObject%d.png", 18))))
	//	return;

	//// Resource_Texture_ShopObject
	//if (FAILED(pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Object_Bmp", CTexture_Bmp::Create(m_pGraphic_Device, L"Resource/Tile/bmp/ShopObject/ShopObject%d.bmp", 15))))
	//	return;	
	//if (FAILED(pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Object", CTexture::Create(m_pGraphic_Device, L"Resource/Tile/png/ShopObject/ShopObject%d.png", 15))))
	//	return;

	// Resource_Texture_TownObject
	if (FAILED(pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Object_Bmp", CTexture_Bmp::Create(m_pGraphic_Device, L"Resource/Tile/bmp/TownObject/TownObject%d.bmp", 44))))
		return;
	if (FAILED(pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Object", CTexture::Create(m_pGraphic_Device, L"Resource/Tile/Png/TownObject/TownObject%d.png", 44))))
		return;

	SetScrollSizes(MM_TEXT, CSize(0, 0));

	Set_WindowSize();

	Safe_Release(pResource_Manager);
}


void CToolView::OnDestroy()
{
	CScrollView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	for (auto& pTile : m_vecTiles)
		Safe_Release(pTile);
	m_vecTiles.clear();

	for (auto& pFarmObject : m_vecObject)
		Safe_Release(pFarmObject);
	m_vecObject.clear();


	Safe_Release(m_pGraphic_Device);
}


// 위아래로 스크롤하였을경우
void CToolView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	for (auto& pTile : m_vecTiles)
	{
		pTile->Set_RenderPos(GetScrollPos(0), GetScrollPos(1));
	}

	for (auto& pFarmObject : m_vecObject)
	{
		pFarmObject->Set_RenderPos(GetScrollPos(0), GetScrollPos(1));
	}

	Invalidate(FALSE);

	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}


// 왼쪽 오른쪽으로 스크롤하였을경우
void CToolView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	for (auto& pTile : m_vecTiles)
	{
		pTile->Set_RenderPos(GetScrollPos(0), GetScrollPos(1));
	}

	for (auto& pFarmObject : m_vecObject)
	{
		pFarmObject->Set_RenderPos(GetScrollPos(0), GetScrollPos(1));
	}

	Invalidate(FALSE);

	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL CToolView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return TRUE;
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	for (auto& pTile : m_vecTiles)
	{
		if (true == pTile->Clicked_Mouse(_vec3(point.x + GetScrollPos(0), point.y + GetScrollPos(1), 0.f)))
			break;
	}

	Invalidate(FALSE);
	
	CScrollView::OnLButtonDown(nFlags, point);
}

void CToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_iNumX = (point.x + GetScrollPos(0)) / 64;
	m_iNumY = (point.y + GetScrollPos(1)) / 64;

	_vec3 vPosition(_vec3(m_iNumX * 64.f + 32.f, m_iNumY * 64.f + 32.f, 0.f));
	CFarmObject*		pFarmObject = CFarmObject::Create(m_pGraphic_Device, &vPosition);

	if (nullptr == pFarmObject)
	{
		for (auto& pFarmObject : m_vecObject)
			Safe_Release(pFarmObject);
		return;
	}

	m_vecObject.push_back(pFarmObject);
	

	Invalidate(FALSE);

	CScrollView::OnRButtonDown(nFlags, point);
}
