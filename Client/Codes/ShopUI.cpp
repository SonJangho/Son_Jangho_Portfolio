#include "stdafx.h"
#include "..\Headers\ShopUI.h"
#include "Camera.h"
#include "Management.h"
#include "Sound_Manager.h"

CShopUI::CShopUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

HRESULT CShopUI::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	for (size_t j = 0; j < 12; j++)
	{
		m_fSlotX[j] = 344.f + 64.f * j;
		m_fSlotY[j] = 569.f;
	}
	for (size_t j = 0; j < 12; j++)
	{
		m_fSlotX[12 + j] = 344.f + 64.f * j;
		m_fSlotY[12 + j] = 638.f;
	}
	for (size_t j = 0; j < 12; j++)
	{
		m_fSlotX[24 + j] = 344.f + 64.f * j;
		m_fSlotY[24 + j] = 705.f;
	}

	rcRect = new RECT[36];
	for (size_t i = 0; i < 36; i++)
		rcRect[i] = Get_Rect(m_fSlotX[i], m_fSlotY[i], 64.f, 64.f);

	CGameObject* pInven = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Inventory");
	if (nullptr == pInven)
		return E_FAIL;

	m_pInventory = pInven;
	m_pInventory->AddRef();

	pItemInfo = m_pInventory->Get_Item();

	if (FAILED(SetUp_Resource()))
		return E_FAIL;
	
	Set_Scale(_vec3(1280.f, 768.f, 1.0f));
	Set_Position(_vec3(g_iBackCX>>1, g_iBackCY>>1, 0.f));

	CGameObject* pPlayer = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr == pPlayer)
		return E_FAIL;
	m_pPlayer = pPlayer;
	m_pPlayer->AddRef();

	CGameObject* pMouse = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Mouse");
	if (nullptr == pMouse)
		return E_FAIL;
	m_pTarget = pMouse;
	m_pTarget->AddRef();

	return NOERROR;
}

