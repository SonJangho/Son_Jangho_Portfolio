#pragma once

#include "Scene.h"

class CGameObject;
class CScene_Farm final : public CScene
{
private:
	explicit CScene_Farm(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Farm() = default;
public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LastUpdate_Scene(const _float& fTimeDelta); // ��ü�鿡 ���� ������ ��� �����Ŀ� ó���� ����� �����Ѵ�.
	virtual void	Render_Scene();
private:
	CGameObject*			m_pPlayer = nullptr;
	vector<CGameObject*>	m_vecObject;
	vector<CGameObject*>	m_vecChest;
	CGameObject*			m_pChange = nullptr;
	_bool					m_bOnce = false;
private:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag); // ��淹-�̾ �߰��ϰ���� ��ü�� ����.
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_DropObject(const _tchar* pLayerTag, CGameObject* pObject);
	HRESULT Ready_Layer_Chest(const _tchar* pLayerTag, _float fX, _float fY);
public:
	static CScene_Farm* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};
