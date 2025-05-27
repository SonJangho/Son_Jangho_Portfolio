#include "stdafx.h"
#include "..\Headers\Scene_Menu.h"
#include "Object_Manager.h"
#include "Resource_Manager.h"
#include "Management.h"
#include "Back_Menu.h"
#include "FocusCam.h"
#include "Sound_Manager.h"

CScene_Menu::CScene_Menu(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
}

HRESULT CScene_Menu::Ready_Scene()
{	
	// For.Layer_BackGround
	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	CSound_Manager::Get_Instance()->SoundPlay(17, TRUE);

	m_pChange = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Change");
	if (nullptr == m_pChange)
		return E_FAIL;

	return NOERROR;
}

_int CScene_Menu::Update_Scene(const _float& fTimeDelta)
{
	POINT		ptMouse;
	ZeroMemory(&ptMouse, sizeof(POINT));

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	RECT rcRect = { 411 , 570, 411+222, 570+173 };
	RECT exitRect = { 671, 570, 671+222, 570 + 173 };

	if (TRUE == PtInRect(&rcRect, ptMouse))
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			if (m_bOnce == false)
			{
				m_bOnce = true;
				m_pChange->Set_Start();
			}

			
		}
	}

	if (m_bOnce == true)
	{
		if (false == m_pChange->Get_Start())
		{
			CSound_Manager::Get_Instance()->SoundStop(17);
			if (FAILED(CManagement::GetInstance()->Ready_Scene(m_pGraphic_Device, SCENE_STAGE)))
				return -1;

			return 0;
		}
	}

	return CScene::Update_Scene(fTimeDelta);
}

_int CScene_Menu::LastUpdate_Scene(const _float& fTimeDelta)
{
	int		iExitCode = 0;

	iExitCode = CScene::LastUpdate_Scene(fTimeDelta);

	return iExitCode;
}

void CScene_Menu::Render_Scene()
{
	CScene::Render_Scene();

	// 디버깅적 요소를 추가해주세요.
}

// 배경에 관련된 객체들을 생성해서 모아 관리하기위해.
HRESULT CScene_Menu::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	CGameObject*			pGameObject = nullptr;

	// For.Back_Stage
	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag, CBack_Menu::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

CScene_Menu * CScene_Menu::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Menu*		pInstance = new CScene_Menu(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSG_BOX("CScene_Menu Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Menu::Free()
{

	CScene::Free();
}
