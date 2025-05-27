#include "stdafx.h"
#include "..\Headers\Back_Cave.h"
#include "Camera.h"

CBack_Cave::CBack_Cave(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

HRESULT CBack_Cave::Ready_GameObject()
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
	m_fFrame = 1;	

	return NOERROR;
}

_int CBack_Cave::Update_GameObject(const _float & fTimeDelta)
{	
	Tile_CollisionPlayer(m_pPlayer, 57.f, 50.f, 32.f);

	for (size_t i = 0; i < 10; i++)
	{
		CGameObject* pMonster = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Monster", i);
		if (nullptr != pMonster)
			Tile_CollisionMonster(pMonster, 32.f, 64.f, 64.f);
	}

	return _int();
}

_int CBack_Cave::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRender_Manager)
		return -1;

	if (FAILED(m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_PRIORITY, this)))
		return -1;

	return _int();
}

void CBack_Cave::Render_GameObject()
{
	if (FAILED(SetUp_RenderState()))
		return;

	CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");
	
	Set_Scale(_vec3(2560.f, 1280.f, 1.f));
	Set_Position(_vec3(2560 >> 1, 1280 >> 1, 0.f));
	TempMatrix = Render_TransForm(pCamera);
	m_pTexture->SetUp_OnGraphicDev(0);
	m_pBuffer->Render_Buffer(TempMatrix);

	if (FAILED(Reset_RenderState()))
		return;
}

HRESULT CBack_Cave::SetUp_TileInfo()
{
	return NOERROR;
}

HRESULT CBack_Cave::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;
	
	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture
	m_pTexture = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_CaveTiles");
	if (nullptr == m_pTexture)
		return E_FAIL;
	m_pTexture->AddRef();

	m_pRect = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Rect");
	if (nullptr == m_pRect)
		return E_FAIL;
	m_pRect->AddRef();

	return NOERROR;
}

HRESULT CBack_Cave::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return NOERROR;
}

HRESULT CBack_Cave::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return NOERROR;
}