_int CShopUI::Update_GameObject(const _float& fTimeDelta)
{
	if (3 == m_pTarget->Get_Frame())
	{
		if (GetKeyState(VK_RBUTTON) & 0x8000 && m_bOnce2 == false)
		{
			m_bOnce2 = true;
			m_bShopOn = true;
			m_pPlayer->Set_StopOn();
			CSound_Manager::Get_Instance()->SoundPlay(28, FALSE);
		}
		else if (!(GetKeyState(VK_RBUTTON) & 0x8000) && m_bOnce2 == true)
			m_bOnce2 = false;
	}

	if (true == m_bShopOn)
	{
		CGameObject* pUI = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_BasicUI");
		if (nullptr == pUI)
			return E_FAIL;
		pUI->Set_UI(false);

		m_fAcc += fTimeDelta;
		if (GetKeyState(VK_ESCAPE) & 0x8000)
		{
			CSound_Manager::Get_Instance()->SoundPlay(27, FALSE);
			CGameObject* pUI = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_BasicUI");
			if (nullptr == pUI)
				return E_FAIL;
			pUI->Set_UI(true);
			m_pPlayer->Set_StopOff();
			m_bShopOn = false;
		}

		POINT		ptMouse;
		ZeroMemory(&ptMouse, sizeof(POINT));

		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		for (size_t i = 0; i < 36; i++)
		{
			if (TRUE == PtInRect(&rcRect[i], ptMouse))
			{
				if (m_bTempOn == false && m_bFirst == false && false == m_bShopItemOn && GetKeyState(VK_LBUTTON) & 0x8000 && false == m_bItemOn && m_fAcc >= 0.1f && m_bOnce == false)
				{
					if (0 != wcscmp(pItemInfo[i].szName, L"NULL"))
					{
						CSound_Manager::Get_Instance()->SoundPlay(20, FALSE);
						m_bOnce = true;
						m_bItemOn = true;
						m_iSetItem = i;
						m_fAcc = 0.f;
					}
				}
				else if (m_bTempOn == false && m_bFirst == false && false == m_bShopItemOn && GetKeyState(VK_LBUTTON) & 0x8000 && true == m_bItemOn && m_fAcc >= 0.1f && m_bOnce == false)
				{
					CSound_Manager::Get_Instance()->SoundPlay(20, FALSE);
					m_bOnce = true;
					m_fAcc = 0.f;
					m_bItemOn = false;
					if (m_iSetItem != i && 0 == wcscmp(pItemInfo[i].szName, pItemInfo[m_iSetItem].szName))
					{
						pItemInfo[i].iCount += pItemInfo[m_iSetItem].iCount;
						wcscpy(pItemInfo[m_iSetItem].szName, L"NULL");
						pItemInfo[m_iSetItem].iCount = 0;
						pItemInfo[m_iSetItem].iNum = m_iSetItem;
						pItemInfo[m_iSetItem].isHandle = false;
					}
					else
					{
						TempItem = pItemInfo[i];
						pItemInfo[i] = pItemInfo[m_iSetItem];
						pItemInfo[m_iSetItem] = TempItem;
					}
				}
				else if (!(GetKeyState(VK_LBUTTON) & 0x8000) && m_bOnce == true)
					m_bOnce = false;
			}
		}

		RECT ParsnipRect = { 52,64,1091,165 };
		RECT PotatoRect = { 52,170,1091,271 };
		RECT StrawberryRect = { 52,276,1091,377 };

		if (TRUE == PtInRect(&ParsnipRect, ptMouse))
		{
			m_bBoldOn = true;
			m_iNum = 0;
		}
		else if (TRUE == PtInRect(&PotatoRect, ptMouse))
		{
			m_bBoldOn = true;
			m_iNum = 1;
		}
		else if (TRUE == PtInRect(&StrawberryRect, ptMouse))
		{
			m_bBoldOn = true;
			m_iNum = 2;
		}
		else
			m_bBoldOn = false;


		if (true == m_bBoldOn)
		{
			CGameObject* pUI = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_BasicUI");
			if (nullptr == pUI)
				return E_FAIL;

			if (m_bItemOn == false && GetKeyState(VK_LBUTTON) & 0x8000 && m_bOnce3 == false)
			{
				m_bOnce3 = true;

				if (0 == m_iNum && (false == m_bShopItemOn || m_iItemNum == 7))
				{
					if (20 > pUI->Get_Gold())
					{

					}
					else
					{
						++m_iCount;
						if (m_iCount <= 99)
						{
							CSound_Manager::Get_Instance()->SoundPlay(14, FALSE);
							m_bShopItemOn = true;
							pUI->Set_Gold(-20);
							m_iItemNum = 7;
						}
						else
							--m_iCount;
					}
				}
				else if (1 == m_iNum && (false == m_bShopItemOn || m_iItemNum == 17))
				{
					if (50 > pUI->Get_Gold())
					{

					}
					else
					{
						++m_iCount;
						if (m_iCount <= 99)
						{
							CSound_Manager::Get_Instance()->SoundPlay(14, FALSE);
							m_bShopItemOn = true;
							pUI->Set_Gold(-50);
							m_iItemNum = 17;
						}
						else
							--m_iCount;
					}
				}
				else if (2 == m_iNum && (false == m_bShopItemOn || m_iItemNum == 18))
				{
					if (100 > pUI->Get_Gold())
					{

					}
					else
					{
						++m_iCount;
						if (m_iCount <= 99)
						{
							CSound_Manager::Get_Instance()->SoundPlay(14, FALSE);
							m_bShopItemOn = true;
							pUI->Set_Gold(-100);
							m_iItemNum = 18;
						}
						else
							--m_iCount;
					}
				}
			}
			else if (!(GetKeyState(VK_LBUTTON) & 0x8000) && m_bOnce3 == true)
				m_bOnce3 = false;
		}

		if (true == m_bShopItemOn)
		{
			for (size_t i = 0; i < 36; i++)
			{
				if (TRUE == PtInRect(&rcRect[i], ptMouse))
				{
					if (m_bFirst == false && false == m_bShopping && GetKeyState(VK_LBUTTON) & 0x8000)
					{
						if (0 == wcscmp(pItemInfo[i].szName, L"NULL"))
						{
							m_bFirst = true;
							if (7 == m_iItemNum)
							{
								m_pInventory->Buy_Item(L"ParsnipSeed", m_iCount, m_iItemNum, i);
								m_iCount = 0;
								m_iItemNum = 0;
								m_fAcc = 0.f;
								m_bShopItemOn = false;
							}
							else if (17 == m_iItemNum)
							{
								m_pInventory->Buy_Item(L"PotatoSeed", m_iCount, m_iItemNum, i);
								m_iCount = 0;
								m_iItemNum = 0;
								m_fAcc = 0.f;
								m_bShopItemOn = false;
							}
							else if (18 == m_iItemNum)
							{
								m_pInventory->Buy_Item(L"StrawberrySeed", m_iCount, m_iItemNum, i);
								m_iCount = 0;
								m_iItemNum = 0;
								m_fAcc = 0.f;
								m_bShopItemOn = false;
							}
						}
						else if (0 != wcscmp(pItemInfo[i].szName, L"NULL"))
						{
							m_bFirst = true;
							if (pItemInfo[i].iNum == m_iItemNum)
							{
								m_bShopItemOn = false;
								pItemInfo[i].iCount += m_iCount;
								m_iCount = 0;
							}
							else
							{
								TempItem = pItemInfo[i];
								m_bShopping = true;
								m_bTempOn = true;

								if (7 == m_iItemNum)
								{
									m_pInventory->Buy_Item(L"ParsnipSeed", m_iCount, m_iItemNum, i);
									m_iCount = 0;
									m_iItemNum = 0;
									m_fAcc = 0.f;
									m_bShopItemOn = false;
								}
								else if (17 == m_iItemNum)
								{
									m_pInventory->Buy_Item(L"PotatoSeed", m_iCount, m_iItemNum, i);
									m_iCount = 0;
									m_iItemNum = 0;
									m_fAcc = 0.f;
									m_bShopItemOn = false;
								}
								else if (18 == m_iItemNum)
								{
									m_pInventory->Buy_Item(L"StrawberrySeed", m_iCount, m_iItemNum, i);
									m_iCount = 0;
									m_iItemNum = 0;
									m_fAcc = 0.f;
									m_bShopItemOn = false;
								}
							}
						}
					}
				}
			}
		}

		if (m_bTempOn == true)
		{
			for (size_t i = 0; i < 36; i++)
			{
				if (TRUE == PtInRect(&rcRect[i], ptMouse))
				{
					if (GetKeyState(VK_LBUTTON) & 0x8000 && m_bFirst == false)
					{
						if (0 != wcscmp(pItemInfo[i].szName, L"NULL"))
						{
							CSound_Manager::Get_Instance()->SoundPlay(20, FALSE);
							m_bFirst = true;
							if (pItemInfo[i].iNum == TempItem.iNum)
							{
								m_bShopItemOn = false;
								m_bTempOn = false;
								m_bShopping = false;
								pItemInfo[i].iCount += TempItem.iCount;
							}
							else
							{
								TempItem2 = pItemInfo[i];
								pItemInfo[i] = TempItem;
								TempItem = TempItem2;
							}
						}
						else
						{
							pItemInfo[i] = TempItem;
							m_bTempOn = false;
							m_bFirst = true;
							m_bShopping = false;
						}
					}
				}
			}
		}



		if (!(GetKeyState(VK_LBUTTON) & 0x8000))
		{
			m_bFirst = false;
		}

		RECT SellRect = { 52,64,1091, 487 };
		if (m_bItemOn == true && GetKeyState(VK_LBUTTON) & 0x8000)
		{
			if (TRUE == PtInRect(&SellRect, ptMouse))
			{
				CSound_Manager::Get_Instance()->SoundPlay(14, FALSE);
				CGameObject* pUI = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_BasicUI");
				if (nullptr == pUI)
					return E_FAIL;

				if (0 == wcscmp(pItemInfo[m_iSetItem].szName, L"ParsnipSeed"))
					pUI->Set_Gold(10 * pItemInfo[m_iSetItem].iCount);
				else if (0 == wcscmp(pItemInfo[m_iSetItem].szName, L"Parsnip"))
					pUI->Set_Gold(40 * pItemInfo[m_iSetItem].iCount);
				else if (0 == wcscmp(pItemInfo[m_iSetItem].szName, L"PotatoSeed"))
					pUI->Set_Gold(25 * pItemInfo[m_iSetItem].iCount);
				else if (0 == wcscmp(pItemInfo[m_iSetItem].szName, L"Potato"))
					pUI->Set_Gold(100 * pItemInfo[m_iSetItem].iCount);
				else if (0 == wcscmp(pItemInfo[m_iSetItem].szName, L"StrawberrySeed"))
					pUI->Set_Gold(50 * pItemInfo[m_iSetItem].iCount);
				else if (0 == wcscmp(pItemInfo[m_iSetItem].szName, L"Strawberry"))
					pUI->Set_Gold(180 * pItemInfo[m_iSetItem].iCount);
				else if (0 == wcscmp(pItemInfo[m_iSetItem].szName, L"Bass"))
					pUI->Set_Gold(100 * pItemInfo[m_iSetItem].iCount);
				else if (0 == wcscmp(pItemInfo[m_iSetItem].szName, L"Salmon"))
					pUI->Set_Gold(100 * pItemInfo[m_iSetItem].iCount);
				else if (0 == wcscmp(pItemInfo[m_iSetItem].szName, L"Sunfish"))
					pUI->Set_Gold(100 * pItemInfo[m_iSetItem].iCount);
				else if (0 == wcscmp(pItemInfo[m_iSetItem].szName, L"Branch"))
					pUI->Set_Gold(10 * pItemInfo[m_iSetItem].iCount);
				else if (0 == wcscmp(pItemInfo[m_iSetItem].szName, L"Stone"))
					pUI->Set_Gold(10 * pItemInfo[m_iSetItem].iCount);
				else if (0 == wcscmp(pItemInfo[m_iSetItem].szName, L"Weed"))
					pUI->Set_Gold(10 * pItemInfo[m_iSetItem].iCount);
				else if (0 == wcscmp(pItemInfo[m_iSetItem].szName, L"Copper"))
					pUI->Set_Gold(100 * pItemInfo[m_iSetItem].iCount);
				else if (0 == wcscmp(pItemInfo[m_iSetItem].szName, L"Slime"))
					pUI->Set_Gold(20 * pItemInfo[m_iSetItem].iCount);
				else if (0 == wcscmp(pItemInfo[m_iSetItem].szName, L"Chest"))
					pUI->Set_Gold(500 * pItemInfo[m_iSetItem].iCount);

				m_bItemOn = false;
				m_bOnce = false;
				wcscpy(pItemInfo[m_iSetItem].szName, L"NULL");
				pItemInfo[m_iSetItem].iCount = 0;
				pItemInfo[m_iSetItem].iNum = m_iSetItem;
				pItemInfo[m_iSetItem].isHandle = false;
			}
		}
	}

	return _int();
}

