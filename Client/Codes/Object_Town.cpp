#include "stdafx.h"
#include "..\Headers\Object_Town.h"
#include "Camera.h"
#include "Management.h"

CObject_Town::CObject_Town(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

HRESULT CObject_Town::Ready_GameObject(_vec3 vPosition, _vec3 vSize, _uint idx)
{
	CGameObject::Ready_GameObject();

	if (FAILED(SetUp_Resource()))
		return E_FAIL;

	m_vSize = vSize;
	m_vPosition = vPosition;

	Set_Scale(m_vSize);
	Set_Position(m_vPosition);
	m_fFrame = idx;

	if(m_fFrame == 3 || m_fFrame == 4)
		m_YsortRect.bottom = (m_matTransform._42 + m_matTransform._22*0.5f)-96.f;
	else if(m_fFrame == 43)
		m_YsortRect.bottom = (m_matTransform._42 + m_matTransform._22*0.5f) - 128.f;
	else
		m_YsortRect.bottom = m_matTransform._42 + m_matTransform._22*0.5f;

	CGameObject* pPlayer = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr == pPlayer)
		return E_FAIL;
	m_pPlayer = pPlayer;
	m_pPlayer->AddRef();

	return NOERROR;
}

_int CObject_Town::Update_GameObject(const _float& fTimeDelta)
{
	if (SCENE_TOWN == CManagement::GetInstance()->Get_Scene())
	{
		_vec3			vDir;
		_vec3			vPosition = *Get_Infomation(STATE_POSITION);

		vDir = *m_pPlayer->Get_Infomation(CGameObject::STATE_POSITION) - vPosition;

		_float			fDistance = D3DXVec3Length(&vDir);

		D3DXVec3Normalize(&vDir, &vDir);

		if (0 == m_fFrame || 1 == m_fFrame || 5 == m_fFrame || 21 == m_fFrame)
		{
			RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

			RECT rcCollision = { 0 };

			RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42 + 32.f, 128.f, 32.f);

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
		else if (2 == m_fFrame || 7 == m_fFrame || 11 == m_fFrame)
		{
			RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

			RECT rcCollision = { 0 };

			RECT rcObject = Get_Rect(m_matTransform._41 + 32.f, m_matTransform._42 + 64.f, 192.f, 96.f);

			if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
			{
				_long lWidth = rcCollision.right - rcCollision.left;
				_long lHeight = rcCollision.bottom - rcCollision.top;

				if (lWidth <= lHeight)
				{
					if (m_pPlayer->Get_X() <= m_matTransform._41 + 32.f)
						m_pPlayer->Push_TargetLeft();
					else if (m_pPlayer->Get_X() > m_matTransform._41 + 32.f)
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
		else if (3 == m_fFrame || 4 == m_fFrame)
		{
			RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

			RECT rcCollision = { 0 };

			RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42 + 2.f*64.f, 64.f, 64.f);

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
					if (m_pPlayer->Get_Y() + 57.f <= m_matTransform._42 + 2.f*64.f)
						m_pPlayer->Push_TargetUp();
					else if (m_pPlayer->Get_Y() + 57.f > m_matTransform._42 + 2.f*64.f)
						m_pPlayer->Push_TargetDown();
				}
			}
		}
		else if (6 == m_fFrame || 8 == m_fFrame)
		{
			RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

			RECT rcCollision = { 0 };

			RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42 + 64.f, 64.f, 128.f);

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
		else if (9 == m_fFrame || 12 == m_fFrame || 13 == m_fFrame || 16 == m_fFrame || 17 == m_fFrame || 18 == m_fFrame ||
			24 == m_fFrame || 25 == m_fFrame || 26 == m_fFrame || 27 == m_fFrame || 28 == m_fFrame || 29 == m_fFrame)
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
		else if (10 == m_fFrame || 14 == m_fFrame || 15 == m_fFrame || 30 == m_fFrame || 31 == m_fFrame ||
			32 == m_fFrame || 33 == m_fFrame || 34 == m_fFrame)
		{
			RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

			RECT rcCollision = { 0 };

			RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42, 64.f, 64.f);

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
					if (m_pPlayer->Get_Y() + 57.f <= m_matTransform._42)
						m_pPlayer->Push_TargetUp();
					else if (m_pPlayer->Get_Y() + 57.f > m_matTransform._42)
						m_pPlayer->Push_TargetDown();
				}
			}
		}
		else if (22 == m_fFrame)
		{
			RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

			RECT rcCollision = { 0 };

			RECT rcObject = Get_Rect(m_matTransform._41 + 3.f*64.f, m_matTransform._42 + 3.f*64.f + 32.f, 10.f*64.f, 64.f);

			if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
			{
				_long lWidth = rcCollision.right - rcCollision.left;
				_long lHeight = rcCollision.bottom - rcCollision.top;

				if (lWidth <= lHeight)
				{
					if (m_pPlayer->Get_X() <= m_matTransform._41 + 3.f*64.f)
						m_pPlayer->Push_TargetLeft();
					else if (m_pPlayer->Get_X() > m_matTransform._41 + 3.f*64.f)
						m_pPlayer->Push_TargetRight();
				}
				else if (lWidth > lHeight)
				{
					if (m_pPlayer->Get_Y() + 57.f <= m_matTransform._42 + 3.f*64.f + 32.f)
						m_pPlayer->Push_TargetUp();
					else if (m_pPlayer->Get_Y() + 57.f > m_matTransform._42 + 3.f*64.f + 32.f)
						m_pPlayer->Push_TargetDown();
				}
			}
		}
		else if (23 == m_fFrame)
		{
			RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

			RECT rcCollision = { 0 };

			RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42, 256.f, 512.f);

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
					if (m_pPlayer->Get_Y() + 57.f <= m_matTransform._42)
						m_pPlayer->Push_TargetUp();
					else if (m_pPlayer->Get_Y() + 57.f > m_matTransform._42)
						m_pPlayer->Push_TargetDown();
				}
			}
		}
		else if (35 == m_fFrame || 36 == m_fFrame || 37 == m_fFrame)
		{
			RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

			RECT rcCollision = { 0 };

			RECT rcObject = Get_Rect(m_matTransform._41 + 32.f, m_matTransform._42 + 3.f*64.f + 32.f, 64.f, 64.f);

			if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
			{
				_long lWidth = rcCollision.right - rcCollision.left;
				_long lHeight = rcCollision.bottom - rcCollision.top;

				if (lWidth <= lHeight)
				{
					if (m_pPlayer->Get_X() <= m_matTransform._41 + 32.f)
						m_pPlayer->Push_TargetLeft();
					else if (m_pPlayer->Get_X() > m_matTransform._41 + 32.f)
						m_pPlayer->Push_TargetRight();
				}
				else if (lWidth > lHeight)
				{
					if (m_pPlayer->Get_Y() + 57.f <= m_matTransform._42 + 3.f*64.f)
						m_pPlayer->Push_TargetUp();
					else if (m_pPlayer->Get_Y() + 57.f > m_matTransform._42 + 3.f*64.f)
						m_pPlayer->Push_TargetDown();
				}
			}
		}
		else if (38 == m_fFrame)
		{
			RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

			RECT rcCollision = { 0 };

			RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42 + 3.f*64.f + 32.f, 100.f, 96.f);

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
					if (m_pPlayer->Get_Y() + 57.f <= m_matTransform._42 + 3.f*64.f + 32.f)
						m_pPlayer->Push_TargetUp();
					else if (m_pPlayer->Get_Y() + 57.f > m_matTransform._42 + 3.f*64.f + 32.f)
						m_pPlayer->Push_TargetDown();
				}
			}
		}
		else if (39 == m_fFrame)
		{
			RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

			RECT rcCollision = { 0 };

			RECT rcObject = Get_Rect(m_matTransform._41 + 3.f*64.f, m_matTransform._42 + 6.f*64.f + 32.f, 10.f*64.f, 3.f*64.f);

			if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
			{
				_long lWidth = rcCollision.right - rcCollision.left;
				_long lHeight = rcCollision.bottom - rcCollision.top;

				if (lWidth <= lHeight)
				{
					if (m_pPlayer->Get_X() <= m_matTransform._41 + 3.f*64.f)
						m_pPlayer->Push_TargetLeft();
					else if (m_pPlayer->Get_X() > m_matTransform._41 + 3.f*64.f)
						m_pPlayer->Push_TargetRight();
				}
				else if (lWidth > lHeight)
				{
					if (m_pPlayer->Get_Y() + 57.f <= m_matTransform._42 + 6.f*64.f + 32.f)
						m_pPlayer->Push_TargetUp();
					else if (m_pPlayer->Get_Y() + 57.f > m_matTransform._42 + 6.f*64.f + 32.f)
						m_pPlayer->Push_TargetDown();
				}
			}
		}
		else if (42 == m_fFrame)
		{
			RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

			RECT rcCollision = { 0 };

			RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42 + 6.f*64.f, 8.f*64.f, 4.f*64.f);

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
					if (m_pPlayer->Get_Y() + 57.f <= m_matTransform._42 + 6.f*64.f)
						m_pPlayer->Push_TargetUp();
					else if (m_pPlayer->Get_Y() + 57.f > m_matTransform._42 + 6.f*64.f)
						m_pPlayer->Push_TargetDown();
				}
			}
		}
	}
	return _int();
}

