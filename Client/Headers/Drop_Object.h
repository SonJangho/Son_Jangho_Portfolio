#pragma once

#include "GameObject.h"

// 로고의 배경이다.
class CDrop_Object final : public CGameObject
{
	enum STATE {BRANCH, STONE, WEED,COPPER,SLIME, END};
private:
	explicit CDrop_Object(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CDrop_Object() = default;
public:
	virtual HRESULT Ready_GameObject(_vec3 vPosition, _vec3 vSize, _float idx);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
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
	_float					m_fTime = 0.f;
	_float					m_bGravity = 0.f;
	_bool					m_bOff = false;
	_bool					m_bIdle = false;
	_float					m_fJump = 0.f;
	_bool					m_bFirst = false;
	_bool					m_bSecond = false;
	_float					m_fJumpX = 0.f;
	_bool					m_bObjectOff = false;
private:
	HRESULT SetUp_Resource(); // 리소스 매니져에서 찾아서 멤버에 대입해놓는다.
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	_bool	isDraw(CGameObject* pCamera);
	_matrix Render_TransForm(CGameObject* pCamera);
	_float Linear(float v0, float v1, float t) {
		return v0 * (1.0f - t) + v1 * t;
	}
public:
	static CDrop_Object* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _vec3 vPosition, _vec3 vSize, _float idx);
protected:
	virtual void Free();
};

