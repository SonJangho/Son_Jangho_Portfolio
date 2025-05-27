#pragma once

// 기존 : 객체들을 모아서 관리한다. (CObject_manager)가 레이어별로 모아서 관리한다.
//.		모아진 순서대로 그린다. 레이어가 맵에 어떤 순서로 들어갔을지 알 수 없다.
// 현재 : 오브젝트 매니져에서는 객체들(레이어)의 업데이트와 라스트 업데이트만 호출한다.
//      렌더의 기능을 수행하기위한 렌더메니져를 추가한다.
// 렌더매니져 : 그려져야하느ㅏㄴ 객체들을 순서대로 보관한다. 수ㅏㄴ서대로 그린다.

#include "Base.h"

class CGameObject;
class CRender_Manager final : public CBase
{
	_DECLARE_SINGLETON(CRender_Manager)
public:
	enum RENDERGROUP {RENDER_PRIORITY, RENDER_TWO, RENDER_YSORT, RENDER_COVER, RENDER_UI, RENDER_INVENTORY, RENDER_MOUSE, RENDER_END};
private:
	explicit CRender_Manager();
	virtual ~CRender_Manager() = default;
public:
	// 일반적으로 객체의 라스트업데이트 안에서 호출한다.
	HRESULT Add_RenderList(RENDERGROUP eGroup, CGameObject* pGameObject);
	void Render_Object();
	void Clear_RenderList();
private: // 그려야하는 객체들을 순서에 맞게 모아놓은 컨테이너.
	list<CGameObject*>			m_RenderList[RENDER_END];
	typedef list<CGameObject*>	RENDERLIST;
private:
	void Render_Priority();
	void Render_Two();
	void Render_YSort();
	void Render_Cover();
	void Render_UI();
	void Render_INVENTORY();
	void Render_Mouse();
protected:
	virtual void Free(void);
};

