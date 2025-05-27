#pragma once

#include "GameObject.h"

// �ΰ��� ����̴�.
class CWarning final : public CGameObject
{
private:
	explicit CWarning(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CWarning() = default;
public:
	virtual HRESULT Ready_GameObject();
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
	_float					m_fNum = 0.f;

private:
	HRESULT SetUp_Resource(); // ���ҽ� �Ŵ������� ã�Ƽ� ����� �����س��´�.
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	_bool	isDraw(CGameObject* pCamera);
	_matrix Render_TransForm(CGameObject* pCamera);
public:
	static CWarning* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

