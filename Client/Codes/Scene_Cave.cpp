#include "stdafx.h"
#include "..\Headers\Scene_Cave.h"
#include "Object_Manager.h"
#include "Resource_Manager.h"
#include "Management.h"
#include "Back_Cave.h"
#include "FocusCam.h"
#include "Player.h"
#include "GreenSlime.h"
#include "Mineral.h"
#include "Drop_Object.h"
#include "Sound_Manager.h"

CScene_Cave::CScene_Cave(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
}

HRESULT CScene_Cave::Ready_Scene()
{	
	// For. Layer_Camera
	if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(L"Layer_Monster")))
		return E_FAIL;

	// For.Layer_BackGround
	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Mineral(L"Layer_Mineral")))
		return E_FAIL;

	m_pPlayer = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr == m_pPlayer)
		return E_FAIL;

	m_pPlayer->Ready_Camera();

	m_pChange = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Change");
	if (nullptr == m_pChange)
		return E_FAIL;

	while(true)
	{
		CGameObject* m_pTarget = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Mineral", m_iCount);
		if (nullptr == m_pTarget)
			break;
		++m_iCount;
		m_vecMineral.push_back(m_pTarget);
	}

	while (true)
	{
		CGameObject* m_pTarget2 = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Monster", m_iCount2);
		if (nullptr == m_pTarget2)
			break;
		++m_iCount2;
		m_vecMonster.push_back(m_pTarget2);
	}

	CSound_Manager::Get_Instance()->SoundStop(0);
	CSound_Manager::Get_Instance()->SoundStop(2);
	CSound_Manager::Get_Instance()->SoundStop(3);

	CSound_Manager::Get_Instance()->SoundPlay(4,TRUE);
	CSound_Manager::Get_Instance()->SoundPlay(5,TRUE);

	return NOERROR;
}

_int CScene_Cave::Update_Scene(const _float& fTimeDelta)
{
	if (m_pPlayer->Get_Y() > 5.f*64.f && m_pPlayer->Get_Y() < 6.f*64.f && m_pPlayer->Get_X() > 6.f*64.f && m_pPlayer->Get_X() < 7.f*64.f)
	{
		if (false == m_bOnce)
		{
			CSound_Manager::Get_Instance()->SoundStop(4);
			CSound_Manager::Get_Instance()->SoundStop(5);
			m_bOnce = true;
			m_pChange->Set_Start();
			m_pPlayer->Set_StopOn();
		}

		if (false == m_pChange->Get_Start())
		{
			CGameObject* pUI = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_BasicUI");

			if (true == pUI->Get_RainDay())
			{
				if (false == CSound_Manager::Get_Instance()->SoundPlaying(2))
					CSound_Manager::Get_Instance()->SoundPlay(2, true);
			}
			else if (90.f + 9.f*15.f == pUI->Get_TimeDegree())
			{
				if (false == CSound_Manager::Get_Instance()->SoundPlaying(18))
					CSound_Manager::Get_Instance()->SoundPlay(18, true);
			}
			else
			{
				if (false == CSound_Manager::Get_Instance()->SoundPlaying(3))
					CSound_Manager::Get_Instance()->SoundPlay(3, true);
			}

			m_pPlayer->Set_StopOff();
			m_pPlayer->Ready_Location(CPlayer::CAVEOUT);

			if (FAILED(CManagement::GetInstance()->Ready_Scene(m_pGraphic_Device, SCENE_ROAD)))
				return -1;

			return 0;
		}
	}

	for (auto& pObject : m_vecMineral)
	{
		if (true == pObject->Create_DropObject())
		{
			Ready_Layer_DropObject(L"Layer_DropObject", pObject);
			pObject->Set_DropObject();
		}
	}

	for (auto& pObject : m_vecMonster)
	{
		if (true == pObject->Create_DropObject())
		{
			Ready_Layer_DropObject(L"Layer_DropObject", pObject);
			pObject->Set_DropObject();
		}
	}

	return CScene::Update_Scene(fTimeDelta);
}

_int CScene_Cave::LastUpdate_Scene(const _float& fTimeDelta)
{
	int		iExitCode = 0;

	iExitCode = CScene::LastUpdate_Scene(fTimeDelta);

	return iExitCode;
}

void CScene_Cave::Render_Scene()
{
	CScene::Render_Scene();

	// 디버깅적 요소를 추가해주세요.
}

