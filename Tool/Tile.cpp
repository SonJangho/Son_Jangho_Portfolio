#include "stdafx.h"
#include "Tile.h"
#include "Resource_Manager.h"

#include "MainFrm.h"

CTile::CTile(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}

TILEINFO CTile::Get_TileInfo()
{
	TILEINFO			TileInfo;
	TileInfo.vPosition = m_vPosition;
	TileInfo.vSize = m_vScale;
	TileInfo.iIndex = m_iTextureIdx;

	return TileInfo;
}

void CTile::Set_RenderPos(_int iScollX, _int iScollY)
{
	m_vRenderPos = m_vPosition;

	m_vRenderPos.x -= iScollX;
	m_vRenderPos.y -= iScollY;
}

HRESULT CTile::Ready_Tile(const _vec3* pPosition, const _vec3* pScale)
{
	if (FAILED(Ready_Resource()))
		return E_FAIL;

	m_vPosition = *pPosition;
	m_vScale = *pScale;

	m_iTextureIdx = 0;

	D3DXMatrixIdentity(&m_matTransform);

	m_vRenderPos = m_vPosition;
	
	m_vPoint[0] = _vec3(m_vPosition.x - pScale->x * 0.5f, m_vPosition.y - pScale->y * 0.5f, 0.f);
	m_vPoint[1] = _vec3(m_vPosition.x + pScale->x * 0.5f, m_vPosition.y	- pScale->y * 0.5f, 0.f);
	m_vPoint[2] = _vec3(m_vPosition.x + pScale->x * 0.5f, m_vPosition.y + pScale->y * 0.5f, 0.f);
	m_vPoint[3] = _vec3(m_vPosition.x - pScale->x * 0.5f, m_vPosition.y + pScale->y *0.5f, 0.f);

	m_vDir[0] = m_vPoint[1] - m_vPoint[0];
	m_vDir[1] = m_vPoint[2] - m_vPoint[1];
	m_vDir[2] = m_vPoint[3] - m_vPoint[2];
	m_vDir[3] = m_vPoint[0] - m_vPoint[3];

	m_vNormal[0] = _vec3(m_vDir[0].y * -1.f, m_vDir[0].x, 0.f);
	m_vNormal[1] = _vec3(m_vDir[1].y * -1.f, m_vDir[1].x, 0.f);
	m_vNormal[2] = _vec3(m_vDir[2].y * -1.f, m_vDir[2].x, 0.f);
	m_vNormal[3] = _vec3(m_vDir[3].y * -1.f, m_vDir[3].x, 0.f);

	for (size_t i = 0; i < 4; ++i)
		D3DXVec3Normalize(&m_vNormal[i], &m_vNormal[i]);

	return NOERROR;
}

HRESULT CTile::Ready_Tile(const TILEINFO * pTileInfo)
{
	if (FAILED(Ready_Resource()))
		return E_FAIL;

	m_vPosition = pTileInfo->vPosition;
	m_vScale = pTileInfo->vSize;

	m_iTextureIdx = pTileInfo->iIndex;

	D3DXMatrixIdentity(&m_matTransform);

	m_vRenderPos = m_vPosition;

	m_vPoint[0] = _vec3(m_vPosition.x - m_vScale.x * 0.5f, m_vPosition.y - m_vScale.y * 0.5f, 0.f);
	m_vPoint[1] = _vec3(m_vPosition.x + m_vScale.x * 0.5f, m_vPosition.y - m_vScale.y * 0.5f, 0.f);
	m_vPoint[2] = _vec3(m_vPosition.x + m_vScale.x * 0.5f, m_vPosition.y + m_vScale.y * 0.5f, 0.f);
	m_vPoint[3] = _vec3(m_vPosition.x - m_vScale.x * 0.5f, m_vPosition.y + m_vScale.y *0.5f, 0.f);


	m_vDir[0] = m_vPoint[1] - m_vPoint[0];
	m_vDir[1] = m_vPoint[2] - m_vPoint[1];
	m_vDir[2] = m_vPoint[3] - m_vPoint[2];
	m_vDir[3] = m_vPoint[0] - m_vPoint[3];

	m_vNormal[0] = _vec3(m_vDir[0].y * -1.f, m_vDir[0].x, 0.f);
	m_vNormal[1] = _vec3(m_vDir[1].y * -1.f, m_vDir[1].x, 0.f);
	m_vNormal[2] = _vec3(m_vDir[2].y * -1.f, m_vDir[2].x, 0.f);
	m_vNormal[3] = _vec3(m_vDir[3].y * -1.f, m_vDir[3].x, 0.f);

	for (size_t i = 0; i < 4; ++i)
		D3DXVec3Normalize(&m_vNormal[i], &m_vNormal[i]);


	return NOERROR;
}

void CTile::Update_Tile()
{
	_matrix			matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vRenderPos.x, m_vRenderPos.y, m_vRenderPos.z);

	m_matTransform = matScale * matTrans;	
}

void CTile::Render_Tile()
{
	if (nullptr == m_pTexture || 
		nullptr == m_pBuffer)
		return;

	if (FAILED(SetUp_RenderState()))
		return;
	

	m_pTexture->SetUp_OnGraphicDev(m_iTextureIdx);
	m_pBuffer->Render_Buffer(m_matTransform);

	if (FAILED(ReSet_RenderState()))
		return;
}

_bool CTile::Clicked_Mouse(_vec3 vMousePos)
{
	// 네개 변에 대해서 모두 오른쪽에 있는지를 체크하자.
	// 네개 변에 대해서 한번이라돠 왼쪽에 있으면 false 

	for (size_t i = 0; i < 4; ++i)
	{
		_vec3 vMouseDir = vMousePos - m_vPoint[i];

	//	// 법선, 방향벡터
		if (0 >= D3DXVec3Dot(&m_vNormal[i], D3DXVec3Normalize(&vMouseDir, &vMouseDir)))
			return false;
	}

	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	_int iIndex = pMainFrm->Get_TileIndex();

	if (iIndex & 0x80000000)
		return false;

	m_iTextureIdx = iIndex;

	return _bool(true);
}

HRESULT CTile::Ready_Resource()
{
	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)CResource_Manager::GetInstance()->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture
	m_pTexture = (CTexture*)CResource_Manager::GetInstance()->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Tiles");
	if (nullptr == m_pTexture)
		return E_FAIL;
	m_pTexture->AddRef();

	return NOERROR;
}

HRESULT CTile::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0x00);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return NOERROR;
}

HRESULT CTile::ReSet_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return NOERROR;
}

CTile * CTile::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3* pPosition, const _vec3* pScale)
{
	CTile*	pInstance = new CTile(pGraphic_Device);

	if (FAILED(pInstance->Ready_Tile(pPosition, pScale)))
	{
		_MSG_BOX("CTile Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;	
}

CTile * CTile::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const TILEINFO * pTileInfo)
{
	CTile*	pInstance = new CTile(pGraphic_Device);

	if (FAILED(pInstance->Ready_Tile(pTileInfo)))
	{
		_MSG_BOX("CTile Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTile::Free()
{
	Safe_Release(m_pBuffer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pGraphic_Device);
}
