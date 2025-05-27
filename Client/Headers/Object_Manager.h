#pragma once

#include "Base.h"


// ��ü���� ��Ƽ� �����ϳ�. 
// ������ ��ü(static) and �� �������� ����� ��ü(dynamic)���� �迭�� ����.

// Layer�� ������ ����./
// "�÷��̾�", �÷��̾ü��(CLayer)
// "����A", ����A��ü��
// "����B", ����B��ü��
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

