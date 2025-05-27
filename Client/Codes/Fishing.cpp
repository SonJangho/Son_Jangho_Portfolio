#include "stdafx.h"
#include "..\Headers\Fishing.h"
#include "Camera.h"
#include "Sound_Manager.h"

CFishing::CFishing(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

HRESULT CFishing::Ready_GameObject()
{
	CGameObject::Ready_GameObject();	

	if (FAILED(SetUp_Resource()))
		return E_FAIL;

	CGameObject* pPlayer = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr == pPlayer)
		return E_FAIL;

	m_pPlayer = pPlayer;
	m_pPlayer->AddRef();

	CGameObject* pTarget = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Tools",6);
	if (nullptr == pTarget)
		return E_FAIL;

	m_pTool = pTarget;
	m_pTool->AddRef();

	return NOERROR;
}

_int CFishing::Update_GameObject(const _float & fTimeDelta)
{
	// If the fishing tool is activated, start the fishing process
	if (m_pTool->Get_FishingOn())
	{
		if(m_bFishing != true)
			// Ready for "hit" state
			m_bHit = true;
	}

	// "Hit" short animation before fishing bar appears
	if (m_bHit == true)
	{
		m_fHitX -= 4.f * 4.f;
		m_fHitY -= 1.f * 4.f;
		if (m_fHitX == -512.f)
		{
			m_fHitX = 0.f;
			m_fHitY = 0.f;
			m_bHit = false;
			m_bFishing = true; // Now start the fishing minigame
		}
	}

	// Fishing minigame main loop
	if (true == m_bFishing)
	{
		// Green bar naturally falls over time
		if (m_fBar < 180.f)
		{
			if (false == CSound_Manager::Get_Instance()->SoundPlaying(32))
				CSound_Manager::Get_Instance()->SoundPlay(32, FALSE);
			m_fBar += 5.f;
		}
		// Press spacebar to make the Green bar rise
		if (m_fBar > -150.f)
			if (GetKeyState(VK_SPACE) & 0x8000)
			{
				if (false == CSound_Manager::Get_Instance()->SoundPlaying(33))
					CSound_Manager::Get_Instance()->SoundPlay(33, FALSE);
				m_fBar -= 10.f;
			}

		if (1 >= fTimeDelta)
			++m_fCount;

		// Fish moves randomly up and down using random numbers
		if (10 <= m_fCount)
		{
			m_fCount = 0.f;
			iRanNum = rand() % 2 + 1;
			iRanNumDown = rand() % 5 + 1;
			iRanNumUp = rand() % 5 + 1;
		}

		if (1 == iRanNum) // Move fish down
		{
			if (m_fFish < 180.f)
			{
				if (1 == iRanNumDown) { m_fFish += 1.f; }
				else if (2 == iRanNumDown) { m_fFish += 2.f; }
				else if (3 == iRanNumDown) { m_fFish += 3.f; }
				else if (4 == iRanNumDown) { m_fFish += 4.f; }
				else if (5 == iRanNumDown) { m_fFish += 5.f; }
			}
		}
		else if (2 == iRanNum) // Move fish up
		{
			if (m_fFish > -150.f)
			{
				if (1 == iRanNumUp) { m_fFish -= 1.f; }
				else if (2 == iRanNumUp) { m_fFish -= 2.f; }
				else if (3 == iRanNumUp) { m_fFish -= 3.f; }
				else if (4 == iRanNumUp) { m_fFish -= 4.f; }
				else if (5 == iRanNumUp) { m_fFish -= 5.f; }
			}
		}

		// Collision detection between Green bar and the fish
		RECT rcFish = Get_Rect(m_pPlayer->Get_X() - 150.f, m_pPlayer->Get_Y() + m_fFish, 57.f, 57.f);
		RECT rcCollision = { 0 };
		RECT rcCheck = Get_Rect(m_pPlayer->Get_X() - 150.f, m_pPlayer->Get_Y() + m_fBar, 28.5f, 73.5f);

		if (TRUE == _bool(IntersectRect(&rcCollision, &rcCheck, &rcFish)))
		{
			// Player is catching the fish
			if (142 > m_fCheck)
				m_fCheck += 0.4f; // Orange bar fills up
			else if (142 < m_fCheck)
			{
				// Fishing successful!
				// Reset state, play sound, add fish to inventory
				CSound_Manager::Get_Instance()->SoundPlay(11,FALSE);
				m_fCheck = 30.f; m_fBar = 180.f;
				m_fFish = 180.f; m_bFishing = false;
				m_fCount = 4; m_pTool->Set_FishingOff();
				m_bGetFish = true;
			}
		}
		else
		{
			m_fCheck -= 0.8f;
			// If Orange bar hits the bottom
			if (0 >= m_fCheck)
			{
				// Fishing failed, reset state
				CSound_Manager::Get_Instance()->SoundStop(12);
				m_fCheck = 30.f; m_fBar = 180.f; m_fFish = 180.f;
				m_bFishing = false; m_fCount = 4;
				m_pTool->Set_FishingOff();
			}
		}
	}

	if (m_bGetFish == true)
	{
		m_fCnt += 7.f * fTimeDelta * 0.5f;

		if (m_fCnt >= 6.f)
		{
			m_fCnt = 0.f;
			m_bGetFish = false;
			m_pPlayer->Set_FrontIdle();

			CGameObject* pInventory = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Inventory");
			
			if(0 == m_pPlayer->Get_FishNumber())
				pInventory->Add_Item(L"Bass",1,11);
			else if (1 == m_pPlayer->Get_FishNumber())
				pInventory->Add_Item(L"Salmon", 1, 12);
			else if (2 == m_pPlayer->Get_FishNumber())
				pInventory->Add_Item(L"Sunfish", 1, 13);
		}
	}
	return _int();
}

