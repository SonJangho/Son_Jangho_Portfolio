#include "stdafx.h"
#include "..\Headers\FishingRod.h"
#include "Camera.h"
#include "Sound_Manager.h"

CFishingRod::CFishingRod(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

HRESULT CFishingRod::Ready_GameObject()
{
	CGameObject::Ready_GameObject();	

	if (FAILED(SetUp_StateDesc()))
		return E_FAIL;

	if (FAILED(SetUp_Resource()))
		return E_FAIL;

	m_eCurrentState = STATE_OFF;

	CGameObject* pPlayer = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr == pPlayer)
		return E_FAIL;

	m_pPlayer = pPlayer;
	m_pPlayer->AddRef();

	CGameObject* pTarget = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_BasicUI");
	if (nullptr == pTarget)
		return E_FAIL;

	m_pUI = pTarget;
	m_pUI->AddRef();

	m_fX = 0.f;
	m_fY = 0.f;

	Set_Scale(_vec3(256.0f * 4.f, 256.0f * 4.f, 1.f));

	Set_Position(_vec3(m_pPlayer->Get_X(), m_pPlayer->Get_Y(), 0.f));

	return NOERROR;
}

_int CFishingRod::Update_GameObject(const _float & fTimeDelta)
{
	Animation(fTimeDelta);

	if (0 == wcscmp(m_pUI->Get_ItemOn().szName, L"FishingRod"))
		m_isToolOn = true;
	else
		m_isToolOn = false;

	if (true == m_isToolOn)
	{
		CGameObject* pTarget = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Fishing");
		if (true == pTarget->Get_FishingOn())
		{
			if (false == m_bFirst)
			{
				m_bFirst = true;
				m_fFrame = 0.f;
			}
			if (1 == m_pPlayer->Get_State())
			{
				m_isRight = true;
				m_eCurrentState = STATE_FISHINGBACK;
			}
			else if (2 == m_pPlayer->Get_State())
			{
				m_isRight = true;
				m_eCurrentState = STATE_FISHINGSIDE;
			}
			else if (3 == m_pPlayer->Get_State())
			{
				m_isRight = true;
				m_eCurrentState = STATE_FISHINGFRONT;
			}
			else if (4 == m_pPlayer->Get_State())
			{
				m_isRight = false;
				m_eCurrentState = STATE_FISHINGSIDE;
			}
		}
		else
		{
			m_bFirst = false;
			if (1 == m_pPlayer->Get_State())
			{
				m_isRight = true;
				m_eCurrentState = STATE_BACK;
			}
			else if (2 == m_pPlayer->Get_State())
			{
				m_isRight = true;
				m_eCurrentState = STATE_SIDE;
			}
			else if (3 == m_pPlayer->Get_State())
			{
				m_isRight = true;
				m_eCurrentState = STATE_FRONT;
			}
			else if (4 == m_pPlayer->Get_State())
			{
				m_isRight = false;
				m_eCurrentState = STATE_SIDE;
			}
		}
	}
	if (false == m_isRight)
		Set_RotationY(D3DXToRadian(180.f));
	else
		Set_RotationY(D3DXToRadian(0.f));

	return _int();
}

_int CFishingRod::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRender_Manager)
		return -1;

	if (STATE_BACK == m_eCurrentState || STATE_FISHINGBACK == m_eCurrentState)
	{
		if (FAILED(m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_TWO, this)))
			return -1;
	}
	else
	{
		if (FAILED(m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_COVER, this)))
			return -1;
	}

	return _int();
}

void CFishingRod::Render_GameObject()
{
	if (nullptr == m_pTexture ||
		nullptr == m_pBuffer)
		return;

	if (STATE_OFF != m_eCurrentState)
	{
		SetUp_RenderState();

		const CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");

		Set_Position(_vec3(m_pPlayer->Get_X(), m_pPlayer->Get_Y(), 0.f));

		POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
		_matrix TempMatrix;
		TempMatrix = m_matTransform;

		memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

		m_pTexture[m_eCurrentState]->SetUp_OnGraphicDev((_uint)m_fFrame);
		m_pBuffer->Render_Buffer(TempMatrix);

		Reset_RenderState();
	}
}

