#include "stdafx.h"
#include "..\Headers\Object_Farm2.h"
#include "Camera.h"
#include "Management.h"
#include "Sound_Manager.h"

CObject_Farm2::CObject_Farm2(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

HRESULT CObject_Farm2::Ready_GameObject(_vec3 vPosition, _vec3 vSize, _uint idx)
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

	CGameObject* pAx = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Tools",1);
	if (nullptr == pAx)
		return E_FAIL;
	m_pAx = pAx;
	m_pAx->AddRef();

	CGameObject* pPick = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Tools",2);
	if (nullptr == pPick)
		return E_FAIL;
	m_pPick = pPick;
	m_pPick->AddRef();

	CGameObject* pSickle = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Tools",3);
	if (nullptr == pSickle)
		return E_FAIL;
	m_pSickle = pSickle;
	m_pSickle->AddRef();

	return NOERROR;
}

_int CObject_Farm2::Update_GameObject(const _float& fTimeDelta)
{
	if (false == m_bObjectOff)
	{
		if (SCENE_FARM == CManagement::GetInstance()->Get_Scene())
		{
			m_bTree = false;
			if (m_bShake == true)
			{
				Object_Shake();
			}
			_vec3			vDir;
			_vec3			vPosition = *Get_Infomation(STATE_POSITION);

			vDir = *m_pPlayer->Get_Infomation(CGameObject::STATE_POSITION) - vPosition;

			_float			fDistance = D3DXVec3Length(&vDir);

			D3DXVec3Normalize(&vDir, &vDir);

			if (fDistance <= 224.0f && m_bBranch == false && m_bBigStump == false && m_bFalldown == false && m_bStone == false &&
				m_bBush == false)
			{
				if (8 == m_fFrame || 9 == m_fFrame || 10 == m_fFrame) // 기둥
				{
					RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

					RECT rcCollision = { 0 };

					RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42 + 64, 150.f, 200.f);

					if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
					{
						m_bTree = true;
					}
				}
				else if (11 == m_fFrame || 12 == m_fFrame || 13 == m_fFrame) // 밑둥
				{
					RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

					RECT rcCollision = { 0 };

					RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42 + 224.f, 64.f, 64.f);

					if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
					{
						_long lWidth = rcCollision.right - rcCollision.left;
						_long lHeight = rcCollision.bottom - rcCollision.top;

						if (lWidth <= lHeight)
						{
							if (m_pPlayer->Get_X() <= m_matTransform._41)
							{
								m_pPlayer->Push_TargetLeft();
							}
							else if (m_pPlayer->Get_X() > m_matTransform._41)
							{
								m_pPlayer->Push_TargetRight();
							}
						}
						else if (lWidth > lHeight)
						{
							if (m_pPlayer->Get_Y() + 57.f <= m_matTransform._42 + 224.f)
							{
								m_pPlayer->Push_TargetUp();
							}
							else if (m_pPlayer->Get_Y() + 57.f > m_matTransform._42 + 224.f)
							{
								m_pPlayer->Push_TargetDown();
							}
						}
					}
				}
				else if (18 == m_fFrame) // 상자
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
							{
								m_pPlayer->Push_TargetLeft();
							}
							else if (m_pPlayer->Get_X() > m_matTransform._41)
							{
								m_pPlayer->Push_TargetRight();
							}
						}
						else if (lWidth > lHeight)
						{
							if (m_pPlayer->Get_Y() + 57.f <= m_matTransform._42 + 32.f)
							{
								m_pPlayer->Push_TargetUp();
							}
							else if (m_pPlayer->Get_Y() + 57.f > m_matTransform._42 + 32.f)
							{
								m_pPlayer->Push_TargetDown();
							}
						}
					}
				}
				else
				{
					RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

					RECT rcCollision = { 0 };

					RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42, m_matTransform._11, m_matTransform._22);

					if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
					{
						_long lWidth = rcCollision.right - rcCollision.left;
						_long lHeight = rcCollision.bottom - rcCollision.top;

						if (lWidth <= lHeight)
						{
							if (m_pPlayer->Get_X() <= m_matTransform._41)
							{
								m_pPlayer->Push_TargetLeft();
							}
							else if (m_pPlayer->Get_X() > m_matTransform._41)
							{
								m_pPlayer->Push_TargetRight();
							}
						}
						else if (lWidth > lHeight)
						{
							if (m_pPlayer->Get_Y() + 57.f <= m_matTransform._42)
							{
								m_pPlayer->Push_TargetUp();
							}
							else if (m_pPlayer->Get_Y() + 57.f > m_matTransform._42)
							{
								m_pPlayer->Push_TargetDown();
							}
						}
					}
				}
			}
		}
		// 장비충돌
		if (true == m_pAx->Get_ToolOn())
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

				if (0 == m_fFrame || 1 == m_fFrame || 2 == m_fFrame) // 나뭇가지, 큰나무밑둥
				{
					RECT rcTmp = m_pPlayer->Get_Rect(m_iX*64.f, m_iY*64.f + 64.f, 32.f, 32.f);

					RECT rcCollision = { 0 };

					RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42, m_matTransform._11, m_matTransform._22);

					if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
					{
						if (0 == m_fFrame)
						{
							_int RanNum = rand() % 2;
							if(0 == RanNum)
								CSound_Manager::Get_Instance()->SoundPlay(7, FALSE);
							else
								CSound_Manager::Get_Instance()->SoundPlay(8, FALSE);

							++m_iCount;
							m_bShake = true;
							if (m_iCount >= 3)
							{
								CSound_Manager::Get_Instance()->SoundPlay(38, FALSE);
								m_fObjectNumb = 3.f;
								m_bCreateObject = true;
								m_State = BigStump;
								m_bBigStump = true;
							}
						}
						else if (1 == m_fFrame || 2 == m_fFrame)
						{
							CSound_Manager::Get_Instance()->SoundPlay(31, FALSE);
							_int RanNum = rand() % 2;
							if (0 == RanNum)
								CSound_Manager::Get_Instance()->SoundPlay(7, FALSE);
							else
								CSound_Manager::Get_Instance()->SoundPlay(8, FALSE);

							m_fObjectNumb = 0.f;
							m_bCreateObject = true;
							m_State = Branch;
							m_bBranch = true;
						}
					}
				}
				else if (8 == m_fFrame || 9 == m_fFrame || 10 == m_fFrame)
				{
					RECT rcTmp = m_pPlayer->Get_Rect(m_iX*64.f, m_iY*64.f + 64.f, 32.f, 32.f);

					RECT rcCollision = { 0 };

					RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42 + 224.f, 64.f, 64.f);
					if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
					{
						_int RanNum = rand() % 2;
						if (0 == RanNum)
							CSound_Manager::Get_Instance()->SoundPlay(7, FALSE);
						else
							CSound_Manager::Get_Instance()->SoundPlay(8, FALSE);

						++m_iCount;
						m_bShake = true;
						m_bWhat = true;
						if (m_iCount >= 5)
						{
							CSound_Manager::Get_Instance()->SoundPlay(26, FALSE);
							m_fObjectNumb = 5.f;
							m_bCreateObject = true;
							m_bShake = false;
							m_bFalldown = true;
						}
					}
				}
				else if (11 == m_fFrame || 12 == m_fFrame || 13 == m_fFrame)
				{
					RECT rcTmp = m_pPlayer->Get_Rect(m_iX*64.f, m_iY*64.f + 64.f, 32.f, 32.f);

					RECT rcCollision = { 0 };

					RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42 + 224.f, 64.f, 64.f);
					if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
					{
						_int RanNum = rand() % 2;
						if (0 == RanNum)
							CSound_Manager::Get_Instance()->SoundPlay(7, FALSE);
						else
							CSound_Manager::Get_Instance()->SoundPlay(8, FALSE);

						++m_iCount;
						m_bShake = true;
						m_bWhat = true;
						if (m_iCount >= 8)
						{
							CSound_Manager::Get_Instance()->SoundPlay(38, FALSE);
							m_fObjectNumb = 4.f;
							m_bCreateObject = true;
							m_bShake = false;
							m_bFalldown = true;
						}
					}
				}
			}
			else if (3 > m_pPlayer->Get_Frame() && m_bFirst == true)
				m_bFirst = false;
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

				if (6 == m_fFrame || 7 == m_fFrame)
				{
					RECT rcTmp = m_pPlayer->Get_Rect(m_iX*64.f, m_iY*64.f + 64.f, 32.f, 32.f);

					RECT rcCollision = { 0 };

					RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42, m_matTransform._11, m_matTransform._22);

					if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
					{
						CSound_Manager::Get_Instance()->SoundPlay(25, FALSE);
						CSound_Manager::Get_Instance()->SoundPlay(24, FALSE);
						m_fObjectNumb = 1.f;
						m_bCreateObject = true;
						m_State = Stone;
						m_bStone = true;
					}
				}
			}
			else if (3 > m_pPlayer->Get_Frame() && m_bFirst == true)
				m_bFirst = false;
		}

		if (true == m_pSickle->Get_ToolOn())
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

				if (14 == m_fFrame || 15 == m_fFrame || 16 == m_fFrame || 17 == m_fFrame)
				{
					RECT rcTmp = m_pPlayer->Get_Rect(m_iX*64.f, m_iY*64.f, 2.f, 2.f);

					RECT rcCollision = { 0 };

					RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42, m_matTransform._11, m_matTransform._22);

					if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
					{
						CSound_Manager::Get_Instance()->SoundPlay(30, FALSE);
						CSound_Manager::Get_Instance()->SoundPlay(21, FALSE);
						m_fObjectNumb = 2.f;
						m_bCreateObject = true;
						m_State = Bush;
						m_bBush = true;
					}
				}
			}
			else if (3 > m_pPlayer->Get_Frame() && m_bFirst == true)
				m_bFirst = false;
		}

		if (true == m_bBigStump)
		{
			if (m_fFrame1 >= 4)
			{
				m_fNum -= 0.05f;
				if (m_fNum <= 0.f)
					m_bObjectOff = true;
			}
			else
				m_fFrame1 += 5 * fTimeDelta * 3.f;
		}
		else if (true == m_bBush || true == m_bStone)
		{
			if (m_fFrame1 >= 6)
			{
				m_fNum -= 0.05f;
				if (m_fNum <= 0.f)
					m_bObjectOff = true;
			}
			else
				m_fFrame1 += 7 * fTimeDelta * 3.f;
		}
		else if (true == m_bBranch)
		{
			if (m_fFrame1 >= 7)
			{
				m_fNum -= 0.05f;
				if (m_fNum <= 0.f)
					m_bObjectOff = true;
			}
			else
				m_fFrame1 += 8 * fTimeDelta * 3.f;
		}
		else if (true == m_bFalldown)
		{
			m_fNum -= 0.05f;
			if (m_fNum <= 0.f)
			{

				m_iCount = 0;
				m_bObjectOff = true;
			}

			m_matTransform._41 += m_fFalldownX;
			m_matTransform._42 -= m_fFalldownY;
			m_matTransform._41 -= m_fSSX;
			m_matTransform._42 += m_fSSY;

			m_fSSX += 0.1;
			m_fSSY = sin(m_fSSX*10.f)*powf(0.5f, m_fSSX);
		}
		if (true == m_bWhat)
		{
			if (m_fFrame2 >= 6)
			{
				m_fFrame2 = 0.f;
				m_bWhat = false;
			}
			else
				m_fFrame2 += 7 * fTimeDelta * 2.f;
		}
		if (true == m_bTree)
		{
			Object_Translucent();
		}
		else if (false == m_bFalldown && false == m_bBigStump && false == m_bBush && false == m_bStone && false == m_bBranch)
		{
			m_fNum += 0.05;
			if (m_fNum >= 1.f)
			{
				m_fNum = 1.f;
			}
		}

		if (18 == m_fFrame)
		{
			CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");
			POINT		ptMouse;
			POINT		ptRealMouse;
			ZeroMemory(&ptMouse, sizeof(POINT));

			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);

			RECT rcChest = Get_Rect(m_matTransform._41, m_matTransform._42 + 32.f, 64.f, 64.f);

			ptRealMouse.x = (pCamera->Get_X() - 640) + ptMouse.x;
			ptRealMouse.y = (pCamera->Get_Y() - 384) + ptMouse.y;

			if (true == PtInRect(&rcChest, ptRealMouse))
			{
				if ((GetKeyState(VK_RBUTTON) & 0x8000) && m_bbbOnce == false)
				{
					m_bbbOnce = true;
					m_bChestOn = true;
				}
				else if (!(GetKeyState(VK_RBUTTON) & 0x8000) && m_bbbOnce == true)
					m_bbbOnce = false;
			}
		}

	}
	return _int();
}

