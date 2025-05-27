#pragma once

#include "GameObject.h"

class CGreenSlime final : public CGameObject
{
	enum STATE { LIVE, DEAD, MARK, END };
private:
	explicit CGreenSlime(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CGreenSlime() = default;
public:
	virtual HRESULT Ready_GameObject(_float fX, _float fY);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	void Set_Collision(_bool bColl) {
		m_bCollision = bColl;
	}
	void Set_DropObject() {
		m_bCreateObject = false;
	}
	_bool	Create_DropObject() {
		return m_bCreateObject;
	}
	_float	Get_Frame() {
		return m_fObjectNumb;
	}
	_float	Get_fX() {
		return m_matTransform._41;
	}
	_float	Get_fY() {
		return m_matTransform._42;
	}
private:
	CTexture*				m_pTexture[END] = { nullptr };
	STATE					m_State;
	CBuffer_RcTex*			m_pBuffer = nullptr;
	_float					m_fFrame = 0.f;
	CShader*				m_pShader = nullptr;
	D3DXMATRIX				TempMatrix;
	CGameObject*			m_pPlayer = nullptr;
	CGameObject*			m_pSword = nullptr;
	_float					m_fNum = 0.f;
	_float					m_fMarkFrame = 0.f;
	_float					m_fMarkNum = 0.f;
	_float					m_fTimeDegree = 0.f;
	_bool					m_bMarkOn = false;
	_bool					m_bMoveOn = false;
	_bool					m_bAttackOn = false;
	_int					m_iCount = 0;
	_float					m_fAcc = 0.f;
	_vec3					vSaveDir;
	_bool					m_bOnce = false;
	_bool					m_bOnceee = false;
	_bool					m_bIdleOn = false;
	_float					m_fIdleAcc = 0.f;
	_int					RanNum = 0;
	_float					m_fRanAcc = 0.f;
	_bool					m_bFirst = false;
	_float					m_iX = 0.f, m_iY = 0.f;
	_bool					m_bDeadOn = false;
	_bool					m_bCollision = false;
	_int					m_iHp = 0.f;
	_bool					m_bObjectOff = false;
	_int					m_iPassNum = 0;
	_float					m_fHitAcc = 0.f;
	_bool					m_bHit = false;
	_float					m_fObjectNumb = 0.f;
	_bool					m_bErase = false;
	_bool					m_bCreateObject = false;
	_bool					m_bThrust = false;
	_float					m_fThrustAcc = 0.f;
	_bool					m_bCollisionOff = false;
private:
	_float Linear(float v0, float v1, float t) {
		return v0 * (1.0f - t) + v1 * t;
	}
private:
	HRESULT SetUp_Resource();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	_bool	isDraw(CGameObject* pCamera);
	_matrix Render_TransForm(CGameObject* pCamera);
	void	Player_Collision();
	void	Monster_Collision(CGameObject* pTarget);
	void	Move_ToTargetMonster(_vec3 vTargetPos, const _float& fTimeDelta);
public:
	static CGreenSlime* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fX, _float fY);
protected:
	virtual void Free();
};