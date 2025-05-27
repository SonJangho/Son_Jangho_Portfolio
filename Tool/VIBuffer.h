#pragma once

#include "Resources.h"

class CVIBuffer abstract : public CResource
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CVIBuffer() = default;
public:
	HRESULT Ready_VIBuffer();
	void Render_Buffer(D3DXMATRIX matTransform);
protected:
	LPDIRECT3DVERTEXBUFFER9			m_pVB = nullptr;
	_uint							m_iVertexSize = 0;
	_uint							m_iNumVertices = 0;
	_ulong							m_dwVtxFVF = 0;	
protected:
	LPDIRECT3DINDEXBUFFER9			m_pIB = nullptr;
	_uint							m_iPolygonSize = 0;	
	_uint							m_iNumPolygons = 0;
	D3DFORMAT						m_IndexFmt;
protected:
	D3DXVECTOR4*					m_pPosition = nullptr;
protected:
	virtual void Free();
};

