#include "stdafx.h"
#include "..\Headers\Inventory.h"
#include "Sound_Manager.h"

CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

HRESULT CInventory::Ready_GameObject()
{
	CGameObject::Ready_GameObject();	

	Set_BasicItem();

	if (FAILED(SetUp_Resource()))
		return E_FAIL;
	
	return NOERROR;
}

_int CInventory::Update_GameObject(const _float & fTimeDelta)
{
	// Open inventory when 'I' key is pressed
	if (GetKeyState('I') & 0x8000 && m_bOnce == false)
	{
		// Play open sound
		CSound_Manager::Get_Instance()->SoundPlay(28, FALSE);

		m_bOnce = true;
		m_bInventoryOn = true; // Inventory window ON
		m_fFrame = 0.f;	       // Set to inventory tab
	}
	else if (!(GetKeyState('I') & 0x8000) && m_bOnce == true)
		m_bOnce = false;

	// Close inventory when ESC is pressed
	if (GetKeyState(VK_ESCAPE) & 0x8000 && m_bOnce2 == false)
	{
		// Play close sound
		CSound_Manager::Get_Instance()->SoundPlay(27, FALSE);

		// Find the player object to control the player's state (pause/unpause)
		CGameObject* pTarget = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
		if (nullptr == pTarget)
			return E_FAIL;

		// Resume player movement
		pTarget->Set_StopOff();
		m_bOnce2 = true;
		// Inventory window OFF
		m_bInventoryOn = false;
	}
	else if (!(GetKeyState(VK_ESCAPE) & 0x8000 && m_bOnce2 == true))
		m_bOnce2 = false;

	if(true == m_bInventoryOn)
	{
		// Freeze player movement when inventory is open
		CGameObject* pTarget = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
		if (nullptr == pTarget)
			return E_FAIL;
		pTarget->Set_StopOn();

		// Get current mouse position in client coordinates
		POINT		ptMouse;
		ZeroMemory(&ptMouse, sizeof(POINT));
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);
		
		// Inventory/Relationship/Crafting tab rectangles
		RECT INVENRECT = Get_Rect(297,40,64,64);
		RECT HOGAMRECT = Get_Rect(361,40,64,64);
		RECT MAKERECT = Get_Rect(425,40,64,64);
		// Tab switching: check which tab is clicked and set m_fFrame accordingly

		if (TRUE == PtInRect(&INVENRECT, ptMouse))
		{
			if (GetKeyState(VK_LBUTTON) & 0x8000 && m_bRect1 == false)
			{
				CSound_Manager::Get_Instance()->SoundPlay(20, FALSE);
				m_bRect1 = true;
				m_fFrame = 0;
			}
			else if (!(GetKeyState(VK_LBUTTON) & 0x8000) && m_bRect1 == true)
				m_bRect1 = false;
		}
		else if (TRUE == PtInRect(&HOGAMRECT, ptMouse))
		{
			if (GetKeyState(VK_LBUTTON) & 0x8000 && m_bRect2 == false)
			{
				CSound_Manager::Get_Instance()->SoundPlay(20, FALSE);
				m_bRect2 = true;
				m_fFrame = 1;
			}
			else if (!(GetKeyState(VK_LBUTTON) & 0x8000) && m_bRect2 == true)
				m_bRect2 = false;
		}
		else if (TRUE == PtInRect(&MAKERECT, ptMouse))
		{
			if (GetKeyState(VK_LBUTTON) & 0x8000 && m_bRect3 == false)
			{
				CSound_Manager::Get_Instance()->SoundPlay(20, FALSE);
				m_bRect3 = true;
				m_fFrame = 2;
			}
			else if (!(GetKeyState(VK_LBUTTON) & 0x8000) && m_bRect3 == true)
				m_bRect3 = false;
		}

		// Inventory tab is active
		if (0 == m_fFrame)
		{
			// If mouse is over slot i
			for (size_t i = 0; i < 36; i++)
			{
				// (A) Pick up an item from slot
				if (TRUE == PtInRect(&rcRect[i], ptMouse))
				{
					if (GetKeyState(VK_LBUTTON) & 0x8000 && false == m_bItemOn && m_fAcc >= 0.1f && m_bFirst == false)
					{
						if (0 != wcscmp(pItemInfo[i].szName, L"NULL"))
						{
							CSound_Manager::Get_Instance()->SoundPlay(20, FALSE);
							m_bFirst = true;
							m_bItemOn = true;
							m_iSetItem = i; // Remember picked slot
							m_fAcc = 0.f;
						}
					}
					// (B) Drop the dragged item (swap or combine)
					else if (GetKeyState(VK_LBUTTON) & 0x8000 && true == m_bItemOn && m_fAcc >= 0.1f && m_bFirst == false)
					{
						m_bFirst = true;
						m_fAcc = 0.f;
						m_bItemOn = false;
						if (m_iSetItem != i && 0 == wcscmp(pItemInfo[i].szName, pItemInfo[m_iSetItem].szName))
						{
							// Combine same type items
							CSound_Manager::Get_Instance()->SoundPlay(20, FALSE);
							pItemInfo[i].iCount += pItemInfo[m_iSetItem].iCount;
							wcscpy(pItemInfo[m_iSetItem].szName, L"NULL");
							pItemInfo[m_iSetItem].iCount = 0;
							pItemInfo[m_iSetItem].iNum = m_iSetItem;
							pItemInfo[m_iSetItem].isHandle = false;
						}
						else
						{
							// Swap different type items
							CSound_Manager::Get_Instance()->SoundPlay(20, FALSE);
							TempItem = pItemInfo[i];
							pItemInfo[i] = pItemInfo[m_iSetItem];
							pItemInfo[m_iSetItem] = TempItem;
						}
					}
					// (C) Reset drag flags when mouse is released
					else if (!(GetKeyState(VK_LBUTTON) & 0x8000) && m_bFirst == true)
						m_bFirst = false;
				}
			}
		}
		// 호감도
		else if (1 == m_fFrame)
		{
			
		}
		// 제작
		else if (2 == m_fFrame)
		{
			// Get current mouse position in screen coordinates
			POINT		ptMouse;
			ZeroMemory(&ptMouse, sizeof(POINT));

			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);

			// Define the RECT area for the crafting (Chest) button in the UI
			RECT CHEST = Get_Rect(297, 186, 64, 92);

			m_bChestOn = false;

			// Check if mouse is inside the CHEST button area
			if (TRUE == PtInRect(&CHEST, ptMouse))
				m_bChestOn = true;
	
			m_bOK = false;
			m_bOK2 = false;

			// Check if player has enough Wood and Copper for crafting.
			for (size_t i = 0; i < 36; i++)
			{
				if (0 == wcscmp(pItemInfo[i].szName, L"Branch"))
				{
					if (pItemInfo[i].iCount >= 20)
						m_bOK = true;
				}
				if (0 == wcscmp(pItemInfo[i].szName, L"Copper"))
				{
					if (pItemInfo[i].iCount >= 5)
						m_bOK2 = true;
				}
			}

			if (true == m_bChestOn)
			{
				if (GetKeyState(VK_LBUTTON) & 0x8000 && false == m_bOnce4)
				{
					m_bOnce4 = true;

					/*CGameObject* pTarget = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
					if (nullptr == pTarget)
						return E_FAIL;*/

					if (m_bOK == true && m_bOK2 == true)
					{
						CSound_Manager::Get_Instance()->SoundPlay(20, FALSE);
						Use_Item(L"Branch", 20);
						Use_Item(L"Copper", 5);
						Add_Item(L"Chest", 1, 21);
						m_bOK = false;
						m_bOK2 = false;
					}
					else
					{
						//실패 
					}
						
				}
				else if (!(GetKeyState(VK_LBUTTON) & 0x8000) && true == m_bOnce4)
					m_bOnce4 = false;
			}


			for (size_t i = 0; i < 36; i++)
			{
				if (TRUE == PtInRect(&rcMakeRect[i], ptMouse))
				{
					if (GetKeyState(VK_LBUTTON) & 0x8000 && false == m_bItemOn && m_fAcc >= 0.1f && m_bFirst == false)
					{
						if (0 != wcscmp(pItemInfo[i].szName, L"NULL"))
						{
							CSound_Manager::Get_Instance()->SoundPlay(20, FALSE);
							m_bFirst = true;
							m_bItemOn = true;
							m_iSetItem = i;
							m_fAcc = 0.f;
						}
					}
					else if (GetKeyState(VK_LBUTTON) & 0x8000 && true == m_bItemOn && m_fAcc >= 0.1f && m_bFirst == false)
					{
						m_bFirst = true;
						m_fAcc = 0.f;
						m_bItemOn = false;
						if (m_iSetItem != i && 0 == wcscmp(pItemInfo[i].szName, pItemInfo[m_iSetItem].szName))
						{
							CSound_Manager::Get_Instance()->SoundPlay(20, FALSE);
							pItemInfo[i].iCount += pItemInfo[m_iSetItem].iCount;
							wcscpy(pItemInfo[m_iSetItem].szName, L"NULL");
							pItemInfo[m_iSetItem].iCount = 0;
							pItemInfo[m_iSetItem].iNum = m_iSetItem;
							pItemInfo[m_iSetItem].isHandle = false;
						}
						else
						{
							CSound_Manager::Get_Instance()->SoundPlay(20, FALSE);
							TempItem = pItemInfo[i];
							pItemInfo[i] = pItemInfo[m_iSetItem];
							pItemInfo[m_iSetItem] = TempItem;
						}
					}
					else if (!(GetKeyState(VK_LBUTTON) & 0x8000) && m_bFirst == true)
						m_bFirst = false;
				}
			}
		}
	}

	m_fAcc += fTimeDelta;
	return _int();
}

