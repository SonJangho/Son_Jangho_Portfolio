#include "stdafx.h"
#include "..\Headers\Back_Road.h"
#include "Camera.h"

CBack_Road::CBack_Road(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

HRESULT CBack_Road::Ready_GameObject()
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

_int CBack_Road::Update_GameObject(const _float & fTimeDelta)
{
	m_fFrame += fTimeDelta * 5;
	if (m_fFrame >= 17)
		m_fFrame = 1;
	
	RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);
	if (rcTmp.right > 64.f * 20.f)
		m_pPlayer->Push_TargetLeft();
	
	RECT rcCollision = { 0 };
	RECT rcObject = Get_Rect(160.f, 544.f, 320.f, 320.f);
	
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (m_pPlayer->Get_X() <= 160.f)
			{
				m_pPlayer->Push_TargetLeft();
			}
			else if (m_pPlayer->Get_X() > 160.f)
			{
				m_pPlayer->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (m_pPlayer->Get_Y() + 57.f <= 544.f)
			{
				m_pPlayer->Push_TargetUp();
			}
			else if (m_pPlayer->Get_Y() + 57.f > 544.f)
			{
				m_pPlayer->Push_TargetDown();
			}
		}
	}

	rcObject = Get_Rect(512.f, 192.f, 1024.f, 384.f);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (m_pPlayer->Get_X() <= 512.f)
			{
				m_pPlayer->Push_TargetLeft();
			}
			else if (m_pPlayer->Get_X() > 512.f)
			{
				m_pPlayer->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (m_pPlayer->Get_Y() + 57.f <= 192.f)
			{
				m_pPlayer->Push_TargetUp();
			}
			else if (m_pPlayer->Get_Y() + 57.f > 192.f)
			{
				m_pPlayer->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(1184, 192, 192, 384);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (m_pPlayer->Get_X() <= 1184)
			{
				m_pPlayer->Push_TargetLeft();
			}
			else if (m_pPlayer->Get_X() > 1184)
			{
				m_pPlayer->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (m_pPlayer->Get_Y() + 57.f <= 192)
			{
				m_pPlayer->Push_TargetUp();
			}
			else if (m_pPlayer->Get_Y() + 57.f > 192)
			{
				m_pPlayer->Push_TargetDown();
			}
		}
	}

	rcObject = Get_Rect(128, 736, 256, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (m_pPlayer->Get_X() <= 128)
			{
				m_pPlayer->Push_TargetLeft();
			}
			else if (m_pPlayer->Get_X() > 128)
			{
				m_pPlayer->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (m_pPlayer->Get_Y() + 57.f <= 736)
			{
				m_pPlayer->Push_TargetUp();
			}
			else if (m_pPlayer->Get_Y() + 57.f > 736)
			{
				m_pPlayer->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(96, 1344, 192, 1152);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (m_pPlayer->Get_X() <= 96)
			{
				m_pPlayer->Push_TargetLeft();
			}
			else if (m_pPlayer->Get_X() > 96)
			{
				m_pPlayer->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (m_pPlayer->Get_Y() + 57.f <= 1344)
			{
				m_pPlayer->Push_TargetUp();
			}
			else if (m_pPlayer->Get_Y() + 57.f > 1344)
			{
				m_pPlayer->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(992, 608, 576, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (m_pPlayer->Get_X() <= 992)
			{
				m_pPlayer->Push_TargetLeft();
			}
			else if (m_pPlayer->Get_X() > 992)
			{
				m_pPlayer->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (m_pPlayer->Get_Y() + 57.f <= 608)
			{
				m_pPlayer->Push_TargetUp();
			}
			else if (m_pPlayer->Get_Y() + 57.f > 608)
			{
				m_pPlayer->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(672, 672, 64, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (m_pPlayer->Get_X() <= 672)
			{
				m_pPlayer->Push_TargetLeft();
			}
			else if (m_pPlayer->Get_X() > 672)
			{
				m_pPlayer->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (m_pPlayer->Get_Y() + 57.f <= 672)
			{
				m_pPlayer->Push_TargetUp();
			}
			else if (m_pPlayer->Get_Y() + 57.f > 672)
			{
				m_pPlayer->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(608, 736, 64, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (m_pPlayer->Get_X() <= 608)
			{
				m_pPlayer->Push_TargetLeft();
			}
			else if (m_pPlayer->Get_X() > 608)
			{
				m_pPlayer->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (m_pPlayer->Get_Y() + 57.f <= 736)
			{
				m_pPlayer->Push_TargetUp();
			}
			else if (m_pPlayer->Get_Y() + 57.f > 736)
			{
				m_pPlayer->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(544, 800, 64, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (m_pPlayer->Get_X() <= 544)
			{
				m_pPlayer->Push_TargetLeft();
			}
			else if (m_pPlayer->Get_X() > 544)
			{
				m_pPlayer->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (m_pPlayer->Get_Y() + 57.f <= 800)
			{
				m_pPlayer->Push_TargetUp();
			}
			else if (m_pPlayer->Get_Y() + 57.f > 800)
			{
				m_pPlayer->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(480, 960, 64, 256);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (m_pPlayer->Get_X() <= 480)
			{
				m_pPlayer->Push_TargetLeft();
			}
			else if (m_pPlayer->Get_X() > 480)
			{
				m_pPlayer->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (m_pPlayer->Get_Y() + 57.f <= 960)
			{
				m_pPlayer->Push_TargetUp();
			}
			else if (m_pPlayer->Get_Y() + 57.f > 960)
			{
				m_pPlayer->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(544, 1152, 64, 128);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (m_pPlayer->Get_X() <= 544)
			{
				m_pPlayer->Push_TargetLeft();
			}
			else if (m_pPlayer->Get_X() > 544)
			{
				m_pPlayer->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (m_pPlayer->Get_Y() + 57.f <= 1152)
			{
				m_pPlayer->Push_TargetUp();
			}
			else if (m_pPlayer->Get_Y() + 57.f > 1152)
			{
				m_pPlayer->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(608, 1248, 64, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (m_pPlayer->Get_X() <= 608)
			{
				m_pPlayer->Push_TargetLeft();
			}
			else if (m_pPlayer->Get_X() > 608)
			{
				m_pPlayer->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (m_pPlayer->Get_Y() + 57.f <= 1248)
			{
				m_pPlayer->Push_TargetUp();
			}
			else if (m_pPlayer->Get_Y() + 57.f > 1248)
			{
				m_pPlayer->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(672, 1312, 64, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (m_pPlayer->Get_X() <= 672)
			{
				m_pPlayer->Push_TargetLeft();
			}
			else if (m_pPlayer->Get_X() > 672)
			{
				m_pPlayer->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (m_pPlayer->Get_Y() + 57.f <= 1312)
			{
				m_pPlayer->Push_TargetUp();
			}
			else if (m_pPlayer->Get_Y() + 57.f > 1312)
			{
				m_pPlayer->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(736, 1376, 64, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (m_pPlayer->Get_X() <= 736)
			{
				m_pPlayer->Push_TargetLeft();
			}
			else if (m_pPlayer->Get_X() > 736)
			{
				m_pPlayer->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (m_pPlayer->Get_Y() + 57.f <= 1376)
			{
				m_pPlayer->Push_TargetUp();
			}
			else if (m_pPlayer->Get_Y() + 57.f > 1376)
			{
				m_pPlayer->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(800, 1440, 64, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (m_pPlayer->Get_X() <= 800)
			{
				m_pPlayer->Push_TargetLeft();
			}
			else if (m_pPlayer->Get_X() > 800)
			{
				m_pPlayer->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (m_pPlayer->Get_Y() + 57.f <= 1440)
			{
				m_pPlayer->Push_TargetUp();
			}
			else if (m_pPlayer->Get_Y() + 57.f > 1440)
			{
				m_pPlayer->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(864, 1632, 64, 320);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (m_pPlayer->Get_X() <= 864)
			{
				m_pPlayer->Push_TargetLeft();
			}
			else if (m_pPlayer->Get_X() > 864)
			{
				m_pPlayer->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (m_pPlayer->Get_Y() + 57.f <= 1632)
			{
				m_pPlayer->Push_TargetUp();
			}
			else if (m_pPlayer->Get_Y() + 57.f > 1632)
			{
				m_pPlayer->Push_TargetDown();
			}
		}
	}
	rcObject = Get_Rect(1088, 1760, 384, 64);
	if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
	{
		_long lWidth = rcCollision.right - rcCollision.left;
		_long lHeight = rcCollision.bottom - rcCollision.top;

		if (lWidth <= lHeight)
		{
			if (m_pPlayer->Get_X() <= 1088)
			{
				m_pPlayer->Push_TargetLeft();
			}
			else if (m_pPlayer->Get_X() > 1088)
			{
				m_pPlayer->Push_TargetRight();
			}
		}
		else if (lWidth > lHeight)
		{
			if (m_pPlayer->Get_Y() + 57.f <= 1760)
			{
				m_pPlayer->Push_TargetUp();
			}
			else if (m_pPlayer->Get_Y() + 57.f > 1760)
			{
				m_pPlayer->Push_TargetDown();
			}
		}
	}

	return _int();
}

_int CBack_Road::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRender_Manager)
		return -1;

	if (FAILED(m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_PRIORITY, this)))
		return -1;

	return _int();
}

void CBack_Road::Render_GameObject()
{
	if (FAILED(SetUp_RenderState()))
		return;

	CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");
	
	Set_Scale(_vec3(320.f * 4, 480.f * 4, 1.f));
	Set_Position(_vec3(640.f, 960.f, 0.f));
	TempMatrix = Render_TransForm(pCamera);
	m_pTexture->SetUp_OnGraphicDev(m_fFrame);
	m_pBuffer->Render_Buffer(TempMatrix);

	TempMatrix = Render_TransForm(pCamera);
	m_pTexture->SetUp_OnGraphicDev(0);
	m_pBuffer->Render_Buffer(TempMatrix);

	if (FAILED(Reset_RenderState()))
		return;
}

HRESULT CBack_Road::SetUp_TileInfo()
{
	return NOERROR;
}

HRESULT CBack_Road::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;
	
	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture
	m_pTexture = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_RoadTiles");
	if (nullptr == m_pTexture)
		return E_FAIL;
	m_pTexture->AddRef();

	m_pRect = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Rect");
	if (nullptr == m_pRect)
		return E_FAIL;
	m_pRect->AddRef();

	return NOERROR;
}

HRESULT CBack_Road::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return NOERROR;
}

HRESULT CBack_Road::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return NOERROR;
}

_matrix CBack_Road::Render_TransForm(CGameObject* pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

_bool CBack_Road::isDraw(CGameObject * pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

CBack_Road * CBack_Road::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBack_Road*		pInstance = new CBack_Road(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CBack_Road Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBack_Road::Free()
{

	Safe_Release(m_pPlayer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRect);

	Safe_Release(m_pBuffer);

	CGameObject::Free();
}
