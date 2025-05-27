#pragma once

#include "VIBuffer.h"

class CBuffer_RcTexPos final : public CVIBuffer
{
private:
	explicit CBuffer_RcTexPos(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CBuffer_RcTexPos() = default;
public:
	HRESULT Ready_Buffer_RcTex();
	virtual void Render_Buffer(D3DXMATRIX matTransform);
	
public:
	static CBuffer_RcTexPos* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

