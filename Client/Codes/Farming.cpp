#include "stdafx.h"
#include "..\Headers\Farming.h"
#include "Camera.h"
#include "Management.h"
#include "Sound_Manager.h"

CFarming::CFarming(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

HRESULT CFarming::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	if (FAILED(SetUp_StateDesc()))
		return E_FAIL;

	if (FAILED(SetUp_Resource()))
		return E_FAIL;

	CGameObject* pPlayer = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr == pPlayer)
		return E_FAIL;
	m_pPlayer = pPlayer;
	m_pPlayer->AddRef();

	CGameObject* pHoe = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Tools");
	if (nullptr == pHoe)
		return E_FAIL;
	m_pHoe = pHoe;
	m_pHoe->AddRef();

	CGameObject* pWateringPot = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Tools",5);
	if (nullptr == pWateringPot)
		return E_FAIL;
	m_pWateringPot = pWateringPot;
	m_pWateringPot->AddRef();

	CGameObject* pTarget = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_BasicUI");
	if (nullptr == pTarget)
		return E_FAIL;
	m_pUI = pTarget;
	m_pUI->AddRef();

	return NOERROR;
}

_int CFarming::Update_GameObject(const _float& fTimeDelta)
{
	if (SCENE_FARM == CManagement::GetInstance()->Get_Scene())
	{
		// ¶¥ ÆÄ±â
		Hoe_Dig();
		
		CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");

		POINT		ptMouse;
		ZeroMemory(&ptMouse, sizeof(POINT));

		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);
		m_iDirtX = ((pCamera->Get_X() - 640) * 0.015625 + ptMouse.x* 0.015625);
		m_iDirtY = ((pCamera->Get_Y() - 384) * 0.015625 + ptMouse.y* 0.015625);

		m_PlayerX = m_pPlayer->Get_X() * 0.015625;
		m_PlayerY = (m_pPlayer->Get_Y() + 32) * 0.015625;

		// ¾¾¾Ñ ½É±â
		Seed_Planting();

		// ¹° ÁÖ±â
		Watering_Crops();

		// ¼öÈ® ÇÏ±â
		Harvest_Crops();
	}

	if (true == m_pUI->Get_NextDay())
	{
		for (auto& pCrop : m_vecCrop)
		{
			if (WATER == pCrop->DirtInfo->DirtState)
			{
				if (0 == pCrop->GrowthStage || 1 == pCrop->GrowthStage)
					pCrop->GrowthStage = 2;
				else
					pCrop->GrowthStage += 1;

				if (pCrop->GrowthStage >= m_pStateDesc[pCrop->Crop].iNumTexture)
					pCrop->GrowthStage -= 1;

				pCrop->DirtInfo->DirtState = BASIC;
			}
			else if (DEADCROP==pCrop->Crop)
			{
			}
			else
			{
				pCrop->Crop = DEADCROP;
				int RanNum = rand() % 4;
				pCrop->GrowthStage = RanNum;
			}
		}

		for (auto& pWater : m_vecWater)
			Safe_Delete(pWater);
		m_vecWater.clear();

		m_pUI->Set_NextDay();
	}

	if (true == m_bWateringStart)
	{
		m_fWaterFrame += 10 * fTimeDelta * 2.f;
		if (m_fWaterFrame > 10.f)
		{
			m_bWateringStart = false;
			m_fWaterFrame = 0.f;
		}
	}

	if(true == m_pUI->Get_RainDay())
	{ 
		for (auto& pDirt : m_vecDirt)
		{
			pDirt->DirtState = WATER;

			DIRTINFO*	WaterInfo = new DIRTINFO;
			ZeroMemory(WaterInfo, sizeof(DIRTINFO));

			WaterInfo->DirtX = pDirt->DirtX;
			WaterInfo->DirtY = pDirt->DirtY;
			WaterInfo->DirtState = WATER;

			for (auto& pWater : m_vecWater)
			{
				if (WaterInfo->DirtX == pWater->DirtX)
					if (WaterInfo->DirtY == pWater->DirtY)
						m_bExist2 = true;
			}
			
			if (false == m_bExist2)
				m_vecWater.push_back(WaterInfo);
			else if (true == m_bExist2)
				Safe_Delete(WaterInfo);

			m_bExist2 = false;
		}
	}
	return _int();
}

