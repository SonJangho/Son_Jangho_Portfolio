#include "stdafx.h"
#include "..\Headers\GreenSlime.h"
#include "Camera.h"
#include "Management.h"
#include "Sound_Manager.h"

CGreenSlime::CGreenSlime(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

HRESULT CGreenSlime::Ready_GameObject(_float fX, _float fY)
{
	CGameObject::Ready_GameObject();

	if (FAILED(SetUp_Resource()))
		return E_FAIL;

	Set_Scale(_vec3(64.f,128.f,1.f));
	Set_Position(_vec3(fX, fY, 0.f));

	CGameObject* pPlayer = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr == pPlayer)
		return E_FAIL;
	m_pPlayer = pPlayer;
	m_pPlayer->AddRef();

	CGameObject* pSword = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Tools", 4);
	if (nullptr == pSword)
		return E_FAIL;
	m_pSword = pSword;
	m_pSword->AddRef();

	m_fSpeedPerSec = 100.f;

	m_State = LIVE;
	m_fNum = 1.f;
	m_fMarkNum = 0.f;
	m_iHp = 50;

	return NOERROR;
}

_int CGreenSlime::Update_GameObject(const _float& fTimeDelta)
{
	if (false == m_bObjectOff)
	{
		m_YsortRect.bottom = m_matTransform._42 + m_matTransform._22 * 0.5f;

		m_fAcc += fTimeDelta;
		m_fIdleAcc += fTimeDelta;

		// Random Idle Movement (when not attacking or moving toward player)
		if (m_bAttackOn == false && m_bMoveOn == false && m_bDeadOn == false)
		{
			if (m_fIdleAcc >= m_fRanAcc)    // After random idle time
			{
				m_fRanAcc = rand() % 2 + 1; // Set next idle duration (1~2sec)
				m_fIdleAcc = 0.f;
				RanNum = rand() % 7;        // Randomly pick a direction (0~6)
			}
			if		(0 == RanNum) { m_matTransform._41 += 0.5f; } // Move right
			else if (1 == RanNum) { m_matTransform._41 -= 0.5f; } // Move left
			else if (2 == RanNum) {} // No moving
			else if (3 == RanNum) { m_matTransform._42 += 0.5f; } // Move down
			else if (4 == RanNum) { m_matTransform._42 -= 0.5f; } // Move up
			else if (5 == RanNum) {} // No moving
			else if (6 == RanNum) {} // No moving
		}

		for (size_t i = 0; i < 7; i++)
		{
			CGameObject* pTarget = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Monster", i);
			if (this != pTarget && false == m_bDeadOn)
				Monster_Collision(pTarget);
		}

		// Calculate distance to player
		_vec3		vTargetPos = _vec3(m_pPlayer->Get_X(), m_pPlayer->Get_Y(), 0.f);
		if (m_bDeadOn == false)
			Player_Collision();

		// Move toward player if within detection range
		Move_ToTargetMonster(vTargetPos, fTimeDelta);

		if (true == m_pSword->Get_ToolOn())
		{
			if (m_pPlayer->Get_State() && 3 <= m_pPlayer->Get_Frame() && m_bFirst == false)
			{
				m_bFirst = true;
				if (1 == m_pPlayer->Get_State())
				{
					m_iX = _int(m_pPlayer->Get_X() + 32) * 0.015625;
					m_iY = _int(m_pPlayer->Get_Y()) * 0.015625;
				}
				else if (2 == m_pPlayer->Get_State())
				{
					m_iX = _int(m_pPlayer->Get_X() + 64 + 32) * 0.015625;
					m_iY = _int(m_pPlayer->Get_Y() + 64) * 0.015625;
				}
				else if (3 == m_pPlayer->Get_State())
				{
					m_iX = _int(m_pPlayer->Get_X() + 32) * 0.015625;
					m_iY = _int(m_pPlayer->Get_Y() + 64 + 64) * 0.015625;
				}
				else if (4 == m_pPlayer->Get_State())
				{
					m_iX = _int(m_pPlayer->Get_X() - 32) * 0.015625;
					m_iY = _int(m_pPlayer->Get_Y() + 64) * 0.015625;
				}

				RECT rcTmp = m_pPlayer->Get_Rect(m_iX*64.f, m_iY*64.f, 64.f, 64.f);

				RECT rcCollision = { 0 };

				RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42 + 32.f, 64.f, 64.f);

				if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
				{
					CSound_Manager::Get_Instance()->SoundPlay(21, FALSE);
					m_bThrust = true;
					m_fThrustAcc = 0.f;
					m_iHp -= rand() % 3 + 9;
					m_iPassNum = 4;
					m_fHitAcc = 0.f;
					m_bHit = true;
					if (m_iHp <= 0)
					{
						CSound_Manager::Get_Instance()->SoundPlay(23, FALSE);
						m_fFrame = 0.f;
						m_bDeadOn = true;
						m_bCreateObject = true;
						m_fObjectNumb = 7.f;
					}
				}
			}
			else if (3 > m_pPlayer->Get_Frame() && m_bFirst == true)
				m_bFirst = false;
		}

		if (true == m_bDeadOn)
		{
			m_fNum -= 0.05f;
			if (m_fNum <= 0.f)
			{
				m_fNum = 0.f;
				m_bObjectOff = true;
			}
		}

		m_fHitAcc += fTimeDelta;
		if (true == m_bHit &&m_fHitAcc >= 0.05f)
		{
			m_iPassNum = 3;
			if (m_fHitAcc >= 0.1f)
			{
				m_iPassNum = 2;
				m_bHit = false;
			}
		}

		if (true == m_bThrust)
		{
			m_fThrustAcc += fTimeDelta;
			if (m_fThrustAcc <= 0.04f)
				Faraway_ToTarget(vTargetPos, fTimeDelta);
			else
				m_bThrust = false;
		}
	}

	return _int();
}