_int CFishing::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRender_Manager)
		return -1;

	if (FAILED(m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_UI, this)))
		return -1;

	return _int();
}

void CFishing::Render_GameObject()
{
	if (nullptr == m_pBuffer ||
		nullptr == m_pTexture ||
		nullptr == m_pShader)
		return;

	if (true == m_bHit)
	{
		CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");

		LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
		if (nullptr == pEffect)
			return;
		pEffect->AddRef();
	
		Set_Scale(_vec3(512.f + m_fHitX, 128.f + m_fHitY, 1.f));
		if (1 == m_pPlayer->Get_State())
			Set_Position(_vec3(m_pPlayer->Get_X(), m_pPlayer->Get_Y() - 200.f, 0.f));
		else if (2 == m_pPlayer->Get_State())
			Set_Position(_vec3(m_pPlayer->Get_X() + 280.f, m_pPlayer->Get_Y(), 0.f));
		else if (3 == m_pPlayer->Get_State())
			Set_Position(_vec3(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 200.f, 0.f));
		else if (4 == m_pPlayer->Get_State())
			Set_Position(_vec3(m_pPlayer->Get_X()- 280.f, m_pPlayer->Get_Y(), 0.f));

		m_pTexture[HIT]->SetUp_OnShader(pEffect, "g_BaseTexture", 0);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);
		TempMatrix = Render_TransForm(pCamera);
		m_pBuffer->Render_Buffer(TempMatrix);
		pEffect->EndPass();
		pEffect->End();

		Safe_Release(pEffect);
	}
	else if (true == m_bFishing)
	{
		CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");

		LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
		if (nullptr == pEffect)
			return;
		pEffect->AddRef();

		Set_Scale(_vec3(192.f, 768.f, 1.f));
		Set_Position(_vec3(m_pPlayer->Get_X() - 150.f, m_pPlayer->Get_Y(), 0.f));
		m_pTexture[BASE]->SetUp_OnShader(pEffect, "g_BaseTexture", 0);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);
		TempMatrix = Render_TransForm(pCamera);
		m_pBuffer->Render_Buffer(TempMatrix);
		pEffect->EndPass();
		pEffect->End();

		Set_Scale(_vec3(48.f, 128.f, 1.f));
		Set_Position(_vec3(m_pPlayer->Get_X() - 150.f, m_pPlayer->Get_Y() + m_fBar, 0.f));
		m_pTexture[CHECKPOINT]->SetUp_OnShader(pEffect, "g_BaseTexture", 0);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);
		TempMatrix = Render_TransForm(pCamera);
		m_pBuffer->Render_Buffer(TempMatrix);
		pEffect->EndPass();
		pEffect->End();

		Set_Scale(_vec3(64.f, 64.f, 1.f));
		Set_Position(_vec3(m_pPlayer->Get_X() - 150.f, m_pPlayer->Get_Y() + m_fFish, 0.f));
		m_pTexture[FISH]->SetUp_OnShader(pEffect, "g_BaseTexture", 0);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);
		TempMatrix = Render_TransForm(pCamera);
		m_pBuffer->Render_Buffer(TempMatrix);
		pEffect->EndPass();
		pEffect->End();

		for (size_t i = 0; i < m_fCheck; i++)
		{
			Set_Scale(_vec3(12.f, 3.f, 1.f));
			Set_Position(_vec3(m_pPlayer->Get_X() - 150.f + 36.f, m_pPlayer->Get_Y() + 227.1f - 3 * i, 0.f));
			m_pTexture[GAUGE]->SetUp_OnShader(pEffect, "g_BaseTexture", 0);
			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(0);
			TempMatrix = Render_TransForm(pCamera);
			m_pBuffer->Render_Buffer(TempMatrix);
			pEffect->EndPass();
			pEffect->End();
		}

		Safe_Release(pEffect);
	}
}
HRESULT CFishing::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture_FishingUI
	m_pTexture[BASE] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingUI_Base");
	if (nullptr == m_pTexture[BASE])
		return E_FAIL;
	m_pTexture[BASE]->AddRef();

	m_pTexture[FISH] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingUI_Fish");
	if (nullptr == m_pTexture[FISH])
		return E_FAIL;
	m_pTexture[FISH]->AddRef();

	m_pTexture[CHECKPOINT] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingUI_CheckPoint");
	if (nullptr == m_pTexture[CHECKPOINT])
		return E_FAIL;
	m_pTexture[CHECKPOINT]->AddRef();

	m_pTexture[HIT] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingUI_Hit");
	if (nullptr == m_pTexture[HIT])
		return E_FAIL;
	m_pTexture[HIT]->AddRef();

	m_pTexture[GAUGE] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingUI_Gauge");
	if (nullptr == m_pTexture[GAUGE])
		return E_FAIL;
	m_pTexture[GAUGE]->AddRef();

	// For.Shader
	m_pShader = (CShader*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Shader_Default");
	if (nullptr == m_pShader)
		return E_FAIL;
	m_pShader->AddRef();


	return NOERROR;
}

HRESULT CFishing::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return NOERROR;
}

HRESULT CFishing::Reset_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return NOERROR;
}

_matrix CFishing::Render_TransForm(CGameObject * pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

CGameObject * CFishing::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFishing*		pInstance = new CFishing(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CFishing Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFishing::Free(void)
{
	Safe_Release(m_pPlayer);
	Safe_Release(m_pTool);
	for (auto& pTexture : m_pTexture)
		Safe_Release(pTexture);
	Safe_Release(m_pShader);
	Safe_Release(m_pBuffer);

	CGameObject::Free();
}
