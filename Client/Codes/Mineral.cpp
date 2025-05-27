#include "stdafx.h"
#include "..\Headers\Mineral.h"
#include "Camera.h"
#include "Management.h"
#include "Sound_Manager.h"
CMineral::CMineral(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

HRESULT CMineral::Ready_GameObject(_float fX, _float fY, _float fFrame)
{
	CGameObject::Ready_GameObject();

	if (FAILED(SetUp_Resource()))
		return E_FAIL;

	m_X = fX;
	m_Y = fY;
	m_fFrame = fFrame;
	Set_Scale(_vec3(64.f,64.f,1.f));
	Set_Position(_vec3(m_X, m_Y, 0.f));

	CGameObject* pPlayer = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr == pPlayer)
		return E_FAIL;
	m_pPlayer = pPlayer;
	m_pPlayer->AddRef();

	CGameObject* pPick = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Tools",2);
	if (nullptr == pPick)
		return E_FAIL;
	m_pPick = pPick;
	m_pPick->AddRef();

	m_fNum = 1.f;

	m_YsortRect.bottom = m_matTransform._42 + m_matTransform._22*0.5f;


	return NOERROR;
}

_int CMineral::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bObjectOff == false)
	{
		CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");
		if (true == isDraw(pCamera) && false == m_bErase)
		{
			Player_Collision();

			for (size_t i = 0; i < 10; i++)
			{
				CGameObject* pMonster = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Monster", i);
				if (nullptr != pMonster)
				{
					Monster_Collision(pMonster);
				}
			}
		}

		if (true == m_pPick->Get_ToolOn())
		{
			if (m_pPlayer->Get_State() && 3 <= m_pPlayer->Get_Frame() && m_bFirst == false)
			{
				m_bFirst = true;
				if (1 == m_pPlayer->Get_State())
				{
					m_iX = _int(m_pPlayer->Get_X() + 32) * 0.015625;
					m_iY = _int(m_pPlayer->Get_Y() + 32 - 64) * 0.015625;
				}
				else if (2 == m_pPlayer->Get_State())
				{
					m_iX = _int(m_pPlayer->Get_X() + 32 + 64) * 0.015625;
					m_iY = _int(m_pPlayer->Get_Y() + 32) * 0.015625;
				}
				else if (3 == m_pPlayer->Get_State())
				{
					m_iX = _int(m_pPlayer->Get_X() + 32) * 0.015625;
					m_iY = _int(m_pPlayer->Get_Y() + 32 + 64) * 0.015625;
				}
				else if (4 == m_pPlayer->Get_State())
				{
					m_iX = _int(m_pPlayer->Get_X() + 32 - 64) * 0.015625;
					m_iY = _int(m_pPlayer->Get_Y() + 32) * 0.015625;
				}

				RECT rcTmp = m_pPlayer->Get_Rect(m_iX*64.f, m_iY*64.f + 64.f, 32.f, 32.f);

				RECT rcCollision = { 0 };

				RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42, m_matTransform._11, m_matTransform._22);

				if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
				{
					CSound_Manager::Get_Instance()->SoundPlay(24, FALSE);
					CSound_Manager::Get_Instance()->SoundPlay(25, FALSE);

					if (0 == m_fFrame || 2 == m_fFrame || 4 == m_fFrame)
						m_fObjectNumb = 1.f;
					else if (6 == m_fFrame)
						m_fObjectNumb = 6.f;

					m_bCreateObject = true;
					++m_fFrame;
					m_bErase = true;
				}
			}
			else if (3 > m_pPlayer->Get_Frame() && m_bFirst == true)
				m_bFirst = false;
		}

		if (true == m_bErase)
		{
			m_fNum -= 0.05f;
			if (m_fNum <= 0.f)
				m_bObjectOff = true;
		}
	}
	return _int();
}

_int CMineral::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (false == m_bObjectOff)
	{
		if (nullptr != m_pRender_Manager)
			m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_YSORT, this);
	}
	return CGameObject::LastUpdate_GameObject(fTimeDelta);
}

void CMineral::Render_GameObject()
{
	if (nullptr == m_pBuffer ||
		nullptr == m_pTexture ||
		nullptr == m_pShader)
		return;

	CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");

	if (true == isDraw(pCamera) && m_bObjectOff == false)
	{
		LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
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
}


HRESULT CMineral::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture
	m_pTexture = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Mineral");
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

HRESULT CMineral::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	// 셰이더 전역변수에 값을 전달한다.
	m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture",m_fFrame);

	Safe_Release(pEffect);

	return NOERROR;
}

_bool CMineral::isDraw(CGameObject * pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

_matrix CMineral::Render_TransForm(CGameObject * pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

void CMineral::Player_Collision()
{
	RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57, 50, 32);
	RECT rcCollision = { 0 };
	RECT rcObject = Get_Rect(m_matTransform._41,m_matTransform._42,64.f,64.f);

	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (m_pPlayer->Get_X() > m_matTransform._41)
			{
				m_pPlayer->Push_TargetRight();

			}
			else if (m_pPlayer->Get_X() <= m_matTransform._41)
			{
				m_pPlayer->Push_TargetLeft();
			}
		}
		else if (lWidth > lHeight)
		{
			if (m_pPlayer->Get_Y() + 57 > m_matTransform._42)
			{
				m_pPlayer->Push_TargetDown();
			}
			else if (m_pPlayer->Get_Y() + 57 <= m_matTransform._42)
			{
				m_pPlayer->Push_TargetUp();
			}
		}
	}
}

void CMineral::Monster_Collision(CGameObject* pTarget)
{
	RECT rcTmp = Get_Rect(pTarget->Get_X2(), pTarget->Get_Y2() + 32.f, 64.f, 64.f);
	RECT rcCollision = { 0 };
	RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42, 64.f, 64.f);

	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X2() > m_matTransform._41)
			{
				pTarget->Push_TargetRight2(6);
			}
			else if (pTarget->Get_X2() <= m_matTransform._41)
			{
				pTarget->Push_TargetLeft2(6);
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y2() + 32.f > m_matTransform._42)
			{
				pTarget->Push_TargetDown2(6);
	
			}
			else if (pTarget->Get_Y2() + 32.f <= m_matTransform._42)
			{
				pTarget->Push_TargetUp2(6);
			}
		}
	}
	else
		pTarget->Set_Collision(false);
}

CMineral * CMineral::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fX, _float fY, _float fFrame)
{
	CMineral*		pInstance = new CMineral(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject(fX, fY, fFrame)))
	{
		_MSG_BOX("CMineral Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMineral::Free()
{
	Safe_Release(m_pTexture);
	Safe_Release(m_pPlayer);
	Safe_Release(m_pPick);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pShader);
	
	CGameObject::Free();
}