_int CFarming::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (SCENE_FARM == CManagement::GetInstance()->Get_Scene())
	{
		if (nullptr != m_pRender_Manager)
			m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_TWO, this);
	}
	return CGameObject::LastUpdate_GameObject(fTimeDelta);
}

void CFarming::Render_GameObject()
{
	SetUp_RenderState();

	CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");

	Set_Scale(_vec3(64.f, 64.f, 1.f));
	for (auto& pDirt : m_vecDirt)
	{
		Set_Position(_vec3(pDirt->DirtX * 64.f + 32.f, pDirt->DirtY * 64.f + 32.f, 0.f));
		if (true == isDraw(pCamera))
		{
			Calculate_Dirt(pDirt);

			TempMatrix = Render_TransForm(pCamera);
			m_pTexture[BASICDIRT]->SetUp_OnGraphicDev(m_iDirt);
			m_pBuffer->Render_Buffer(TempMatrix);
		}
	}
	for (auto& pWater : m_vecWater)
	{
		Set_Position(_vec3(pWater->DirtX * 64.f + 32.f, pWater->DirtY * 64.f + 32.f, 0.f));
		if (true == isDraw(pCamera))
		{
			if (pWater->DirtState == WATER)
			{
				Calculate_WaterDirt(pWater);

				TempMatrix = Render_TransForm(pCamera);
				m_pTexture[WATERDIRT]->SetUp_OnGraphicDev(m_iDirt);
				m_pBuffer->Render_Buffer(TempMatrix);
			}
		}
	}

	if (1 != m_vecCrop.empty())
	{
		Set_Scale(_vec3(64.f, 128.f, 1.f));
		for (auto& pCrop : m_vecCrop)
		{
			Set_Position(_vec3(pCrop->DirtInfo->DirtX * 64.f + 32.f, pCrop->DirtInfo->DirtY * 64.f, 0.f));
			if (true == isDraw(pCamera))
			{
				TempMatrix = Render_TransForm(pCamera);
				m_pTexture[pCrop->Crop]->SetUp_OnGraphicDev(pCrop->GrowthStage);
				m_pBuffer->Render_Buffer(TempMatrix);
			}
		}
	}

	if (m_bWateringStart == true)
	{
		Set_Scale(_vec3(64.f, 64.f, 1.f));
		Set_Position(_vec3(m_iWaterX * 64.f + 32.f, m_iWaterY * 64.f + 32.f, 0.f));
		TempMatrix = Render_TransForm(pCamera);
		m_pWatering->SetUp_OnGraphicDev(m_fWaterFrame);
		m_pBuffer->Render_Buffer(TempMatrix);
	}

	Reset_RenderState();
}

HRESULT CFarming::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture
	m_pTexture[BASICDIRT] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_BasicDirt");
	if (nullptr == m_pTexture[BASICDIRT])
		return E_FAIL;
	m_pTexture[BASICDIRT]->AddRef();

	m_pTexture[WATERDIRT] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_WaterDirt");
	if (nullptr == m_pTexture[WATERDIRT])
		return E_FAIL;
	m_pTexture[WATERDIRT]->AddRef();

	m_pTexture[PARSNIP] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Parsnip");
	if (nullptr == m_pTexture[PARSNIP])
		return E_FAIL;
	m_pTexture[PARSNIP]->AddRef();

	m_pTexture[STRAWBERRY] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Strawberry");
	if (nullptr == m_pTexture[STRAWBERRY])
		return E_FAIL;
	m_pTexture[STRAWBERRY]->AddRef();

	m_pTexture[POTATO] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Potato");
	if (nullptr == m_pTexture[POTATO])
		return E_FAIL;
	m_pTexture[POTATO]->AddRef();

	m_pTexture[DEADCROP] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_DeadCrop");
	if (nullptr == m_pTexture[DEADCROP])
		return E_FAIL;
	m_pTexture[DEADCROP]->AddRef();

	m_pWatering = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Watering");
	if (nullptr == m_pWatering)
		return E_FAIL;
	m_pWatering->AddRef();

	// For.Shader
	m_pShader = (CShader*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Shader_Default");
	if (nullptr == m_pShader)
		return E_FAIL;
	m_pShader->AddRef();

	return NOERROR;
}
HRESULT CFarming::SetUp_StateDesc()
{
	m_pStateDesc = new STATEDESC[END];
	ZeroMemory(m_pStateDesc, sizeof(STATEDESC) * END);

	m_pStateDesc[PARSNIP].eState = PARSNIP;
	m_pStateDesc[PARSNIP].iNumTexture = 6;

	m_pStateDesc[STRAWBERRY].eState = STRAWBERRY;
	m_pStateDesc[STRAWBERRY].iNumTexture = 7;

	m_pStateDesc[POTATO].eState = POTATO;
	m_pStateDesc[POTATO].iNumTexture = 7;

	return NOERROR;
}
HRESULT CFarming::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return NOERROR;
}

