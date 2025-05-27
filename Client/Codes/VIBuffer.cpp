#include "stdafx.h"
#include "..\Headers\VIBuffer.h"


CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CResource(pGraphic_Device)
{
}

HRESULT CVIBuffer::Ready_VIBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	
	// 정점 버퍼를 생성한다.(정점(Vertex구조체)가 배열형태로 할당된다.)
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexSize * m_iNumVertices, 0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, 0)))
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iPolygonSize * m_iNumPolygons, 0, m_IndexFmt, D3DPOOL_MANAGED, &m_pIB, 0)))
		return E_FAIL;

	return NOERROR;
}

void CVIBuffer::Render_Buffer(D3DXMATRIX matTransform)
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pVB)
		return;

	// 공간을 잠궈서 다른 스레드의 접근을 차단한다.
	void*		pVertices = nullptr;
	m_pVB->Lock(0, 0, &pVertices, 0);

	// 벡터와 행렬의 곱을 수행해주는 함수.
	// Vec3의 w값을 1로 채워서 곱셈을 수행한다.
	for (size_t i = 0; i < 4; i++)
	{
		*(_vec4*)((_byte*)pVertices + i * m_iVertexSize) = m_pPosition[i];

		D3DXVec4Transform((_vec4*)((_byte*)pVertices + i * m_iVertexSize), (_vec4*)((_byte*)pVertices + i * m_iVertexSize), &matTransform);
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

void CVIBuffer::Free()
{
	Safe_Delete_Array(m_pPosition);

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	CResource::Free();

}
