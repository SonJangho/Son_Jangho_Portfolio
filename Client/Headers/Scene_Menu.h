#pragma once

#include "Scene.h"

class CGameObject;
class CScene_Menu final : public CScene
{
private:
	explicit CScene_Menu(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Menu() = default;
public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LastUpdate_Scene(const _float& fTimeDelta); // 객체들에 대한 갱신이 모두 끝난후에 처리할 기능을 수행한다.
	virtual void	Render_Scene();
private:
	CGameObject*	m_pPlayer = nullptr;
	CGameObject*	m_pChange = nullptr;
	_bool			m_bOnce = false;
private:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag); // 배경레-이어에 추가하고싶은 객체를 생성.
public:
	static CScene_Menu* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};
