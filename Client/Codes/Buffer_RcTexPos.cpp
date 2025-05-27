#include "stdafx.h"
#include "..\Headers\Buffer_RcTexPos.h"


CBuffer_RcTexPos::CBuffer_RcTexPos(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{

}

HRESULT CBuffer_RcTexPos::Ready_Buffer_RcTex()
{
	m_iVertexSize = sizeof(VTXTEXPOS);
	m_iNumVertices = 4;
	m_iNumPolygons = 2;
	// D3DFVF_XYZRHW : 3차원(왼손좌표계)에서 2차원(윈도우좌표계)으로 모든 변환을 거쳤다는 가정하에 
	// 2차원좌표공간에 위치를 설정할 수 있는 특징을 가지고 있다.
	m_dwVtxFVF = D3DFVF_XYZRHW | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1);


	m_iPolygonSize = sizeof(POLYGON16);
	m_iNumPolygons = 2;
	m_IndexFmt = D3DFMT_INDEX16;

	// 위에서 설정한 내용과 같이 정점 배열 버퍼를 생성한다.
	if (FAILED(Ready_VIBuffer())) // 정점배열을 생성했어.
		return E_FAIL;	

	// 그 공간이 아직 쓰레기야.
	// 공간에 값을 채우자.

	m_pPosition = new D3DXVECTOR4[4];

	// 공간을 잠궈서 다른 스레드의 접근을 차단한다.
	VTXTEXPOS*		pVertices = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = D3DXVECTOR4(-0.5f, -0.5f, 0.f, 1.f);
	m_pPosition[0] = pVertices[0].vPosition;
	pVertices[0].vTexUV = D3DXVECTOR2(0.0f, 0.f);
	pVertices[0].vViewPortPos = _vec3(pVertices[0].vPosition.x, pVertices[0].vPosition.y, pVertices[0].vPosition.z);


	pVertices[1].vPosition = D3DXVECTOR4(0.5f, -0.5f, 0.f, 1.f);
	m_pPosition[1] = pVertices[1].vPosition;
	pVertices[1].vTexUV = D3DXVECTOR2(1.0f, 0.f);
	pVertices[1].vViewPortPos = _vec3(pVertices[1].vPosition.x, pVertices[1].vPosition.y, pVertices[1].vPosition.z);

	pVertices[2].vPosition = D3DXVECTOR4(0.5f, 0.5f, 0.f, 1.f);
	m_pPosition[2] = pVertices[2].vPosition;
	pVertices[2].vTexUV = D3DXVECTOR2(1.0f, 1.f);
	pVertices[2].vViewPortPos = _vec3(pVertices[2].vPosition.x, pVertices[2].vPosition.y, pVertices[2].vPosition.z);

	pVertices[3].vPosition = D3DXVECTOR4(-0.5f, 0.5f, 0.f, 1.f);
	m_pPosition[3] = pVertices[3].vPosition;
	pVertices[3].vTexUV = D3DXVECTOR2(0.0f, 1.f);
	pVertices[3].vViewPortPos = _vec3(pVertices[3].vPosition.x, pVertices[3].vPosition.y, pVertices[3].vPosition.z);
	
	m_pVB->Unlock();

	POLYGON16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	m_pIB->Unlock();

	return NOERROR;
}

void CBuffer_RcTexPos::Render_Buffer(D3DXMATRIX matTransform)
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pVB)
		return;

	// 공간을 잠궈서 다른 스레드의 접근을 차단한다.
	VTXTEXPOS*		pVertices = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	// 벡터와 행렬의 곱을 수행해주는 함수.
	// Vec3의 w값을 1로 채워서 곱셈을 수행한다.
	for (size_t i = 0; i < 4; i++)
	{
		pVertices[i].vPosition = m_pPosition[i];

		D3DXVec4Transform(&pVertices[i].vPosition, &pVertices[i].vPosition, &matTransform);
		pVertices[i].vViewPortPos = _vec3(pVertices[i].vPosition.x, pVertices[i].vPosition.y, pVertices[i].vPosition.z);

	}


	// 벡터와 행렬의 곱을 수행해주는 함수.
	// Vec3의 w값을 0로 채워서 곱셈을 수행한다.
	//D3DXVec3TransformNormal();

	m_pVB->Unlock();

	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iVertexSize);

	m_pGraphic_Device->SetFVF(m_dwVtxFVF);

	m_pGraphic_Device->SetIndices(m_pIB);

	// 정점들을 이용해서 삼각형들을 그린다.
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, 0, m_iNumPolygons);

}

CBuffer_RcTexPos * CBuffer_RcTexPos::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBuffer_RcTexPos*		pInstance = new CBuffer_RcTexPos(pGraphic_Device);

	if (FAILED(pInstance->Ready_Buffer_RcTex()))
	{
		_MSG_BOX("CBuffer_RcTexPos Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBuffer_RcTexPos::Free()
{
	CVIBuffer::Free();
}