_int CShopUI::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (true == m_bShopOn)
	{
		if (nullptr != m_pRender_Manager)
			m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_UI, this);
	}

	return CGameObject::LastUpdate_GameObject(fTimeDelta);
}

void CShopUI::Render_GameObject()
{
	if (nullptr == m_pBuffer ||
		nullptr == m_pIcon ||
		nullptr == m_pShader)
		return;

	LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return;
	pEffect->AddRef();

	Set_Scale(_vec3(1280.f, 768.f, 1.0f));
	Set_Position(_vec3(g_iBackCX >> 1, g_iBackCY >> 1, 0.f));
	m_pShopUI->SetUp_OnShader(pEffect, "g_BaseTexture", 0);
	//pEffect->SetFloat("m_fNum", m_fNum);
	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);
	m_pBuffer->Render_Buffer(m_matTransform);
	pEffect->EndPass();
	pEffect->End();

	for (size_t i = 0; i < 12; i++)
	{
		if (0 != wcscmp(pItemInfo[i].szName, L"NULL"))
		{
			Set_Scale(_vec3(56.f, 56.0f, 1.f));
			Set_Position(_vec3(344.f + 64.f * i, 569.f, 0.f));
			m_pIcon[pItemInfo[i].iNum]->SetUp_OnShader(pEffect, "g_BaseTexture", m_fFrame);
			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(0);
			m_pBuffer->Render_Buffer(m_matTransform);
			pEffect->EndPass();
			pEffect->End();

			if (10 <= pItemInfo[i].iCount)
			{
				Set_Scale(_vec3(24.f, 24.0f, 1.f));
				Set_Position(_vec3(344.f + 17.f + 64.f * i, 569.f + 26.f, 0.f));
				m_pSmallNum->SetUp_OnShader(pEffect, "g_BaseTexture", (_uint)(pItemInfo[i].iCount) / 10);
				pEffect->Begin(nullptr, 0);
				pEffect->BeginPass(0);
				m_pBuffer->Render_Buffer(m_matTransform);
				pEffect->EndPass();
				pEffect->End();
			}
			if (1 != pItemInfo[i].iCount)
			{
				Set_Scale(_vec3(24.f, 24.0f, 1.f));
				Set_Position(_vec3(344.f + 31.f + 64.f * i, 569.f + 26.f, 0.f));
				m_pSmallNum->SetUp_OnShader(pEffect, "g_BaseTexture", (_uint)(pItemInfo[i].iCount) % 10);
				pEffect->Begin(nullptr, 0);
				pEffect->BeginPass(0);
				m_pBuffer->Render_Buffer(m_matTransform);
				pEffect->EndPass();
				pEffect->End();
			}
		}
	}

	for (size_t i = 12; i < 24; i++)
	{
		if (0 != wcscmp(pItemInfo[i].szName, L"NULL"))
		{
			Set_Scale(_vec3(56.f, 56.0f, 1.f));
			Set_Position(_vec3(344.f + 64.f * (i-12), 638.f, 0.f));
			m_pIcon[pItemInfo[i].iNum]->SetUp_OnShader(pEffect, "g_BaseTexture", m_fFrame);
			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(0);
			m_pBuffer->Render_Buffer(m_matTransform);
			pEffect->EndPass();
			pEffect->End();

			if (10 <= pItemInfo[i].iCount)
			{
				Set_Scale(_vec3(24.f, 24.0f, 1.f));
				Set_Position(_vec3(344.f + 17.f + 64.f * (i - 12), 638.f + 26.f, 0.f));
				m_pSmallNum->SetUp_OnShader(pEffect, "g_BaseTexture", (_uint)(pItemInfo[i].iCount) / 10);
				pEffect->Begin(nullptr, 0);
				pEffect->BeginPass(0);
				m_pBuffer->Render_Buffer(m_matTransform);
				pEffect->EndPass();
				pEffect->End();
			}
			if (1 != pItemInfo[i].iCount)
			{
				Set_Scale(_vec3(24.f, 24.0f, 1.f));
				Set_Position(_vec3(344.f + 31.f + 64.f * (i - 12), 638.f + 26.f, 0.f));
				m_pSmallNum->SetUp_OnShader(pEffect, "g_BaseTexture", (_uint)(pItemInfo[i].iCount) % 10);
				pEffect->Begin(nullptr, 0);
				pEffect->BeginPass(0);
				m_pBuffer->Render_Buffer(m_matTransform);
				pEffect->EndPass();
				pEffect->End();
			}
		}
	}

	for (size_t i = 24; i < 36; i++)
	{
		if (0 != wcscmp(pItemInfo[i].szName, L"NULL"))
		{
			Set_Scale(_vec3(56.f, 56.0f, 1.f));
			Set_Position(_vec3(344.f + 64.f * (i - 24), 705.f, 0.f));
			m_pIcon[pItemInfo[i].iNum]->SetUp_OnShader(pEffect, "g_BaseTexture", m_fFrame);
			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(0);
			m_pBuffer->Render_Buffer(m_matTransform);
			pEffect->EndPass();
			pEffect->End();

			if (10 <= pItemInfo[i].iCount)
			{
				Set_Scale(_vec3(24.f, 24.0f, 1.f));
				Set_Position(_vec3(344.f + 17.f + 64.f *(i - 24), 705.f + 26.f, 0.f));
				m_pSmallNum->SetUp_OnShader(pEffect, "g_BaseTexture", (_uint)(pItemInfo[i].iCount) / 10);
				pEffect->Begin(nullptr, 0);
				pEffect->BeginPass(0);
				m_pBuffer->Render_Buffer(m_matTransform);
				pEffect->EndPass();
				pEffect->End();
			}
			if (1 != pItemInfo[i].iCount)
			{
				Set_Scale(_vec3(24.f, 24.0f, 1.f));
				Set_Position(_vec3(344.f + 31.f + 64.f *(i - 24), 705.f + 26.f, 0.f));
				m_pSmallNum->SetUp_OnShader(pEffect, "g_BaseTexture", (_uint)(pItemInfo[i].iCount) % 10);
				pEffect->Begin(nullptr, 0);
				pEffect->BeginPass(0);
				m_pBuffer->Render_Buffer(m_matTransform);
				pEffect->EndPass();
				pEffect->End();
			}
		}
	}

	// GOLD
	CGameObject* pUI = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_BasicUI");
	if (nullptr == pUI)
		return;

	m_iGold = pUI->Get_Gold();

	Set_Scale(_vec3(32.f, 32.0f, 1.f));

	if (0 == m_iGold / 10)
		m_iGoldCheck = 1;
	else if (0 == m_iGold / 100)
		m_iGoldCheck = 2;
	else if (0 == m_iGold / 1000)
		m_iGoldCheck = 3;
	else if (0 == m_iGold / 10000)
		m_iGoldCheck = 4;
	else if (0 == m_iGold / 100000)
		m_iGoldCheck = 5;
	else if (0 == m_iGold / 1000000)
		m_iGoldCheck = 6;
	else if (0 == m_iGold / 10000000)
		m_iGoldCheck = 7;
	else if (0 == m_iGold / 100000000)
		m_iGoldCheck = 8;

	if (m_iGoldCheck >= 8)
	{
		Set_Position(_vec3(68.05f + (0 * 24), 540.f, 0.f));
		m_pBigNum->SetUp_OnShader(pEffect, "g_BaseTexture", m_iGold / 10000000);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);
		m_pBuffer->Render_Buffer(m_matTransform);
		pEffect->EndPass();
		pEffect->End();
	}

	if (m_iGoldCheck >= 8 || m_iGoldCheck >= 7)
	{
		Set_Position(_vec3(68.05f + (1 * 24), 540.f, 0.f));
		m_pBigNum->SetUp_OnShader(pEffect, "g_BaseTexture", (m_iGold / 1000000) % 10);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);
		m_pBuffer->Render_Buffer(m_matTransform);
		pEffect->EndPass();
		pEffect->End();
	}

	if (m_iGoldCheck >= 8 || m_iGoldCheck >= 7 || m_iGoldCheck >= 6)
	{
		Set_Position(_vec3(68.05f + (2 * 24), 540.f, 0.f));
		m_pBigNum->SetUp_OnShader(pEffect, "g_BaseTexture", (m_iGold / 100000) % 10);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);
		m_pBuffer->Render_Buffer(m_matTransform);
		pEffect->EndPass();
		pEffect->End();
	}

	if (m_iGoldCheck >= 8 || m_iGoldCheck >= 7 || m_iGoldCheck >= 6 || m_iGoldCheck >= 5)
	{
		Set_Position(_vec3(68.05f + (3 * 24), 540.f, 0.f));
		m_pBigNum->SetUp_OnShader(pEffect, "g_BaseTexture", (m_iGold / 10000) % 10);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);
		m_pBuffer->Render_Buffer(m_matTransform);
		pEffect->EndPass();
		pEffect->End();
	}

	if (m_iGoldCheck >= 8 || m_iGoldCheck >= 7 || m_iGoldCheck >= 6 || m_iGoldCheck >= 5 || m_iGoldCheck >= 4)
	{
		Set_Position(_vec3(68.05f + (4 * 24), 540.f, 0.f));
		m_pBigNum->SetUp_OnShader(pEffect, "g_BaseTexture", m_iGold / 1000 % 10);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);
		m_pBuffer->Render_Buffer(m_matTransform);
		pEffect->EndPass();
		pEffect->End();
	}

	if (m_iGoldCheck >= 8 || m_iGoldCheck >= 7 || m_iGoldCheck >= 6 || m_iGoldCheck >= 5 || m_iGoldCheck >= 4 || m_iGoldCheck >= 3)
	{
		Set_Position(_vec3(68.05f + (5 * 24), 540.f, 0.f));
		m_pBigNum->SetUp_OnShader(pEffect, "g_BaseTexture", (m_iGold / 100) % 10);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);
		m_pBuffer->Render_Buffer(m_matTransform);
		pEffect->EndPass();
		pEffect->End();
	}

	if (m_iGoldCheck >= 8 || m_iGoldCheck >= 7 || m_iGoldCheck >= 6 || m_iGoldCheck >= 5 || m_iGoldCheck >= 4 || m_iGoldCheck >= 3 || m_iGoldCheck >= 2)
	{
		Set_Position(_vec3(68.05f + (6 * 24), 540.f, 0.f));
		m_pBigNum->SetUp_OnShader(pEffect, "g_BaseTexture", (m_iGold / 10) % 10);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);
		m_pBuffer->Render_Buffer(m_matTransform);
		pEffect->EndPass();
		pEffect->End();
	}

	if (m_iGoldCheck >= 8 || m_iGoldCheck >= 7 || m_iGoldCheck >= 6 || m_iGoldCheck >= 5 || m_iGoldCheck >= 4 || m_iGoldCheck >= 3 || m_iGoldCheck >= 2 || m_iGoldCheck >= 1)
	{
		Set_Position(_vec3(68.05f + (7 * 24), 540.f, 0.f));
		m_pBigNum->SetUp_OnShader(pEffect, "g_BaseTexture", (m_iGold) % 10);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);
		m_pBuffer->Render_Buffer(m_matTransform);
		pEffect->EndPass();
		pEffect->End();
	}

	if (true == m_bBoldOn)
	{
		Set_Scale(_vec3(2048.f, 128.f, 1.f));
		Set_Position(_vec3(572.f, 115.f + m_iNum * 106, 0.f));
		m_pTexture[m_iNum]->SetUp_OnShader(pEffect, "g_BaseTexture", (_uint)m_fFrame);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);
		m_pBuffer->Render_Buffer(m_matTransform);
		pEffect->EndPass();
		pEffect->End();
	}

	if (true == m_bItemOn)
	{
		POINT		ptMouse;
		ZeroMemory(&ptMouse, sizeof(POINT));

		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		Set_Scale(_vec3(64.f, 64.0f, 1.f));
		Set_Position(_vec3(ptMouse.x + 50.f, ptMouse.y + 40.f, 0.f));
		m_pIcon[pItemInfo[m_iSetItem].iNum]->SetUp_OnShader(pEffect, "g_BaseTexture", (_uint)m_fFrame);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);
		m_pBuffer->Render_Buffer(m_matTransform);
		pEffect->EndPass();
		pEffect->End();

		if (10 <= pItemInfo[m_iSetItem].iCount)
		{
			Set_Scale(_vec3(24.f, 24.0f, 1.f));
			Set_Position(_vec3(ptMouse.x + 50.f + 18.f, ptMouse.y + 40.f + 26.f, 0.f));
			m_pSmallNum->SetUp_OnShader(pEffect, "g_BaseTexture", (_uint)(pItemInfo[m_iSetItem].iCount) / 10);
			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(0);
			m_pBuffer->Render_Buffer(m_matTransform);
			pEffect->EndPass();
			pEffect->End();
		}
		if (1 != pItemInfo[m_iSetItem].iCount)
		{
			Set_Scale(_vec3(24.f, 24.0f, 1.f));
			Set_Position(_vec3(ptMouse.x + 50.f + 33.f, ptMouse.y + 40.f + 26.f, 0.f));
			m_pSmallNum->SetUp_OnShader(pEffect, "g_BaseTexture", (_uint)(pItemInfo[m_iSetItem].iCount) % 10);
			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(0);
			m_pBuffer->Render_Buffer(m_matTransform);
			pEffect->EndPass();
			pEffect->End();
		}
	}

	if (true == m_bShopping)
	{
		POINT		ptMouse;
		ZeroMemory(&ptMouse, sizeof(POINT));

		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		Set_Scale(_vec3(64.f, 64.0f, 1.f));
		Set_Position(_vec3(ptMouse.x + 50.f, ptMouse.y + 40.f, 0.f));
		m_pIcon[TempItem.iNum]->SetUp_OnShader(pEffect, "g_BaseTexture", (_uint)m_fFrame);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);
		m_pBuffer->Render_Buffer(m_matTransform);
		pEffect->EndPass();
		pEffect->End();

		if (10 <= TempItem.iCount)
		{
			Set_Scale(_vec3(24.f, 24.0f, 1.f));
			Set_Position(_vec3(ptMouse.x + 50.f + 18.f, ptMouse.y + 40.f + 26.f, 0.f));
			m_pSmallNum->SetUp_OnShader(pEffect, "g_BaseTexture", (_uint)(TempItem.iCount) / 10);
			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(0);
			m_pBuffer->Render_Buffer(m_matTransform);
			pEffect->EndPass();
			pEffect->End();
		}
		if (1 != TempItem.iCount)
		{
			Set_Scale(_vec3(24.f, 24.0f, 1.f));
			Set_Position(_vec3(ptMouse.x + 50.f + 33.f, ptMouse.y + 40.f + 26.f, 0.f));
			m_pSmallNum->SetUp_OnShader(pEffect, "g_BaseTexture", (_uint)(TempItem.iCount) % 10);
			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(0);
			m_pBuffer->Render_Buffer(m_matTransform);
			pEffect->EndPass();
			pEffect->End();
		}
	}

	if (true == m_bShopItemOn && false == m_bShopping)
	{
		POINT		ptMouse;
		ZeroMemory(&ptMouse, sizeof(POINT));

		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		Set_Scale(_vec3(64.f, 64.0f, 1.f));
		Set_Position(_vec3(ptMouse.x + 50.f, ptMouse.y + 40.f, 0.f));

		m_pIcon[m_iItemNum]->SetUp_OnShader(pEffect, "g_BaseTexture", (_uint)m_fFrame);
		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);
		m_pBuffer->Render_Buffer(m_matTransform);
		pEffect->EndPass();
		pEffect->End();

		if (10 <= m_iCount)
		{
			Set_Scale(_vec3(24.f, 24.0f, 1.f));
			Set_Position(_vec3(ptMouse.x + 50.f+18.f, ptMouse.y + 40.f+26.f, 0.f));
			m_pSmallNum->SetUp_OnShader(pEffect, "g_BaseTexture", (_uint)m_iCount / 10);
			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(0);
			m_pBuffer->Render_Buffer(m_matTransform);
			pEffect->EndPass();
			pEffect->End();
		}
		if (1 != m_iCount)
		{
			Set_Scale(_vec3(24.f, 24.0f, 1.f));
			Set_Position(_vec3(ptMouse.x + 50.f+33.f, ptMouse.y + 40.f+26.f, 0.f));
			m_pSmallNum->SetUp_OnShader(pEffect, "g_BaseTexture", (_uint)m_iCount % 10);
			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(0);
			m_pBuffer->Render_Buffer(m_matTransform);
			pEffect->EndPass();
			pEffect->End();
		}
	}

	Safe_Release(pEffect);
}