_int CGreenSlime::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (nullptr != m_pRender_Manager)
		if (false == m_bObjectOff)
			m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_YSORT, this);
		
	return CGameObject::LastUpdate_GameObject(fTimeDelta);
}

void CGreenSlime::Render_GameObject()
{
	if (nullptr == m_pBuffer ||
		nullptr == m_pTexture ||
		nullptr == m_pShader)
		return;

	LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return;
	pEffect->AddRef();

	CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");

	if (false == m_bDeadOn)
	{
		Set_Scale(_vec3(64.f, 128.f, 1.f));
		m_pTexture[m_State]->SetUp_OnShader(pEffect, "g_BaseTexture", m_fFrame);
		pEffect->SetFloat("m_fNum", m_fNum);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(m_iPassNum);
		TempMatrix = Render_TransForm(pCamera);
		m_pBuffer->Render_Buffer(TempMatrix);
		pEffect->EndPass();
		pEffect->End();

		if (true == m_bMarkOn)
		{
			m_pTexture[MARK]->SetUp_OnShader(pEffect, "g_BaseTexture", m_fMarkFrame);
			pEffect->SetFloat("m_fNum", m_fMarkNum);
			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(2);
			TempMatrix = Render_TransForm(pCamera);
			m_pBuffer->Render_Buffer(TempMatrix);
			pEffect->EndPass();
			pEffect->End();
		}
	}
	else if (true == m_bDeadOn)
	{
		Set_Scale(_vec3(160.f, 160.f, 1.f));
		m_pTexture[DEAD]->SetUp_OnShader(pEffect, "g_BaseTexture", m_fFrame);
		pEffect->SetFloat("m_fNum", m_fNum);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(2);
		TempMatrix = Render_TransForm(pCamera);
		m_pBuffer->Render_Buffer(TempMatrix);
		pEffect->EndPass();
		pEffect->End();
	}
	
	Safe_Release(pEffect);
}

