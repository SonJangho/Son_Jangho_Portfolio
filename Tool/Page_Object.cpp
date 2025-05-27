// Page_Object.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Page_Object.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CPage_Object 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPage_Object, CPropertyPage)

CPage_Object::CPage_Object()
	: CPropertyPage(IDD_PAGE_OBJECT)
{

}

CPage_Object::~CPage_Object()
{
}

void CPage_Object::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	DDX_Control(pDX, IDC_COMBO, m_Combo);
}


BEGIN_MESSAGE_MAP(CPage_Object, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO, &CPage_Object::OnCbnSelchangeCombo)
	ON_STN_CLICKED(IDC_PICTURE, &CPage_Object::OnStnClickedPicture)
	ON_BN_CLICKED(IDC_BUTTON2, &CPage_Object::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPage_Object::OnBnClickedButton3)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CPage_Object::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CPage_Object::OnBnClickedButton4)
END_MESSAGE_MAP()

BOOL CPage_Object::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	for (size_t i = 0; i < 44; i++)
	{
		_tchar		szName[128] = L"";
		wsprintf(szName, L"FarmObject%d", i);

		m_Combo.AddString(szName);
	}

	m_Combo.SetCurSel(0);

	m_pBmp = (CTexture_Bmp*)CResource_Manager::GetInstance()->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Object_Bmp");
	if (nullptr == m_pBmp)
		return FALSE;

	m_pBmp->AddRef();

	m_pBmp->SetUp_OnControl(&m_Picture, m_Combo.GetCurSel());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CPage_Object::OnDestroy()
{
	CPropertyPage::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Safe_Release(m_pBmp);
}


// CPage_Object 메시지 처리기입니다.

// 오브젝트 선택
void CPage_Object::OnCbnSelchangeCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_pBmp)
		return;

	m_pBmp->SetUp_OnControl(&m_Picture, m_Combo.GetCurSel());
}

// 그림
void CPage_Object::OnStnClickedPicture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// 저장
void CPage_Object::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	if (FAILED(pMainFrm->Save_ObjectInfo()))
		return;
}

// 불러오기
void CPage_Object::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	if (FAILED(pMainFrm->Load_ObjectInfo()))
		return;

	return;
}


// 타일불러오기
void CPage_Object::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	if (FAILED(pMainFrm->Load_TileInfo()))
		return;

	return;
}


void CPage_Object::OnBnClickedButton4()
{
	UpdateData(TRUE);

	// CMainFrame객체의 주소를 받아온다.
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();
	if (nullptr == pMainFrm)
		return;

	if (FAILED(pMainFrm->Ready_Object()))
		return;
}
