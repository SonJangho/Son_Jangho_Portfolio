#include "stdafx.h"
#include "..\Headers\Back_Farm.h"
#include "Camera.h"

CBack_Farm::CBack_Farm(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

HRESULT CBack_Farm::Ready_GameObject()
{
	if (FAILED(CGameObject::Ready_GameObject()))
		return E_FAIL;

	if (FAILED(SetUp_TileInfo()))
		return E_FAIL;

	if (FAILED(SetUp_Resource()))
		return E_FAIL;	

	CGameObject* pPlayer = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr == pPlayer)
		return E_FAIL;
	m_pPlayer = pPlayer;
	m_pPlayer->AddRef();

	m_fFrame = 0.f;
	return NOERROR;
}

_int CBack_Farm::Update_GameObject(const _float & fTimeDelta)
{
	m_fFrame += fTimeDelta * 2;
	if (m_fFrame >= 2.f)
		m_fFrame = 0.f;

	RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

	if (rcTmp.top - 30.f < 64.f * 0.f)
		m_pPlayer->Push_TargetDown();

	if (rcTmp.bottom > 64.f * 30.f)
		m_pPlayer->Push_TargetUp();

	if (rcTmp.right > 64.f * 40.5f)
		m_pPlayer->Push_TargetLeft();

	if (rcTmp.left < 64.f * 0)
		m_pPlayer->Push_TargetRight();

	RECT rcCollision = { 0 };
	RECT rcTile = Get_Rect(35.f*64.f, 18.f*64.f, 384.f, 384.f);
	if (TRUE == _bool(IntersectRect(&rcCollision, &rcTile, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (m_pPlayer->Get_X() <= 35.f*64.f)
			{
				m_pPlayer->Push_TargetLeft();
			}
			else if (m_pPlayer->Get_X() > 35.f*64.f)
			{
				m_pPlayer->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (m_pPlayer->Get_Y() + 57.f <= 18.f*64.f)
			{
				m_pPlayer->Push_TargetUp();
			}
			else if (m_pPlayer->Get_Y() + 57.f > 18.f*64.f)
			{
				m_pPlayer->Push_TargetDown();
			}
		}
	}

	return _int();
}

_int CBack_Farm::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRender_Manager)
		return -1;

	if (FAILED(m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_PRIORITY, this)))
		return -1;

	return _int();
}

void CBack_Farm::Render_GameObject()
{
	if (FAILED(SetUp_RenderState()))
		return;

	CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");
	
	Set_Scale(_vec3(384.f,384.f,1.0f));
	Set_Position(_vec3(35.f*64.f,18.5f*64.f,0.f));
	TempMatrix = Render_TransForm(pCamera);
	m_pWater->SetUp_OnGraphicDev(m_fFrame);
	m_pBuffer->Render_Buffer(TempMatrix);

		for (size_t i = 0; i < m_iNumTileY; i++)
		{
			for (size_t j = 0; j < m_iNumTileX; j++)
			{
				size_t	iIndex = i * m_iNumTileX + j;
				
					Set_Scale(m_vecTiles[iIndex]->vSize);
					Set_Position(m_vecTiles[iIndex]->vPosition);

					TempMatrix = Render_TransForm(pCamera);

					if (true == isDraw(pCamera))
					{
						m_pTexture->SetUp_OnGraphicDev(m_vecTiles[iIndex]->iIndex);
						m_pBuffer->Render_Buffer(TempMatrix);

						if (GetKeyState(VK_F1))
						{
							m_pRect->SetUp_OnGraphicDev(0);
							m_pBuffer->Render_Buffer(TempMatrix);
						}
					}
			}
		}

	if (FAILED(Reset_RenderState()))
		return;
}

HRESULT CBack_Farm::SetUp_TileInfo()
{
	_ulong			dwByte = 0;

	HANDLE			hFile = CreateFile(L"../Bin/Data/FarmTile.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	ReadFile(hFile, &m_iNumTileX, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &m_iNumTileY, sizeof(_uint), &dwByte, nullptr);	

	m_iNumTiles = m_iNumTileX * m_iNumTileY;

	m_vecTiles.reserve(m_iNumTiles);

	for (;;)
	{
		TILEINFO*		pTileInfo = new TILEINFO;
		ZeroMemory(pTileInfo, sizeof(TILEINFO));

		ReadFile(hFile, pTileInfo, sizeof(TILEINFO), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pTileInfo);
			break;
		}

		m_vecTiles.push_back(pTileInfo);
	}

	CloseHandle(hFile);

	return NOERROR;
}

HRESULT CBack_Farm::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;
	
	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture
	m_pTexture = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_FarmTiles");
	if (nullptr == m_pTexture)
		return E_FAIL;
	m_pTexture->AddRef();

	m_pWater = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_WaterTiles");
	if (nullptr == m_pWater)
		return E_FAIL;
	m_pWater->AddRef();

	m_pRect = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Rect");
	if (nullptr == m_pRect)
		return E_FAIL;
	m_pRect->AddRef();

	return NOERROR;
}

HRESULT CBack_Farm::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return NOERROR;
}

HRESULT CBack_Farm::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return NOERROR;
}

_matrix CBack_Farm::Render_TransForm(CGameObject* pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

_bool CBack_Farm::isDraw(CGameObject * pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

CBack_Farm * CBack_Farm::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBack_Farm*		pInstance = new CBack_Farm(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CBack_Farm Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBack_Farm::Free()
{
	for (auto& pTile : m_vecTiles)	
		Safe_Delete(pTile);
	m_vecTiles.clear();

	Safe_Release(m_pPlayer);
	Safe_Release(m_pWater);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRect);

	Safe_Release(m_pBuffer);

	CGameObject::Free();
}