HRESULT CShopUI::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	m_pShopUI = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_ShopUI");
	if (nullptr == m_pShopUI)
		return E_FAIL;
	m_pShopUI->AddRef();

	m_pTexture[0] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Bold_Parsnip");
	if (nullptr == m_pTexture[0])
		return E_FAIL;
	m_pTexture[0]->AddRef();

	m_pTexture[1] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Bold_Potato");
	if (nullptr == m_pTexture[1])
		return E_FAIL;
	m_pTexture[1]->AddRef();

	m_pTexture[2] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Bold_Strawberry");
	if (nullptr == m_pTexture[2])
		return E_FAIL;
	m_pTexture[2]->AddRef();

	// Icon
	m_pIcon[pItemInfo[0].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Hoe");
	if (nullptr == m_pIcon[pItemInfo[0].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[0].iNum]->AddRef();

	m_pIcon[pItemInfo[1].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Pick");
	if (nullptr == m_pIcon[pItemInfo[1].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[1].iNum]->AddRef();

	m_pIcon[pItemInfo[2].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Ax");
	if (nullptr == m_pIcon[pItemInfo[2].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[2].iNum]->AddRef();

	m_pIcon[pItemInfo[3].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Sickle");
	if (nullptr == m_pIcon[pItemInfo[3].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[3].iNum]->AddRef();

	m_pIcon[pItemInfo[4].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_WateringPot");
	if (nullptr == m_pIcon[pItemInfo[4].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[4].iNum]->AddRef();

	m_pIcon[pItemInfo[5].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Sword");
	if (nullptr == m_pIcon[pItemInfo[5].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[5].iNum]->AddRef();

	m_pIcon[pItemInfo[6].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_FishingRod");
	if (nullptr == m_pIcon[pItemInfo[6].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[6].iNum]->AddRef();

	m_pIcon[pItemInfo[7].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_ParsnipSeed");
	if (nullptr == m_pIcon[pItemInfo[7].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[7].iNum]->AddRef();

	m_pIcon[pItemInfo[8].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Parsnip");
	if (nullptr == m_pIcon[pItemInfo[8].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[8].iNum]->AddRef();

	m_pIcon[pItemInfo[9].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Strawberry");
	if (nullptr == m_pIcon[pItemInfo[9].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[9].iNum]->AddRef();

	m_pIcon[pItemInfo[10].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Potato");
	if (nullptr == m_pIcon[pItemInfo[10].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[10].iNum]->AddRef();

	m_pIcon[pItemInfo[11].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Fish_Bass");
	if (nullptr == m_pIcon[pItemInfo[11].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[11].iNum]->AddRef();

	m_pIcon[pItemInfo[12].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Fish_Salmon");
	if (nullptr == m_pIcon[pItemInfo[12].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[12].iNum]->AddRef();

	m_pIcon[pItemInfo[13].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Fish_Sunfish");
	if (nullptr == m_pIcon[pItemInfo[13].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[13].iNum]->AddRef();

	m_pIcon[pItemInfo[14].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Branch");
	if (nullptr == m_pIcon[pItemInfo[14].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[14].iNum]->AddRef();

	m_pIcon[pItemInfo[15].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Stone");
	if (nullptr == m_pIcon[pItemInfo[15].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[15].iNum]->AddRef();

	m_pIcon[pItemInfo[16].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Weed");
	if (nullptr == m_pIcon[pItemInfo[16].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[16].iNum]->AddRef();

	m_pIcon[pItemInfo[17].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_PotatoSeed");
	if (nullptr == m_pIcon[pItemInfo[17].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[17].iNum]->AddRef();

	m_pIcon[pItemInfo[18].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_StrawberrySeed");
	if (nullptr == m_pIcon[pItemInfo[18].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[18].iNum]->AddRef();

	m_pIcon[pItemInfo[19].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Copper");
	if (nullptr == m_pIcon[pItemInfo[19].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[19].iNum]->AddRef();

	m_pIcon[pItemInfo[20].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Slime");
	if (nullptr == m_pIcon[pItemInfo[20].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[20].iNum]->AddRef();

	m_pIcon[pItemInfo[21].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Chest");
	if (nullptr == m_pIcon[pItemInfo[21].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[21].iNum]->AddRef();

	m_pSmallNum = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_SmallNumber");
	if (nullptr == m_pSmallNum)
		return E_FAIL;
	m_pSmallNum->AddRef();

	m_pBigNum = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_BigNumber");
	if (nullptr == m_pBigNum)
		return E_FAIL;
	m_pBigNum->AddRef();

	// For.Shader
	m_pShader = (CShader*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Shader_Default");
	if (nullptr == m_pShader)
		return E_FAIL;
	m_pShader->AddRef();

	return NOERROR;
}

HRESULT CShopUI::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	return NOERROR;
}

_bool CShopUI::isDraw(CGameObject * pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

_matrix CShopUI::Render_TransForm(CGameObject * pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

CShopUI * CShopUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CShopUI*		pInstance = new CShopUI(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CShopUI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShopUI::Free()
{
	Safe_Delete_Array(rcRect);

	Safe_Release(m_pInventory);
	Safe_Release(m_pPlayer);
	Safe_Release(m_pTarget);

	Safe_Release(m_pShopUI);
	for (auto& pIcon : m_pIcon)
		Safe_Release(pIcon);
	for (auto& pTexture : m_pTexture)
		Safe_Release(pTexture);
	Safe_Release(m_pSmallNum);
	Safe_Release(m_pBigNum);

	Safe_Release(m_pBuffer);
	Safe_Release(m_pShader);
	
	CGameObject::Free();
}
