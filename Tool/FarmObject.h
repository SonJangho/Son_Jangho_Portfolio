#pragma once

#include "Base.h"

class CBuffer_RcTex;
class CTexture;
class CFarmObject final : public CBase
{
private:
	explicit CFarmObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CFarmObject() = default;
public:
	OBJECTINFO Get_ObjectInfo();
public:
	void Set_RenderPos(_int iScollX, _int iScollY);
public:
	HRESULT Ready_Object(const _vec3* pPosition);
	HRESULT Ready_Object(const OBJECTINFO* pObjectInfo);
	void Update_Object();
	void Render_Object();
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
	static CFarmObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3* pPosition);
	static CFarmObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const OBJECTINFO* pOjbectInfo);
protected:
	virtual void Free();
};

