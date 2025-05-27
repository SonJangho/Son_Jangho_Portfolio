#include "stdafx.h"
#include "..\Headers\Back_Stage.h"
#include "Camera.h"

CBack_Stage::CBack_Stage(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

HRESULT CBack_Stage::Ready_GameObject()
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

	return NOERROR;
}

_int CBack_Stage::Update_GameObject(const _float & fTimeDelta)
{
	RECT rcTmp = m_pPlayer->Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

	RECT rcCollision = { 0 };

	for (size_t i = 0; i < m_iNumTileY; i++)
	{
		for (size_t j = 0; j < m_iNumTileX; j++)
		{
			size_t	iIndex = i * m_iNumTileX + j;

			RECT rcTile = Get_Rect(m_vecTiles[iIndex]->vPosition.x, m_vecTiles[iIndex]->vPosition.y, m_vecTiles[iIndex]->vSize.x, m_vecTiles[iIndex]->vSize.y);
			
			if (TRUE == _bool(IntersectRect(&rcCollision, &rcTile, &rcTmp)))
			{
				_long lWidth = rcCollision.right - rcCollision.left;
				_long lHeight = rcCollision.bottom - rcCollision.top;

				if (3 == m_vecTiles[iIndex]->iIndex || 11 == m_vecTiles[iIndex]->iIndex || 12 == m_vecTiles[iIndex]->iIndex ||
					14 == m_vecTiles[iIndex]->iIndex || 16 == m_vecTiles[iIndex]->iIndex || 17 == m_vecTiles[iIndex]->iIndex)
				{
					if (lWidth <= lHeight)
					{
						if (m_pPlayer->Get_X() <= m_vecTiles[iIndex]->vPosition.x)
						{
							m_pPlayer->Push_TargetLeft();
						}
						else if (m_pPlayer->Get_X() > m_vecTiles[iIndex]->vPosition.x)
						{
							m_pPlayer->Push_TargetRight();
						}
					}
					else if (lWidth > lHeight)
					{
						if (m_pPlayer->Get_Y() +57.f <= m_vecTiles[iIndex]->vPosition.y)
						{
							m_pPlayer->Push_TargetUp();
						}
						else if (m_pPlayer->Get_Y() + 57.f > m_vecTiles[iIndex]->vPosition.y)
						{
							m_pPlayer->Push_TargetDown();

						}
					}
				}
			}
		}
	}
	return _int();
}

_int CBack_Stage::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRender_Manager)
		return -1;

	if (FAILED(m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_PRIORITY, this)))
		return -1;

	return _int();
}

void CBack_Stage::Render_GameObject()
{
	if (FAILED(SetUp_RenderState()))
		return;

	CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");

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

HRESULT CBack_Stage::SetUp_TileInfo()
{
	_ulong			dwByte = 0;

	HANDLE			hFile = CreateFile(L"../Bin/Data/HouseTile.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
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

HRESULT CBack_Stage::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;
	
	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture
	m_pTexture = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Tiles");
	if (nullptr == m_pTexture)
		return E_FAIL;
	m_pTexture->AddRef();

	m_pRect = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Rect");
	if (nullptr == m_pRect)
		return E_FAIL;
	m_pRect->AddRef();

	return NOERROR;
}

HRESULT CBack_Stage::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return NOERROR;
}

HRESULT CBack_Stage::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return NOERROR;
}

_matrix CBack_Stage::Render_TransForm(CGameObject* pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

_bool CBack_Stage::isDraw(CGameObject* pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

CBack_Stage * CBack_Stage::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBack_Stage*		pInstance = new CBack_Stage(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CBack_Stage Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBack_Stage::Free()
{
	for (auto& pTile : m_vecTiles)	
		Safe_Delete(pTile);
	m_vecTiles.clear();

	Safe_Release(m_pPlayer);
	Safe_Release(m_pTexture);

	Safe_Release(m_pRect);

	Safe_Release(m_pBuffer);

	CGameObject::Free();
}