_int CObject_Farm2::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (SCENE_FARM == CManagement::GetInstance()->Get_Scene())
	{
		if(false == m_bObjectOff)
			if (nullptr != m_pRender_Manager)
				m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_YSORT, this);
	}
	return CGameObject::LastUpdate_GameObject(fTimeDelta);
}

void CObject_Farm2::Render_GameObject()
{
	if (nullptr == m_pBuffer ||
		nullptr == m_pTexture ||
		nullptr == m_pShader)
		return;

	CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");

	if (true == isDraw(pCamera) && m_fFrame1 <= 0.f)
	{
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
	else if (true == m_bBigStump || true == m_bBush || m_bStone == true || m_bBranch == true)
	{
		LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
		if (nullptr == pEffect)
			return;
		pEffect->AddRef();

		m_pDelete[m_State]->SetUp_OnShader(pEffect, "g_BaseTexture", m_fFrame1);
		
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

	if (true == m_bWhat)
	{
		LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
		if (nullptr == pEffect)
			return;
		pEffect->AddRef();

		m_pDelete[4]->SetUp_OnShader(pEffect, "g_BaseTexture", m_fFrame2);

		pEffect->SetFloat("m_fNum", m_fNum);
		pEffect->Begin(nullptr, 0);

		pEffect->BeginPass(2);

		TempMatrix = Render_TransForm(pCamera);
		TempMatrix._11 = 64.f;
		TempMatrix._22 = 64.f;
		TempMatrix._42 += 170.f;

		m_pBuffer->Render_Buffer(TempMatrix);

		pEffect->EndPass();

		pEffect->End();

		Safe_Release(pEffect);
	}
	
}

HRESULT CObject_Farm2::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture
	m_pTexture = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FarmObject");
	if (nullptr == m_pTexture)
		return E_FAIL;
	m_pTexture->AddRef();

	m_pDelete[0] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_BigStumpDelete");
	if (nullptr == m_pDelete[0])
		return E_FAIL;
	m_pDelete[0]->AddRef();

	m_pDelete[1] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_BushDelete");
	if (nullptr == m_pDelete[1])
		return E_FAIL;
	m_pDelete[1]->AddRef();

	m_pDelete[2] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_StoneDelete");
	if (nullptr == m_pDelete[2])
		return E_FAIL;
	m_pDelete[2]->AddRef();

	m_pDelete[3] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_BranchDelete");
	if (nullptr == m_pDelete[3])
		return E_FAIL;
	m_pDelete[3]->AddRef();

	m_pDelete[4] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Branchcrash");
	if (nullptr == m_pDelete[4])
		return E_FAIL;
	m_pDelete[4]->AddRef();

	// For.Shader
	m_pShader = (CShader*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Shader_Default");
	if (nullptr == m_pShader)
		return E_FAIL;
	m_pShader->AddRef();

	return NOERROR;
}

HRESULT CObject_Farm2::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	// 셰이더 전역변수에 값을 전달한다.
	m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture",m_fFrame);

	Safe_Release(pEffect);

	return NOERROR;
}

_bool CObject_Farm2::isDraw(CGameObject * pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

_matrix CObject_Farm2::Render_TransForm(CGameObject * pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

void CObject_Farm2::Object_Shake()
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

void CObject_Farm2::Object_Translucent()
{
	m_fNum -= 0.05f;
	if (0.5f >= m_fNum)
		m_fNum = 0.5f;
}

CObject_Farm2 * CObject_Farm2::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _vec3 vPosition, _vec3 vSize, _uint idx)
{
	CObject_Farm2*		pInstance = new CObject_Farm2(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject(vPosition, vSize, idx)))
	{
		_MSG_BOX("CNightEffect Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_Farm2::Free()
{
	Safe_Release(m_pPlayer);
	Safe_Release(m_pAx);
	Safe_Release(m_pPick);
	Safe_Release(m_pSickle);
	for (auto& pDelete : m_pDelete)
		Safe_Release(pDelete);
	Safe_Release(m_pTexture);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pShader);
	
	CGameObject::Free();
}
