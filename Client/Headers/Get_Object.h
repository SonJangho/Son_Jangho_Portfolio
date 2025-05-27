#pragma once

#include "GameObject.h"

// 로고의 배경이다.
class CGet_Object final : public CGameObject
{
	enum STATE { BRANCH, STONE, WEED, BASS, SALMON, SUNFISH,COPPER,SLIME, END };
private:
	explicit CGet_Object(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CGet_Object() = default;
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	void Set_Object(_int iFrame) {
		m_State = (STATE)iFrame;
		m_bObjectOn = true;
		m_fAcc = 0.f;
		m_fNum = 1.f;
	}
private:
	CTexture*				m_pTexture[END] = { nullptr };
	STATE					m_State;
	CBuffer_RcTex*			m_pBuffer = nullptr;
	_float					m_fFrame = 0.f;
	CShader*				m_pShader = nullptr;
	D3DXMATRIX				TempMatrix;
	CGameObject*			m_pPlayer = nullptr;
	CTexture*				m_pRect = nullptr;
	_vec3					m_vPosition;
	_vec3					m_vSize;
	_bool					m_bObjectOn = false;
	_float					m_fNum = 0.f;
	_float					m_fAcc = 0.f;
private:
	HRESULT SetUp_Resource(); // 리소스 매니져에서 찾아서 멤버에 대입해놓는다.
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	_bool	isDraw(CGameObject* pCamera);
	_matrix Render_TransForm(CGameObject* pCamera);
	_float Linear(float v0, float v1, float t) {
		return v0 * (1.0f - t) + v1 * t;
	}
public:
	static CGet_Object* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

