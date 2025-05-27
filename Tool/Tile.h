#pragma once

#include "Base.h"

class CBuffer_RcTex;
class CTexture;
class CTile final : public CBase
{
private:
	explicit CTile(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTile() = default;
public:
	TILEINFO Get_TileInfo();
public:
	void Set_RenderPos(_int iScollX, _int iScollY);
public:
	HRESULT Ready_Tile(const _vec3* pPosition, const _vec3* pScale);
	HRESULT Ready_Tile(const TILEINFO* pTileInfo);
	void Update_Tile();
	void Render_Tile();
	_bool Clicked_Mouse(_vec3 vMousePos);
private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	CBuffer_RcTex*				m_pBuffer = nullptr;
	CTexture*					m_pTexture = nullptr;
private:
	_vec3						m_vPosition;
	_vec3						m_vRenderPos;

	_vec3						m_vScale;
	_matrix						m_matTransform;
private:
	_vec3						m_vPoint[4];
	_vec3						m_vDir[4];
	_vec3						m_vNormal[4];
private:
	_int						m_iTextureIdx = 0;

private:
	HRESULT Ready_Resource();
	HRESULT SetUp_RenderState();
	HRESULT ReSet_RenderState();
public:
	static CTile* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3* pPosition, const _vec3* pScale);
	static CTile* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const TILEINFO* pTileInfo);
protected:
	virtual void Free();
};

