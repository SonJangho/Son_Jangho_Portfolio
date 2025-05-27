#pragma once

#include "GameObject.h"

class CObject_House final : public CGameObject
{
private:
	explicit CObject_House(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CObject_House() = default;
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	CTexture*				m_pFurniture[10] = { nullptr };
	CTexture*				m_pRect = nullptr;
	CBuffer_RcTex*			m_pBuffer = nullptr;
private:
	D3DXMATRIX				TempMatrix;
	CGameObject*			m_pPlayer;
private:
	HRESULT SetUp_Resource();
	HRESULT SetUp_RenderState();
	HRESULT Reset_RenderState();
	_matrix Render_TransForm(CGameObject* pCamera);
	_bool	isDraw(CGameObject* pCamera);
public:
	static CObject_House* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