_matrix CBack_Cave::Render_TransForm(CGameObject* pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

_bool CBack_Cave::isDraw(CGameObject * pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

void CBack_Cave::Tile_CollisionMonster(CGameObject * pTarget, _float fY, _float SizeX, _float SizeY)
{
	RECT rcTmp = Get_Rect(pTarget->Get_X2(), pTarget->Get_Y2() + fY, SizeX, SizeY);
	RECT rcCollision = { 0 };
	RECT rcObject = Get_Rect(1248, 1120, 1856, 64);

	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X2() <= 1248)
			{
				pTarget->Push_TargetLeft2(3);
			}
			else if (pTarget->Get_X2() > 1248)
			{
				pTarget->Push_TargetRight2(3);
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y2() + fY <= 1120)
			{
				pTarget->Push_TargetUp2(3);
			}
			else if (pTarget->Get_Y2() + fY > 1120)
			{
				pTarget->Push_TargetDown2(3);
			}
		}
	}


	rcObject = Get_Rect(2272, 1056, 192, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X2() <= 2272)
			{
				pTarget->Push_TargetLeft2(3);
			}
			else if (pTarget->Get_X2() > 2272)
			{
				pTarget->Push_TargetRight2(3);
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y2() + fY <= 1056)
			{
				pTarget->Push_TargetUp2(3);
			}
			else if (pTarget->Get_Y2() + fY > 1056)
			{
				pTarget->Push_TargetDown2(3);
			}
		}
	}
	rcObject = Get_Rect(2400, 992, 64, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X2() <= 2400)
			{
				pTarget->Push_TargetLeft2(3);
			}
			else if (pTarget->Get_X2() > 2400)
			{
				pTarget->Push_TargetRight2(3);
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y2() + fY <= 992)
			{
				pTarget->Push_TargetUp2(3);
			}
			else if (pTarget->Get_Y2() + fY > 992)
			{
				pTarget->Push_TargetDown2(3);
			}
		}
	}
	rcObject = Get_Rect(2464, 768, 64, 384);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X2() <= 2464)
			{
				pTarget->Push_TargetLeft2(3);
			}
			else if (pTarget->Get_X2() > 2464)
			{
				pTarget->Push_TargetRight2(3);
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y2() + fY <= 768)
			{
				pTarget->Push_TargetUp2(3);
			}
			else if (pTarget->Get_Y2() + fY > 768)
			{
				pTarget->Push_TargetDown2(3);
			}
		}
	}
	rcObject = Get_Rect(2368, 544, 128, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X2() <= 2368)
			{
				pTarget->Push_TargetLeft2(3);
			}
			else if (pTarget->Get_X2() > 2368)
			{
				pTarget->Push_TargetRight2(3);
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y2() + fY <= 544)
			{
				pTarget->Push_TargetUp2(3);
			}
			else if (pTarget->Get_Y2() + fY > 544)
			{
				pTarget->Push_TargetDown2(3);
			}
		}
	}
	rcObject = Get_Rect(2240, 480, 128, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X2() <= 2240)
			{
				pTarget->Push_TargetLeft2(3);
			}
			else if (pTarget->Get_X2() > 2240)
			{
				pTarget->Push_TargetRight2(3);
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y2() + fY <= 480)
			{
				pTarget->Push_TargetUp2(3);
			}
			else if (pTarget->Get_Y2() + fY > 480)
			{
				pTarget->Push_TargetDown2(3);
			}
		}
	}
	rcObject = Get_Rect(1312, 416, 1728, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X2() <= 1312)
			{
				pTarget->Push_TargetLeft2(3);
			}
			else if (pTarget->Get_X2() > 1312)
			{
				pTarget->Push_TargetRight2(3);
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y2() + fY <= 416)
			{
				pTarget->Push_TargetUp2(3);
			}
			else if (pTarget->Get_Y2() + fY > 416)
			{
				pTarget->Push_TargetDown2(3);
			}
		}
	}
	rcObject = Get_Rect(1056, 480, 832, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X2() <= 1056)
			{
				pTarget->Push_TargetLeft2(3);
			}
			else if (pTarget->Get_X2() > 1056)
			{
				pTarget->Push_TargetRight2(3);
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y2() + fY <= 480)
			{
				pTarget->Push_TargetUp2(3);
			}
			else if (pTarget->Get_Y2() + fY > 480)
			{
				pTarget->Push_TargetDown2(3);
			}
		}
	}
	rcObject = Get_Rect(992, 554, 704, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X2() <= 992)
			{
				pTarget->Push_TargetLeft2(3);
			}
			else if (pTarget->Get_X2() > 992)
			{
				pTarget->Push_TargetRight2(3);
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y2() + fY <= 554)
			{
				pTarget->Push_TargetUp2(3);
			}
			else if (pTarget->Get_Y2() + fY > 554)
			{
				pTarget->Push_TargetDown2(3);
			}
		}
	}
	rcObject = Get_Rect(992, 608, 448, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X2() <= 992)
			{
				pTarget->Push_TargetLeft2(3);
			}
			else if (pTarget->Get_X2() > 992)
			{
				pTarget->Push_TargetRight2(3);
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y2() + fY <= 608)
			{
				pTarget->Push_TargetUp2(3);
			}
			else if (pTarget->Get_Y2() + fY > 608)
			{
				pTarget->Push_TargetDown2(3);
			}
		}
	}
	rcObject = Get_Rect(320, 416, 128, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X2() <= 320)
			{
				pTarget->Push_TargetLeft2(3);
			}
			else if (pTarget->Get_X2() > 320)
			{
				pTarget->Push_TargetRight2(3);
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y2() + fY <= 416)
			{
				pTarget->Push_TargetUp2(3);
			}
			else if (pTarget->Get_Y2() + fY > 416)
			{
				pTarget->Push_TargetDown2(3);
			}
		}
	}
	rcObject = Get_Rect(192, 480, 128, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X2() <= 192)
			{
				pTarget->Push_TargetLeft2(3);
			}
			else if (pTarget->Get_X2() > 192)
			{
				pTarget->Push_TargetRight2(3);
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y2() + fY <= 480)
			{
				pTarget->Push_TargetUp2(3);
			}
			else if (pTarget->Get_Y2() + fY > 480)
			{
				pTarget->Push_TargetDown2(3);
			}
		}
	}
	rcObject = Get_Rect(96, 704, 64, 384);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X2() <= 96)
			{
				pTarget->Push_TargetLeft2(3);
			}
			else if (pTarget->Get_X2() > 96)
			{
				pTarget->Push_TargetRight2(3);
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y2() + fY <= 704)
			{
				pTarget->Push_TargetUp2(3);
			}
			else if (pTarget->Get_Y2() + fY > 704)
			{
				pTarget->Push_TargetDown2(3);
			}
		}
	}
	rcObject = Get_Rect(160, 928, 64, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X2() <= 160)
			{
				pTarget->Push_TargetLeft2(3);
			}
			else if (pTarget->Get_X2() > 160)
			{
				pTarget->Push_TargetRight2(3);
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y2() + fY <= 928)
			{
				pTarget->Push_TargetUp2(3);
			}
			else if (pTarget->Get_Y2() + fY > 928)
			{
				pTarget->Push_TargetDown2(3);
			}
		}
	}
	rcObject = Get_Rect(224, 992, 64, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X2() <= 224)
			{
				pTarget->Push_TargetLeft2(3);
			}
			else if (pTarget->Get_X2() > 224)
			{
				pTarget->Push_TargetRight2(3);
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y2() + fY <= 992)
			{
				pTarget->Push_TargetUp2(3);
			}
			else if (pTarget->Get_Y2() + fY > 992)
			{
				pTarget->Push_TargetDown2(3);
			}
		}
	}
	rcObject = Get_Rect(288, 1056, 64, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X2() <= 288)
			{
				pTarget->Push_TargetLeft2(3);
			}
			else if (pTarget->Get_X2() > 288)
			{
				pTarget->Push_TargetRight2(3);
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y2() + fY <= 1056)
			{
				pTarget->Push_TargetUp2(3);
			}
			else if (pTarget->Get_Y2() + fY > 1056)
			{
				pTarget->Push_TargetDown2(3);
			}
		}
	}
}

