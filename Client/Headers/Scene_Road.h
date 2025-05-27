#pragma once

#include "Scene.h"

class CGameObject;
class CScene_Road final : public CScene
{
private:
	explicit CScene_Road(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Road() = default;
public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LastUpdate_Scene(const _float& fTimeDelta); // ��ü�鿡 ���� ������ ��� �����Ŀ� ó���� ����� �����Ѵ�.
	virtual void	Render_Scene();
private:
	CGameObject*	m_pPlayer = nullptr;
	CGameObject*	m_pChange = nullptr;
	_bool			m_bOnce = false;
private:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag); // ��淹-�̾ �߰��ϰ���� ��ü�� ����.
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
public:
	static CScene_Road* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};
