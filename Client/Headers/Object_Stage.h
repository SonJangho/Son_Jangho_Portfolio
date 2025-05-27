#pragma once

#include "GameObject.h"

class CObject_Stage final : public CGameObject
{
private:
	explicit CObject_Stage(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CObject_Stage() = default;
public:
	virtual HRESULT Ready_GameObject(_float fX, _float fY, _float fSizeX, _float fSizeY, _int Texture, _int Ysort);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	CTexture*				m_pFurniture[10] = { nullptr };
	CTexture*				m_pRect = nullptr;
	CBuffer_RcTex*			m_pBuffer = nullptr;
	CGameObject*			m_pPlayer;
private:
	D3DXMATRIX				TempMatrix;
	_int					m_Ysort = 0;
	_int					m_iTexture = 0;
private:
	HRESULT SetUp_Resource();
	HRESULT SetUp_RenderState();
	HRESULT Reset_RenderState();
	_matrix Render_TransForm(CGameObject* pCamera);
	_bool	isDraw(CGameObject* pCamera);
public:
	static CObject_Stage* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fX, _float fY, _float fSizeX, _float fSizeY ,_int Texture, const _int &Ysort = 0);
protected:
	virtual void Free();
};

