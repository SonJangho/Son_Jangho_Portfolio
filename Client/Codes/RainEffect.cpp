#include "stdafx.h"
#include "..\Headers\RainEffect.h"
#include "Camera.h"
#include "Management.h"

CRainEffect::CRainEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

HRESULT CRainEffect::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	if (FAILED(SetUp_Resource()))
		return E_FAIL;

	m_fRainX = rand() % (50*64);
	m_fRainY = rand() % (30*64);
	m_fFrame = rand() % 4;

	Set_Scale(_vec3(64.f, 64.f, 1.f));
	Set_Position(_vec3(m_fRainX, m_fRainY, 0.f));

	CGameObject* pUI = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_BasicUI");
	if (nullptr == pUI)
		return E_FAIL;
	m_pUI = pUI;
	m_pUI->AddRef();

	return NOERROR;
}

_int CRainEffect::Update_GameObject(const _float& fTimeDelta)
{
	if (SCENE_FARM == CManagement::GetInstance()->Get_Scene() || SCENE_TOWN == CManagement::GetInstance()->Get_Scene() || SCENE_ROAD == CManagement::GetInstance()->Get_Scene())
	{
		if (true == m_pUI->Get_RainDay())
		{
			if (m_fFrame == 0)
			{
				m_matTransform._41 -= 4.f;
				m_matTransform._42 += 8.f;

				m_fAcc += fTimeDelta;
				if (m_fAcc >= 0.5f)
				{
					m_fAcc = 0.f;
					m_fFrame = 1.f;
				}
			}
			else
			{
				m_fFrame += 12.f*fTimeDelta;
				if (m_fFrame >= 4)
				{
					m_matTransform._41 = rand() % (50 * 64);
					m_matTransform._42 = rand() % (30 * 64);
					m_fFrame = 0;
				}
			}
		}
	}

	return _int();
}

_int CRainEffect::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (SCENE_FARM == CManagement::GetInstance()->Get_Scene() || SCENE_TOWN == CManagement::GetInstance()->Get_Scene() || SCENE_ROAD == CManagement::GetInstance()->Get_Scene())
	{
		if (true == m_pUI->Get_RainDay())
		{
			if (nullptr != m_pRender_Manager)
				m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_COVER, this);
		}
	}
	return CGameObject::LastUpdate_GameObject(fTimeDelta);
}

void CRainEffect::Render_GameObject()
{
	if (nullptr == m_pBuffer ||
		nullptr == m_pShader)
		return;

	CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");
	
	LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return;
	pEffect->AddRef();

	if (true == isDraw(pCamera))
	{
		m_pRain->SetUp_OnShader(pEffect, "g_BaseTexture", m_fFrame);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);
		TempMatrix = Render_TransForm(pCamera);
		m_pBuffer->Render_Buffer(TempMatrix);
		pEffect->EndPass();
		pEffect->End();
	}

	Safe_Release(pEffect);
}

HRESULT CRainEffect::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture

	m_pRain = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Rain");
	if (nullptr == m_pRain)
		return E_FAIL;
	m_pRain->AddRef();

	m_pRainUI = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_RainUI");
	if (nullptr == m_pRainUI)
		return E_FAIL;
	m_pRainUI->AddRef();

	// For.Shader
	m_pShader = (CShader*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Shader_Default");
	if (nullptr == m_pShader)
		return E_FAIL;
	m_pShader->AddRef();

	return NOERROR;
}

HRESULT CRainEffect::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	return NOERROR;
}

_bool CRainEffect::isDraw(CGameObject * pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

_matrix CRainEffect::Render_TransForm(CGameObject * pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

CRainEffect * CRainEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRainEffect*		pInstance = new CRainEffect(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CRainEffect Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRainEffect::Free()
{
	Safe_Release(m_pUI);
	Safe_Release(m_pRain);
	Safe_Release(m_pRainUI);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pShader);
	
	CGameObject::Free();
}
