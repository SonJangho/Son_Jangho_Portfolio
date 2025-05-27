#include "stdafx.h"
#include "..\Headers\Back_Logo.h"
#include "Render_Manager.h"
#include "Resource_Manager.h"

CBack_Logo::CBack_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

HRESULT CBack_Logo::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	if (FAILED(SetUp_Resource()))
		return E_FAIL;

	Set_Scale(_vec3(2048.f , 1024.f, 1.f));
	Set_Position(_vec3(g_iBackCX * 0.5f, g_iBackCY * 0.5f, 0.0f));

	return NOERROR;
}

_int CBack_Logo::Update_GameObject(const _float& fTimeDelta)
{
	m_fFrame += 10.0f * fTimeDelta;

	if (m_fFrame >= 2.0f)
		m_fFrame = 0.f;
	
	return _int();
}

_int CBack_Logo::LastUpdate_GameObject(const _float& fTimeDelta)
{
	
	if (nullptr != m_pRender_Manager)
		m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_PRIORITY, this);

	return CGameObject::LastUpdate_GameObject(fTimeDelta);
}

void CBack_Logo::Render_GameObject()
{
	if (nullptr == m_pBuffer ||
		nullptr == m_pTexture ||
		nullptr == m_pShader)
		return;

	LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return;
	pEffect->AddRef();


	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	// 특정 셰이더파일을 이용해서 그립니다. 
	//_uint		iNumPasses = 0;
	//pEffect->Begin(&iNumPasses, 0);
	pEffect->Begin(nullptr, 1);

	// 특정 패스를 이용합니다. 
	pEffect->BeginPass(0);

	// 자원을 그립니다. 
	m_pBuffer->Render_Buffer(m_matTransform);

	// 패스를 종료합니다. 
	pEffect->EndPass();

	// 셰이더파일을 이용한 렌더링을 종료합니다. 
	pEffect->End();

	Safe_Release(pEffect);

}

HRESULT CBack_Logo::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture
	m_pTexture = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_LOGO, L"Resource_Texture_Logo");
	if (nullptr == m_pTexture)
		return E_FAIL;
	m_pTexture->AddRef();

	// For.Shader
	m_pShader = (CShader*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Shader_Default");
	if (nullptr == m_pShader)
		return E_FAIL;
	m_pShader->AddRef();



	return NOERROR;
}

HRESULT CBack_Logo::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	// 셰이더 전역변수에 값을 전달한다.
	m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture",m_fFrame);

	Safe_Release(pEffect);

	return NOERROR;
}

CBack_Logo * CBack_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBack_Logo*		pInstance = new CBack_Logo(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBack_Logo::Free()
{
	Safe_Release(m_pBuffer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pShader);
	
	CGameObject::Free();
}