HRESULT CGreenSlime::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture
	m_pTexture[LIVE] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_GreenSlime");
	if (nullptr == m_pTexture[LIVE])
		return E_FAIL;
	m_pTexture[LIVE]->AddRef();

	m_pTexture[DEAD] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_GreenSlimeDead");
	if (nullptr == m_pTexture[DEAD])
		return E_FAIL;
	m_pTexture[DEAD]->AddRef();

	m_pTexture[MARK] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Mark");
	if (nullptr == m_pTexture[MARK])
		return E_FAIL;
	m_pTexture[MARK]->AddRef();

	// For.Shader
	m_pShader = (CShader*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Shader_Default");
	if (nullptr == m_pShader)
		return E_FAIL;
	m_pShader->AddRef();

	return NOERROR;
}

HRESULT CGreenSlime::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	// 셰이더 전역변수에 값을 전달한다.
	m_pTexture[m_State]->SetUp_OnShader(pEffect, "g_BaseTexture",m_fFrame);

	Safe_Release(pEffect);

	return NOERROR;
}

_bool CGreenSlime::isDraw(CGameObject * pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

_matrix CGreenSlime::Render_TransForm(CGameObject * pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

void CGreenSlime::Player_Collision()
{
	RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57, 50, 32);
	RECT rcCollision = { 0 };
	if (true == m_bCollisionOff)
	{
		m_fSizeX = 20.f;
		m_fSizeY = 20.f;
	}
	else
	{
		m_fSizeX = 64.f;
		m_fSizeY = 64.f;
	}
	RECT rcObject = Get_Rect(m_matTransform._41,m_matTransform._42 + 32.f, m_fSizeX, m_fSizeY);

	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (m_pPlayer->Get_X() > m_matTransform._41)
			{
				Push_TargetLeft2(3);
				m_pPlayer->Push_TargetRight();
			}
			else if (m_pPlayer->Get_X() <= m_matTransform._41)
			{
				Push_TargetRight2(3);
				m_pPlayer->Push_TargetLeft();
			}
		}
		else if (lWidth > lHeight)
		{
			if (m_pPlayer->Get_Y() + 57 > m_matTransform._42 + 32.f)
			{
				Push_TargetUp2(3);
				m_pPlayer->Push_TargetDown();
			}
			else if (m_pPlayer->Get_Y() + 57 <= m_matTransform._42 + 32.f)
			{
				Push_TargetDown2(3);
				m_pPlayer->Push_TargetUp();
			}
		}
	}
}

void CGreenSlime::Monster_Collision(CGameObject* pTarget)
{
	RECT rcTmp = Get_Rect(pTarget->Get_X2(), pTarget->Get_Y2() + 32.f, 64.f, 64.f);
	RECT rcCollision = { 0 };
	RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42 + 32.f, 64.f, 64.f);

	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X2() > m_matTransform._41)
			{
				Push_TargetLeft2(3);
				pTarget->Push_TargetRight2(3);
			}
			else if (pTarget->Get_X2() <= m_matTransform._41)
			{
				Push_TargetRight2(3);
				pTarget->Push_TargetLeft2(3);
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y2() + 32.f > m_matTransform._42 + 32.f)
			{
				Push_TargetUp2(3);
				pTarget->Push_TargetDown2(3);
			}
			else if (pTarget->Get_Y2() + 32.f <= m_matTransform._42 + 32.f)
			{
				Push_TargetDown2(3);
				pTarget->Push_TargetUp2(3);
			}
		}
	}
}

