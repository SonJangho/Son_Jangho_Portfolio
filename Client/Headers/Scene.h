#pragma once

#include "Base.h"

// �� ���� �θ� �Ǵ� Ŭ������.
// �� ������ ���������� ������ �� �ִ�. �� �������� �θ�.
class CObject_Manager;
class CRender_Manager;
class CResource_Manager;
class CScene abstract : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene() = default;
public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LastUpdate_Scene(const _float& fTimeDelta); // ��ü�鿡 ���� ������ ��� �����Ŀ� ó���� ����� �����Ѵ�.
	virtual void	Render_Scene();
protected:	
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	CObject_Manager*		m_pObject_Manager = nullptr;
	CRender_Manager*		m_pRender_Manager = nullptr;
	CResource_Manager*		m_pResource_Manager = nullptr;
protected:
	virtual void Free();
};

