#pragma once

#include "Base.h"

// 객체들을 여러개 가진다.

class CGameObject;
class CLayer final : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer() = default;
public:
	CGameObject* Get_Object(const _uint& iCnt) const;
public:
	HRESULT Add_GameObject(CGameObject* pGameObject) {
		m_ObjectList.push_back(pGameObject); 
		return NOERROR; }
public:
	HRESULT Ready_Layer();
	_int Update_Layer(const _float& fTimeDelta);
	_int LastUpdate_Layer(const _float& fTimeDelta);
	void Render_Layer();
private:
	list<CGameObject*>			m_ObjectList;
	typedef list<CGameObject*>	OBJECTLIST;
public:
	static CLayer* Create();
protected:
	virtual void Free();
};

