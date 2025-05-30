#include "stdafx.h"
#include "..\Headers\Scene_Town.h"
#include "Object_Manager.h"
#include "Resource_Manager.h"
#include "Management.h"
#include "Back_Town.h"
#include "FocusCam.h"
#include "Player.h"
#include "Sound_Manager.h"

CScene_Town::CScene_Town(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
}

HRESULT CScene_Town::Ready_Scene()
{	
	// For. Layer_Camera
	if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;

	// For.Layer_BackGround
	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	m_pPlayer = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr == m_pPlayer)
		return E_FAIL;

	m_pPlayer->Ready_Camera();

	m_pChange = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Change");
	if (nullptr == m_pChange)
		return E_FAIL;

	if (false == CSound_Manager::Get_Instance()->SoundPlaying(0) && false == CSound_Manager::Get_Instance()->SoundPlaying(19))
		CSound_Manager::Get_Instance()->SoundPlay(19, FALSE);

	return NOERROR;
}

_int CScene_Town::Update_Scene(const _float& fTimeDelta)
{
	if (m_pPlayer->Get_X() < 0.f*64.f)
	{
		if (false == m_bOnce)
		{
			CSound_Manager::Get_Instance()->SoundStop(19);
			m_bOnce = true;
			m_pChange->Set_Start();
			m_pPlayer->Set_StopOn();
		}

		if (false == m_pChange->Get_Start())
		{
			m_pPlayer->Set_StopOff();
			m_pPlayer->Ready_Location(CPlayer::FARMIN);

			if (FAILED(CManagement::GetInstance()->Ready_Scene(m_pGraphic_Device, SCENE_FARM)))
				return -1;

			return 0;
		}
	}

	if (m_pPlayer->Get_Y() < 0.f*64.f)
	{
		if (false == m_bOnce)
		{
			CSound_Manager::Get_Instance()->SoundStop(19);
			m_bOnce = true;
			m_pChange->Set_Start();
			m_pPlayer->Set_StopOn();
		}

		if (false == m_pChange->Get_Start())
		{
			m_pPlayer->Set_StopOff();
			m_pPlayer->Ready_Location(CPlayer::ROADIN);

			if (FAILED(CManagement::GetInstance()->Ready_Scene(m_pGraphic_Device, SCENE_ROAD)))
				return -1;

			return 0;
		}
	}

	if (m_pPlayer->Get_X() <= 36.5f * 64.f && m_pPlayer->Get_X() >= 34.5f * 64.f && m_pPlayer->Get_Y() < 10.95f * 64.f && m_pPlayer->Get_Y() > 9.5f * 64.f)
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
			m_pPlayer->Ready_Location(CPlayer::SHOPIN);

			if (FAILED(CManagement::GetInstance()->Ready_Scene(m_pGraphic_Device, SCENE_SHOP)))
				return -1;

			return 0;
		}
	}
	return CScene::Update_Scene(fTimeDelta);
}

_int CScene_Town::LastUpdate_Scene(const _float& fTimeDelta)
{
	int		iExitCode = 0;

	iExitCode = CScene::LastUpdate_Scene(fTimeDelta);

	return iExitCode;
}

void CScene_Town::Render_Scene()
{
	CScene::Render_Scene();

	// 디버깅적 요소를 추가해주세요.
}

// 배경에 관련된 객체들을 생성해서 모아 관리하기위해.
HRESULT CScene_Town::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	CGameObject*			pGameObject = nullptr;

	// For.Back_Stage
	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag, CBack_Town::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Town::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CFocusCam::Create(m_pGraphic_Device, g_iBackCX * 0.5f, 50.0f * 64.0f - g_iBackCX * 0.5f,
			g_iBackCY * 0.5f, 30.0f * 64.0f - g_iBackCY * 0.5f ))))
		return E_FAIL;

	return NOERROR;
}

CScene_Town * CScene_Town::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Town*		pInstance = new CScene_Town(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSG_BOX("CScene_Town Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Town::Free()
{
	CScene::Free();
}
