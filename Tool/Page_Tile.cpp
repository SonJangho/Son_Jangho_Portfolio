// Page_Tile.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "Page_Tile.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CPage_Tile ��ȭ �����Դϴ�.

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


// CPage_Tile �޽��� ó�����Դϴ�.


void CPage_Tile::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	
	// CMainFrame��ü�� �ּҸ� �޾ƿ´�.
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

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	

	return 0;
}


BOOL CPage_Tile::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CPage_Tile::OnCbnSelchangeCombo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (nullptr == m_pBmp)
		return;	

	m_pBmp->SetUp_OnControl(&m_Picture, m_Combo.GetCurSel());
}


void CPage_Tile::OnDestroy()
{
	CPropertyPage::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	Safe_Release(m_pBmp);
}


// ����
void CPage_Tile::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	if (FAILED(pMainFrm->Save_TileInfo()))
		return;

	return;
}


// �ҷ�����
void CPage_Tile::OnBnClickedButton3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	if (FAILED(pMainFrm->Load_TileInfo()))
		return;

	return;
}


void CPage_Tile::OnStnClickedPicture()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CPage_Tile::OnEnChangeEdit2()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CPropertyPage::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CPage_Tile::OnCbnSelchangeCombo1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
