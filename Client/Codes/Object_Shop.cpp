#include "stdafx.h"
#include "..\Headers\Object_Shop.h"
#include "Camera.h"
#include "Management.h"

CObject_Shop::CObject_Shop(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

HRESULT CObject_Shop::Ready_GameObject(_vec3 vPosition, _vec3 vSize, _uint idx)
{
	CGameObject::Ready_GameObject();

	if (FAILED(SetUp_Resource()))
		return E_FAIL;

	m_vSize = vSize;
	m_vPosition = vPosition;

	Set_Scale(m_vSize);
	Set_Position(m_vPosition);
	m_fFrame = idx;

	m_fSX = m_matTransform._41;
	m_fSY = m_matTransform._42;

	m_YsortRect.bottom = m_matTransform._42 + m_matTransform._22*0.5f;

	CGameObject* pPlayer = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr == pPlayer)
		return E_FAIL;
	m_pPlayer = pPlayer;
	m_pPlayer->AddRef();

	return NOERROR;
}

_int CObject_Shop::Update_GameObject(const _float& fTimeDelta)
{
	if (SCENE_SHOP == CManagement::GetInstance()->Get_Scene())
	{
		CGameObject* pTarget = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_ShopUI");
		if (nullptr == pTarget)
			return E_FAIL;

		if (false == pTarget->Get_ShopOn())
		{
			_vec3			vDir;
			_vec3			vPosition = *Get_Infomation(STATE_POSITION);

			vDir = *m_pPlayer->Get_Infomation(CGameObject::STATE_POSITION) - vPosition;

			_float			fDistance = D3DXVec3Length(&vDir);

			D3DXVec3Normalize(&vDir, &vDir);

			if (0 == m_fFrame)
			{
				RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

				RECT rcCollision = { 0 };

				RECT rcObject = Get_Rect(m_matTransform._41 - 32.f, m_matTransform._42 - 32.f, 128.f, 128.f);

				if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
				{
					_long lWidth = rcCollision.right - rcCollision.left;
					_long lHeight = rcCollision.bottom - rcCollision.top;

					if (lWidth <= lHeight)
					{
						if (m_pPlayer->Get_X() <= m_matTransform._41)
							m_pPlayer->Push_TargetLeft();
						else if (m_pPlayer->Get_X() > m_matTransform._41)
							m_pPlayer->Push_TargetRight();
					}
					else if (lWidth > lHeight)
					{
						if (m_pPlayer->Get_Y() + 57.f <= m_matTransform._42 - 32.f)
							m_pPlayer->Push_TargetUp();
						else if (m_pPlayer->Get_Y() + 57.f > m_matTransform._42 - 32.f)
							m_pPlayer->Push_TargetDown();
					}
				}
			}
			else if (1 == m_fFrame || 2 == m_fFrame)
			{
				RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

				RECT rcCollision = { 0 };

				RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42 + 32.f, 64.f, 64.f);

				if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
				{
					_long lWidth = rcCollision.right - rcCollision.left;
					_long lHeight = rcCollision.bottom - rcCollision.top;

					if (lWidth <= lHeight)
					{
						if (m_pPlayer->Get_X() <= m_matTransform._41)
							m_pPlayer->Push_TargetLeft();
						else if (m_pPlayer->Get_X() > m_matTransform._41)
							m_pPlayer->Push_TargetRight();
					}
					else if (lWidth > lHeight)
					{
						if (m_pPlayer->Get_Y() + 57.f <= m_matTransform._42 + 32.f)
							m_pPlayer->Push_TargetUp();
						else if (m_pPlayer->Get_Y() + 57.f > m_matTransform._42 + 32.f)
							m_pPlayer->Push_TargetDown();
					}
				}
			}
			else if (3 == m_fFrame)
			{
				RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

				RECT rcCollision = { 0 };

				RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42 + 96.f, 64.f, 64.f);

				if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
				{
					_long lWidth = rcCollision.right - rcCollision.left;
					_long lHeight = rcCollision.bottom - rcCollision.top;

					if (lWidth <= lHeight)
					{
						if (m_pPlayer->Get_X() <= m_matTransform._41)
							m_pPlayer->Push_TargetLeft();
						else if (m_pPlayer->Get_X() > m_matTransform._41)
							m_pPlayer->Push_TargetRight();
					}
					else if (lWidth > lHeight)
					{
						if (m_pPlayer->Get_Y() + 57.f <= m_matTransform._42 + 96.f)
							m_pPlayer->Push_TargetUp();
						else if (m_pPlayer->Get_Y() + 57.f > m_matTransform._42 + 96.f)
							m_pPlayer->Push_TargetDown();
					}
				}
			}
			else if (4 == m_fFrame)
			{
				RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

				RECT rcCollision = { 0 };

				RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42 + 96.f, 64.f, 5.f*64.f);

				if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
				{
					_long lWidth = rcCollision.right - rcCollision.left;
					_long lHeight = rcCollision.bottom - rcCollision.top;

					if (lWidth <= lHeight)
					{
						if (m_pPlayer->Get_X() <= m_matTransform._41)
							m_pPlayer->Push_TargetLeft();
						else if (m_pPlayer->Get_X() > m_matTransform._41)
							m_pPlayer->Push_TargetRight();
					}
					else if (lWidth > lHeight)
					{
						if (m_pPlayer->Get_Y() + 57.f <= m_matTransform._42 + 96.f)
							m_pPlayer->Push_TargetUp();
						else if (m_pPlayer->Get_Y() + 57.f > m_matTransform._42 + 96.f)
							m_pPlayer->Push_TargetDown();
					}
				}
			}
			else if (5 == m_fFrame)
			{
				RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

				RECT rcCollision = { 0 };

				RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42 + 64.f, 2.f*64.f, 2.f*64.f);

				if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
				{
					_long lWidth = rcCollision.right - rcCollision.left;
					_long lHeight = rcCollision.bottom - rcCollision.top;

					if (lWidth <= lHeight)
					{
						if (m_pPlayer->Get_X() <= m_matTransform._41)
							m_pPlayer->Push_TargetLeft();
						else if (m_pPlayer->Get_X() > m_matTransform._41)
							m_pPlayer->Push_TargetRight();
					}
					else if (lWidth > lHeight)
					{
						if (m_pPlayer->Get_Y() + 57.f <= m_matTransform._42 + 64.f)
							m_pPlayer->Push_TargetUp();
						else if (m_pPlayer->Get_Y() + 57.f > m_matTransform._42 + 64.f)
							m_pPlayer->Push_TargetDown();
					}
				}
			}
			else if (6 == m_fFrame)
			{
				RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

				RECT rcCollision = { 0 };

				RECT rcObject = Get_Rect(m_matTransform._41 - 8.f, m_matTransform._42 + 64.f + 32.f, 6.f*64.f + 4, 64.f);

				if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
				{
					_long lWidth = rcCollision.right - rcCollision.left;
					_long lHeight = rcCollision.bottom - rcCollision.top;

					if (lWidth <= lHeight)
					{
						if (m_pPlayer->Get_X() <= m_matTransform._41 - 8.f)
							m_pPlayer->Push_TargetLeft();
						else if (m_pPlayer->Get_X() > m_matTransform._41 - 8.f)
							m_pPlayer->Push_TargetRight();
					}
					else if (lWidth > lHeight)
					{
						if (m_pPlayer->Get_Y() + 57.f <= m_matTransform._42 + 64.f + 32.f)
							m_pPlayer->Push_TargetUp();
						else if (m_pPlayer->Get_Y() + 57.f > m_matTransform._42 + 64.f + 32.f)
							m_pPlayer->Push_TargetDown();
					}
				}
			}
			else if (7 == m_fFrame)
			{
				RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

				RECT rcCollision = { 0 };

				RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42 + 64.f + 32.f, 6.f*64.f, 64.f);

				if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
				{
					_long lWidth = rcCollision.right - rcCollision.left;
					_long lHeight = rcCollision.bottom - rcCollision.top;

					if (lWidth <= lHeight)
					{
						if (m_pPlayer->Get_X() <= m_matTransform._41)
							m_pPlayer->Push_TargetLeft();
						else if (m_pPlayer->Get_X() > m_matTransform._41)
							m_pPlayer->Push_TargetRight();
					}
					else if (lWidth > lHeight)
					{
						if (m_pPlayer->Get_Y() + 57.f <= m_matTransform._42 + 64.f + 32.f)
							m_pPlayer->Push_TargetUp();
						else if (m_pPlayer->Get_Y() + 57.f > m_matTransform._42 + 64.f + 32.f)
							m_pPlayer->Push_TargetDown();
					}
				}
			}
			else if (15 == m_fFrame)
			{
				RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

				RECT rcCollision = { 0 };

				RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42, 64.f, 128.f);

				if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
				{
					_long lWidth = rcCollision.right - rcCollision.left;
					_long lHeight = rcCollision.bottom - rcCollision.top;

					if (lWidth <= lHeight)
					{
						if (m_pPlayer->Get_X() <= m_matTransform._41)
							m_pPlayer->Push_TargetLeft();
						else if (m_pPlayer->Get_X() > m_matTransform._41)
							m_pPlayer->Push_TargetRight();
					}
					else if (lWidth > lHeight)
					{
						if (m_pPlayer->Get_Y() + 57.f <= m_matTransform._42 + 32.f)
							m_pPlayer->Push_TargetUp();
						else if (m_pPlayer->Get_Y() + 57.f > m_matTransform._42 + 32.f)
							m_pPlayer->Push_TargetDown();
					}
				}

				CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");

				CGameObject* pMouse = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Mouse");
				if (nullptr == pMouse)
					return E_FAIL;

				RECT rcTmp2 = Get_Rect(pCamera->Get_X() - 640 + pMouse->Get_X(), pCamera->Get_Y() - 384 + pMouse->Get_Y(), 1.f, 1.f);

				RECT rcCollision2 = { 0 };

				RECT rcObject2 = Get_Rect(m_matTransform._41, m_matTransform._42, 64.f, 128.f);

				if (true == _bool(IntersectRect(&rcCollision2, &rcObject2, &rcTmp2)))
					m_bShopOn = true;
				else
					m_bShopOn = false;
			}

			RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

			RECT rcCollision = { 0 };

			RECT rcObject = Get_Rect(9.f*64.f, 6.f*64.f, 64.f, 2.f*64.f);

			if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
			{
				_long lWidth = rcCollision.right - rcCollision.left;
				_long lHeight = rcCollision.bottom - rcCollision.top;

				if (lWidth <= lHeight)
				{
					if (m_pPlayer->Get_X() <= 9.f*64.f)
						m_pPlayer->Push_TargetLeft();
					else if (m_pPlayer->Get_X() > 9.f*64.f)
						m_pPlayer->Push_TargetRight();
				}
				else if (lWidth > lHeight)
				{
					if (m_pPlayer->Get_Y() + 57.f <= 6.f*64.f)
						m_pPlayer->Push_TargetUp();
					else if (m_pPlayer->Get_Y() + 57.f > 6.f*64.f)
						m_pPlayer->Push_TargetDown();
				}
			}
		}
		else
			m_bShopOn = false;
	}
	return _int();
}

