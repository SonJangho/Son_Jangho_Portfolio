#include "stdafx.h"
#include "..\Headers\Management.h"
#include "Scene_Logo.h"
#include "Scene_Menu.h"
#include "Scene_Stage.h"
#include "Scene_Farm.h"
#include "Scene_House.h"
#include "Scene_Town.h"
#include "Scene_Shop.h"
#include "Scene_Road.h"
#include "Scene_Cave.h"

_IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
{
}

HRESULT CManagement::Ready_Scene(LPDIRECT3DDEVICE9 pGraphic_Device, SCENEID eID)
{
	_ulong	dwRefCnt = 0;

	if (dwRefCnt = Safe_Release(m_pCurrentScene))
	{
		_MSG_BOX("CScene Release Failed");
		return E_FAIL;
	}

	m_iScene = eID;

	switch (eID)
	{
	case SCENE_LOGO:
		m_pCurrentScene = CScene_Logo::Create(pGraphic_Device);
		break;	
	case SCENE_MENU:
		m_pCurrentScene = CScene_Menu::Create(pGraphic_Device);
		break;
	case SCENE_STAGE:
		m_pCurrentScene = CScene_Stage::Create(pGraphic_Device);
		break;
	case SCENE_HOUSE:
		m_pCurrentScene = CScene_House::Create(pGraphic_Device);
		break;
	case SCENE_FARM:
		m_pCurrentScene = CScene_Farm::Create(pGraphic_Device);
		break;
	case SCENE_TOWN:
		m_pCurrentScene = CScene_Town::Create(pGraphic_Device);
		break;
	case SCENE_SHOP:
		m_pCurrentScene = CScene_Shop::Create(pGraphic_Device);
		break;
	case SCENE_ROAD:
		m_pCurrentScene = CScene_Road::Create(pGraphic_Device);
		break;
	case SCENE_CAVE:
		m_pCurrentScene = CScene_Cave::Create(pGraphic_Device);
		break;
	}

	if (nullptr == m_pCurrentScene)
		return E_FAIL;

	return NOERROR;
}

_int CManagement::Update_Scene(const _float& fTimeDelta)
{
	if (nullptr == m_pCurrentScene)
		return -1;

	_int		iExitCode = 0;

	iExitCode = m_pCurrentScene->Update_Scene(fTimeDelta);
	if (iExitCode & 0x80000000)
		return iExitCode;

	iExitCode = m_pCurrentScene->LastUpdate_Scene(fTimeDelta);
	if (iExitCode & 0x80000000)
		return iExitCode;

	return iExitCode;
}

void CManagement::Render_Scene()
{
	if (nullptr == m_pCurrentScene)
		return;

	m_pCurrentScene->Render_Scene();
}

void CManagement::Free()
{
	_ulong		dwRefCnt = 0;

	if (dwRefCnt = Safe_Release(m_pCurrentScene))	
		_MSG_BOX("CManagement Release Failed");

}
