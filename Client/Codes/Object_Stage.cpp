#include "stdafx.h"
#include "..\Headers\Object_Stage.h"
#include "Camera.h"

CObject_Stage::CObject_Stage(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

HRESULT CObject_Stage::Ready_GameObject(_float fX, _float fY, _float fSizeX, _float fSizeY, _int Texture, _int Ysort)
{
	if (FAILED(CGameObject::Ready_GameObject()))
		return E_FAIL;

	if (FAILED(SetUp_Resource()))
		return E_FAIL;	

	CGameObject* pPlayer = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr == pPlayer)
		return E_FAIL;
	m_pPlayer = pPlayer;

	m_pPlayer->AddRef();

	m_fX = fX;
	m_fY = fY;
	m_fSizeX = fSizeX;
	m_fSizeY = fSizeY;
	m_Ysort = Ysort;
	m_iTexture = Texture;

	Set_Scale(_vec3(m_fSizeX, m_fSizeY, 1.f));
	Set_Position(_vec3(m_fX, m_fY, 0.f));

	return NOERROR;
}

_int CObject_Stage::Update_GameObject(const _float & fTimeDelta)
{
	if (1 == m_iTexture)
	{
		return 0;
	}
	if (0 == m_Ysort)
	{

		RECT rcTmp = m_pPlayer->Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

		RECT rcCollision = { 0 };

		RECT rcObject = Get_Rect(m_fX, m_fY, m_fSizeX, m_fSizeY);

		// Check if the player and the object are colliding
		// rcCollision: intersection area, rcObject: object rect, rcTmp: player rect
		if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp))) 
		{
			// Calculate the width and height of the overlapping area
			_long lWidth = rcCollision.right - rcCollision.left;
			_long lHeight = rcCollision.bottom - rcCollision.top;

			// Determine the collision direction based on overlap size
			if (lWidth <= lHeight)
			{
				// Player is on the left side of the object: push left
				if (m_pPlayer->Get_X() <= m_fX)
				{
					m_pPlayer->Push_TargetLeft();

				}
				// Player is on the right side of the object: push right
				else if (m_pPlayer->Get_X() > m_fX)
				{
					m_pPlayer->Push_TargetRight();

				}
			}
			// If the overlap is taller vertically than horizontally
			else if (lWidth > lHeight)
			{
				// Player is above the object: push up
				if (m_pPlayer->Get_Y() + 57.f <= m_fY)
				{
					m_pPlayer->Push_TargetUp();

				}
				// Player is below the object: push down
				else if (m_pPlayer->Get_Y() + 57.f > m_fY)
				{
					m_pPlayer->Push_TargetDown();

				}
			}
		}
	}

	return _int();
}

_int CObject_Stage::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRender_Manager)
		return -1;

	if (0 == m_Ysort)
	{
		if (FAILED(m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_YSORT, this)))
			return -1;
	}
	else if (1 == m_Ysort)
	{
		if (FAILED(m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_PRIORITY, this)))
			return -1;
	}
	else if (2 == m_Ysort)
	{
		if (FAILED(m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_COVER, this)))
			return -1;
	}

	return _int();
}

void CObject_Stage::Render_GameObject()
{
	if (FAILED(SetUp_RenderState()))
		return;

	CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");

	TempMatrix = Render_TransForm(pCamera);

	CGameObject* pTarget = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_BasicUI");
	if (m_iTexture == 6)
	{
		if (true == pTarget->Get_RainDay() || 90 + 9 * 15 <= pTarget->Get_TimeDegree())
			m_pFurniture[6]->SetUp_OnGraphicDev(1);
		else
			m_pFurniture[6]->SetUp_OnGraphicDev(0);

			m_pBuffer->Render_Buffer(TempMatrix);
	}
	else
	{
		m_pFurniture[m_iTexture]->SetUp_OnGraphicDev(0);
		m_pBuffer->Render_Buffer(TempMatrix);
	}
	if (GetKeyState(VK_F1))
	{
		m_pRect->SetUp_OnGraphicDev(0);
		m_pBuffer->Render_Buffer(TempMatrix);
	}

	if (FAILED(Reset_RenderState()))
		return;
}

HRESULT CObject_Stage::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;
	
	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	m_pFurniture[0] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_TV");
	if (nullptr == m_pFurniture[0])
		return E_FAIL;
	m_pFurniture[0]->AddRef();

	m_pFurniture[1] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Bed0");
	if (nullptr == m_pFurniture[1])
		return E_FAIL;
	m_pFurniture[1]->AddRef();

	m_pFurniture[2] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Bed1");
	if (nullptr == m_pFurniture[2])
		return E_FAIL;
	m_pFurniture[2]->AddRef();

	m_pFurniture[3] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Table");
	if (nullptr == m_pFurniture[3])
		return E_FAIL;
	m_pFurniture[3]->AddRef();

	m_pFurniture[4] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Chair");
	if (nullptr == m_pFurniture[4])
		return E_FAIL;
	m_pFurniture[4]->AddRef();

	m_pFurniture[5] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Fireplace");
	if (nullptr == m_pFurniture[5])
		return E_FAIL;
	m_pFurniture[5]->AddRef();

	m_pFurniture[6] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Window");
	if (nullptr == m_pFurniture[6])
		return E_FAIL;
	m_pFurniture[6]->AddRef();

	m_pFurniture[7] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Carpet");
	if (nullptr == m_pFurniture[7])
		return E_FAIL;
	m_pFurniture[7]->AddRef();

	m_pRect = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Rect");
	if (nullptr == m_pRect)
		return E_FAIL;
	m_pRect->AddRef();

	return NOERROR;
}

HRESULT CObject_Stage::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return NOERROR;
}

HRESULT CObject_Stage::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return NOERROR;
}

_matrix CObject_Stage::Render_TransForm(CGameObject* pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

_bool CObject_Stage::isDraw(CGameObject * pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

CObject_Stage * CObject_Stage::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fX, _float fY, _float fSizeX, _float fSizeY, _int Texture, const _int &Ysort)
{
	CObject_Stage*		pInstance = new CObject_Stage(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject(fX, fY, fSizeX, fSizeY, Texture, Ysort)))
	{
		_MSG_BOX("CObject_Stage Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_Stage::Free()
{
	Safe_Release(m_pPlayer);
	
	for (auto& pFurniture : m_pFurniture)
		Safe_Release(pFurniture);

	Safe_Release(m_pRect);

	Safe_Release(m_pBuffer);

	CGameObject::Free();
}
