#include "stdafx.h"
#include "..\Headers\Scene_Logo.h"
#include "Object_Manager.h"
#include "Resource_Manager.h"
#include "Back_Logo.h"
#include "Logo.h"
#include "Management.h"
#include "Loading.h"
#include "Font_Manager.h"
#include "Mouse.h"
#include "Sound_Manager.h"
#include "Sound.h"
#include "SceneChange.h"

CScene_Logo::CScene_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
	, m_pFont_Manager(CFont_Manager::GetInstance())
{
	m_pFont_Manager->AddRef();
}

HRESULT CScene_Logo::Ready_Scene()
{
	// 로고씬에서 사용할 리소스들을 로드한다.
	if (FAILED(Ready_Resource()))
		return E_FAIL;

	// For.Layer_BackGround
	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	// For.Layer_Mouse
	if (FAILED(Ready_Layer_Mouse(L"Layer_Mouse")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Change(L"Layer_Change")))
		return E_FAIL;

	m_pLoading = CLoading::Create(m_pGraphic_Device, SCENE_MENU);
	if (nullptr == m_pLoading)
		return E_FAIL;

	m_pChange = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Change");
	if (nullptr == m_pChange)
		return E_FAIL;

	return NOERROR;
}

_int CScene_Logo::Update_Scene(const _float& fTimeDelta)
{
	if (true == m_pLoading->Get_Finish())
	{
		if (m_bOnce == false)
		{
			m_bOnce = true;
			m_pChange->Set_Start();
		}

		if (false == m_pChange->Get_Start())
		{

			if (FAILED(CManagement::GetInstance()->Ready_Scene(m_pGraphic_Device, SCENE_MENU)))
				return -1;

			return 0;
		}
	}

	return CScene::Update_Scene(fTimeDelta);
}

_int CScene_Logo::LastUpdate_Scene(const _float& fTimeDelta)
{
	int		iExitCode = 0;

	iExitCode = CScene::LastUpdate_Scene(fTimeDelta);

	return iExitCode;
}

void CScene_Logo::Render_Scene()
{
	
	CScene::Render_Scene();

	// 로딩상태 출력.
	if (nullptr == m_pFont_Manager)
		return;

	_matrix			matTrans;

	D3DXMatrixTranslation(&matTrans, g_iBackCX >> 1, g_iBackCY - 50.f, 0.f);

	/*m_pFont_Manager->Render_Font(L"Font_Batang", m_pLoading->Get_String(), D3DXCOLOR(0.f, 0.f, 0.f, 1.f), &matTrans);*/
}

HRESULT CScene_Logo::Ready_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Resource_Texture_Logo
	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_LOGO, L"Resource_Texture_Logo", CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Logo/Logo%d.png",2))))
		return E_FAIL;

	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Mouse", CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/UI/Cursor%d.png",4))))
		return E_FAIL;

	if (FAILED(m_pResource_Manager->Add_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_SceneChange", CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Effect/SceneChange.png"))))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(BASIC_BGM)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(BASIC_BGM2)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(RAIN_BGM)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(BACK_BASIC_BGM)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(CAVE_BGM)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(BACK_CAVE_BGM)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(AM06)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(AX1)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(AX2)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(CHEST)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(DOOR)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(FISHGET)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(FISHING)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(GETITEM)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(GOLD)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(HIT)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(HOE)))
		return E_FAIL;
	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(MENU)))
		return E_FAIL;
	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(NIGHT)))
		return E_FAIL;
	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(PELICAN)))
		return E_FAIL;
	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(PICKITEM)))
		return E_FAIL;
	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(SICKLE)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(SLIMEATTACK)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(SLIMEDEAD)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(STONE)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(TOOLPICK)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(TREE)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(TURNOFF)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(TURNON)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(WATERPOT)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(WEED)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(BRANCH)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(DOWNFISHING)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(UPFISHING)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(FISHINGSTART)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(HOOK)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(HURT)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(MONSTERHIT)))
		return E_FAIL;

	if (FAILED(CSound_Manager::Get_Instance()->Add_SoundWaveFile(STUMP)))
		return E_FAIL;

	return NOERROR;
}

// 배경에 관련된 객체들을 생성해서 모아 관리하기위해.
HRESULT CScene_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	// For.Back_Logo
	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_DYNAMIC, pLayerTag, CBack_Logo::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	return NOERROR;
}


HRESULT CScene_Logo::Ready_Layer_Mouse(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
		CMouse::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Logo::Ready_Layer_Change(const _tchar * pLayerTag)
{
	if (FAILED(m_pObject_Manager->Add_Object(CObject_Manager::TYPE_STATIC, pLayerTag,
		CSceneChange::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

CScene_Logo * CScene_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Logo*		pInstance = new CScene_Logo(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSG_BOX("CScene_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Logo::Free()
{
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pLoading);

	m_pResource_Manager->Clear_Resource(CResource_Manager::TYPE_LOGO);

	CScene::Free();
}
