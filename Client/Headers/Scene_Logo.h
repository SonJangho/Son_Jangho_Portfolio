#pragma once

#include "Scene.h"

class CLoading;
class CFont_Manager;
class CGameObject;
class CScene_Logo final : public CScene
{
private:
	explicit CScene_Logo(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Logo() = default;
public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LastUpdate_Scene(const _float& fTimeDelta); // 객체들에 대한 갱신이 모두 끝난후에 처리할 기능을 수행한다.
	virtual void	Render_Scene();
private:
	CLoading*		m_pLoading = nullptr;
	CFont_Manager*	m_pFont_Manager = nullptr;
	CGameObject*	m_pChange = nullptr;
	_bool			m_bOnce = false;
private:
	HRESULT Ready_Resource();
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag); // 배경레-이어에 추가하고싶은 객체를 생성.
	HRESULT Ready_Layer_Mouse(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Change(const _tchar* pLayerTag);
	//HRESULT Ready_Layer_Player(const _tchar* pLayerTag); // 배경레-이어에 추가하고싶은 객체를 생성.
public:
	static CScene_Logo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

