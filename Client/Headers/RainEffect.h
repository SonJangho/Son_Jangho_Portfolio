#pragma once

#include "GameObject.h"

// 로고의 배경이다.
class CRainEffect final : public CGameObject
{
private:
	explicit CRainEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRainEffect() = default;
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	CTexture*				m_pRain = nullptr;
	CTexture*				m_pRainUI = nullptr;
	CBuffer_RcTex*			m_pBuffer = nullptr;
	_float					m_fFrame = 0.f;
	CShader*				m_pShader = nullptr;
	D3DXMATRIX				TempMatrix;
	CGameObject*			m_pUI = nullptr;
	_float					m_fNum = 0.f;
	_float					m_fRainX = 0.f, m_fRainY = 0.f;
	_int					m_RandomFrame = 0;
	_float					m_fAcc = 0.f;
private:
	HRESULT SetUp_Resource(); // 리소스 매니져에서 찾아서 멤버에 대입해놓는다.
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	_bool	isDraw(CGameObject* pCamera);
	_matrix Render_TransForm(CGameObject* pCamera);
public:
	static CRainEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