HRESULT CFishingRod::SetUp_StateDesc()
{
	m_pStateDesc = new STATEDESC[STATE_END];
	ZeroMemory(m_pStateDesc, sizeof(STATEDESC) * STATE_END);

	m_pStateDesc[STATE_BACK].eState = STATE_BACK;
	m_pStateDesc[STATE_BACK].iNumTexture = 7;

	m_pStateDesc[STATE_SIDE].eState = STATE_SIDE;
	m_pStateDesc[STATE_SIDE].iNumTexture = 7;

	m_pStateDesc[STATE_FRONT].eState = STATE_FRONT;
	m_pStateDesc[STATE_FRONT].iNumTexture = 7;

	m_pStateDesc[STATE_FISHINGBACK].eState = STATE_FISHINGBACK;
	m_pStateDesc[STATE_FISHINGBACK].iNumTexture = 2;

	m_pStateDesc[STATE_FISHINGFRONT].eState = STATE_FISHINGFRONT;
	m_pStateDesc[STATE_FISHINGFRONT].iNumTexture = 2;

	m_pStateDesc[STATE_FISHINGSIDE].eState = STATE_FISHINGSIDE;
	m_pStateDesc[STATE_FISHINGSIDE].iNumTexture = 2;

	return NOERROR;
}

HRESULT CFishingRod::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture_FishingRod
	m_pTexture[STATE_FRONT] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingRodFront");
	if (nullptr == m_pTexture[STATE_FRONT])
		return E_FAIL;
	m_pTexture[STATE_FRONT]->AddRef();

	m_pTexture[STATE_BACK] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingRodBack");
	if (nullptr == m_pTexture[STATE_BACK])
		return E_FAIL;
	m_pTexture[STATE_BACK]->AddRef();

	m_pTexture[STATE_SIDE] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingRodSide");
	if (nullptr == m_pTexture[STATE_SIDE])
		return E_FAIL;
	m_pTexture[STATE_SIDE]->AddRef();

	m_pTexture[STATE_FISHINGFRONT] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingFront");
	if (nullptr == m_pTexture[STATE_FISHINGFRONT])
		return E_FAIL;
	m_pTexture[STATE_FISHINGFRONT]->AddRef();

	m_pTexture[STATE_FISHINGBACK] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingBack");
	if (nullptr == m_pTexture[STATE_FISHINGBACK])
		return E_FAIL;
	m_pTexture[STATE_FISHINGBACK]->AddRef();

	m_pTexture[STATE_FISHINGSIDE] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingSide");
	if (nullptr == m_pTexture[STATE_FISHINGSIDE])
		return E_FAIL;
	m_pTexture[STATE_FISHINGSIDE]->AddRef();

	return NOERROR;
}

HRESULT CFishingRod::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return NOERROR;
}

HRESULT CFishingRod::Reset_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return NOERROR;
}

HRESULT CFishingRod::Animation(_float fTimeDelta)
{
	if (m_eCurrentState == STATE_BACK || m_eCurrentState == STATE_SIDE || m_eCurrentState == STATE_FRONT)
	{
		if (m_bSlow == false)
			m_fFrame += m_pStateDesc[m_eCurrentState].iNumTexture * fTimeDelta * 3.f;
		else
			m_fFrame += m_pStateDesc[m_eCurrentState].iNumTexture * fTimeDelta * 0.3f;
		if (m_fFrame >= m_pStateDesc[m_eCurrentState].iNumTexture)
		{
			m_fFrame -= 2;
			if (m_bSlow == false)
			{
				CSound_Manager::Get_Instance()->SoundPlay(34, FALSE);
				m_bSlow = true;
				m_iRanNumb = rand() % 3 + 4;
			}
			if (1 >= fTimeDelta)
				++m_iCnt;
		
			if (m_iRanNumb <= m_iCnt)
			{
				m_iCnt = 0;
				m_bFishingStart = true;
				CSound_Manager::Get_Instance()->SoundPlay(15, FALSE);
			}
		}

		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			m_iCnt = 0;
			m_fFrame = 0;
			m_eCurrentState = STATE_OFF;
			m_bSlow = false;
		}
	}
	else if (m_eCurrentState == STATE_FISHINGSIDE || m_eCurrentState == STATE_FISHINGFRONT || m_eCurrentState == STATE_FISHINGBACK)
	{
		m_fFrame += m_pStateDesc[m_eCurrentState].iNumTexture * fTimeDelta * 5.f;
		if (m_fFrame >= m_pStateDesc[m_eCurrentState].iNumTexture)
			m_fFrame -= 2;

		CGameObject* pTarget = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Fishing");
		if (false == pTarget->Get_FishingOn())
		{
			m_fFrame = 0;
			m_eCurrentState = STATE_OFF;
		}
	}
	return NOERROR;
}

CGameObject * CFishingRod::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFishingRod*		pInstance = new CFishingRod(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CFishingRod Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFishingRod::Free(void)
{
	Safe_Delete_Array(m_pStateDesc);

	Safe_Release(m_pPlayer);
	Safe_Release(m_pUI);
	for (auto& pTexture : m_pTexture)
		Safe_Release(pTexture);
	Safe_Release(m_pBuffer);

	CGameObject::Free();
}
