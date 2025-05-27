// Page_Tile.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Page_Tile.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CPage_Tile 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPage_Tile, CPropertyPage)

CPage_Tile::CPage_Tile()
	: CPropertyPage(IDD_PAGE_TILE)
	, m_iNumTileX(0)
	, m_iNumTileY(0)
{

}

CPage_Tile::~CPage_Tile()
{
}

void CPage_Tile::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iNumTileX);
	DDX_Text(pDX, IDC_EDIT2, m_iNumTileY);
	DDX_Control(pDX, IDC_COMBO, m_Combo);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	DDX_Control(pDX, IDC_COMBO1, m_TileCombo);
}


BEGIN_MESSAGE_MAP(CPage_Tile, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage_Tile::OnBnClickedButton1)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_COMBO, &CPage_Tile::OnCbnSelchangeCombo)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON2, &CPage_Tile::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPage_Tile::OnBnClickedButton3)
	ON_STN_CLICKED(IDC_PICTURE, &CPage_Tile::OnStnClickedPicture)
	ON_EN_CHANGE(IDC_EDIT2, &CPage_Tile::OnEnChangeEdit2)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CPage_Tile::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CPage_Tile 메시지 처리기입니다.


void CPage_Tile::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	
	// CMainFrame객체의 주소를 받아온다.
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();
	if (nullptr == pMainFrm)
		return;
	
	if (FAILED(pMainFrm->Ready_Tiles(m_iNumTileX, m_iNumTileY)))
		return;
}


int CPage_Tile::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	

	return 0;
}


BOOL CPage_Tile::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	for (size_t i = 0; i < 75; i++)
	{
		_tchar		szName[128] = L"";
		wsprintf(szName, L"HouseTile%d", i);

		m_Combo.AddString(szName);
	}

	m_Combo.SetCurSel(0);

	m_pBmp = (CTexture_Bmp*)CResource_Manager::GetInstance()->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Tiles_Bmp");
	if (nullptr == m_pBmp)
		return FALSE;	

	m_pBmp->AddRef();

	m_pBmp->SetUp_OnControl(&m_Picture, m_Combo.GetCurSel());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CPage_Tile::OnCbnSelchangeCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_pBmp)
		return;	

	m_pBmp->SetUp_OnControl(&m_Picture, m_Combo.GetCurSel());
}


void CPage_Tile::OnDestroy()
{
	CPropertyPage::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Safe_Release(m_pBmp);
}


// 저장
void CPage_Tile::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	if (FAILED(pMainFrm->Save_TileInfo()))
		return;

	return;
}


// 불러오기
void CPage_Tile::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	if (FAILED(pMainFrm->Load_TileInfo()))
		return;

	return;
}


void CPage_Tile::OnStnClickedPicture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CPage_Tile::OnEnChangeEdit2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CPage_Tile::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
