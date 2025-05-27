#pragma once

#include "GameObject.h"

class CBack_Town final : public CGameObject
{
private:
	explicit CBack_Town(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CBack_Town() = default;
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	CTexture*				m_pTexture = nullptr;
	CTexture*				m_pRect = nullptr;
	CBuffer_RcTex*			m_pBuffer = nullptr;
private:
	_uint					m_iNumTileX;
	_uint					m_iNumTileY;
	_uint					m_iNumTiles;
	vector<TILEINFO*>		m_vecTiles;
	D3DXMATRIX				TempMatrix;
	CGameObject*			m_pPlayer;
private:
	HRESULT SetUp_TileInfo();
	HRESULT SetUp_Resource();
	HRESULT SetUp_RenderState();
	HRESULT Reset_RenderState();
	_matrix Render_TransForm(CGameObject* pCamera);
	_bool	isDraw(CGameObject* pCamera);
public:
	static CBack_Town* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

