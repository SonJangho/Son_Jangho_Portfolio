#pragma once

#include "Scene.h"

class CGameObject;
class CScene_Stage final : public CScene
{
private:
	explicit CScene_Stage(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Stage() = default;
public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LastUpdate_Scene(const _float& fTimeDelta); // 객체들에 대한 갱신이 모두 끝난후에 처리할 기능을 수행한다.
	virtual void	Render_Scene();
private:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag); // 배경레-이어에 추가하고싶은 객체를 생성.
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag); // 배경레-이어에 추가하고싶은 객체를 생성.
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BasicUI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Shadow(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Tools(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Object(const _tchar* pLayerTag);
	HRESULT Ready_Layer_ShopObject(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Inventory(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Farming(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Fishing(const _tchar* pLayerTag);
	HRESULT	Ready_Layer_Effect(const _tchar* pLayerTag);
	HRESULT Ready_Layer_ShopUI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_NPC(const _tchar* pLayerTag);
	HRESULT	SetUp_FarmObjectInfo();
	HRESULT	SetUp_ShopObjectInfo();
	HRESULT	SetUp_TownObjectInfo();
private:
	CGameObject*		m_pPlayer = nullptr;
	CGameObject*		m_pChange = nullptr;
	vector<OBJECTINFO*>	m_vecObject;
	vector<OBJECTINFO*>	m_vecObject2;
	vector<OBJECTINFO*>	m_vecObject3;
	_bool				m_bOnce = false;
	_int				m_iSize = 0;
public:
	static CScene_Stage* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

