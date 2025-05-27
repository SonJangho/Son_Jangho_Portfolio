#pragma once


// 한번에 여러장의 텍스쳐를 가지는 객체.
// L"Texture_Player_Attack" : 공격모션에 해당하는 여러장의 이미지(CTexture)
// L"Texture_Player_Idle" : 공격모션에 해당하는 여러장의 이미지(CTexture)
#include "Resources.h"

class CTexture final : public CResource
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTexture() = default;	
public:
	HRESULT Ready_Texture(const _tchar* pFilePath, const _uint& iCnt);
	HRESULT SetUp_OnGraphicDev(const _uint& iIndex = 0);
private:
	vector<LPDIRECT3DTEXTURE9>			m_vecTextures;
	typedef vector<LPDIRECT3DTEXTURE9>	VECTEXTURES;
public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _uint& iCnt = 1);
protected:
	virtual void Free();
};

