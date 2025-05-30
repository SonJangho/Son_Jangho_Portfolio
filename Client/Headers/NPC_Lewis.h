#pragma once

#include "GameObject.h"

class CNPC_Lewis final : public CGameObject
{
private:
	explicit CNPC_Lewis(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CNPC_Lewis() = default;
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
	HRESULT SetUp_Resource(); // 리소스 매니져에서 찾아서 멤버에 대입해놓는다.
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	_bool	isDraw(CGameObject* pCamera);
	_matrix Render_TransForm(CGameObject* pCamera);
public:
	static CNPC_Lewis* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

