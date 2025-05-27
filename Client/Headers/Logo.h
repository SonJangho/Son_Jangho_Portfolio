#pragma once

#include "GameObject.h"

class CTexture;
class CBuffer_RcTex;
class CLogo final : public CGameObject
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLogo() = default;
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	CTexture*				m_pTexture = nullptr;
	CBuffer_RcTex*			m_pBuffer = nullptr;
private:
	_float					m_fFrame = 0.f;
private:
	HRESULT SetUp_Resource(); // 리소스 매니져에서 찾아서 멤버에 대입해놓는다.
	HRESULT SetUp_RenderState();
	HRESULT Reset_RenderState();
public:
	static CLogo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