_int CInventory::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRender_Manager)
		return -1;

	if (true == m_bInventoryOn)
		if (FAILED(m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_INVENTORY, this)))
			return -1;

	return _int();
}

void CInventory::Render_GameObject()
{
	if (nullptr == m_pTexture ||
		nullptr == m_pBuffer)
		return;

	SetUp_RenderState();

	Set_Scale(_vec3(2048.f, 1024.0f, 1.f));
	Set_Position(_vec3(g_iBackCX >> 1, g_iBackCY >> 1, 0.f));
	m_pTexture->SetUp_OnGraphicDev((_uint)m_fFrame);
	m_pBuffer->Render_Buffer(m_matTransform);

	if (0 == m_fFrame)
	{
		for (size_t i = 0; i < 12; i++)
		{
			if (0 != wcscmp(pItemInfo[i].szName, L"NULL"))
			{
				Set_Scale(_vec3(64.f, 64.0f, 1.f));
				Set_Position(_vec3(294.2f + 64.f * i, 144.f, 0.f));
				m_pIcon[pItemInfo[i].iNum]->SetUp_OnGraphicDev((_uint)m_fFrame);
				m_pBuffer->Render_Buffer(m_matTransform);

				if (10 <= pItemInfo[i].iCount)
				{
					Set_Scale(_vec3(24.f, 24.0f, 1.f));
					Set_Position(_vec3(294.2f + 17.f + 64.f * i, 144.f + 26.f, 0.f));
					m_pSmallNum->SetUp_OnGraphicDev((_uint)(pItemInfo[i].iCount) / 10);
					m_pBuffer->Render_Buffer(m_matTransform);
				}
				if (1 != pItemInfo[i].iCount)
				{
					Set_Scale(_vec3(24.f, 24.0f, 1.f));
					Set_Position(_vec3(294.2f + 31.f + 64.f * i, 144.f + 26.f, 0.f));
					m_pSmallNum->SetUp_OnGraphicDev((_uint)(pItemInfo[i].iCount) % 10);
					m_pBuffer->Render_Buffer(m_matTransform);
				}
			}
		}

		for (size_t i = 12; i < 24; i++)
		{
			if (0 != wcscmp(pItemInfo[i].szName, L"NULL"))
			{
				Set_Scale(_vec3(64.f, 64.0f, 1.f));
				Set_Position(_vec3(294.2f + 64.f * (i - 12), 144.f + 96.f, 0.f));
				m_pIcon[pItemInfo[i].iNum]->SetUp_OnGraphicDev((_uint)m_fFrame);
				m_pBuffer->Render_Buffer(m_matTransform);

				if (10 <= pItemInfo[i].iCount)
				{
					Set_Scale(_vec3(24.f, 24.0f, 1.f));
					Set_Position(_vec3(294.2f + 17.f + 64.f * (i - 12), 144.f + 96.f + 26.f, 0.f));
					m_pSmallNum->SetUp_OnGraphicDev((_uint)(pItemInfo[i].iCount) / 10);
					m_pBuffer->Render_Buffer(m_matTransform);
				}
				if (1 != pItemInfo[i].iCount)
				{
					Set_Scale(_vec3(24.f, 24.0f, 1.f));
					Set_Position(_vec3(294.2f + 31.f + 64.f * (i - 12), 144.f + 96.f + 26.f, 0.f));
					m_pSmallNum->SetUp_OnGraphicDev((_uint)(pItemInfo[i].iCount) % 10);
					m_pBuffer->Render_Buffer(m_matTransform);
				}
			}
		}

		for (size_t i = 24; i < 36; i++)
		{
			if (0 != wcscmp(pItemInfo[i].szName, L"NULL"))
			{
				Set_Scale(_vec3(64.f, 64.0f, 1.f));
				Set_Position(_vec3(294.2f + 64.f * (i - 24), 144.f + 96.f + 70.3f, 0.f));
				m_pIcon[pItemInfo[i].iNum]->SetUp_OnGraphicDev((_uint)m_fFrame);
				m_pBuffer->Render_Buffer(m_matTransform);

				if (10 <= pItemInfo[i].iCount)
				{
					Set_Scale(_vec3(24.f, 24.0f, 1.f));
					Set_Position(_vec3(294.2f + 17.f + 64.f * (i - 24), 144.f + 96.f + 70.3f + 26.f, 0.f));
					m_pSmallNum->SetUp_OnGraphicDev((_uint)(pItemInfo[i].iCount) / 10);
					m_pBuffer->Render_Buffer(m_matTransform);
				}
				if (1 != pItemInfo[i].iCount)
				{
					Set_Scale(_vec3(24.f, 24.0f, 1.f));
					Set_Position(_vec3(294.2f + 31.f + 64.f * (i - 24), 144.f + 96.f + 70.3f + 26.f, 0.f));
					m_pSmallNum->SetUp_OnGraphicDev((_uint)(pItemInfo[i].iCount) % 10);
					m_pBuffer->Render_Buffer(m_matTransform);
				}
			}
		}

		if (true == m_bItemOn)
		{
			POINT		ptMouse;
			ZeroMemory(&ptMouse, sizeof(POINT));

			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);

			Set_Scale(_vec3(64.f, 64.0f, 1.f));
			Set_Position(_vec3(ptMouse.x + 50.f, ptMouse.y + 40.f, 0.f));

			m_pIcon[pItemInfo[m_iSetItem].iNum]->SetUp_OnGraphicDev((_uint)m_fFrame);
			m_pBuffer->Render_Buffer(m_matTransform);

			if (10 <= pItemInfo[m_iSetItem].iCount)
			{
				Set_Scale(_vec3(24.f, 24.0f, 1.f));
				Set_Position(_vec3(ptMouse.x + 50.f + 18.f, ptMouse.y + 40.f + 26.f, 0.f));
				m_pSmallNum->SetUp_OnGraphicDev((_uint)(pItemInfo[m_iSetItem].iCount) / 10);
				m_pBuffer->Render_Buffer(m_matTransform);
			}
			if (1 != pItemInfo[m_iSetItem].iCount)
			{
				Set_Scale(_vec3(24.f, 24.0f, 1.f));
				Set_Position(_vec3(ptMouse.x + 50.f + 33.f, ptMouse.y + 40.f + 26.f, 0.f));
				m_pSmallNum->SetUp_OnGraphicDev((_uint)(pItemInfo[m_iSetItem].iCount) % 10);
				m_pBuffer->Render_Buffer(m_matTransform);
			}
		}
	}
	// 호감도
	else if (1 == m_fFrame)
	{

	}
	// 제작
	else if (2 == m_fFrame)
	{
		for (size_t i = 0; i < 12; i++)
		{
			if (0 != wcscmp(pItemInfo[i].szName, L"NULL"))
			{
				Set_Scale(_vec3(64.f, 64.0f, 1.f));
				Set_Position(_vec3(296.f + 64.f * i, 460.f, 0.f));
				m_pIcon[pItemInfo[i].iNum]->SetUp_OnGraphicDev((_uint)0);
				m_pBuffer->Render_Buffer(m_matTransform);

				if (10 <= pItemInfo[i].iCount)
				{
					Set_Scale(_vec3(24.f, 24.0f, 1.f));
					Set_Position(_vec3(296.f + 17.f + 64.f * i, 460.f + 26.f, 0.f));
					m_pSmallNum->SetUp_OnGraphicDev((_uint)(pItemInfo[i].iCount) / 10);
					m_pBuffer->Render_Buffer(m_matTransform);
				}
				if (1 != pItemInfo[i].iCount)
				{
					Set_Scale(_vec3(24.f, 24.0f, 1.f));
					Set_Position(_vec3(296.f + 31.f + 64.f * i, 460.f + 26.f, 0.f));
					m_pSmallNum->SetUp_OnGraphicDev((_uint)(pItemInfo[i].iCount) % 10);
					m_pBuffer->Render_Buffer(m_matTransform);
				}
			}
		}

		for (size_t i = 12; i < 24; i++)
		{
			if (0 != wcscmp(pItemInfo[i].szName, L"NULL"))
			{
				Set_Scale(_vec3(64.f, 64.0f, 1.f));
				Set_Position(_vec3(296.f + 64.f * (i - 12), 530.f, 0.f));
				m_pIcon[pItemInfo[i].iNum]->SetUp_OnGraphicDev((_uint)0);
				m_pBuffer->Render_Buffer(m_matTransform);

				if (10 <= pItemInfo[i].iCount)
				{
					Set_Scale(_vec3(24.f, 24.0f, 1.f));
					Set_Position(_vec3(296.f + 17.f + 64.f * (i - 12), 530.f + 26.f, 0.f));
					m_pSmallNum->SetUp_OnGraphicDev((_uint)(pItemInfo[i].iCount) / 10);
					m_pBuffer->Render_Buffer(m_matTransform);
				}
				if (1 != pItemInfo[i].iCount)
				{
					Set_Scale(_vec3(24.f, 24.0f, 1.f));
					Set_Position(_vec3(296.f + 31.f + 64.f * (i - 12), 530.f + 26.f, 0.f));
					m_pSmallNum->SetUp_OnGraphicDev((_uint)(pItemInfo[i].iCount) % 10);
					m_pBuffer->Render_Buffer(m_matTransform);
				}
			}
		}

		for (size_t i = 24; i < 36; i++)
		{
			if (0 != wcscmp(pItemInfo[i].szName, L"NULL"))
			{
				Set_Scale(_vec3(64.f, 64.0f, 1.f));
				Set_Position(_vec3(296.f + 64.f * (i - 24), 600.f, 0.f));
				m_pIcon[pItemInfo[i].iNum]->SetUp_OnGraphicDev((_uint)0);
				m_pBuffer->Render_Buffer(m_matTransform);

				if (10 <= pItemInfo[i].iCount)
				{
					Set_Scale(_vec3(24.f, 24.0f, 1.f));
					Set_Position(_vec3(296.f + 17.f + 64.f * (i - 24), 600.f + 26.f, 0.f));
					m_pSmallNum->SetUp_OnGraphicDev((_uint)(pItemInfo[i].iCount) / 10);
					m_pBuffer->Render_Buffer(m_matTransform);
				}
				if (1 != pItemInfo[i].iCount)
				{
					Set_Scale(_vec3(24.f, 24.0f, 1.f));
					Set_Position(_vec3(296.f + 31.f + 64.f * (i - 24), 600.f + 26.f, 0.f));
					m_pSmallNum->SetUp_OnGraphicDev((_uint)(pItemInfo[i].iCount) % 10);
					m_pBuffer->Render_Buffer(m_matTransform);
				}
			}
		}

		if (true == m_bItemOn)
		{
			POINT		ptMouse;
			ZeroMemory(&ptMouse, sizeof(POINT));

			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);

			Set_Scale(_vec3(64.f, 64.0f, 1.f));
			Set_Position(_vec3(ptMouse.x + 50.f, ptMouse.y + 40.f, 0.f));

			m_pIcon[pItemInfo[m_iSetItem].iNum]->SetUp_OnGraphicDev((_uint)0);
			m_pBuffer->Render_Buffer(m_matTransform);

			if (10 <= pItemInfo[m_iSetItem].iCount)
			{
				Set_Scale(_vec3(24.f, 24.0f, 1.f));
				Set_Position(_vec3(ptMouse.x + 50.f + 18.f, ptMouse.y + 40.f + 26.f, 0.f));
				m_pSmallNum->SetUp_OnGraphicDev((_uint)(pItemInfo[m_iSetItem].iCount) / 10);
				m_pBuffer->Render_Buffer(m_matTransform);
			}
			if (1 != pItemInfo[m_iSetItem].iCount)
			{
				Set_Scale(_vec3(24.f, 24.0f, 1.f));
				Set_Position(_vec3(ptMouse.x + 50.f + 33.f, ptMouse.y + 40.f + 26.f, 0.f));
				m_pSmallNum->SetUp_OnGraphicDev((_uint)(pItemInfo[m_iSetItem].iCount) % 10);
				m_pBuffer->Render_Buffer(m_matTransform);
			}
		}

		if (m_bOK == true && m_bOK2 == true)
		{
			Set_Scale(_vec3(64.f, 128.0f, 1.f));
			Set_Position(_vec3(297.f, 176.f, 0.f));
			m_pTex[1]->SetUp_OnGraphicDev((_uint)18);
			m_pBuffer->Render_Buffer(m_matTransform);
		}

		POINT		ptMouse;
		ZeroMemory(&ptMouse, sizeof(POINT));

		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		if (m_bChestOn == true)
		{
			Set_Scale(_vec3(512.f, 256.0f, 1.f));
			Set_Position(_vec3(ptMouse.x + 230.f, ptMouse.y + 140.f, 0.f));
			m_pTex[0]->SetUp_OnGraphicDev((_uint)0);
			m_pBuffer->Render_Buffer(m_matTransform);
		}
	}

	Reset_RenderState();
}

