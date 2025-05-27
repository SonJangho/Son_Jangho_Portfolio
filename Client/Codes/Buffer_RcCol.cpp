#include "stdafx.h"
#include "..\Headers\Buffer_RcCol.h"


CBuffer_RcCol::CBuffer_RcCol(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)	
{

}

HRESULT CBuffer_RcCol::Ready_Buffer_RcCol()
{
	m_iVertexSize = sizeof(VTXCOL);
	m_iNumVertices = 4;
	m_iNumPolygons = 2;
	// D3DFVF_XYZRHW : 3����(�޼���ǥ��)���� 2����(��������ǥ��)���� ��� ��ȯ�� ���ƴٴ� �����Ͽ� 
	// 2������ǥ������ ��ġ�� ������ �� �ִ� Ư¡�� ������ �ִ�.
	m_dwVtxFVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;


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
	VTXCOL*		pVertices = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = D3DXVECTOR4(-0.5f, -0.5f, 0.f, 1.f);
	m_pPosition[0] = pVertices[0].vPosition;
	pVertices[0].dwColor = D3DXCOLOR(1.0f, 0.f, 0.f, 1.f);

	pVertices[1].vPosition = D3DXVECTOR4(0.5f, -0.5f, 0.f, 1.f);
	m_pPosition[1] = pVertices[1].vPosition;
	pVertices[1].dwColor = D3DXCOLOR(1.0f, 1.f, 0.f, 1.f);

	pVertices[2].vPosition = D3DXVECTOR4(0.5f, 0.5f, 0.f, 1.f);
	m_pPosition[2] = pVertices[2].vPosition;
	pVertices[2].dwColor = D3DXCOLOR(1.0f, 0.f, 0.f, 1.f);

	pVertices[3].vPosition = D3DXVECTOR4(-0.5f, 0.5f, 0.f, 1.f);
	m_pPosition[3] = pVertices[3].vPosition;
	pVertices[3].dwColor = D3DXCOLOR(1.0f, 0.f, 0.f, 1.f);	
	
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



CBuffer_RcCol * CBuffer_RcCol::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBuffer_RcCol*		pInstance = new CBuffer_RcCol(pGraphic_Device);

	if (FAILED(pInstance->Ready_Buffer_RcCol()))
	{
		_MSG_BOX("CBuffer_RcCol Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBuffer_RcCol::Free()
{
	CVIBuffer::Free();
}
