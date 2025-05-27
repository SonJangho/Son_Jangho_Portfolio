#include "stdafx.h"
#include "..\Headers\Chest.h"
#include "Sound_Manager.h"

CChest::CChest(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

HRESULT CChest::Ready_GameObject()
{
	CGameObject::Ready_GameObject();	

	Set_BasicItem();

	CGameObject* pTarget = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Inventory");
	if (nullptr == pTarget)
		return E_FAIL;

	m_pInventory = pTarget;
	m_pInventory->AddRef();

	pItemInfo = m_pInventory->Get_Item();

	if (FAILED(SetUp_Resource()))
		return E_FAIL;
	
	return NOERROR;
}

_int CChest::Update_GameObject(const _float & fTimeDelta)
{
	CGameObject* pChest = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Chest",0);
	
	if (true == pChest->Get_ChestOn())
	{
		CSound_Manager::Get_Instance()->SoundPlay(9, FALSE);
		m_bChestUIOn = true;
		pChest->Set_ChestOn();
	}

	if (GetKeyState(VK_ESCAPE) & 0x8000 && m_bOnce2 == false)
	{
		CSound_Manager::Get_Instance()->SoundPlay(9, FALSE);
		CGameObject* pTarget = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
		if (nullptr == pTarget)
			return E_FAIL;

		CGameObject* pUI = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_BasicUI");
		if (nullptr == pUI)
			return E_FAIL;

		pUI->Set_UI(true);

		pTarget->Set_StopOff();
		m_bOnce2 = true;
		m_bChestUIOn = false;
	}
	else if (!(GetKeyState(VK_ESCAPE) & 0x8000 && m_bOnce2 == true))
		m_bOnce2 = false;

	if (true == m_bChestUIOn)
	{
		CGameObject* pTarget = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
		if (nullptr == pTarget)
			return E_FAIL;

		CGameObject* pUI = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_BasicUI");
		if (nullptr == pUI)
			return E_FAIL;
		pUI->Set_UI(false);

		pTarget->Set_StopOn();

		POINT		ptMouse;
		ZeroMemory(&ptMouse, sizeof(POINT));

		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		for (size_t i = 0; i < 36; i++)
		{
			// Check if mouse is over an inventory slot
			if (TRUE == PtInRect(&rcRect[i], ptMouse))
			{
				// On left mouse click, move item from inventory to chest
				if (GetKeyState(VK_LBUTTON) & 0x8000 && m_fAcc >= 0.1f && m_bFirst == false)
				{
					if (0 != wcscmp(pItemInfo[i].szName, L"NULL"))
					{
						CSound_Manager::Get_Instance()->SoundPlay(20, FALSE);
						m_bFirst = true;
						// Find empty slot in chest and move item there
						for (size_t j = 0; j < 36; j++)
						{
							if (0 == wcscmp(ChestItemInfo[j].szName, L"NULL"))
							{
								ChestItemInfo[j] = pItemInfo[i];
								break;
							}
						}
						// Clear the inventory slot after moving
						wcscpy(pItemInfo[i].szName, L"NULL");
						pItemInfo[i].iCount = 0;
						pItemInfo[i].isHandle = false;
					}
				}
				// Mouse button released: reset flag
				else if (!(GetKeyState(VK_LBUTTON) & 0x8000) && m_bFirst == true)
					m_bFirst = false;
			}

			// Check if mouse is over a chest slot
			if (TRUE == PtInRect(&rcChestRect[i], ptMouse))
			{
				// On left mouse click, move item from chest to inventory
				if (GetKeyState(VK_LBUTTON) & 0x8000 && m_fAcc >= 0.1f && m_bFirst2 == false)
				{
					if (0 != wcscmp(ChestItemInfo[i].szName, L"NULL"))
					{
						CSound_Manager::Get_Instance()->SoundPlay(20, FALSE);
						m_bFirst2 = true;

						// Find empty slot in inventory and move item there
						for (size_t j = 0; j < 36; j++)
						{
							if (0 == wcscmp(pItemInfo[j].szName, L"NULL"))
							{
								pItemInfo[j] = ChestItemInfo[i];
								break;
							}
						}
						// Clear the chest slot after moving
						wcscpy(ChestItemInfo[i].szName, L"NULL");
						ChestItemInfo[i].iCount = 0;
						ChestItemInfo[i].isHandle = false;
					}
				}
				// Mouse button released: reset flag
				else if (!(GetKeyState(VK_LBUTTON) & 0x8000) && m_bFirst2 == true)
					m_bFirst2 = false;
			}
		}

	}

	m_fAcc += fTimeDelta;
	return _int();
}

_int CChest::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRender_Manager)
		return -1;

	if (true == m_bChestUIOn)
		if (FAILED(m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_INVENTORY, this)))
			return -1;

	return _int();
}

