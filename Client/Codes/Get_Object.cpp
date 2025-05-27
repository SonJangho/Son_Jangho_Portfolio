#include "stdafx.h"
#include "..\Headers\Get_Object.h"
#include "Camera.h"
#include "Management.h"

CGet_Object::CGet_Object(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

HRESULT CGet_Object::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	if (FAILED(SetUp_Resource()))
		return E_FAIL;
	
	Set_Scale(_vec3(256.f, 128.f, 1.0f));
	Set_Position(_vec3(120.f, 600.f, 0.f));

	m_fNum = 1.f;

	return NOERROR;
}

_int CGet_Object::Update_GameObject(const _float& fTimeDelta)
{
	if (true == m_bObjectOn)
	{
		m_fAcc += fTimeDelta;
		if(m_fAcc>=1.5f)
			m_fNum -= 0.05f;
		
		if (m_fNum <= 0.f)
		{
			m_fAcc = 0.f;
			m_fNum = 1.f;
			m_bObjectOn = false;
		}
	}

	return _int();
}

_int CGet_Object::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (true == m_bObjectOn)
	{
		if (nullptr != m_pRender_Manager)
			m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_UI, this);
	}
	
	return CGameObject::LastUpdate_GameObject(fTimeDelta);
}

void CGet_Object::Render_GameObject()
{
	if (nullptr == m_pBuffer ||
		nullptr == m_pTexture ||
		nullptr == m_pShader)
		return;

	LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return;
	pEffect->AddRef();
	if (m_State == 6)
	{
		Set_Scale(_vec3(512.f, 128.f, 1.0f));
		Set_Position(_vec3(120.f + 32.f, 600.f, 0.f));
	}
	else
	{
		Set_Scale(_vec3(256.f, 128.f, 1.0f));
		Set_Position(_vec3(120.f, 600.f, 0.f));
	}

	m_pTexture[m_State]->SetUp_OnShader(pEffect, "g_BaseTexture", 0);
	
	pEffect->SetFloat("m_fNum", m_fNum);

	pEffect->Begin(nullptr, 0);

	pEffect->BeginPass(2);

	m_pBuffer->Render_Buffer(m_matTransform);

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CGet_Object::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture
	m_pTexture[BRANCH] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Get_Branch");
	if (nullptr == m_pTexture[BRANCH])
		return E_FAIL;
	m_pTexture[BRANCH]->AddRef();

	m_pTexture[STONE] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Get_Stone");
	if (nullptr == m_pTexture[STONE])
		return E_FAIL;
	m_pTexture[STONE]->AddRef();

	m_pTexture[WEED] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Get_Weed");
	if (nullptr == m_pTexture[WEED])
		return E_FAIL;
	m_pTexture[WEED]->AddRef();

	m_pTexture[COPPER] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Get_Copper");
	if (nullptr == m_pTexture[COPPER])
		return E_FAIL;
	m_pTexture[COPPER]->AddRef();

	m_pTexture[SLIME] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Get_Slime");
	if (nullptr == m_pTexture[SLIME])
		return E_FAIL;
	m_pTexture[SLIME]->AddRef();

	// For.Shader
	m_pShader = (CShader*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Shader_Default");
	if (nullptr == m_pShader)
		return E_FAIL;
	m_pShader->AddRef();

	return NOERROR;
}

HRESULT CGet_Object::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	// 셰이더 전역변수에 값을 전달한다.
	m_pTexture[m_State]->SetUp_OnShader(pEffect, "g_BaseTexture",m_fFrame);

	Safe_Release(pEffect);

	return NOERROR;
}

_bool CGet_Object::isDraw(CGameObject * pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

_matrix CGet_Object::Render_TransForm(CGameObject * pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

CGet_Object * CGet_Object::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGet_Object*		pInstance = new CGet_Object(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CGet_Object Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGet_Object::Free()
{
	for (auto& pTexture : m_pTexture)
		Safe_Release(pTexture);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pShader);
	
	CGameObject::Free();
}
