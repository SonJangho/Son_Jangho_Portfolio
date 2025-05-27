#pragma once

#include "GameObject.h"

// �ΰ��� ����̴�.
class CSceneChange final : public CGameObject
{
private:
	explicit CSceneChange(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CSceneChange() = default;
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	void Set_Start() {
		m_bStart = true;
		m_bOn = true;
		m_fNum = 0.f;
		m_bHi = false;
	}
	_bool Get_Start() {
		return m_bOn;
	}
private:
	CTexture*				m_pTexture= nullptr;
	CBuffer_RcTex*			m_pBuffer = nullptr;
	_float					m_fFrame = 0.f;
	CShader*				m_pShader = nullptr;
	D3DXMATRIX				TempMatrix;
	_float					m_fNum = 0.f;
	_float					m_fAcc = 0.f;
	_bool					m_bOn = false;
	_bool					m_bStart = false;
	_bool					m_bHi = false;
private:
	HRESULT SetUp_Resource(); // ���ҽ� �Ŵ������� ã�Ƽ� ����� �����س��´�.
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	_bool	isDraw(CGameObject* pCamera);
	_matrix Render_TransForm(CGameObject* pCamera);
public:
	static CSceneChange* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

