#pragma once

#include "Base.h"


// 객체들을 모아서 관리하낟. 
// 정적인 객체(static) and 각 씬에서만 사용할 객체(dynamic)들을 배열로 구분.

// Layer로 구분한 이유./
// "플레이어", 플레이어객체들(CLayer)
// "몬스터A", 몬스터A객체들
// "몬스터B", 몬스터B객체들
class CLayer;
class CGameObject;
class CObject_Manager final : public CBase
{
	_DECLARE_SINGLETON(CObject_Manager)
public:
	enum OBJECTTYPE { TYPE_STATIC, TYPE_DYNAMIC, TYPE_END }; 
private:
	explicit CObject_Manager();
	virtual ~CObject_Manager() = default;
public:
	CGameObject* Get_Object(OBJECTTYPE eType, const _tchar* pLayerTag, const _uint& iCnt = 0);
public:
	HRESULT Add_Object(OBJECTTYPE eType, const _tchar* pLayerTag, CGameObject* pGameObject);
	_int Update_Layer(const _float& fTimeDelta);
	_int LastUpdate_Layer(const _float& fTimeDelta);
	void Render_Layer();
	_ulong Clear_Dynamic_Object();
	void Clear_Layer(OBJECTTYPE eType, const _tchar* pLayerTag);

private:
	map<const _tchar*, CLayer*>			m_mapLayer[TYPE_END];
	typedef map<const _tchar*, CLayer*>	MAPLAYER;
private:
	CLayer* Find_Layer(OBJECTTYPE eType, const _tchar* pLayerTag);	
protected:
	virtual void Free(void);
};