void CGreenSlime::Move_ToTargetMonster(_vec3 vTargetPos, const _float & fTimeDelta)
{
	_vec3			vDir;
	_vec3			vPosition = *Get_Infomation(STATE_POSITION);

	vDir = vTargetPos - vPosition;

	_float			fDistance = D3DXVec3Length(&vDir);

	D3DXVec3Normalize(&vDir, &vDir);

	if (fDistance >= 600.0f)
	{
		m_bMoveOn = false;
	}

	// If within 250 units, start chasing player
	if (fDistance <= 250.0f && m_bMoveOn == false && m_bAttackOn == false)
	{
		m_fMarkNum = 1.f;
		m_bMarkOn = true;
		m_bMoveOn = true; // Start moving toward player
		m_fMarkFrame = 0.f;
	}

	// If within 100 units, trigger attack mode
	if (fDistance <= 100.0f && m_bAttackOn == false && m_fAcc >= 3.f)
	{
		m_fMarkNum = 1.f;
		m_bMarkOn = true;
		m_bAttackOn = true; // Trigger attack mode
		m_fFrame = 4.f; // Switch to attack animation
		m_fMarkFrame = 1.f;
	}

	if (true == m_bMarkOn)
	{
		if (m_fMarkNum > 0.f)
			m_fMarkNum -= 0.01f;
	}

	// Chasing movement
	if (m_bMoveOn == true && m_bAttackOn == false && m_bIdleOn == false)
	{
		if(false == m_bCollision) 
			// Move toward player
			vPosition += vDir * m_fSpeedPerSec * fTimeDelta;
	}

	if (true == m_bAttackOn && false == m_bDeadOn)
	{
		// Advance attack animation frame
		m_fFrame += fTimeDelta * 6.f;

		// Prepare for dash
		// hold frame at 6, save direction, brief charge time
		if (m_fFrame >= 6.f && m_fFrame < 7.f)
		{
			if (false == m_bOnce)
			{
				m_bOnce = true;
				// Save direction to player at attack start
				vSaveDir = vDir;
			}
			++m_iCount;
			// Hold animation on charge frame
			m_fFrame = 6.f;

			// After a short delay, start the dash
			if (m_iCount == 50)
			{
				// Play dash sound
				CSound_Manager::Get_Instance()->SoundPlay(22, FALSE);
				m_iCount = 0;
				// Move to dash phase
				m_fFrame = 7.f;
				m_bOnce = false;
			}
		}

		// Execute the dash
		if (m_fFrame >= 7.f)
		{
			++m_iCount;
			// Hold animation on dash frame
			m_fFrame = 7.f;
			// Dash rapidly toward player
			vPosition += vSaveDir * 700 * fTimeDelta;
			// Temporarily shrink collision box during dash
			m_bCollisionOff = true;
			RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57, 50, 32);
			RECT rcCollision = { 0 };
			RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42 + 32.f, 64.f, 64.f);

			if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
			{
				// Trigger hit effect on player
				m_pPlayer->Set_Blink(true);
			}

			// After the dash, reset states
			if (m_iCount == 10)
			{
				m_bCollisionOff = false;
				m_iCount = 0;
				m_bAttackOn = false;
				m_bIdleOn = true;
				m_fFrame = 0.f;
				m_fAcc = 0.f;
			}
		}
	}
	else
	{
		m_fFrame += fTimeDelta * 6.f;
		if (m_bDeadOn == true)
		{
			if (m_fFrame >= 5.f)
				m_fFrame = 0.f;
		}
		else
		{
			if (m_fFrame >= 4.f)
				m_fFrame = 0.f;
		}
	}

	if (m_fAcc >= 2.f)
	{
		m_bIdleOn = false;
	}

	memcpy(&m_matTransform.m[STATE_POSITION][0], &vPosition, sizeof(_vec3));
}

CGreenSlime * CGreenSlime::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fX, _float fY)
{
	CGreenSlime*		pInstance = new CGreenSlime(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject(fX, fY)))
	{
		_MSG_BOX("CGreenSlime Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGreenSlime::Free()
{
	for (auto& pTexture : m_pTexture)
		Safe_Release(pTexture);
	
	Safe_Release(m_pPlayer);
	Safe_Release(m_pSword);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pShader);
	
	CGameObject::Free();
}