_int CObject_Shop::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (SCENE_SHOP == CManagement::GetInstance()->Get_Scene())
	{
		if(false == m_bObjectOff)
			if (nullptr != m_pRender_Manager)
				m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_YSORT, this);
	}
	return CGameObject::LastUpdate_GameObject(fTimeDelta);
}

void CObject_Shop::Render_GameObject()
{
	if (nullptr == m_pBuffer ||
		nullptr == m_pTexture ||
		nullptr == m_pShader)
		return;

	CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");

	LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return;
	pEffect->AddRef();

	m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture", m_fFrame);
	
	// 특정 셰이더파일을 이용해서 그립니다. 
	//_uint		iNumPasses = 0;
	//pEffect->Begin(&iNumPasses, 0);
	pEffect->SetFloat("m_fNum", m_fNum);
	
	pEffect->Begin(nullptr, 0);

	// 특정 패스를 이용합니다. 
	pEffect->BeginPass(2);

	TempMatrix = Render_TransForm(pCamera);

	// 자원을 그립니다. 
	m_pBuffer->Render_Buffer(TempMatrix);

	// 패스를 종료합니다. 
	pEffect->EndPass();

	// 셰이더파일을 이용한 렌더링을 종료합니다. 
	pEffect->End();

	Safe_Release(pEffect);
	
}

