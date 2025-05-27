#pragma once

#include "GameObject.h"

class CShadow final : public CGameObject
{
private:
	explicit CShadow(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CShadow() = default;
public:
	virtual HRESULT Ready_GameObject();	
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	CGameObject*			m_pPlayer = nullptr;
	CTexture*				m_pTexture = nullptr;
	CBuffer_RcTex*			m_pBuffer = nullptr;
private:
	_float					m_fFrame = 0.f;
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