_int CObject_Town::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (SCENE_TOWN == CManagement::GetInstance()->Get_Scene())
	{
		if (nullptr != m_pRender_Manager)
		{
			if (m_fFrame == 19 || m_fFrame == 20)
				m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_TWO, this);
			else
				m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_YSORT, this);
		}
	}
	return CGameObject::LastUpdate_GameObject(fTimeDelta);
}

void CObject_Town::Render_GameObject()
{
	if (nullptr == m_pBuffer ||
		nullptr == m_pTexture ||
		nullptr == m_pShader)
		return;

	
	CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");
	if (true == isDraw(pCamera))
	{
		LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
		if (nullptr == pEffect)
			return;
		pEffect->AddRef();

		m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture", m_fFrame);

		// 특정 셰이더파일을 이용해서 그립니다. 

		pEffect->Begin(nullptr, 0);

		// 특정 패스를 이용합니다. 
		pEffect->BeginPass(0);

		TempMatrix = Render_TransForm(pCamera);

		// 자원을 그립니다. 
		m_pBuffer->Render_Buffer(TempMatrix);

		// 패스를 종료합니다. 
		pEffect->EndPass();

		// 셰이더파일을 이용한 렌더링을 종료합니다. 
		pEffect->End();

		Safe_Release(pEffect);
	}
}

HRESULT CObject_Town::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture
	m_pTexture = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_TownObject");
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

HRESULT CObject_Town::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	// 셰이더 전역변수에 값을 전달한다.
	m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture",m_fFrame);

	Safe_Release(pEffect);

	return NOERROR;
}

_bool CObject_Town::isDraw(CGameObject * pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

_matrix CObject_Town::Render_TransForm(CGameObject * pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

CObject_Town * CObject_Town::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _vec3 vPosition, _vec3 vSize, _uint idx)
{
	CObject_Town*		pInstance = new CObject_Town(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject(vPosition, vSize, idx)))
	{
		_MSG_BOX("CObject_Town Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_Town::Free()
{
	Safe_Release(m_pPlayer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pShader);
	
	CGameObject::Free();
}