void CChest::Render_GameObject()
{
	if (nullptr == m_pTexture ||
		nullptr == m_pBuffer)
		return;

	SetUp_RenderState();

	Set_Scale(_vec3(2048.f, 1024.0f, 1.f));
	Set_Position(_vec3(g_iBackCX >> 1, g_iBackCY >> 1, 0.f));
	m_pTexture->SetUp_OnGraphicDev((_uint)m_fFrame);
	m_pBuffer->Render_Buffer(m_matTransform);

	for (size_t i = 0; i < 36; i++)
	{
		if (0 != wcscmp(pItemInfo[i].szName, L"NULL"))
		{
			Set_Scale(_vec3(64.f, 64.0f, 1.f));
			Set_Position(_vec3(m_fSlotX[i], m_fSlotY[i], 0.f));
			m_pIcon[pItemInfo[i].iNum]->SetUp_OnGraphicDev((_uint)m_fFrame);
			m_pBuffer->Render_Buffer(m_matTransform);

			if (10 <= pItemInfo[i].iCount)
			{
				Set_Scale(_vec3(24.f, 24.0f, 1.f));
				Set_Position(_vec3(m_fSlotX[i] + 17.f, m_fSlotY[i] + 26.f, 0.f));
				m_pSmallNum->SetUp_OnGraphicDev((_uint)(pItemInfo[i].iCount) / 10);
				m_pBuffer->Render_Buffer(m_matTransform);
			}
			if (1 != pItemInfo[i].iCount)
			{
				Set_Scale(_vec3(24.f, 24.0f, 1.f));
				Set_Position(_vec3(m_fSlotX[i] + 31.f, m_fSlotY[i] + 26.f, 0.f));
				m_pSmallNum->SetUp_OnGraphicDev((_uint)(pItemInfo[i].iCount) % 10);
				m_pBuffer->Render_Buffer(m_matTransform);
			}
		}
	}

	for (size_t i = 0; i < 36; i++)
	{
		if (0 != wcscmp(ChestItemInfo[i].szName, L"NULL"))
		{
			Set_Scale(_vec3(64.f, 64.0f, 1.f));
			Set_Position(_vec3(m_fSlotX2[i], m_fSlotY2[i], 0.f));
			m_pIcon[ChestItemInfo[i].iNum]->SetUp_OnGraphicDev((_uint)m_fFrame);
			m_pBuffer->Render_Buffer(m_matTransform);

			if (10 <= ChestItemInfo[i].iCount)
			{
				Set_Scale(_vec3(24.f, 24.0f, 1.f));
				Set_Position(_vec3(m_fSlotX2[i] + 17.f, m_fSlotY2[i] + 26.f, 0.f));
				m_pSmallNum->SetUp_OnGraphicDev((_uint)(ChestItemInfo[i].iCount) / 10);
				m_pBuffer->Render_Buffer(m_matTransform);
			}
			if (1 != ChestItemInfo[i].iCount)
			{
				Set_Scale(_vec3(24.f, 24.0f, 1.f));
				Set_Position(_vec3(m_fSlotX2[i] + 31.f, m_fSlotY2[i] + 26.f, 0.f));
				m_pSmallNum->SetUp_OnGraphicDev((_uint)(ChestItemInfo[i].iCount) % 10);
				m_pBuffer->Render_Buffer(m_matTransform);
			}
		}
	}
	

	Reset_RenderState();
}