// 배경에 관련된 객체들을 생성해서 모아 관리하기위해.
HRESULT CScene_Cave::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	CGameObject*			pGameObject = nullptr;

	// For.Back_Stage
	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag, CBack_Cave::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Cave::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CFocusCam::Create(m_pGraphic_Device, g_iBackCX * 0.5f, 40.0f * 64.0f - g_iBackCX * 0.5f,
			g_iBackCY * 0.5f, 20.0f * 64.0f - g_iBackCY * 0.5f ))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Cave::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CGreenSlime::Create(m_pGraphic_Device, 64.f*21.f, 64.f* 10.f))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CGreenSlime::Create(m_pGraphic_Device, 64.f*17.f, 64.f* 11.f))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CGreenSlime::Create(m_pGraphic_Device, 64.f*18.f, 64.f* 14.f))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CGreenSlime::Create(m_pGraphic_Device, 64.f*15.f, 64.f* 13.f))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CGreenSlime::Create(m_pGraphic_Device, 64.f*11.f, 64.f* 15.f))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CGreenSlime::Create(m_pGraphic_Device, 64.f*10.f, 64.f* 12.f))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CGreenSlime::Create(m_pGraphic_Device, 64.f*6.f, 64.f* 14.f))))
		return E_FAIL;



	return NOERROR;
}

HRESULT CScene_Cave::Ready_Layer_Mineral(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 34.f*64.f, 8.f* 64.f, 6))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 32.f*64.f, 7.f* 64.f, 6))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 33.f*64.f, 16.f* 64.f, 6))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 37.f*64.f, 10.f* 64.f, 6))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 37.f*64.f, 11.f* 64.f, 6))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 37.f*64.f, 13.f* 64.f, 6))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 37.f*64.f, 14.f* 64.f, 6))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 35.f*64.f, 10.f* 64.f, 6))))
		return E_FAIL;


	// 돌멩이
	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 28.f*64.f, 8.f* 64.f, 0))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 31.f*64.f, 8.f* 64.f, 2))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 34.f*64.f, 8.f* 64.f, 4))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 35.f*64.f, 11.f* 64.f, 0))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 35.f*64.f, 14.f* 64.f, 2))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 33.f*64.f, 15.f* 64.f, 4))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 31.f*64.f, 15.f* 64.f, 0))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 32.f*64.f, 10.f* 64.f, 2))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 33.f*64.f, 12.f* 64.f, 4))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 31.f*64.f, 13.f* 64.f, 0))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 30.f*64.f, 11.f* 64.f, 2))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 27.f*64.f, 9.f* 64.f, 4))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 28.f*64.f, 10.f* 64.f, 0))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 29.f*64.f, 12.f* 64.f, 2))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 30.f*64.f, 12.f* 64.f, 4))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 28.f*64.f, 14.f* 64.f, 0))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 26.f*64.f, 13.f* 64.f, 2))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 25.f*64.f, 11.f* 64.f, 4))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 24.f*64.f, 7.f* 64.f, 0))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 24.f*64.f, 16.f* 64.f, 2))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CMineral::Create(m_pGraphic_Device, 25.f*64.f, 15.f* 64.f, 4))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Cave::Ready_Layer_DropObject(const _tchar * pLayerTag, CGameObject * pObject)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (1 == pObject->Get_Frame())
	{
		if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
			CDrop_Object::Create(m_pGraphic_Device, _vec3(pObject->Get_fX(), pObject->Get_fY(), 0.f), _vec3(64.f, 64.f, 1.f), 1))))
			return E_FAIL;
	}
	else if (6 == pObject->Get_Frame())
	{
		if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
			CDrop_Object::Create(m_pGraphic_Device, _vec3(pObject->Get_fX(), pObject->Get_fY(), 0.f), _vec3(64.f, 64.f, 1.f), 3))))
			return E_FAIL;
	}
	else if (7 == pObject->Get_Frame())
	{
		if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
			CDrop_Object::Create(m_pGraphic_Device, _vec3(pObject->Get_fX(), pObject->Get_fY(), 0.f), _vec3(64.f, 64.f, 1.f), 4))))
			return E_FAIL;
	}

	return NOERROR;
}

CScene_Cave * CScene_Cave::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Cave*		pInstance = new CScene_Cave(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSG_BOX("CScene_Cave Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Cave::Free()
{


	m_pResource_Manager->Clear_Resource(CResource_Manager::TYPE_SHOP);

	CScene::Free();
}
