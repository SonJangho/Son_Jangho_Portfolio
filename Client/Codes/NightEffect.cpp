#include "stdafx.h"
#include "..\Headers\NightEffect.h"
#include "Camera.h"
#include "Management.h"
#include "Sound_Manager.h"

CNightEffect::CNightEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

HRESULT CNightEffect::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	if (FAILED(SetUp_Resource()))
		return E_FAIL;

	Set_Scale(_vec3(g_iBackCX, g_iBackCY, 1.f));
	Set_Position(_vec3(g_iBackCX >> 1, g_iBackCY >> 1, 0.f));

	CGameObject* pUI = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_BasicUI");
	if (nullptr == pUI)
		return E_FAIL;
	m_pUI = pUI;
	m_pUI->AddRef();

	m_fTimeDegree = 90.f;

	return NOERROR;
}

_int CNightEffect::Update_GameObject(const _float& fTimeDelta)
{
	// Increase alpha value (make screen darker) as night progresses
	if (200 <= m_pUI->Get_TimeDegree())
	{
		if (m_fTimeDegree == m_pUI->Get_TimeDegree()) // 210~240
		{
			m_fTimeDegree += 1;
			if (m_fTimeDegree == 240)
			{
				m_fTimeDegree = 90;
			}
			m_fNum += 0.021f;

			if (0.8f <= m_fNum)
				m_fNum = 0.8f;
		}
	}

	// Decrease alpha value (make screen brighter) as day comes
	if(120 >= m_pUI->Get_TimeDegree() && 90 <= m_pUI->Get_TimeDegree())
	if (m_fTimeDegree == m_pUI->Get_TimeDegree())
	{
		m_fTimeDegree += 1;
		if (m_fTimeDegree == 120)
			m_fTimeDegree = 200;
		m_fNum -= 0.028f;

		if (0.f >= m_fNum)
			m_fNum = 0.f;
	}

	return _int();
}

_int CNightEffect::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (SCENE_FARM == CManagement::GetInstance()->Get_Scene() || SCENE_TOWN == CManagement::GetInstance()->Get_Scene() || SCENE_ROAD == CManagement::GetInstance()->Get_Scene())
	{
		if (nullptr != m_pRender_Manager)
			m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_COVER, this);
	}
	return CGameObject::LastUpdate_GameObject(fTimeDelta);
}

void CNightEffect::Render_GameObject()
{
	if (nullptr == m_pBuffer ||
		nullptr == m_pTexture ||
		nullptr == m_pShader)
		return;

	LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return;
	pEffect->AddRef();

	// Set the scale and position for full screen overlay
	Set_Scale(_vec3(g_iBackCX, g_iBackCY, 1.f));
	Set_Position(_vec3(g_iBackCX >> 1, g_iBackCY >> 1, 0.f));
	// Pass the current alpha value to shader
	m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture", 0);
	pEffect->SetFloat("m_fNum", m_fNum);
	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(2); // Use PS_DELETE in shader (alpha blending)
	m_pBuffer->Render_Buffer(m_matTransform);
	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CNightEffect::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture
	m_pTexture = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_NightEffect");
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

HRESULT CNightEffect::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	// 셰이더 전역변수에 값을 전달한다.
	m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture",m_fFrame);

	Safe_Release(pEffect);

	return NOERROR;
}

_bool CNightEffect::isDraw(CGameObject * pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

_matrix CNightEffect::Render_TransForm(CGameObject * pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

CNightEffect * CNightEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CNightEffect*		pInstance = new CNightEffect(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CNightEffect Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNightEffect::Free()
{
	Safe_Release(m_pUI);
	Safe_Release(m_pTexture);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pShader);
	
	CGameObject::Free();
}
