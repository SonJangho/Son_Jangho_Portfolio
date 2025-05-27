#include "stdafx.h"
#include "..\Headers\Texture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CResource(pGraphic_Device)
{

}

HRESULT CTexture::Ready_Texture(const _tchar * pFilePath, const _uint & iCnt)
{
	m_vecTextures.reserve(iCnt);

	for (size_t i = 0; i < iCnt; ++i)
	{
		LPDIRECT3DTEXTURE9	pTexture = nullptr;
		_tchar				szFileName[128] = L"";
	
		wsprintf(szFileName, pFilePath, i);

		// 
		if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, szFileName, &pTexture)))
			return E_FAIL;

		m_vecTextures.push_back(pTexture);
	}

	return NOERROR;
}

HRESULT CTexture::SetUp_OnGraphicDev(const _uint & iIndex)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	if (m_vecTextures.size() <= iIndex)
		return E_FAIL;	

	m_pGraphic_Device->SetTexture(0, m_vecTextures[iIndex]);

	return NOERROR;
}

HRESULT CTexture::SetUp_OnShader(LPD3DXEFFECT pEffect, const char * pConstantName, const _uint & iIndex)
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == pEffect)
		return E_FAIL;

	if (m_vecTextures.size() <= iIndex)
		return E_FAIL;

	pEffect->SetTexture(pConstantName, m_vecTextures[iIndex]);

	return NOERROR;
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFilePath, const _uint & iCnt)
{
	CTexture*		pInstance = new CTexture(pGraphic_Device);

	if (FAILED(pInstance->Ready_Texture(pFilePath, iCnt)))
	{
		_MSG_BOX("CTexture Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTexture::Free()
{
	for (auto& pTexture : m_vecTextures)	
		Safe_Release(pTexture);

	m_vecTextures.clear();

	CResource::Free();
}
