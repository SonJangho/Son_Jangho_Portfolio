#include "stdafx.h"
#include "..\Headers\ButterflyEffect.h"
#include "Camera.h"
#include "Management.h"

CButterflyEffect::CButterflyEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

HRESULT CButterflyEffect::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	if (FAILED(SetUp_Resource()))
		return E_FAIL;

	m_fButterflyX = rand() % (50*64);
	m_fButterflyY = rand() % (30*64);
	m_fFrame = rand()%3;

	Set_Scale(_vec3(64.f, 64.f, 1.f));
	Set_Position(_vec3(m_fButterflyX, m_fButterflyY, 0.f));

	CGameObject* pUI = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_BasicUI");
	if (nullptr == pUI)
		return E_FAIL;
	m_pUI = pUI;
	m_pUI->AddRef();

	m_RandomFrame = rand() % 4;

	return NOERROR;
}

_int CButterflyEffect::Update_GameObject(const _float& fTimeDelta)
{
	if (SCENE_FARM == CManagement::GetInstance()->Get_Scene() || SCENE_TOWN == CManagement::GetInstance()->Get_Scene() || SCENE_ROAD == CManagement::GetInstance()->Get_Scene())
	{
		if (false == m_pUI->Get_RainDay())
		{
			m_matTransform._41 = Linear(m_matTransform._41, m_fButterflyX, 0.1f);

			m_fFrame += 7.f*fTimeDelta;

			if (m_fFrame >= 3.f)
			{	
				m_fFrame = 0.f;
				m_bUp = true;
				if (false == m_bOnce)
				{
					m_bOnce = true;
					_int RanNumb = rand() % 6;
					if(0 == RanNumb)
						m_fButterflyY = m_matTransform._42 - 10;
					else if(1 == RanNumb)
						m_fButterflyY = m_matTransform._42 + 5;
					else if (2 == RanNumb)
						m_fButterflyY = m_matTransform._42 - 5;
					else if (3 == RanNumb)
						m_fButterflyY = m_matTransform._42 + 10;
					else if (4 == RanNumb)
						m_fButterflyY = m_matTransform._42 + 20;
					else if (5 == RanNumb)
						m_fButterflyY = m_matTransform._42 - 20;


					RanNumb = rand() % 6;

					if (0 == RanNumb)
						m_fButterflyX = m_matTransform._41 - 7;
					else if (1 == RanNumb)
						m_fButterflyX = m_matTransform._41 + 5;
					else if (2 == RanNumb)
						m_fButterflyX = m_matTransform._41 - 5;
					else if (3 == RanNumb)
						m_fButterflyX = m_matTransform._41 + 7;
					else if (4 == RanNumb)
						m_fButterflyX = m_matTransform._41 + 2;
					else if (5 == RanNumb)
						m_fButterflyX = m_matTransform._41 - 2;

				}
			}	

			if (true == m_bUp)
			{
				m_matTransform._42 = Linear(m_matTransform._42, m_fButterflyY, 0.1f);
				++m_iCount;
				if (10 == m_iCount)
				{
					m_iCount = 0;
					m_bOnce = false;
				}
			}
		}
	}

	return _int();
}

_int CButterflyEffect::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (SCENE_FARM == CManagement::GetInstance()->Get_Scene() || SCENE_TOWN == CManagement::GetInstance()->Get_Scene() || SCENE_ROAD == CManagement::GetInstance()->Get_Scene())
	{
		if (false == m_pUI->Get_RainDay())
		{
			if (nullptr != m_pRender_Manager)
				m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_COVER, this);
		}
	}
	return CGameObject::LastUpdate_GameObject(fTimeDelta);
}

void CButterflyEffect::Render_GameObject()
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
		if (m_fFrame >= 3.f)
			m_fFrame = 0.f;
		m_pButterfly[m_RandomFrame]->SetUp_OnShader(pEffect, "g_BaseTexture", (_uint)m_fFrame);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);
		TempMatrix = Render_TransForm(pCamera);
		m_pBuffer->Render_Buffer(TempMatrix);
		pEffect->EndPass();
		pEffect->End();
	}

	Safe_Release(pEffect);
}

HRESULT CButterflyEffect::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture

	m_pButterfly[0] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Butterfly0");
	if (nullptr == m_pButterfly[0])
		return E_FAIL;
	m_pButterfly[0]->AddRef();

	m_pButterfly[1] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Butterfly1");
	if (nullptr == m_pButterfly[1])
		return E_FAIL;
	m_pButterfly[1]->AddRef();

	m_pButterfly[2] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Butterfly2");
	if (nullptr == m_pButterfly[2])
		return E_FAIL;
	m_pButterfly[2]->AddRef();

	m_pButterfly[3] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Butterfly3");
	if (nullptr == m_pButterfly[3])
		return E_FAIL;
	m_pButterfly[3]->AddRef();


	// For.Shader
	m_pShader = (CShader*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Shader_Default");
	if (nullptr == m_pShader)
		return E_FAIL;
	m_pShader->AddRef();

	return NOERROR;
}

HRESULT CButterflyEffect::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	return NOERROR;
}

_bool CButterflyEffect::isDraw(CGameObject * pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

_matrix CButterflyEffect::Render_TransForm(CGameObject * pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

CButterflyEffect * CButterflyEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CButterflyEffect*		pInstance = new CButterflyEffect(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CButterflyEffect Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CButterflyEffect::Free()
{
	Safe_Release(m_pUI);

	for (auto& pTexture : m_pButterfly)
		Safe_Release(pTexture);

	Safe_Release(m_pBuffer);
	Safe_Release(m_pShader);
	
	CGameObject::Free();
}
