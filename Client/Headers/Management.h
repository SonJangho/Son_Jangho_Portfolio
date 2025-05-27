#pragma once

#include "Base.h"

// 현재 사용해야할 씬을ㅇ 보관 관리한다.
// 이 객체는 오로지 내 게임내에 하나만 존재한다.
class CScene;
class CManagement final : public CBase
{
	_DECLARE_SINGLETON(CManagement)
private:
	explicit CManagement();
	virtual ~CManagement() = default;
public:
	HRESULT Ready_Scene(LPDIRECT3DDEVICE9 pGraphic_Device, SCENEID eID);
	_int Update_Scene(const _float& fTimeDelta);
	void Render_Scene();
public:
	_int	Get_Scene() {
		return m_iScene;
	}
private:
	// 현재 내 게임에 보여줘야할 씬의 주소를 보관한다.
	CScene*			m_pCurrentScene = nullptr;
	_int			m_iScene = 0;

protected:
	virtual void Free();

};

