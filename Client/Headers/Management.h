#pragma once

#include "Base.h"

// ���� ����ؾ��� ������ ���� �����Ѵ�.
// �� ��ü�� ������ �� ���ӳ��� �ϳ��� �����Ѵ�.
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
	// ���� �� ���ӿ� ��������� ���� �ּҸ� �����Ѵ�.
	CScene*			m_pCurrentScene = nullptr;
	_int			m_iScene = 0;

protected:
	virtual void Free();

};

