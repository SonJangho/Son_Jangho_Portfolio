#pragma once

#include "Scene.h"

class CGameObject;
class CScene_Cave final : public CScene
{
private:
	explicit CScene_Cave(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Cave() = default;
public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LastUpdate_Scene(const _float& fTimeDelta); // 객체들에 대한 갱신이 모두 끝난후에 처리할 기능을 수행한다.
	virtual void	Render_Scene();
private:
	CGameObject*			m_pPlayer = nullptr;
	CGameObject*			m_pChange = nullptr;
	vector<CGameObject*>	m_vecMineral;
	vector<CGameObject*>	m_vecMonster;
	_bool					m_bOnce = false;
	_int					m_iCount = 0;
	_int					m_iCount2 = 0;
private:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag); // 배경레-이어에 추가하고싶은 객체를 생성.
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Mineral(const _tchar* pLayerTag);
	HRESULT Ready_Layer_DropObject(const _tchar* pLayerTag, CGameObject* pObject);
public:
	static CScene_Cave* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};
