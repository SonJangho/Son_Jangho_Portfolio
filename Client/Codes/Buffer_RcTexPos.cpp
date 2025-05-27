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
	// D3DFVF_XYZRHW : 3����(�޼���ǥ��)���� 2����(��������ǥ��)���� ��� ��ȯ�� ���ƴٴ� �����Ͽ� 
	// 2������ǥ������ ��ġ�� ������ �� �ִ� Ư¡�� ������ �ִ�.
	m_dwVtxFVF = D3DFVF_XYZRHW | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1);


	m_iPolygonSize = sizeof(POLYGON16);
	m_iNumPolygons = 2;
	m_IndexFmt = D3DFMT_INDEX16;

	// ������ ������ ����� ���� ���� �迭 ���۸� �����Ѵ�.
	if (FAILED(Ready_VIBuffer())) // �����迭�� �����߾�.
		return E_FAIL;	

	// �� ������ ���� �������.
	// ������ ���� ä����.

	m_pPosition = new D3DXVECTOR4[4];

	// ������ ��ż� �ٸ� �������� ������ �����Ѵ�.
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

	// ������ ��ż� �ٸ� �������� ������ �����Ѵ�.
	VTXTEXPOS*		pVertices = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	// ���Ϳ� ����� ���� �������ִ� �Լ�.
	// Vec3�� w���� 1�� ä���� ������ �����Ѵ�.
	for (size_t i = 0; i < 4; i++)
	{
		pVertices[i].vPosition = m_pPosition[i];

		D3DXVec4Transform(&pVertices[i].vPosition, &pVertices[i].vPosition, &matTransform);
		pVertices[i].vViewPortPos = _vec3(pVertices[i].vPosition.x, pVertices[i].vPosition.y, pVertices[i].vPosition.z);

	}


	// ���Ϳ� ����� ���� �������ִ� �Լ�.
	// Vec3�� w���� 0�� ä���� ������ �����Ѵ�.
	//D3DXVec3TransformNormal();

	m_pVB->Unlock();

	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iVertexSize);

	m_pGraphic_Device->SetFVF(m_dwVtxFVF);

	m_pGraphic_Device->SetIndices(m_pIB);

	// �������� �̿��ؼ� �ﰢ������ �׸���.
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
