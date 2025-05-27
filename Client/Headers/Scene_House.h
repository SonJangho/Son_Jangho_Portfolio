#pragma once

#include "Scene.h"

class CGameObject;
class CScene_House final : public CScene
{
private:
	explicit CScene_House(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_House() = default;
public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LastUpdate_Scene(const _float& fTimeDelta); // ��ü�鿡 ���� ������ ��� �����Ŀ� ó���� ����� �����Ѵ�.
	virtual void	Render_Scene();
private:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag); // ��淹-�̾ �߰��ϰ���� ��ü�� ����.
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Object(const _tchar* pLayerTag);
private:
	CGameObject*	m_pPlayer = nullptr;
	CGameObject*	m_pChange = nullptr;
	_bool			m_bOnce = false;
public:
	static CScene_House* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

