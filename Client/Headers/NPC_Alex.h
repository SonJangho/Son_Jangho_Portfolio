#pragma once

#include "GameObject.h"

class CNPC_Alex final : public CGameObject
{
private:
	explicit CNPC_Alex(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CNPC_Alex() = default;
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	CTexture*				m_pTexture= nullptr;
	CTexture*				m_pShadow = nullptr;
	CBuffer_RcTex*			m_pBuffer = nullptr;
	_float					m_fFrame = 0.f;
	CShader*				m_pShader = nullptr;
	D3DXMATRIX				TempMatrix;
	CGameObject*			m_pPlayer = nullptr;
	CGameObject*			m_pUI = nullptr;
	_float					m_fNum = 0.f;
	_float					m_fTimeDegree = 0.f;
private:
	HRESULT SetUp_Resource(); // ���ҽ� �Ŵ������� ã�Ƽ� ����� �����س��´�.
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	_bool	isDraw(CGameObject* pCamera);
	_matrix Render_TransForm(CGameObject* pCamera);
public:
	static CNPC_Alex* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

