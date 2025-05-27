// Page_Object.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "Page_Object.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CPage_Object ��ȭ �����Դϴ�.

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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

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
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CPage_Object::OnDestroy()
{
	CPropertyPage::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	Safe_Release(m_pBmp);
}


// CPage_Object �޽��� ó�����Դϴ�.

// ������Ʈ ����
void CPage_Object::OnCbnSelchangeCombo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (nullptr == m_pBmp)
		return;

	m_pBmp->SetUp_OnControl(&m_Picture, m_Combo.GetCurSel());
}

// �׸�
void CPage_Object::OnStnClickedPicture()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

// ����
void CPage_Object::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	if (FAILED(pMainFrm->Save_ObjectInfo()))
		return;
}

// �ҷ�����
void CPage_Object::OnBnClickedButton3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	if (FAILED(pMainFrm->Load_ObjectInfo()))
		return;

	return;
}


// Ÿ�Ϻҷ�����
void CPage_Object::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	if (FAILED(pMainFrm->Load_TileInfo()))
		return;

	return;
}


void CPage_Object::OnBnClickedButton4()
{
	UpdateData(TRUE);

	// CMainFrame��ü�� �ּҸ� �޾ƿ´�.
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();
	if (nullptr == pMainFrm)
		return;

	if (FAILED(pMainFrm->Ready_Object()))
		return;
}