void CBack_Cave::Tile_CollisionPlayer(CGameObject * pTarget, _float fY, _float SizeX, _float SizeY)
{
	RECT rcTmp = Get_Rect(pTarget->Get_X(), pTarget->Get_Y() + fY, SizeX, SizeY);
	RECT rcCollision = { 0 };
	RECT rcObject = Get_Rect(1248, 1120, 1856, 64);

	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X() <= 1248)
			{
				pTarget->Push_TargetLeft();
			}
			else if (pTarget->Get_X() > 1248)
			{
				pTarget->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y() + fY <= 1120)
			{
				pTarget->Push_TargetUp();
			}
			else if (pTarget->Get_Y() + fY > 1120)
			{
				pTarget->Push_TargetDown();
			}
		}
	}


	rcObject = Get_Rect(2272, 1056, 192, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X() <= 2272)
			{
				pTarget->Push_TargetLeft();
			}
			else if (pTarget->Get_X() > 2272)
			{
				pTarget->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y() + fY <= 1056)
			{
				pTarget->Push_TargetUp();
			}
			else if (pTarget->Get_Y() + fY > 1056)
			{
				pTarget->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(2400, 992, 64, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X() <= 2400)
			{
				pTarget->Push_TargetLeft();
			}
			else if (pTarget->Get_X() > 2400)
			{
				pTarget->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y() + fY <= 992)
			{
				pTarget->Push_TargetUp();
			}
			else if (pTarget->Get_Y() + fY > 992)
			{
				pTarget->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(2464, 768, 64, 384);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X() <= 2464)
			{
				pTarget->Push_TargetLeft();
			}
			else if (pTarget->Get_X() > 2464)
			{
				pTarget->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y() + fY <= 768)
			{
				pTarget->Push_TargetUp();
			}
			else if (pTarget->Get_Y() + fY > 768)
			{
				pTarget->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(2368, 544, 128, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X() <= 2368)
			{
				pTarget->Push_TargetLeft();
			}
			else if (pTarget->Get_X() > 2368)
			{
				pTarget->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y() + fY <= 544)
			{
				pTarget->Push_TargetUp();
			}
			else if (pTarget->Get_Y() + fY > 544)
			{
				pTarget->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(2240, 480, 128, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X() <= 2240)
			{
				pTarget->Push_TargetLeft();
			}
			else if (pTarget->Get_X() > 2240)
			{
				pTarget->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y() + fY <= 480)
			{
				pTarget->Push_TargetUp();
			}
			else if (pTarget->Get_Y() + fY > 480)
			{
				pTarget->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(1312, 416, 1728, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X() <= 1312)
			{
				pTarget->Push_TargetLeft();
			}
			else if (pTarget->Get_X() > 1312)
			{
				pTarget->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y() + fY <= 416)
			{
				pTarget->Push_TargetUp();
			}
			else if (pTarget->Get_Y() + fY > 416)
			{
				pTarget->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(1056, 480, 832, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X() <= 1056)
			{
				pTarget->Push_TargetLeft();
			}
			else if (pTarget->Get_X() > 1056)
			{
				pTarget->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y() + fY <= 480)
			{
				pTarget->Push_TargetUp();
			}
			else if (pTarget->Get_Y() + fY > 480)
			{
				pTarget->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(992, 554, 704, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X() <= 992)
			{
				pTarget->Push_TargetLeft();
			}
			else if (pTarget->Get_X() > 992)
			{
				pTarget->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y() + fY <= 554)
			{
				pTarget->Push_TargetUp();
			}
			else if (pTarget->Get_Y() + fY > 554)
			{
				pTarget->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(992, 608, 448, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X() <= 992)
			{
				pTarget->Push_TargetLeft();
			}
			else if (pTarget->Get_X() > 992)
			{
				pTarget->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y() + fY <= 608)
			{
				pTarget->Push_TargetUp();
			}
			else if (pTarget->Get_Y() + fY > 608)
			{
				pTarget->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(320, 416, 128, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X() <= 320)
			{
				pTarget->Push_TargetLeft();
			}
			else if (pTarget->Get_X() > 320)
			{
				pTarget->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y() + fY <= 416)
			{
				pTarget->Push_TargetUp();
			}
			else if (pTarget->Get_Y() + fY > 416)
			{
				pTarget->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(192, 480, 128, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X() <= 192)
			{
				pTarget->Push_TargetLeft();
			}
			else if (pTarget->Get_X() > 192)
			{
				pTarget->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y() + fY <= 480)
			{
				pTarget->Push_TargetUp();
			}
			else if (pTarget->Get_Y() + fY > 480)
			{
				pTarget->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(96, 704, 64, 384);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X() <= 96)
			{
				pTarget->Push_TargetLeft();
			}
			else if (pTarget->Get_X() > 96)
			{
				pTarget->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y() + fY <= 704)
			{
				pTarget->Push_TargetUp();
			}
			else if (pTarget->Get_Y() + fY > 704)
			{
				pTarget->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(160, 928, 64, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X() <= 160)
			{
				pTarget->Push_TargetLeft();
			}
			else if (pTarget->Get_X() > 160)
			{
				pTarget->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y() + fY <= 928)
			{
				pTarget->Push_TargetUp();
			}
			else if (pTarget->Get_Y() + fY > 928)
			{
				pTarget->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(224, 992, 64, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X() <= 224)
			{
				pTarget->Push_TargetLeft();
			}
			else if (pTarget->Get_X() > 224)
			{
				pTarget->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y() + fY <= 992)
			{
				pTarget->Push_TargetUp();
			}
			else if (pTarget->Get_Y() + fY > 992)
			{
				pTarget->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(288, 1056, 64, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (pTarget->Get_X() <= 288)
			{
				pTarget->Push_TargetLeft();
			}
			else if (pTarget->Get_X() > 288)
			{
				pTarget->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (pTarget->Get_Y() + fY <= 1056)
			{
				pTarget->Push_TargetUp();
			}
			else if (pTarget->Get_Y() + fY > 1056)
			{
				pTarget->Push_TargetDown();
			}
		}
	}
}

CBack_Cave * CBack_Cave::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBack_Cave*		pInstance = new CBack_Cave(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CBack_Cave Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBack_Cave::Free()
{

	Safe_Release(m_pPlayer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRect);

	Safe_Release(m_pBuffer);

	CGameObject::Free();
}
