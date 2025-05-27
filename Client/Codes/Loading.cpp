#include "stdafx.h"
#include "..\Headers\Loading.h"
#include "Resource_Manager.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphic_Device(pGraphicDev)
	, m_pResource_Manager(CResource_Manager::GetInstance())
{
	m_pGraphic_Device->AddRef();
	m_pResource_Manager->AddRef();
}

HRESULT CLoading::Ready_Loading(SCENEID eSceneID)
{
	m_eSceneID = eSceneID;

	InitializeCriticalSection(&m_Critical_Section);

	// 내 코드를 읽어나가기위한 새로운 흐름을 생성한다.
	m_hThread = (HANDLE)_beginthreadex(nullptr, 1, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return NOERROR;
}

HRESULT CLoading::Loading_Resource_Stage()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	CResource*			pResource = nullptr;

	lstrcpy(m_szString, L"Loading.....");

	// For.Rect
	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Rect.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Rect", pResource)))
		return E_FAIL;

	// For.Resource_Texture_Player_Idle
	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Idle/PlayerFrontIdle.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Front_Idle", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Idle/PlayerBackIdle.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Back_Idle", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Idle/PlayerSideIdle.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Side_Idle", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Idle/PlayerFrontHIdle.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Front_HIdle", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Idle/PlayerBackHIdle.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Back_HIdle", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Idle/PlayerSideHIdle.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Side_HIdle", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Shadow.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Shadow", pResource)))
		return E_FAIL;

	// For.Resource_Texture_Player_Run
	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Run/PlayerFrontRun%d.png", 8);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Front_Run", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Run/PlayerBackRun%d.png", 8);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Back_Run", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Run/PlayerSideRun%d.png", 8);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Side_Run", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/HRun/PlayerFrontHRun%d.png", 8);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Front_HRun", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/HRun/PlayerBackHRun%d.png", 8);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Back_HRun", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/HRun/PlayerSideHRun%d.png", 8);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Side_HRun", pResource)))
		return E_FAIL;

	// For.Resource_Texture_Player_Swing
	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Swing/PlayerFrontSwing%d.png", 5);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Front_Swing", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Swing/PlayerBackSwing%d.png", 5);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Back_Swing", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Swing/PlayerSideSwing%d.png", 5);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Side_Swing", pResource)))
		return E_FAIL;

	// For.Resource_Texture_Player_Slash
	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Slash/PlayerFrontSlash%d.png", 6);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Front_Slash", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Slash/PlayerBackSlash%d.png", 6);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Back_Slash", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Slash/PlayerSideSlash%d.png", 6);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Side_Slash", pResource)))
		return E_FAIL;

	// For.Resource_Texture_Player_Watering
	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Watering/PlayerFrontWater%d.png", 3);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Front_Watering", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Watering/PlayerBackWater%d.png", 3);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Back_Watering", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Watering/PlayerSideWater%d.png", 3);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Side_Watering", pResource)))
		return E_FAIL;

	// For.Resource_Texture_Player_Harvest
	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Harvest/PlayerFrontHarvest%d.png", 4);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Front_Harvest", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Harvest/PlayerBackHarvest%d.png", 4);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Back_Harvest", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Harvest/PlayerSideHarvest%d.png", 4);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Side_Harvest", pResource)))
		return E_FAIL;

	// For.Resource_Texture_Player_Fishing
	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Fishing/PlayerFrontFishSwing%d.png", 6);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Front_FishSwing", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Fishing/PlayerBackFishSwing%d.png", 6);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Back_FishSwing", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Fishing/PlayerSideFishSwing%d.png", 6);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Side_FishSwing", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Fishing/PlayerFrontFishing%d.png", 2);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Front_Fishing", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Fishing/PlayerBackFishing%d.png", 2);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Back_Fishing", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Fishing/PlayerSideFishing%d.png", 2);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Side_Fishing", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Player/Fishing/PlayerGetFishing.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_GetFishing", pResource)))
		return E_FAIL;

	// For.Resource_Texture_Tiles
	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tiles/HouseTile/HouseTile%d.png", 18);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Tiles", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tiles/FarmTile/FarmTile%d.png", 40);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FarmTiles", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tiles/TownTile/TownTile%d.png", 75);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_TownTiles", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tiles/RoadTile/RoadTile%d.png",17);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_RoadTiles", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tiles/FarmTile/Water%d.png", 2);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_WaterTiles", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tiles/CaveTile/CaveTile.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_CaveTiles", pResource)))
		return E_FAIL;

	//For. Resource_Furniture
	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/House/TV.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_TV", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/House/Bed0.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Bed0", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/House/Bed1.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Bed1", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/House/Table.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Table", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/House/Chair.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Chair", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/House/Window%d.png",2);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Window", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/House/Fireplace.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Fireplace", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/House/Carpet.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Carpet", pResource)))
		return E_FAIL;

	//For. Resource_Tools
	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/Hoe/HoeFront%d.png",5);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_HoeFront", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/Hoe/HoeBack%d.png", 5);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_HoeBack", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/Hoe/HoeSide%d.png",5);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_HoeSide", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/Hoe/HoeIcon.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_HoeIcon", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/Ax/AxFront%d.png", 5);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_AxFront", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/Ax/AxBack%d.png", 5);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_AxBack", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/Ax/AxSide%d.png", 5);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_AxSide", pResource)))
		return E_FAIL;


	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/Pick/PickFront%d.png", 5);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_PickFront", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/Pick/PickBack%d.png", 5);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_PickBack", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/Pick/PickSide%d.png", 5);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_PickSide", pResource)))
		return E_FAIL;



	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/Sickle/SickleFront%d.png", 6);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_SickleFront", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/Sickle/SickleBack%d.png", 6);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_SickleBack", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/Sickle/SickleSide%d.png", 6);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_SickleSide", pResource)))
		return E_FAIL;



	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/Sword/SwordFront%d.png", 6);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_SwordFront", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/Sword/SwordBack%d.png", 6);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_SwordBack", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/Sword/SwordSide%d.png", 6);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_SwordSide", pResource)))
		return E_FAIL;



	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/WateringPot/WateringPotFront%d.png", 3);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_WateringPotFront", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/WateringPot/WateringPotBack%d.png", 3);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_WateringPotBack", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/WateringPot/WateringPotSide%d.png", 3);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_WateringPotSide", pResource)))
		return E_FAIL;


	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/FishingRod/FishingRodFront%d.png", 7);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingRodFront", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/FishingRod/FishingRodSide%d.png", 7);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingRodSide", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/FishingRod/FishingRodBack%d.png", 7);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingRodBack", pResource)))
		return E_FAIL;


	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/FishingRod/FishingFront%d.png", 2);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingFront", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/FishingRod/FishingSide%d.png", 2);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingSide", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Tools/FishingRod/FishingBack%d.png", 2);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingBack", pResource)))
		return E_FAIL;


	//For. Resource_UI
	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/BasicUI.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_BasicUI", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/Inventory%d.png",3);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Inventory", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/SelectItem.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_SelectItem", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/Check%d.png",2);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_CheckTile", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/SmallNumber%d.png", 10);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_SmallNumber", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/BigNumber%d.png", 10);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_BigNumber", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/Date/Date%d.png", 28);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Date", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/Time/Time%d.png", 121);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Time", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/ClockNeedle.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_ClockNeedle", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/Get_Object/Get_Branch.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Get_Branch", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/Get_Object/Get_Stone.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Get_Stone", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/Get_Object/Get_Weed.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Get_Weed", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/Get_Object/Get_Copper.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Get_Copper", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/Get_Object/Get_Slime.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Get_Slime", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/ShopUI/Shop_UI.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_ShopUI", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/ShopUI/Bold_Parsnip.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Bold_Parsnip", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/ShopUI/Bold_Potato.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Bold_Potato", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/ShopUI/Bold_Strawberry.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Bold_Strawberry", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/RainUI.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_RainUI", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/Warning.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Warning", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/ChestBox.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_ChestBox", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/ChestUI.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_ChestUI", pResource)))
		return E_FAIL;


	//For. Resource_ItemIcon

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/InventoryIcon/Icon_Hoe.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Hoe", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/InventoryIcon/Icon_Pick.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Pick", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/InventoryIcon/Icon_Ax.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Ax", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/InventoryIcon/Icon_Sickle.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Sickle", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/InventoryIcon/Icon_WateringPot.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_WateringPot", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/InventoryIcon/Icon_Sword.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Sword", pResource)))
		return E_FAIL;


	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/InventoryIcon/Icon_FishingRod.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_FishingRod", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Icon/Icon_Strawberry.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Strawberry", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Icon/Icon_Parsnip.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Parsnip", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Icon/Icon_Potato.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Potato", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Icon/Icon_StrawberrySeed.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_StrawberrySeed", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Icon/Icon_ParsnipSeed.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_ParsnipSeed", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Icon/Icon_PotatoSeed.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_PotatoSeed", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Icon/Icon_Branch.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Branch", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Icon/Icon_Fish_Bass.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Fish_Bass", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Icon/Icon_Fish_Salmon.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Fish_Salmon", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Icon/Icon_Fish_Sunfish.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Fish_Sunfish", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Icon/Icon_Stone.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Stone", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Icon/Icon_Weed.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Weed", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Icon/Icon_Copper.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Copper", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Icon/Icon_Slime.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Slime", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Icon/Icon_Chest.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Chest", pResource)))
		return E_FAIL;

	//For. Resource_Map_FarmObject

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Object/FarmObject%d.png",19);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FarmObject", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Object/House.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_House", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Object/BigStumpDelete%d.png",5);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_BigStumpDelete", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Object/BushDelete%d.png", 7);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_BushDelete", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Object/StoneDelete%d.png", 7);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_StoneDelete", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Object/BranchDelete%d.png", 8);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_BranchDelete", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Object/Branchcrash%d.png", 7);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Branchcrash", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Object/Watering%d.png", 10);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Watering", pResource)))
		return E_FAIL;


	// For. Resource_Farming

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Farming/Dirt/BasicDirt%d.png",16);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_BasicDirt", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Farming/Dirt/WaterDirt%d.png", 16);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_WaterDirt", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Farming/Crops/Parsnip/Parsnip%d.png", 6);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Parsnip", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Farming/Crops/Strawberry/Strawberry%d.png", 7);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Strawberry", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Farming/Crops/Potato/Potato%d.png", 7);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Potato", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Farming/Crops/DeadCrop/DeadCrop%d.png", 4);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_DeadCrop", pResource)))
		return E_FAIL;

	// For. Resource_Fishing
	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/FishingUI.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingUI_Base", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/FishingUI_Fish.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingUI_Fish", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/Hit.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingUI_Hit", pResource)))
		return E_FAIL; 

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/FishingUI_CheckPoint.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingUI_CheckPoint", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/FishingUI_Gauge.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FishingUI_Gauge", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/Menu.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Menu", pResource)))
		return E_FAIL;


	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/Menu%d.png",2);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Menu2", pResource)))
		return E_FAIL;

	//For. Resource_Map_ShopObject
	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Object/ShopObject/ShopObject%d.png", 17);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_ShopObject", pResource)))
		return E_FAIL;

	//For. Resource_Map_TownObject
	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Object/TownObject/TownObject%d.png", 44);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_TownObject", pResource)))
		return E_FAIL;

	//For. Resource_Map_CaveObject
	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Object/CaveObject/Slime/GreenSlime%d.png", 8);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_GreenSlime", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Object/CaveObject/Slime/GreenSlimeDead%d.png", 5);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_GreenSlimeDead", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Object/CaveObject/Slime/Mark%d.png",2);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Mark", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Object/CaveObject/Mineral/Mineral%d.png", 8);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Mineral", pResource)))
		return E_FAIL;

	//For. Resource_Effect
	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Effect/Night.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_NightEffect", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Effect/Rain%d.png",4);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Rain", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Effect/RainAlpha.png");
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_RainAlpha", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Effect/Butterfly/Butterfly0_%d.png",3);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Butterfly0", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Effect/Butterfly/Butterfly1_%d.png", 3);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Butterfly1", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Effect/Butterfly/Butterfly2_%d.png", 3);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Butterfly2", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Effect/Butterfly/Butterfly3_%d.png", 3);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Butterfly3", pResource)))
		return E_FAIL;

	//For. Resource_NPC
	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Object/NPC/Alex/Alex%d.png", 20);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Alex", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Object/NPC/Lewis/Lewis%d.png", 15);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Lewis", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Object/NPC/Penny/Penny%d.png", 16);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Penny", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Object/NPC/Robin/Robin%d.png", 12);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Robin", pResource)))
		return E_FAIL;

	pResource = CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Object/NPC/Vincent/Vincent%d.png", 14);
	if (nullptr == pResource)
		return E_FAIL;
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Vincent", pResource)))
		return E_FAIL;

	 
	lstrcpy(m_szString, L"Buffer Loading....");
	

	lstrcpy(m_szString, L"Mesh Loading.......");
	

	lstrcpy(m_szString, L"Complete.......");

	m_isFinish = true;

	return NOERROR;
}

_uint CLoading::Thread_Main(void * pArg)
{
	CLoading* pLoading = (CLoading*)pArg;

	HRESULT			hr = 0;

	EnterCriticalSection(pLoading->Get_Critical_Section());

	switch (pLoading->Get_SceneID())
	{
	case SCENE_STAGE:
		hr = pLoading->Loading_Resource_Stage();
		break;
	case SCENE_MENU:
		hr = pLoading->Loading_Resource_Stage();
		break;
	case SCENE_FARM:
		break;
	}

	if (FAILED(hr))
	{
		LeaveCriticalSection(pLoading->Get_Critical_Section());
		return -1;
	}

	LeaveCriticalSection(pLoading->Get_Critical_Section());

	return 0;
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENEID eSceneID)
{
	CLoading*		pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eSceneID)))
	{
		_MSG_BOX("CLoading Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoading::Free()
{
	// 새롭게 생성한 스레드가 자기할일(Thread_Main)을 완전히 종료할때ㅑ까지 기다려.
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_Critical_Section);

	CloseHandle(m_hThread);

	Safe_Release(m_pResource_Manager);
	Safe_Release(m_pGraphic_Device);
}
