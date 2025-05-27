#include "stdafx.h"
#include "..\Headers\Warning.h"
#include "Camera.h"
#include "Management.h"

CWarning::CWarning(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

HRESULT CWarning::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	if (FAILED(SetUp_Resource()))
		return E_FAIL;

	Set_Scale(_vec3(g_iBackCX, g_iBackCY, 1.f));
	Set_Position(_vec3(g_iBackCX >> 1, g_iBackCY >> 1, 0.f));

	CGameObject* pPlayer = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr == pPlayer)
		return E_FAIL;
	m_pPlayer = pPlayer;
	m_pPlayer->AddRef();

	return NOERROR;
}

_int CWarning::Update_GameObject(const _float& fTimeDelta)
{
	if (SCENE_CAVE == CManagement::GetInstance()->Get_Scene())
	{
		if (true == m_pPlayer->Get_Hurt())
		{
			m_pPlayer->Set_Hurt();
			m_fNum = 1.f;
		}

		if (m_fNum > 0.f)
			m_fNum -= fTimeDelta;
	}
	return _int();
}

_int CWarning::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (SCENE_CAVE == CManagement::GetInstance()->Get_Scene())
	{
		if (nullptr != m_pRender_Manager)
			m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_COVER, this);
	}
	return CGameObject::LastUpdate_GameObject(fTimeDelta);
}

void CWarning::Render_GameObject()
{
	if (nullptr == m_pBuffer ||
		nullptr == m_pTexture ||
		nullptr == m_pShader)
		return;

	LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return;
	pEffect->AddRef();

	m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture", 0);
	pEffect->SetFloat("m_fNum", m_fNum);
	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(2);
	m_pBuffer->Render_Buffer(m_matTransform);
	pEffect->EndPass();
	pEffect->End();
	
	Safe_Release(pEffect);
}

HRESULT CWarning::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture
	m_pTexture = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Warning");
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

HRESULT CWarning::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	// 셰이더 전역변수에 값을 전달한다.
	m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture",m_fFrame);

	Safe_Release(pEffect);

	return NOERROR;
}

_bool CWarning::isDraw(CGameObject * pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

_matrix CWarning::Render_TransForm(CGameObject * pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

CWarning * CWarning::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWarning*		pInstance = new CWarning(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CWarning Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWarning::Free()
{
	Safe_Release(m_pPlayer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pShader);
	
	CGameObject::Free();
}
