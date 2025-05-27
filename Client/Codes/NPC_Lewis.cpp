#include "stdafx.h"
#include "..\Headers\NPC_Lewis.h"
#include "Camera.h"
#include "Management.h"

CNPC_Lewis::CNPC_Lewis(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

HRESULT CNPC_Lewis::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	if (FAILED(SetUp_Resource()))
		return E_FAIL;

	Set_Scale(_vec3(64.f, 128.f, 1.f));
	Set_Position(_vec3(64*26.5, 64*10.5, 0.f));

	CGameObject* pPlayer = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr == pPlayer)
		return E_FAIL;
	m_pPlayer = pPlayer;
	m_pPlayer->AddRef();

	CGameObject* pUI = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_BasicUI");
	if (nullptr == pUI)
		return E_FAIL;
	m_pUI = pUI;
	m_pUI->AddRef();

	m_fNum = 0.f;

	m_fFrame = 0.f;

	return NOERROR;
}

_int CNPC_Lewis::Update_GameObject(const _float& fTimeDelta)
{
	m_YsortRect.bottom = m_matTransform._42 + m_matTransform._22*0.5f;

	if (90 + 9*3 <= m_pUI->Get_TimeDegree() && 90 + 9 * 12 >= m_pUI->Get_TimeDegree())
	{
		if (m_fNum < 1.f)
			m_fNum += 0.05f;

	/*	m_fFrame += fTimeDelta * 8;
		if (m_fFrame >= 20.f)
			m_fFrame = 12.f;*/
	}

	if (90 + 9 * 13 <= m_pUI->Get_TimeDegree())
	{
		if (m_fNum > 0.f)
			m_fNum -= 0.05f;
	}

	return _int();
}

_int CNPC_Lewis::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (SCENE_TOWN == CManagement::GetInstance()->Get_Scene())
	{
		if (false == m_pUI->Get_RainDay())
			if (nullptr != m_pRender_Manager)
				m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_YSORT, this);
	}
	return CGameObject::LastUpdate_GameObject(fTimeDelta);
}

void CNPC_Lewis::Render_GameObject()
{
	if (nullptr == m_pBuffer ||
		nullptr == m_pTexture ||
		nullptr == m_pShader)
		return;

	CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return;
	pEffect->AddRef();

	m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture", m_fFrame);
	pEffect->SetFloat("m_fNum", m_fNum);
	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(2);
	TempMatrix = Render_TransForm(pCamera);
	m_pBuffer->Render_Buffer(TempMatrix);
	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CNPC_Lewis::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture
	m_pTexture = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Lewis");
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

HRESULT CNPC_Lewis::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	// 셰이더 전역변수에 값을 전달한다.
	m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture",m_fFrame);

	Safe_Release(pEffect);

	return NOERROR;
}

_bool CNPC_Lewis::isDraw(CGameObject * pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

_matrix CNPC_Lewis::Render_TransForm(CGameObject * pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

CNPC_Lewis * CNPC_Lewis::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CNPC_Lewis*		pInstance = new CNPC_Lewis(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CNPC_Lewis Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNPC_Lewis::Free()
{
	Safe_Release(m_pPlayer);
	Safe_Release(m_pUI);
	Safe_Release(m_pTexture);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pShader);
	
	CGameObject::Free();
}
