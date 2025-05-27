#include "stdafx.h"
#include "..\Headers\Scene_House.h"
#include "Object_Manager.h"
#include "Resource_Manager.h"
#include "Management.h"
#include "Back_House.h"
#include "Player.h"
#include "FocusCam.h"
#include "Object_Stage.h"
#include "Sound_Manager.h"

CScene_House::CScene_House(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
}

HRESULT CScene_House::Ready_Scene()
{	
	// For. Layer_Camera
	if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;

	// For.Layer_BackGround
	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Object(L"Layer_Object")))
		return E_FAIL;

	m_pPlayer = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr == m_pPlayer)
		return E_FAIL;

	m_pPlayer->Ready_Camera();

	m_pChange = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Change");
	if (nullptr == m_pChange)
		return E_FAIL;

	return NOERROR;
}

_int CScene_House::Update_Scene(const _float& fTimeDelta)
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

_int CScene_House::LastUpdate_Scene(const _float& fTimeDelta)
{
	int		iExitCode = 0;

	iExitCode = CScene::LastUpdate_Scene(fTimeDelta);

	return iExitCode;
}

void CScene_House::Render_Scene()
{
	CScene::Render_Scene();

	// 디버깅적 요소를 추가해주세요.
}

// 배경에 관련된 객체들을 생성해서 모아 관리하기위해.
HRESULT CScene_House::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	CGameObject*			pGameObject = nullptr;

	// For.Back_Stage
	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag, CBack_House::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_House::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CFocusCam::Create(m_pGraphic_Device, g_iBackCX * 0.5f, 30.0f * 64.0f - g_iBackCX * 0.5f,
			g_iBackCY * 0.5f, 23.0f * 64.0f - g_iBackCY * 0.5f ))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_House::Ready_Layer_Object(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CObject_Stage::Create(m_pGraphic_Device, 11.f*64.f, 11.f*64.f, 32.f*4.f, 64.f*4.f, 0))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CObject_Stage::Create(m_pGraphic_Device, 19.f * 64.f, 15.f*64.f, 32.f*4.f, 64.f*4.f, 1,1))))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CObject_Stage::Create(m_pGraphic_Device, 19.f * 64.f, 15.f*64.f, 32.f*4.f, 64.f*4.f, 2, 2))))
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
		CObject_Stage::Create(m_pGraphic_Device, 15.f * 64.f, 15.f*64.f, 64.f*4.f, 32.f*4.f, 7, 1))))
		return E_FAIL;

	return NOERROR;
}

CScene_House * CScene_House::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_House*		pInstance = new CScene_House(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSG_BOX("CScene_House Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_House::Free()
{
	CScene::Free();
}