HRESULT CInventory::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture_Inventory
	m_pTexture = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Inventory");
	if (nullptr == m_pTexture)
		return E_FAIL;
	m_pTexture->AddRef();

	m_pTex[0] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_ChestBox");
	if (nullptr == m_pTex[0])
		return E_FAIL;
	m_pTex[0]->AddRef();

	m_pTex[1] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FarmObject");
	if (nullptr == m_pTex[1])
		return E_FAIL;
	m_pTex[1]->AddRef();

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

	return NOERROR;
}

HRESULT CInventory::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return NOERROR;
}

HRESULT CInventory::Reset_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return NOERROR;
}

HRESULT CInventory::Set_BasicItem()
{
	pItemInfo = new ITEMINFO[36];
	ZeroMemory(pItemInfo, sizeof(ITEMINFO));

	wcscpy(pItemInfo[0].szName, L"Hoe");
	pItemInfo[0].iCount = 1;
	pItemInfo[0].iNum = 0;
	pItemInfo[0].isHandle = false;

	wcscpy(pItemInfo[1].szName, L"Pick");
	pItemInfo[1].iCount = 1;
	pItemInfo[1].iNum = 1;
	pItemInfo[1].isHandle = false;

	wcscpy(pItemInfo[2].szName, L"Ax");
	pItemInfo[2].iCount = 1;
	pItemInfo[2].iNum = 2;
	pItemInfo[2].isHandle = false;

	wcscpy(pItemInfo[3].szName, L"Sickle");
	pItemInfo[3].iCount = 1;
	pItemInfo[3].iNum = 3;
	pItemInfo[3].isHandle = false;

	wcscpy(pItemInfo[4].szName, L"WateringPot");
	pItemInfo[4].iCount = 1;
	pItemInfo[4].iNum = 4;
	pItemInfo[4].isHandle = false;

	wcscpy(pItemInfo[5].szName, L"Sword");
	pItemInfo[5].iCount = 1;
	pItemInfo[5].iNum = 5;
	pItemInfo[5].isHandle = false;

	wcscpy(pItemInfo[6].szName, L"FishingRod");
	pItemInfo[6].iCount = 1;
	pItemInfo[6].iNum = 6;
	pItemInfo[6].isHandle = false;

	wcscpy(pItemInfo[7].szName, L"ParsnipSeed");
	pItemInfo[7].iCount = 15;
	pItemInfo[7].iNum = 7;
	pItemInfo[7].isHandle = true;

	for (size_t i = 8; i < 36; i++)
	{
		wcscpy(pItemInfo[i].szName, L"NULL");
		pItemInfo[i].iCount = 0;
		pItemInfo[i].iNum = i;
		pItemInfo[i].isHandle = false;
	}

	for (size_t j = 0; j < 12; j++)
	{
		m_fSlotX[j] = 5 + 294.2f + 64.f * j;
		m_fSlotY[j] = 9 + 144.f;
	}

	for (size_t j = 0; j < 12; j++)
	{
		m_fSlotX[12 + j] = 5 + 294.2f + 64.f * j;
		m_fSlotY[12 + j] = 9 + 144.f + 95.2f;
	}

	for (size_t j = 0; j < 12; j++)
	{
		m_fSlotX[24 + j] = 5 + 294.2f + 64.f * j;
		m_fSlotY[24 + j] = 9 + 144.f + 95.2f + 70.9f;
	}

	rcRect = new RECT[36];

	for (size_t i = 0; i < 36; i++)
	{
		rcRect[i] = Get_Rect(m_fSlotX[i], m_fSlotY[i], 64.f, 64.f);
	}

	//////////////////////
	for (size_t j = 0; j < 12; j++)
	{
		m_fSlotX2[j] = 5 + 296.f + 64.f * j;
		m_fSlotY2[j] = 9 + 460.f;
	}

	for (size_t j = 0; j < 12; j++)
	{ 
		m_fSlotX2[12 + j] = 5 + 296.f + 64.f * j;
		m_fSlotY2[12 + j] = 9 + 530.f;
	}

	for (size_t j = 0; j < 12; j++)
	{
		m_fSlotX2[24 + j] = 5 + 296.f + 64.f * j;
		m_fSlotY2[24 + j] = 9 + 600.f;
	}

	rcMakeRect = new RECT[36];
	for (size_t i = 0; i < 36; i++)
	{
		rcMakeRect[i] = Get_Rect(m_fSlotX2[i], m_fSlotY2[i], 64.f, 64.f);
	}

	return NOERROR;
}

CGameObject * CInventory::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CInventory*		pInstance = new CInventory(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CInventory Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInventory::Free(void)
{
	Safe_Delete_Array(pItemInfo);
	Safe_Delete_Array(rcRect);
	Safe_Delete_Array(rcMakeRect);

	for (auto& pTex : m_pTex)
		Safe_Release(pTex);
	for (auto& m_pIcon : m_pIcon)
		Safe_Release(m_pIcon);
	Safe_Release(m_pSmallNum);
	Safe_Release(m_pTexture);
	Safe_Release(m_pBuffer);

	CGameObject::Free();
}
