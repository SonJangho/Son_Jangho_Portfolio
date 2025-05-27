#pragma once

#include "GameObject.h"

class CMouse final : public CGameObject
{
private:
	explicit CMouse(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CMouse() = default;
public:
	virtual HRESULT Ready_GameObject();	
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	_float Get_Frame() {
		return m_fFrame;
	}
private:
	CTexture*				m_pTexture = nullptr;
	CBuffer_RcTex*			m_pBuffer = nullptr;
private:
	_float					m_fFrame = 0.f;
	_float					m_fRealX = 0.f;
	_float					m_fRealY = 0.f;
private:
	HRESULT SetUp_Resource();
	HRESULT SetUp_RenderState();
	HRESULT Reset_RenderState();
	HRESULT Animation(_float fTimeDelta);
public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device/*, const MONSTERDESC* pMonsterDesc = nullptr*/);	
protected:
	virtual void Free(void);
};

