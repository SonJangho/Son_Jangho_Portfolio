#include "stdafx.h"
#include "..\Headers\Scene_Road.h"
#include "Object_Manager.h"
#include "Resource_Manager.h"
#include "Management.h"
#include "Back_Road.h"
#include "FocusCam.h"
#include "Player.h"
#include "Sound_Manager.h"

CScene_Road::CScene_Road(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
}

HRESULT CScene_Road::Ready_Scene()
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

	return NOERROR;
}

_int CScene_Road::Update_Scene(const _float& fTimeDelta)
{
	if (m_pPlayer->Get_Y() > 30.f*64.f)
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
			m_pPlayer->Ready_Location(CPlayer::ROADOUT);

			if (FAILED(CManagement::GetInstance()->Ready_Scene(m_pGraphic_Device, SCENE_TOWN)))
				return -1;

			return 0;
		}
	}

	if (m_pPlayer->Get_Y() > 3.5f*64.f && m_pPlayer->Get_Y() < 4.5f*64.f && m_pPlayer->Get_X() > 16.f*64.f && m_pPlayer->Get_X() < 17.f*64.f)
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
			m_pPlayer->Ready_Location(CPlayer::CAVEIN);

			if (FAILED(CManagement::GetInstance()->Ready_Scene(m_pGraphic_Device, SCENE_CAVE)))
				return -1;

			return 0;
		}
	}

	return CScene::Update_Scene(fTimeDelta);
}

_int CScene_Road::LastUpdate_Scene(const _float& fTimeDelta)
{
	int		iExitCode = 0;

	iExitCode = CScene::LastUpdate_Scene(fTimeDelta);

	return iExitCode;
}

void CScene_Road::Render_Scene()
{
	CScene::Render_Scene();

	// 디버깅적 요소를 추가해주세요.
}

// 배경에 관련된 객체들을 생성해서 모아 관리하기위해.
HRESULT CScene_Road::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	CGameObject*			pGameObject = nullptr;

	// For.Back_Stage
	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag, CBack_Road::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Road::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag,
		CFocusCam::Create(m_pGraphic_Device, g_iBackCX * 0.5f, 20.0f * 64.0f - g_iBackCX * 0.5f,
			g_iBackCY * 0.5f, 30.0f * 64.0f - g_iBackCY * 0.5f ))))
		return E_FAIL;

	return NOERROR;
}

CScene_Road * CScene_Road::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Road*		pInstance = new CScene_Road(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSG_BOX("CScene_Road Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Road::Free()
{
	m_pResource_Manager->Clear_Resource(CResource_Manager::TYPE_SHOP);

	CScene::Free();
}