HRESULT CFarming::Reset_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return NOERROR;
}
void CFarming::Hoe_Dig()
{
	if (true == m_pHoe->Get_ToolOn())
	{
		if (m_pPlayer->Get_State() && 3 <= m_pPlayer->Get_Frame() && m_bFirst == false)
		{
			m_bFirst = true;
			if (1 == m_pPlayer->Get_State())
			{
				m_iDirtX = _int(m_pPlayer->Get_X()) * 0.015625;
				m_iDirtY = _int(m_pPlayer->Get_Y() + 30 - 64) * 0.015625;
			}
			else if (2 == m_pPlayer->Get_State())
			{
				m_iDirtX = _int(m_pPlayer->Get_X() + 64) * 0.015625;
				m_iDirtY = _int(m_pPlayer->Get_Y() + 30) * 0.015625;
			}
			else if (3 == m_pPlayer->Get_State())
			{
				m_iDirtX = _int(m_pPlayer->Get_X()) * 0.015625;
				m_iDirtY = _int(m_pPlayer->Get_Y() + 30 + 64) * 0.015625;
			}
			else if (4 == m_pPlayer->Get_State())
			{
				m_iDirtX = _int(m_pPlayer->Get_X() - 64) * 0.015625;
				m_iDirtY = _int(m_pPlayer->Get_Y() + 30) * 0.015625;
			}

			DIRTINFO*	DirtInfo = new DIRTINFO;
			ZeroMemory(DirtInfo, sizeof(DIRTINFO));

			DirtInfo->DirtX = m_iDirtX;
			DirtInfo->DirtY = m_iDirtY;
			DirtInfo->DirtState = BASIC;

			for (auto& pDirt : m_vecDirt)
			{
				if (pDirt->DirtX == m_iDirtX)
				{
					if (pDirt->DirtY == m_iDirtY)
					{
						m_bExist = true;
						break;
					}
				}
			}
			if (false == m_bExist)
			{
				CSound_Manager::Get_Instance()->SoundPlay(16, FALSE);
				m_vecDirt.push_back(DirtInfo);
			}
			else if (true == m_bExist)
				Safe_Delete(DirtInfo);

			m_bExist = false;
		}
		else if (3 > m_pPlayer->Get_Frame() && m_bFirst == true)
			m_bFirst = false;
	}
	
}
void CFarming::Seed_Planting()
{
	m_bCheck = false;
	// Check if the selected item is a valid seed
	if (0 == wcscmp(m_pUI->Get_ItemOn().szName, L"ParsnipSeed") ||
		0 == wcscmp(m_pUI->Get_ItemOn().szName, L"StrawberrySeed") ||
		0 == wcscmp(m_pUI->Get_ItemOn().szName, L"PotatoSeed"))
	{
		// Check if player is next to a plantable dirt
		for (auto& pFarming : m_vecDirt)
		{
			// Check if dirt X is adjacent to the player
			if (pFarming->DirtX == m_PlayerX + 1 ||
				pFarming->DirtX == m_PlayerX ||
				pFarming->DirtX == m_PlayerX - 1)
			{
			// Check if dirt Y is adjacent to the player
				if (pFarming->DirtY == m_PlayerY + 1 ||
					pFarming->DirtY == m_PlayerY ||
					pFarming->DirtY == m_PlayerY - 1)
				{
				// Check if this dirt is the selected tile
					if (pFarming->DirtX == m_iDirtX)
					{
						if (pFarming->DirtY == m_iDirtY)
						{
							// This tile is plantable
							m_bCheck = true;
							break;
						}
					}
					else
						m_bCheck = false;
				}
			}
		}

		if (true == m_bCheck)
		{
			// If left mouse button is clicked and not already clicked
			if (GetKeyState(VK_LBUTTON) & 0x8000 && m_isClicked == false)
			{
				m_isClicked = true;
				// Create a new crop info struct for the planted crop
				CROPINFO*	CropInfo = new CROPINFO;
				ZeroMemory(CropInfo, sizeof(CROPINFO));
				// Find the dirt tile where the crop will be planted
				for (auto& pDirt : m_vecDirt)
				{
					if (pDirt->DirtX == m_iDirtX)
						if (pDirt->DirtY == m_iDirtY)
						{
							CropInfo->DirtInfo = pDirt;
							break;
						}
				}
				// Check if there is already a crop on this dirt tile
				for (auto& pCrop : m_vecCrop)
				{
					if (pCrop->DirtInfo->DirtX == CropInfo->DirtInfo->DirtX)
						if (pCrop->DirtInfo->DirtY == CropInfo->DirtInfo->DirtY)
						{
							m_bExist = true;
							break;
						}
				}

				if (false == m_bExist)
				{
					CGameObject* pInventory = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Inventory");
					if (nullptr == pInventory)
						return;

					if (0 == wcscmp(m_pUI->Get_ItemOn().szName, L"ParsnipSeed"))
					{
						CropInfo->Crop = PARSNIP;
						pInventory->Use_Item(L"ParsnipSeed", 1);
					}
					else if (0 == wcscmp(m_pUI->Get_ItemOn().szName, L"StrawberrySeed"))
					{
						CropInfo->Crop = STRAWBERRY;
						pInventory->Use_Item(L"StrawberrySeed", 1);
					}
					else if (0 == wcscmp(m_pUI->Get_ItemOn().szName, L"PotatoSeed"))
					{
						CropInfo->Crop = POTATO;
						pInventory->Use_Item(L"PotatoSeed", 1);
					}

					_int RanNum = rand() % 2;
					CropInfo->GrowthStage = RanNum;
					CSound_Manager::Get_Instance()->SoundPlay(20, FALSE);
					m_vecCrop.push_back(CropInfo);
				}
				else if (true == m_bExist)
					Safe_Delete(CropInfo);

				m_bExist = false;
			}
			else if (!(GetKeyState(VK_LBUTTON) & 0x8000) && m_isClicked == true)
			{
				m_isClicked = false;
			}
		}
	}
}
void CFarming::Watering_Crops()
{
	if (true == m_pWateringPot->Get_ToolOn())
	{
		if (m_pPlayer->Get_State() && 1 <= m_pPlayer->Get_Frame() && m_bFirst2 == false)
		{
			CSound_Manager::Get_Instance()->SoundPlay(29, FALSE);
			m_bFirst2 = true;
			if (1 == m_pPlayer->Get_State())
			{
				m_iDirtX = _int(m_pPlayer->Get_X()) * 0.015625;
				m_iDirtY = _int(m_pPlayer->Get_Y() + 30 - 64) * 0.015625;
			}
			else if (2 == m_pPlayer->Get_State())
			{
				m_iDirtX = _int(m_pPlayer->Get_X() + 64) * 0.015625;
				m_iDirtY = _int(m_pPlayer->Get_Y() + 30) * 0.015625;
			}
			else if (3 == m_pPlayer->Get_State())
			{
				m_iDirtX = _int(m_pPlayer->Get_X()) * 0.015625;
				m_iDirtY = _int(m_pPlayer->Get_Y() + 30 + 64) * 0.015625;
			}
			else if (4 == m_pPlayer->Get_State())
			{
				m_iDirtX = _int(m_pPlayer->Get_X() - 64) * 0.015625;
				m_iDirtY = _int(m_pPlayer->Get_Y() + 30) * 0.015625;
			}

			for (auto& pCrop : m_vecCrop)
			{
				if (pCrop->DirtInfo->DirtX == m_iDirtX)
				{
					if (pCrop->DirtInfo->DirtY == m_iDirtY)
					{
						m_bWateringStart = true;
						m_iWaterX = m_iDirtX;
						m_iWaterY = m_iDirtY;
						pCrop->DirtInfo->DirtState = WATER;

						DIRTINFO*	WaterInfo = new DIRTINFO;
						ZeroMemory(WaterInfo, sizeof(DIRTINFO));
						
						WaterInfo->DirtX = m_iDirtX;
						WaterInfo->DirtY = m_iDirtY;
						WaterInfo->DirtState = WATER;

						for (auto& pDirt : m_vecWater)
						{
							if (pDirt->DirtX == m_iDirtX)
							{
								if (pDirt->DirtY == m_iDirtY)
								{
									m_bExist = true;
									break;
								}
							}
						}
						if (false == m_bExist)
							m_vecWater.push_back(WaterInfo);
						else if (true == m_bExist)
							Safe_Delete(WaterInfo);

						m_bExist = false;
					}
				}
			}
		}
		else if (1 > m_pPlayer->Get_Frame() && m_bFirst2 == true)
			m_bFirst2 = false;
	}
}
void CFarming::Harvest_Crops()
{
	m_bCheck2 = false;

	if (1 != m_vecCrop.empty())
	{
		for (auto& pCrop : m_vecCrop)
		{
			if (pCrop->GrowthStage + 1 >= m_pStateDesc[pCrop->Crop].iNumTexture)
			{
				if (pCrop->DirtInfo->DirtX == m_PlayerX + 1 || 
					pCrop->DirtInfo->DirtX == m_PlayerX || 
					pCrop->DirtInfo->DirtX == m_PlayerX - 1)
				{
					if (pCrop->DirtInfo->DirtY == m_PlayerY + 1 || 
						pCrop->DirtInfo->DirtY == m_PlayerY || 
						pCrop->DirtInfo->DirtY == m_PlayerY - 1)
					{
						if (pCrop->DirtInfo->DirtX == m_iDirtX)
						{
							if (pCrop->DirtInfo->DirtY == m_iDirtY)
							{
								m_bCheck2 = true;
								break;
							}
							else
								m_bCheck2 = false;
						}
						else
							m_bCheck2 = false;
					}
				}
			}
			else
				m_bCheck2 = false;
		}
	}
	else
		m_bCheck2 = false;
	
	// If right-clicking and ready to harvest
	if ((GetKeyState(VK_RBUTTON) & 0x8000) && false == m_isClicked2 && m_bCheck2 == true)
	{
		m_isClicked2 = true;
		
		// Find the crop to harvest
		for (auto& pCrop : m_vecCrop)
		{
			if (pCrop->GrowthStage + 1 >= m_pStateDesc[pCrop->Crop].iNumTexture)
				if (pCrop->DirtInfo->DirtX == m_iDirtX)
					if (pCrop->DirtInfo->DirtY == m_iDirtY)
					{
						m_bErase = true;
						break;
					}
			++m_iDeleteIndex;
		}

		// If found, play sound, add crop item to inventory, and erase crop
		if (m_bErase == true)
		{
			CSound_Manager::Get_Instance()->SoundPlay(13, FALSE);
			CGameObject* pInventory = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Inventory");
			if (nullptr == pInventory)
				return;

			m_bErase = false;
			// Add harvested item based on crop type
			if (PARSNIP == m_vecCrop[m_iDeleteIndex]->Crop)
			{
				m_iCrop = 1;
				pInventory->Add_Item(L"Parsnip", 1, 8);
			}
			else if (STRAWBERRY == m_vecCrop[m_iDeleteIndex]->Crop)
			{
				m_iCrop = 2;
				pInventory->Add_Item(L"Strawberry", 4, 9);
			}
			else if (POTATO == m_vecCrop[m_iDeleteIndex]->Crop)
			{
				m_iCrop = 3;
				pInventory->Add_Item(L"Potato", 3, 10);
			}
			// Delete crop data from memory and vector
			Safe_Delete(m_vecCrop[m_iDeleteIndex]);
			m_vecCrop.erase(m_vecCrop.begin() + m_iDeleteIndex);
		}
		m_iDeleteIndex = 0;
	}
	// Mouse up handling
	else if (!(GetKeyState(VK_RBUTTON) & 0x8000) && true == m_isClicked2)
	{
		m_isClicked2 = false;
	}
}
void CFarming::Calculate_Dirt(DIRTINFO* pDirt)
{
	m_iDirt = 0;
	for (auto& pDirt2 : m_vecDirt)
	{
		if (pDirt->DirtX + 1 == pDirt2->DirtX)
		{
			if (pDirt->DirtY == pDirt2->DirtY)
			{
				m_iDirt = 10;
				for (auto& pDirt3 : m_vecDirt)
				{
					if (pDirt->DirtY + 1 == pDirt3->DirtY)
					{
						if (pDirt->DirtX == pDirt3->DirtX)
						{
							m_iDirt = 1;
							for (auto& pDirt4 : m_vecDirt)
							{
								if (pDirt->DirtY - 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 4;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtX - 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtX - 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 2;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtY - 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
					else if (pDirt->DirtY - 1 == pDirt3->DirtY)
					{
						if (pDirt->DirtX == pDirt3->DirtX)
						{
							m_iDirt = 7;
							for (auto& pDirt4 : m_vecDirt)
							{
								if (pDirt->DirtY + 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 4;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtX - 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtX - 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 8;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtY + 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
					else if (pDirt->DirtX - 1 == pDirt3->DirtX)
					{
						if (pDirt->DirtY == pDirt3->DirtY)
						{
							m_iDirt = 11;
							for (auto& pDirt4 : m_vecDirt)
							{
								if (pDirt->DirtY + 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 2;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtY - 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtY - 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 8;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtY + 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else if (pDirt->DirtX - 1 == pDirt2->DirtX)
		{
			if (pDirt->DirtY == pDirt2->DirtY)
			{
				m_iDirt = 12;
				for (auto& pDirt3 : m_vecDirt)
				{
					if (pDirt->DirtY + 1 == pDirt3->DirtY)
					{
						if (pDirt->DirtX == pDirt3->DirtX)
						{
							m_iDirt = 3;
							for (auto& pDirt4 : m_vecDirt)
							{
								if (pDirt->DirtY - 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 6;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtX + 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtX + 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 2;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtY - 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
					else if (pDirt->DirtY - 1 == pDirt3->DirtY)
					{
						if (pDirt->DirtX == pDirt3->DirtX)
						{
							m_iDirt = 9;
							for (auto& pDirt4 : m_vecDirt)
							{
								if (pDirt->DirtY + 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 6;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtX + 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtX + 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 8;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtY + 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
					else if (pDirt->DirtX + 1 == pDirt3->DirtX)
					{
						if (pDirt->DirtY == pDirt3->DirtY)
						{
							m_iDirt = 11;
							for (auto& pDirt4 : m_vecDirt)
							{
								if (pDirt->DirtY + 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 2;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtY - 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtY - 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 8;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtY + 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else if (pDirt->DirtY + 1 == pDirt2->DirtY)
		{
			if (pDirt->DirtX == pDirt2->DirtX)
			{
				m_iDirt = 13;
				for (auto& pDirt3 : m_vecDirt)
				{
					if (pDirt->DirtX + 1 == pDirt3->DirtX)
					{
						if (pDirt->DirtY == pDirt3->DirtY)
						{
							m_iDirt = 1;
							for (auto& pDirt4 : m_vecDirt)
							{
								if (pDirt->DirtX - 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 2;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtY - 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtY - 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 4;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtX - 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
					else if (pDirt->DirtX - 1 == pDirt3->DirtX)
					{
						if (pDirt->DirtY == pDirt3->DirtY)
						{
							m_iDirt = 3;
							for (auto& pDirt4 : m_vecDirt)
							{
								if (pDirt->DirtX + 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 2;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtY - 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtY - 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 6;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtX + 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
					else if (pDirt->DirtY - 1 == pDirt3->DirtY)
					{
						if (pDirt->DirtX == pDirt3->DirtX)
						{
							m_iDirt = 14;
							for (auto& pDirt4 : m_vecDirt)
							{
								if (pDirt->DirtX + 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 4;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtX - 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtX - 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 6;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtX + 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else if (pDirt->DirtY - 1 == pDirt2->DirtY)
		{
			if (pDirt->DirtX == pDirt2->DirtX)
			{
				m_iDirt = 15;
				for (auto& pDirt3 : m_vecDirt)
				{
					if (pDirt->DirtX + 1 == pDirt3->DirtX)
					{
						if (pDirt->DirtY == pDirt3->DirtY)
						{
							m_iDirt = 7;
							for (auto& pDirt4 : m_vecDirt)
							{
								if (pDirt->DirtX - 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 8;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtY + 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtY + 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 4;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtX - 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
					else if (pDirt->DirtX - 1 == pDirt3->DirtX)
					{
						if (pDirt->DirtY == pDirt3->DirtY)
						{
							m_iDirt = 9;
							for (auto& pDirt4 : m_vecDirt)
							{
								if (pDirt->DirtX + 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 8;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtY + 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtY + 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 6;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtX + 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
					else if (pDirt->DirtY + 1 == pDirt3->DirtY)
					{
						if (pDirt->DirtX == pDirt3->DirtX)
						{
							m_iDirt = 14;
							for (auto& pDirt4 : m_vecDirt)
							{
								if (pDirt->DirtX + 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 4;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtX - 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtX - 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 6;
										for (auto& pDirt5 : m_vecDirt)
										{
											if (pDirt->DirtX + 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
void CFarming::Calculate_WaterDirt(DIRTINFO * pDirt)
{
	m_iDirt = 0;
	for (auto& pDirt2 : m_vecWater)
	{
		if (pDirt->DirtX + 1 == pDirt2->DirtX)
		{
			if (pDirt->DirtY == pDirt2->DirtY)
			{
				m_iDirt = 10;
				for (auto& pDirt3 : m_vecWater)
				{
					if (pDirt->DirtY + 1 == pDirt3->DirtY)
					{
						if (pDirt->DirtX == pDirt3->DirtX)
						{
							m_iDirt = 1;
							for (auto& pDirt4 : m_vecWater)
							{
								if (pDirt->DirtY - 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 4;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtX - 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtX - 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 2;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtY - 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
					else if (pDirt->DirtY - 1 == pDirt3->DirtY)
					{
						if (pDirt->DirtX == pDirt3->DirtX)
						{
							m_iDirt = 7;
							for (auto& pDirt4 : m_vecWater)
							{
								if (pDirt->DirtY + 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 4;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtX - 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtX - 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 8;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtY + 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
					else if (pDirt->DirtX - 1 == pDirt3->DirtX)
					{
						if (pDirt->DirtY == pDirt3->DirtY)
						{
							m_iDirt = 11;
							for (auto& pDirt4 : m_vecWater)
							{
								if (pDirt->DirtY + 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 2;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtY - 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtY - 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 8;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtY + 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else if (pDirt->DirtX - 1 == pDirt2->DirtX)
		{
			if (pDirt->DirtY == pDirt2->DirtY)
			{
				m_iDirt = 12;
				for (auto& pDirt3 : m_vecWater)
				{
					if (pDirt->DirtY + 1 == pDirt3->DirtY)
					{
						if (pDirt->DirtX == pDirt3->DirtX)
						{
							m_iDirt = 3;
							for (auto& pDirt4 : m_vecWater)
							{
								if (pDirt->DirtY - 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 6;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtX + 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtX + 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 2;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtY - 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
					else if (pDirt->DirtY - 1 == pDirt3->DirtY)
					{
						if (pDirt->DirtX == pDirt3->DirtX)
						{
							m_iDirt = 9;
							for (auto& pDirt4 : m_vecWater)
							{
								if (pDirt->DirtY + 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 6;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtX + 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtX + 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 8;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtY + 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
					else if (pDirt->DirtX + 1 == pDirt3->DirtX)
					{
						if (pDirt->DirtY == pDirt3->DirtY)
						{
							m_iDirt = 11;
							for (auto& pDirt4 : m_vecWater)
							{
								if (pDirt->DirtY + 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 2;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtY - 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtY - 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 8;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtY + 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else if (pDirt->DirtY + 1 == pDirt2->DirtY)
		{
			if (pDirt->DirtX == pDirt2->DirtX)
			{
				m_iDirt = 13;
				for (auto& pDirt3 : m_vecWater)
				{
					if (pDirt->DirtX + 1 == pDirt3->DirtX)
					{
						if (pDirt->DirtY == pDirt3->DirtY)
						{
							m_iDirt = 1;
							for (auto& pDirt4 : m_vecWater)
							{
								if (pDirt->DirtX - 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 2;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtY - 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtY - 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 4;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtX - 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
					else if (pDirt->DirtX - 1 == pDirt3->DirtX)
					{
						if (pDirt->DirtY == pDirt3->DirtY)
						{
							m_iDirt = 3;
							for (auto& pDirt4 : m_vecWater)
							{
								if (pDirt->DirtX + 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 2;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtY - 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtY - 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 6;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtX + 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
					else if (pDirt->DirtY - 1 == pDirt3->DirtY)
					{
						if (pDirt->DirtX == pDirt3->DirtX)
						{
							m_iDirt = 14;
							for (auto& pDirt4 : m_vecWater)
							{
								if (pDirt->DirtX + 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 4;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtX - 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtX - 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 6;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtX + 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else if (pDirt->DirtY - 1 == pDirt2->DirtY)
		{
			if (pDirt->DirtX == pDirt2->DirtX)
			{
				m_iDirt = 15;
				for (auto& pDirt3 : m_vecWater)
				{
					if (pDirt->DirtX + 1 == pDirt3->DirtX)
					{
						if (pDirt->DirtY == pDirt3->DirtY)
						{
							m_iDirt = 7;
							for (auto& pDirt4 : m_vecWater)
							{
								if (pDirt->DirtX - 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 8;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtY + 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtY + 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 4;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtX - 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
					else if (pDirt->DirtX - 1 == pDirt3->DirtX)
					{
						if (pDirt->DirtY == pDirt3->DirtY)
						{
							m_iDirt = 9;
							for (auto& pDirt4 : m_vecWater)
							{
								if (pDirt->DirtX + 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 8;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtY + 1 == pDirt5->DirtY)
											{
												if (pDirt->DirtX == pDirt5->DirtX)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtY + 1 == pDirt4->DirtY)
								{
									if (pDirt->DirtX == pDirt4->DirtX)
									{
										m_iDirt = 6;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtX + 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
					else if (pDirt->DirtY + 1 == pDirt3->DirtY)
					{
						if (pDirt->DirtX == pDirt3->DirtX)
						{
							m_iDirt = 14;
							for (auto& pDirt4 : m_vecWater)
							{
								if (pDirt->DirtX + 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 4;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtX - 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
								if (pDirt->DirtX - 1 == pDirt4->DirtX)
								{
									if (pDirt->DirtY == pDirt4->DirtY)
									{
										m_iDirt = 6;
										for (auto& pDirt5 : m_vecWater)
										{
											if (pDirt->DirtX + 1 == pDirt5->DirtX)
											{
												if (pDirt->DirtY == pDirt5->DirtY)
												{
													m_iDirt = 5;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
_bool CFarming::isDraw(CGameObject * pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

_matrix CFarming::Render_TransForm(CGameObject * pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

CFarming * CFarming::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFarming*		pInstance = new CFarming(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CFarming Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFarming::Free()
{
	Safe_Delete_Array(m_pStateDesc);

	for (auto& pDirt : m_vecDirt)
		Safe_Delete(pDirt);
	m_vecDirt.clear();

	for (auto& pCrop : m_vecCrop)
		Safe_Delete(pCrop);
	m_vecCrop.clear();

	for (auto& pWater : m_vecWater)
		Safe_Delete(pWater);
	m_vecWater.clear();

	Safe_Release(m_pPlayer);
	Safe_Release(m_pHoe);
	Safe_Release(m_pWateringPot);
	Safe_Release(m_pUI);
	Safe_Release(m_pWatering);
	Safe_Release(m_pBuffer);
	for (auto& pTexture : m_pTexture)
		Safe_Release(pTexture);
	Safe_Release(m_pShader);
	
	CGameObject::Free();
}