HRESULT CObject_Shop::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture
	m_pTexture = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_ShopObject");
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

HRESULT CObject_Shop::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	// 셰이더 전역변수에 값을 전달한다.
	m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture",m_fFrame);

	Safe_Release(pEffect);

	return NOERROR;
}

_bool CObject_Shop::isDraw(CGameObject * pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

_matrix CObject_Shop::Render_TransForm(CGameObject * pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

void CObject_Shop::Object_Shake()
{
	--m_iShakeTime;
	
	if (true == m_bSequence)
	{
		m_bSequence = false;
		m_matTransform._41 += 5;
	}
	else if (false == m_bSequence)
	{
		m_bSequence = true;
		m_matTransform._41 -= 5;
	}

	if (m_iShakeTime == 0)
	{
		m_bSequence = false;
		m_iShakeTime = 4;
		m_bShake = false;
		m_matTransform._41 = m_fSX;
		m_matTransform._42 = m_fSY;
	}

}

CObject_Shop * CObject_Shop::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _vec3 vPosition, _vec3 vSize, _uint idx)
{
	CObject_Shop*		pInstance = new CObject_Shop(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject(vPosition, vSize, idx)))
	{
		_MSG_BOX("CObject_Shop Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_Shop::Free()
{
	Safe_Release(m_pPlayer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pShader);
	
	CGameObject::Free();
}
