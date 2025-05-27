#pragma once

#include "Base.h"

// 각 씬의 부모가 되는 클래스다.
// 내 게임은 여러씬으로 관리될 수 있다. 그 여러씬의 부모.
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
	virtual _int	LastUpdate_Scene(const _float& fTimeDelta); // 객체들에 대한 갱신이 모두 끝난후에 처리할 기능을 수행한다.
	virtual void	Render_Scene();
protected:	
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	CObject_Manager*		m_pObject_Manager = nullptr;
	CRender_Manager*		m_pRender_Manager = nullptr;
	CResource_Manager*		m_pResource_Manager = nullptr;
protected:
	virtual void Free();
};

