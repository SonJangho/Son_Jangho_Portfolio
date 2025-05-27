#include "stdafx.h"
#include "Texture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CResource(pGraphic_Device)
{

}

// L"../Bin/Reosurce/Texture/TextureName.png"
// L"../Bin/Reosurce/Texture/TextureName_00%d.png"

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

		//_uint		iLength = lstrlen(szFileName);

		//for (size_t i = 0; i < iLength; ++i)
		//{
		//	if ('.' == szFileName[i])
		//	{
		//		szFileName[i + 1] = 'b';
		//		szFileName[i + 2] = 'm';
		//		szFileName[i + 3] = 'p';
		//		break;
		//	}
		//}

		//if (FAILED(D3DXSaveTextureToFile(szFileName, D3DXIFF_BMP, pTexture, nullptr)))
		//	return E_FAIL;
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
