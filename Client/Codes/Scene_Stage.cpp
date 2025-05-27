#include "stdafx.h"
#include "..\Headers\Scene_Stage.h"
#include "Object_Manager.h"
#include "Resource_Manager.h"
#include "Management.h"
#include "Back_Stage.h"
#include "Player.h"
#include "FocusCam.h"
#include "BasicUI.h"
#include "Shadow.h"
#include "Hoe.h"
#include "Ax.h"
#include "Pick.h"
#include "Sickle.h"
#include "Sword.h"
#include "WateringPot.h"
#include "FishingRod.h"
#include "Object_Stage.h"
#include "Inventory.h"
#include "Object_Farm.h"
#include "Object_Farm2.h"
#include "Object_Shop.h"
#include "Object_Town.h"
#include "Farming.h"
#include "Fishing.h"
#include "NightEffect.h"
#include "Get_Object.h"
#include "SceneChange.h"
#include "ShopUI.h"
#include "RainEffect.h"
#include "RainAlpha.h"
#include "ButterflyEffect.h"
#include "Warning.h"
#include "NPC_Alex.h"
#include "NPC_Robin.h"
#include "NPC_Lewis.h"
#include "NPC_Vincent.h"
#include "NPC_Penny.h"
#include "Sound_Manager.h"

CScene_Stage::CScene_Stage(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
}

HRESULT CScene_Stage::Ready_Scene()
{	
	if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Inventory(L"Layer_Inventory")))
		return E_FAIL;

	if (FAILED(Ready_Layer_BasicUI(L"Layer_BasicUI")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(L"Layer_Player")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Tools(L"Layer_Tools")))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Object(L"Layer_Object")))
		return E_FAIL;

	if (FAILED(Ready_Layer_ShopObject(L"Layer_ShopObject")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Farming(L"Layer_Farming")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Fishing(L"Layer_Fishing")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(L"Layer_Effect")))
		return E_FAIL;

	if (FAILED(Ready_Layer_ShopUI(L"Layer_ShopUI")))
		return E_FAIL;

	if (FAILED(Ready_Layer_NPC(L"Layer_NPC")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Shadow(L"Layer_Shadow")))
		return E_FAIL;

	m_pPlayer = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr == m_pPlayer)
		return E_FAIL;

	m_pChange = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Change");
	if (nullptr == m_pChange)
		return E_FAIL;

	CSound_Manager::Get_Instance()->SoundPlay(3, TRUE);

	return NOERROR;
}

_int CScene_Stage::Update_Scene(const _float& fTimeDelta)
{
	if (m_pPlayer->Get_Y() >= 19.0f * 64.0f)
	{ 
		if (false == m_bOnce)
		{
			CSound_Manager::Get_Instance()->SoundPlay(10, FALSE);
			m_bOnce = true;
			m_pChange->Set_Start();
			m_pPlayer->Set_StopOn();
		}
		
		if (false == m_pChange->Get_Start())
		{
			m_pPlayer->Set_StopOff();

			m_pPlayer->Ready_Location(CPlayer::HOMEOUT);

			if (FAILED(CManagement::GetInstance()->Ready_Scene(m_pGraphic_Device, SCENE_FARM)))
				return -1;

			return 0;
		}
	}

	return CScene::Update_Scene(fTimeDelta);
}

_int CScene_Stage::LastUpdate_Scene(const _float& fTimeDelta)
{
	int		iExitCode = 0;

	iExitCode = CScene::LastUpdate_Scene(fTimeDelta);

	return iExitCode;
}

void CScene_Stage::Render_Scene()
{
	CScene::Render_Scene();

	// 디버깅적 요소를 추가해주세요.
}