HRESULT CChest::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture_Inventory
	m_pTexture = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_ChestUI");
	if (nullptr == m_pTexture)
		return E_FAIL;
	m_pTexture->AddRef();

	m_pIcon[0] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Hoe");
	if (nullptr == m_pIcon[0])
		return E_FAIL;
	m_pIcon[0]->AddRef();

	m_pIcon[1] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Pick");
	if (nullptr == m_pIcon[1])
		return E_FAIL;
	m_pIcon[1];

	m_pIcon[2] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Ax");
	if (nullptr == m_pIcon[2])
		return E_FAIL;
	m_pIcon[2]->AddRef();

	m_pIcon[3] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Sickle");
	if (nullptr == m_pIcon[3])
		return E_FAIL;
	m_pIcon[3]->AddRef();

	m_pIcon[4] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_WateringPot");
	if (nullptr == m_pIcon[4])
		return E_FAIL;
	m_pIcon[4]->AddRef();

	m_pIcon[5] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Sword");
	if (nullptr == m_pIcon[5])
		return E_FAIL;
	m_pIcon[5]->AddRef();

	m_pIcon[6] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_FishingRod");
	if (nullptr == m_pIcon[6])
		return E_FAIL;
	m_pIcon[6]->AddRef();

	m_pIcon[7] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_ParsnipSeed");
	if (nullptr == m_pIcon[7])
		return E_FAIL;
	m_pIcon[7]->AddRef();

	m_pIcon[8] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Parsnip");
	if (nullptr == m_pIcon[8])
		return E_FAIL;
	m_pIcon[8]->AddRef();

	m_pIcon[9] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Strawberry");
	if (nullptr == m_pIcon[9])
		return E_FAIL;
	m_pIcon[9]->AddRef();

	m_pIcon[10] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Potato");
	if (nullptr == m_pIcon[10])
		return E_FAIL;
	m_pIcon[10]->AddRef();

	m_pIcon[11] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Fish_Bass");
	if (nullptr == m_pIcon[11])
		return E_FAIL;
	m_pIcon[11]->AddRef();

	m_pIcon[12] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Fish_Salmon");
	if (nullptr == m_pIcon[12])
		return E_FAIL;
	m_pIcon[12]->AddRef();

	m_pIcon[13] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Fish_Sunfish");
	if (nullptr == m_pIcon[13])
		return E_FAIL;
	m_pIcon[13]->AddRef();

	m_pIcon[14] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Branch");
	if (nullptr == m_pIcon[14])
		return E_FAIL;
	m_pIcon[14]->AddRef();

	m_pIcon[15] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Stone");
	if (nullptr == m_pIcon[15])
		return E_FAIL;
	m_pIcon[15]->AddRef();

	m_pIcon[16] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Weed");
	if (nullptr == m_pIcon[16])
		return E_FAIL;
	m_pIcon[16]->AddRef();

	m_pIcon[17] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_PotatoSeed");
	if (nullptr == m_pIcon[17])
		return E_FAIL;
	m_pIcon[17]->AddRef();

	m_pIcon[18] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_StrawberrySeed");
	if (nullptr == m_pIcon[18])
		return E_FAIL;
	m_pIcon[18]->AddRef();

	m_pIcon[19] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Copper");
	if (nullptr == m_pIcon[19])
		return E_FAIL;
	m_pIcon[19]->AddRef();

	m_pIcon[20] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Slime");
	if (nullptr == m_pIcon[20])
		return E_FAIL;
	m_pIcon[20]->AddRef();

	m_pIcon[21] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Chest");
	if (nullptr == m_pIcon[21])
		return E_FAIL;
	m_pIcon[21]->AddRef();

	m_pSmallNum = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_SmallNumber");
	if (nullptr == m_pSmallNum)
		return E_FAIL;
	m_pSmallNum->AddRef();

	return NOERROR;
}

HRESULT CChest::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return NOERROR;
}

HRESULT CChest::Reset_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return NOERROR;
}

HRESULT CChest::Set_BasicItem()
{
	ChestItemInfo = new ITEMINFO[36];
	ZeroMemory(ChestItemInfo, sizeof(ITEMINFO));

	for (size_t i = 0; i < 36; i++)
	{
		wcscpy(ChestItemInfo[i].szName, L"NULL");
		ChestItemInfo[i].iCount = 0;
		ChestItemInfo[i].iNum = i;
		ChestItemInfo[i].isHandle = false;
	}

	for (size_t j = 0; j < 12; j++)
	{
		m_fSlotX[j] = 268.f + 64.f * j;
		m_fSlotY[j] = 500.f;
	}

	for (size_t j = 0; j < 12; j++)
	{
		m_fSlotX[12 + j] = 268.f + 64.f * j;
		m_fSlotY[12 + j] = 568.f;
	}

	for (size_t j = 0; j < 12; j++)
	{
		m_fSlotX[24 + j] = 268.f + 64.f * j;
		m_fSlotY[24 + j] = 636.f;
	}

	for (size_t i = 0; i < 36; i++)
	{
		rcRect[i] = Get_Rect(m_fSlotX[i], m_fSlotY[i], 64.f, 64.f);
	}

	//////////////////////
	for (size_t j = 0; j < 12; j++)
	{
		m_fSlotX2[j] = 264.f + 64.f * j;
		m_fSlotY2[j] = 188.f;
	}

	for (size_t j = 0; j < 12; j++)
	{ 
		m_fSlotX2[12 + j] = 264.f + 64.f * j;
		m_fSlotY2[12 + j] = 256.f;
	}

	for (size_t j = 0; j < 12; j++)
	{
		m_fSlotX2[24 + j] = 264.f + 64.f * j;
		m_fSlotY2[24 + j] = 324.f;
	}

	for (size_t i = 0; i < 36; i++)
	{
		rcChestRect[i] = Get_Rect(m_fSlotX2[i], m_fSlotY2[i], 64.f, 64.f);
	}

	return NOERROR;
}

CGameObject * CChest::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CChest*		pInstance = new CChest(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CChest Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChest::Free(void)
{
	Safe_Delete_Array(ChestItemInfo);

	Safe_Release(m_pInventory);

	for (auto& m_pIcon : m_pIcon)
		Safe_Release(m_pIcon);
	Safe_Release(m_pSmallNum);
	Safe_Release(m_pTexture);
	Safe_Release(m_pBuffer);

	CGameObject::Free();
}
