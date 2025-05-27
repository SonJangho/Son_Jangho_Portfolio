#pragma once

#include "GameObject.h"

// 로고의 배경이다.
class CObject_Town final : public CGameObject
{
private:
	explicit CObject_Town(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CObject_Town() = default;
public:
	virtual HRESULT Ready_GameObject(_vec3 vPosition, _vec3 vSize, _uint idx);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	CTexture*				m_pTexture= nullptr;
	CBuffer_RcTex*			m_pBuffer = nullptr;
	_float					m_fFrame = 0.f;
	CShader*				m_pShader = nullptr;
	D3DXMATRIX				TempMatrix;
	CGameObject*			m_pPlayer = nullptr;
	CGameObject*			m_pCamera = nullptr;
	CTexture*				m_pRect = nullptr;
	_int					m_iX = 0;
	_int					m_iY = 0;
	_bool					m_bObjectOff = false;
	_float					m_fNumb = 0.f;
	_float					m_fNum = 1.f;
	_float					m_AccNum = 0.f;
	_int					m_iCount = 0;
	_bool					m_bFirst = false;
	_vec3					m_vPosition;
	_vec3					m_vSize;

private:
	HRESULT SetUp_Resource(); // 리소스 매니져에서 찾아서 멤버에 대입해놓는다.
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	_bool	isDraw(CGameObject* pCamera);
	_matrix Render_TransForm(CGameObject* pCamera);
public:
	static CObject_Town* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _vec3 vPosition, _vec3 vSize, _uint idx);
protected:
	virtual void Free();
};