// 배경에 관련된 객체들을 생성해서 모아 관리하기위해.
HRESULT CScene_Stage::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	CGameObject*			pGameObject = nullptr;

	// For.Back_Stage
	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag, CBack_Stage::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Player(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	// For.Player	
	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag, CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CFocusCam::Create(m_pGraphic_Device, g_iBackCX * 0.5f, 30.0f * 64.0f - g_iBackCX * 0.5f,
			g_iBackCY * 0.5f, 23.0f * 64.0f - g_iBackCY * 0.5f ))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_BasicUI(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag, CBasicUI::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag, CGet_Object::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Shadow(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag, CShadow::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Tools(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag, CHoe::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag, CAx::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag, CPick::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag, CSickle::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag, CSword::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag, CWateringPot::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag, CFishingRod::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Object(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CObject_Stage::Create(m_pGraphic_Device,11.f*64.f,11.f*64.f,32.f*4.f, 64.f*4.f ,0))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CObject_Stage::Create(m_pGraphic_Device, 19.f * 64.f, 15.f*64.f, 32.f*4.f, 64.f*4.f, 1,1))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CObject_Stage::Create(m_pGraphic_Device, 19.f * 64.f, 15.f*64.f, 32.f*4.f, 64.f*4.f, 2,2))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CObject_Stage::Create(m_pGraphic_Device, 15.f * 64.f, 11.f*64.f, 32.f*4.f, 64.f*4.f, 3))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CObject_Stage::Create(m_pGraphic_Device, 13.5f * 64.f, 11.2f*64.f, 16.f*4.f, 32.f*4.f, 4))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CObject_Stage::Create(m_pGraphic_Device, 18.f * 64.f, 8.f*64.f, 32.f*4.f, 128.f*4.f, 5))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CObject_Stage::Create(m_pGraphic_Device, 12.5f * 64.f, 9.f*64.f, 16.f*4.f, 32.f*4.f, 6))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CObject_Stage::Create(m_pGraphic_Device, 15.f * 64.f, 15.f*64.f, 64.f*4.f, 32.f*4.f, 7,1))))
		return E_FAIL;
	
	//FarmObject
	SetUp_FarmObjectInfo();

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
		CObject_Farm::Create(m_pGraphic_Device))))
		return E_FAIL;

	for (auto& pObject : m_vecObject)
	{
		if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
			CObject_Farm2::Create(m_pGraphic_Device, pObject->vPosition, pObject->vSize, pObject->iIndex))))
			return E_FAIL;
	}

	//TownObject
	SetUp_TownObjectInfo();

	for (auto& pObject : m_vecObject3)
	{
		if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
			CObject_Town::Create(m_pGraphic_Device, pObject->vPosition, pObject->vSize, pObject->iIndex))))
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_ShopObject(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	//ShopObject
	SetUp_ShopObjectInfo();

	for (auto& pObject : m_vecObject2)
	{
		if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
			CObject_Shop::Create(m_pGraphic_Device, pObject->vPosition, pObject->vSize, pObject->iIndex))))
			return E_FAIL;
	}

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
		CObject_Shop::Create(m_pGraphic_Device, _vec3(4.8f*64.f, 5.4f*64.f, 0.f), _vec3(16.f*4.f, 32.f*4.f, 1.f), 15))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Inventory(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
		CInventory::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Farming(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
		CFarming::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Fishing(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
		CFishing::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
		CSceneChange::Create(m_pGraphic_Device))))
		return E_FAIL;

	for (size_t i = 0; i < 230; i++)
	{
		if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
			CRainEffect::Create(m_pGraphic_Device))))
			return E_FAIL;
	}

	for (size_t i = 0; i < 15; i++)
	{
		if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
			CButterflyEffect::Create(m_pGraphic_Device))))
			return E_FAIL;
	}

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
		CRainAlpha::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
		CNightEffect::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
		CWarning::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_ShopUI(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
		CShopUI::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_NPC(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
		CNPC_Alex::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
		CNPC_Lewis::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
		CNPC_Robin::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
		CNPC_Vincent::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
		CNPC_Penny::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::SetUp_FarmObjectInfo()
{
	_ulong			dwByte = 0;

	HANDLE			hFile = CreateFile(L"../Bin/Data/FarmObject6.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	for (;;)
	{
		OBJECTINFO*		pObjectInfo = new OBJECTINFO;
		ZeroMemory(pObjectInfo, sizeof(OBJECTINFO));

		ReadFile(hFile, pObjectInfo, sizeof(OBJECTINFO), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pObjectInfo);
			break;
		}

		m_vecObject.push_back(pObjectInfo);
	}

	CloseHandle(hFile);

	return NOERROR;
}

HRESULT CScene_Stage::SetUp_ShopObjectInfo()
{
	_ulong			dwByte = 0;

	HANDLE			hFile = CreateFile(L"../Bin/Data/ShopObject3.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	for (;;)
	{
		OBJECTINFO*		pObjectInfo = new OBJECTINFO;
		ZeroMemory(pObjectInfo, sizeof(OBJECTINFO));

		ReadFile(hFile, pObjectInfo, sizeof(OBJECTINFO), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pObjectInfo);
			break;
		}

		m_vecObject2.push_back(pObjectInfo);
	}

	CloseHandle(hFile);

	return NOERROR;
}

HRESULT CScene_Stage::SetUp_TownObjectInfo()
{
	_ulong			dwByte = 0;

	HANDLE			hFile = CreateFile(L"../Bin/Data/TownObject10.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	for (;;)
	{
		OBJECTINFO*		pObjectInfo = new OBJECTINFO;
		ZeroMemory(pObjectInfo, sizeof(OBJECTINFO));

		ReadFile(hFile, pObjectInfo, sizeof(OBJECTINFO), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pObjectInfo);
			break;
		}

		m_vecObject3.push_back(pObjectInfo);
	}

	CloseHandle(hFile);

	return NOERROR;
}

CScene_Stage * CScene_Stage::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Stage*		pInstance = new CScene_Stage(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSG_BOX("CScene_Stage Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Stage::Free()
{
	for (auto& pFarmObject : m_vecObject)
		Safe_Delete(pFarmObject);
	for (auto& pShopObject : m_vecObject2)
		Safe_Delete(pShopObject);
	for (auto& pShopObject : m_vecObject3)
		Safe_Delete(pShopObject);
	m_vecObject.clear();

	CScene::Free();
}
