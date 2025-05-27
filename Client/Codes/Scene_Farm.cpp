#include "stdafx.h"
#include "..\Headers\Scene_Farm.h"
#include "Object_Manager.h"
#include "Resource_Manager.h"
#include "Management.h"
#include "Back_Farm.h"
#include "FocusCam.h"
#include "Player.h"
#include "Object_Farm.h"
#include "Object_Farm2.h"
#include "Drop_Object.h"
#include "Chest.h"
#include "Sound_Manager.h"

CScene_Farm::CScene_Farm(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
}

HRESULT CScene_Farm::Ready_Scene()
{	
	// For. Layer_Camera
	if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;

	// For.Layer_BackGround
	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	for (size_t i = 9; i < 235; i++)
	{
		CGameObject* m_pTarget = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Object", i);
		if (nullptr == m_pTarget)
			return E_FAIL;

		m_vecObject.push_back(m_pTarget);
	}

	m_pPlayer = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr == m_pPlayer)
		return E_FAIL;

	m_pPlayer->Ready_Camera();

	m_pChange = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Change");
	if (nullptr == m_pChange)
		return E_FAIL;

	if(false == CSound_Manager::Get_Instance()->SoundPlaying(0))
		CSound_Manager::Get_Instance()->SoundPlay(0, FALSE);

	return NOERROR;
}

_int CScene_Farm::Update_Scene(const _float& fTimeDelta)
{
	if (m_pPlayer->Get_Y() < 5.f*64.f && m_pPlayer->Get_Y() > 4.f*64.f && m_pPlayer->Get_X() > 31.5f*64.f && m_pPlayer->Get_X() < 33.5f*64.f)
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

			m_pPlayer->Ready_Location(CPlayer::HOMEIN);

			if (FAILED(CManagement::GetInstance()->Ready_Scene(m_pGraphic_Device, SCENE_HOUSE)))
				return -1;

			return 0;
		}
	}

	if (m_pPlayer->Get_X() > 40.f*64.f)
	{
		if (false == m_bOnce)
		{
			m_bOnce = true;
			m_pChange->Set_Start();
			m_pPlayer->Set_StopOn();
		}

		if (false == m_pChange->Get_Start())
		{
			m_pPlayer->Set_StopOff();
			m_pPlayer->Ready_Location(CPlayer::TOWNIN);

			if (FAILED(CManagement::GetInstance()->Ready_Scene(m_pGraphic_Device, SCENE_TOWN)))
				return -1;

			return 0;
		}
	}

	for (auto& pObject : m_vecObject)
	{
		if (true == pObject->Create_DropObject())
		{
			Ready_Layer_DropObject(L"Layer_DropObject", pObject);
			pObject->Set_DropObject();
		}
	}

	CGameObject* pUI = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_BasicUI");
	if (true == pUI->Get_Install())
	{
		Ready_Layer_Chest(L"Layer_Chest", pUI->Get_DirtX(), pUI->Get_DirtY());
		pUI->Set_Install();
	}

	return CScene::Update_Scene(fTimeDelta);
}

_int CScene_Farm::LastUpdate_Scene(const _float& fTimeDelta)
{
	int		iExitCode = 0;

	iExitCode = CScene::LastUpdate_Scene(fTimeDelta);

	return iExitCode;
}

void CScene_Farm::Render_Scene()
{
	CScene::Render_Scene();

	// 디버깅적 요소를 추가해주세요.
}

// 배경에 관련된 객체들을 생성해서 모아 관리하기위해.
HRESULT CScene_Farm::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	CGameObject*			pGameObject = nullptr;

	// For.Back_Stage
	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag, CBack_Farm::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Farm::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CFocusCam::Create(m_pGraphic_Device, g_iBackCX * 0.5f, 40.0f * 64.0f - g_iBackCX * 0.5f,
			g_iBackCY * 0.5f, 30.0f * 64.0f - g_iBackCY * 0.5f ))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Farm::Ready_Layer_DropObject(const _tchar * pLayerTag, CGameObject* pObject)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (3 == pObject->Get_Frame())
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
				CDrop_Object::Create(m_pGraphic_Device, _vec3(pObject->Get_fX(), pObject->Get_fY(), 0.f), _vec3(48.f, 48.f, 1.f), 0))))
				return E_FAIL;
		}
	}
	else if (4 == pObject->Get_Frame())
	{
		for (size_t i = 0; i < 3; i++)
		{
			if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
				CDrop_Object::Create(m_pGraphic_Device, _vec3(pObject->Get_fX(), pObject->Get_fY() + 64.f * 3.2f, 0.f), _vec3(48.f, 48.f, 1.f), 0))))
				return E_FAIL;
		}
	}
	else if (5 == pObject->Get_Frame())
	{
		for (size_t i = 0; i < 6; i++)
		{
			if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
				CDrop_Object::Create(m_pGraphic_Device, _vec3(pObject->Get_fX(), pObject->Get_fY() + 64.f * 2.8f, 0.f), _vec3(48.f, 48.f, 1.f), 0))))
				return E_FAIL;
		}
	}
	else
		if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
			CDrop_Object::Create(m_pGraphic_Device, _vec3(pObject->Get_fX(), pObject->Get_fY(), 0.f), _vec3(48.f, 48.f, 1.f), pObject->Get_Frame()))))
			return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Farm::Ready_Layer_Chest(const _tchar * pLayerTag, _float fX, _float fY)
{
	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
		CObject_Farm2::Create(m_pGraphic_Device, _vec3(fX * 64.f, fY * 64.f, 0.f), _vec3(64.f, 128.f, 1.f), 18))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
		CChest::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

CScene_Farm * CScene_Farm::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Farm*		pInstance = new CScene_Farm(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSG_BOX("CScene_Farm Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Farm::Free()
{
	m_pResource_Manager->Clear_Resource(CResource_Manager::TYPE_FARM);

	CScene::Free();
}
