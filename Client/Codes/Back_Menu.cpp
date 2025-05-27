#include "stdafx.h"
#include "..\Headers\Back_Menu.h"
#include "Render_Manager.h"
#include "Resource_Manager.h"

CBack_Menu::CBack_Menu(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

HRESULT CBack_Menu::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	if (FAILED(SetUp_Resource()))
		return E_FAIL;

	Set_Scale(_vec3(2048.f , 1024.f, 1.f));
	Set_Position(_vec3(g_iBackCX * 0.5f, g_iBackCY * 0.5f, 0.0f));

	return NOERROR;
}

_int CBack_Menu::Update_GameObject(const _float& fTimeDelta)
{
	
	POINT		ptMouse;
	ZeroMemory(&ptMouse, sizeof(POINT));

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	RECT rcRect = { 411 , 570, 411 + 222, 570 + 173 };
	RECT exitRect = { 671, 570, 671 + 222, 570 + 173 };

	m_bNew = false;
	m_bExit = false;
	if (TRUE == PtInRect(&rcRect, ptMouse))
	{
		m_bNew = true;
	}

	if (TRUE == PtInRect(&exitRect, ptMouse))
	{
		m_bExit = true;
	}


	return _int();
}

_int CBack_Menu::LastUpdate_GameObject(const _float& fTimeDelta)
{
	
	if (nullptr != m_pRender_Manager)
		m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_PRIORITY, this);

	return CGameObject::LastUpdate_GameObject(fTimeDelta);
}

void CBack_Menu::Render_GameObject()
{
	if (nullptr == m_pBuffer ||
		nullptr == m_pTexture ||
		nullptr == m_pShader)
		return;

	LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return;
	pEffect->AddRef();

	Set_Scale(_vec3(2048.f, 1024.f, 1.f));
	Set_Position(_vec3(g_iBackCX * 0.5f, g_iBackCY * 0.5f, 0.0f));
	m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture", 0);
	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);
	m_pBuffer->Render_Buffer(m_matTransform);
	pEffect->EndPass();
	pEffect->End();

	if (true == m_bNew)
	{
		Set_Scale(_vec3(256.f, 256.f, 1.f));
		Set_Position(_vec3(522.f, 656.5f, 0.0f));
		m_pTexture1->SetUp_OnShader(pEffect, "g_BaseTexture", 0);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);
		m_pBuffer->Render_Buffer(m_matTransform);
		pEffect->EndPass();
		pEffect->End();
	}

	if (true == m_bExit)
	{
		Set_Scale(_vec3(256.f, 256.f, 1.f));
		Set_Position(_vec3( 782.f, 656.5f, 0.0f));
		m_pTexture1->SetUp_OnShader(pEffect, "g_BaseTexture", 1);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);
		m_pBuffer->Render_Buffer(m_matTransform);
		pEffect->EndPass();
		pEffect->End();
	}


	Safe_Release(pEffect);

}

HRESULT CBack_Menu::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture
	m_pTexture = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Menu");
	if (nullptr == m_pTexture)
		return E_FAIL;
	m_pTexture->AddRef();

	m_pTexture1 = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Menu2");
	if (nullptr == m_pTexture1)
		return E_FAIL;
	m_pTexture1->AddRef();

	// For.Shader
	m_pShader = (CShader*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Shader_Default");
	if (nullptr == m_pShader)
		return E_FAIL;
	m_pShader->AddRef();



	return NOERROR;
}

HRESULT CBack_Menu::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	// 셰이더 전역변수에 값을 전달한다.
	m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture",m_fFrame);

	Safe_Release(pEffect);

	return NOERROR;
}

CBack_Menu * CBack_Menu::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBack_Menu*		pInstance = new CBack_Menu(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CBack_Menu Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBack_Menu::Free()
{
	Safe_Release(m_pBuffer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pTexture1);
	Safe_Release(m_pShader);
	
	CGameObject::Free();
}
