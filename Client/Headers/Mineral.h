#pragma once

#include "GameObject.h"

class CMineral final : public CGameObject
{
private:
	explicit CMineral(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CMineral() = default;
public:
	virtual HRESULT Ready_GameObject(_float fX, _float fY, _float fFrame);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
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
	CTexture*				m_pTexture = nullptr;
	STATE					m_State;
	CBuffer_RcTex*			m_pBuffer = nullptr;
	_float					m_fFrame = 0.f;
	CShader*				m_pShader = nullptr;
	D3DXMATRIX				TempMatrix;
	CGameObject*			m_pPlayer = nullptr;
	CGameObject*			m_pPick = nullptr;
	_float					m_fNum = 0.f;
	_float					m_X = 0.f, m_Y = 0.f;
	_bool					m_bFirst = false;
	_int					m_iX = 0, m_iY = 0;
	_float					m_fObjectNumb = 0.f;
	_bool					m_bErase = false;
	_bool					m_bCreateObject = false;
	_bool					m_bObjectOff = false;
private:
	HRESULT SetUp_Resource();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	_bool	isDraw(CGameObject* pCamera);
	_matrix Render_TransForm(CGameObject* pCamera);
	void	Player_Collision();
	void	Monster_Collision(CGameObject* pTarget);
public:
	static CMineral* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fX, _float fY, _float fFrame);
protected:
	virtual void Free();
};