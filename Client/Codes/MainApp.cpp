#include "stdafx.h"
#include "..\Headers\MainApp.h"
#include "Graphic_Device.h"
#include "Management.h"
#include "Resource_Manager.h"
#include "Font_Manager.h"
#include "Sound_Manager.h"


CMainApp::CMainApp()
	: m_pGraphic_Instance(CGraphic_Device::GetInstance())
	, m_pGraphic_Device(nullptr)
	, m_pManagement(CManagement::GetInstance())
	, m_pResource_Manager(CResource_Manager::GetInstance())
{
	m_pGraphic_Instance->AddRef();
	m_pManagement->AddRef();
	m_pResource_Manager->AddRef();
}

HRESULT CMainApp::Ready_MainApp()
{
	if (FAILED(Ready_Default_Setting(CGraphic_Device::MODE_FULL, g_iBackCX, g_iBackCY)))
		return E_FAIL;

	if (FAILED(Ready_Render_State()))
		return E_FAIL;

	if (FAILED(Ready_Resource_Static()))
		return E_FAIL;

	if (FAILED(Ready_Start_Scene(SCENE_LOGO)))
		return E_FAIL;

	srand((unsigned)time(NULL));

	return NOERROR;
}

_int CMainApp::Update_MainApp(const _float & fTimeDelta)
{
	if (nullptr == m_pManagement)
		return -1;

	m_fTimeAcc += fTimeDelta;

	return m_pManagement->Update_Scene(fTimeDelta);
}

void CMainApp::Render_MainApp()
{
	if (nullptr == m_pGraphic_Device || 
		nullptr == m_pManagement)
		return;

	// �����(Ÿ��)�� ���̹��ۿ� ���ٽǹ��۸� �ʱ�ȭ�մϴ�.

	// D3DXCOLOR()
	// D3DCOLOR_ARGB()
	// D3DCOLOR_XRGB()
	m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.f, 0);

	// �׸������� �غ� �Ѵ�.
	m_pGraphic_Device->BeginScene();

	// ��ü���� �ĸ���ۿ� �׸���.
	m_pManagement->Render_Scene();

	// �׸����۾��� ���� �δ´�.
	m_pGraphic_Device->EndScene();

	// ����� �ĸ��� �����Ѵ�.
	// ������۸� ȭ�鿡 �����ش�.
	m_pGraphic_Device->Present(nullptr, nullptr, nullptr, nullptr);

	++m_iRenderCnt;
	
	if (m_fTimeAcc >= 1.f)
	{
		wsprintf(m_szFPS, L"FPS:%d", m_iRenderCnt);
		m_fTimeAcc = 0.f;
		m_iRenderCnt = 0;
	}

	//SetWindowText(g_hWnd, m_szFPS);
}

HRESULT CMainApp::Ready_Default_Setting(CGraphic_Device::WINMODE eMode, const _uint& iBackCX, const _uint& iBackCY)
{
	// �׷���ī�� ��ġ�� �ʱ�ȭ�մϴ�.	
	if (nullptr != m_pGraphic_Instance)
	{	
		if (FAILED(m_pGraphic_Instance->Ready_Graphic_Device(eMode, iBackCX, iBackCY)))
			return E_FAIL;

		m_pGraphic_Device = m_pGraphic_Instance->Get_GraphicDev();
		m_pGraphic_Device->AddRef();
	}

	// �Է���ġ�� �ʱ�ȭ �մϴ�.
	if (FAILED(CSound_Manager::Get_Instance()->Ready_Sound_Manager()))
		return E_FAIL;

	// ����� ��Ʈ�� �߰��Ѵ�.
	if (FAILED(CFont_Manager::GetInstance()->Add_Font(m_pGraphic_Device, L"Font_Batang", FW_HEAVY, L"����")))
		return E_FAIL;

	if (FAILED(CFont_Manager::GetInstance()->Add_Font(m_pGraphic_Device, L"Font_Hoonmin", FW_HEAVY, L"��ü�� �ƹ�����ü")))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMainApp::Ready_Render_State()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return NOERROR;
}

HRESULT CMainApp::Ready_Resource_Static()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Resource_Buffer_RcCol	
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcCol", CBuffer_RcCol::Create(m_pGraphic_Device))))
		return E_FAIL;
	// For.Resource_Buffer_RcTex
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	// For.Resource_Buffer_RcTexPos
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTexPos", CBuffer_RcTexPos::Create(m_pGraphic_Device))))
		return E_FAIL;
	// For.Resource_Shader_Default	
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Shader_Default", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Default.fx"))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMainApp::Ready_Start_Scene(SCENEID eID)
{
	// ���� ������ ���� �����Ϻ��. 
	if (nullptr != m_pManagement)
	{
		if (FAILED(m_pManagement->Ready_Scene(m_pGraphic_Device, eID)))
			return E_FAIL;
	}

	return NOERROR;
}

CMainApp * CMainApp::Create()
{
	CMainApp*		pInstance = new CMainApp();

	if (FAILED(pInstance->Ready_MainApp()))
	{
		_MSG_BOX("CMainApp Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pResource_Manager);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pGraphic_Instance);

	Safe_Release(m_pManagement);
}
