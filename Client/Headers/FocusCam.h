#pragma once

#include "Camera.h"

class CFocusCam final : public CCamera
{
private:
	explicit CFocusCam(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CFocusCam() = default;
public:
	HRESULT SetUp_Focus(CGameObject* pTarget);
public:
	virtual HRESULT Ready_GameObject(const _float & fScrollMinX, const _float & fScrollMaxX, const _float & fScrollMinY, const _float & fScrollMaxY);
	virtual _int Update_GameObject(const _float & fTimeDelta);
	virtual _int LastUpdate_GameObject();
	virtual void Render_GameObject();
private:
	_bool			m_isLimit = false;
	POSITION		m_ScrollMin, m_ScrollMax;
	CGameObject*	m_pTarget = nullptr;
	_uint			m_dwTime;
	_float			m_X;
	_float			m_Y;
	_bool			m_bFirst = false;
private:
	_float Linear(float v0, float v1, float t) {
		return v0 * (1.0f - t) + v1 * t;
	}
public:
	static CFocusCam* Create(LPDIRECT3DDEVICE9 pGraphic_Device,const _float& fScrollMinX = 0.f, const _float& fScrollMaxX = 0.f
	, const _float& fScrollMinY = 0.f, const _float& fScrollMaxY = 0.f);
protected:
	virtual void Free();


};

