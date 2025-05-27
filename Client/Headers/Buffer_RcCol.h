#pragma once

#include "VIBuffer.h"

class CBuffer_RcCol final : public CVIBuffer
{
private:
	explicit CBuffer_RcCol(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CBuffer_RcCol() = default;
public:
	HRESULT Ready_Buffer_RcCol();

	
public:
	static CBuffer_RcCol* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

