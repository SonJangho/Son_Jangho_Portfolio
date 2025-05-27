#pragma once

// ���� : ��ü���� ��Ƽ� �����Ѵ�. (CObject_manager)�� ���̾�� ��Ƽ� �����Ѵ�.
//.		����� ������� �׸���. ���̾ �ʿ� � ������ ������ �� �� ����.
// ���� : ������Ʈ �Ŵ��������� ��ü��(���̾�)�� ������Ʈ�� ��Ʈ ������Ʈ�� ȣ���Ѵ�.
//      ������ ����� �����ϱ����� �����޴����� �߰��Ѵ�.
// �����Ŵ��� : �׷������ϴ����� ��ü���� ������� �����Ѵ�. ����������� �׸���.

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
	// �Ϲ������� ��ü�� ��Ʈ������Ʈ �ȿ��� ȣ���Ѵ�.
	HRESULT Add_RenderList(RENDERGROUP eGroup, CGameObject* pGameObject);
	void Render_Object();
	void Clear_RenderList();
private: // �׷����ϴ� ��ü���� ������ �°� ��Ƴ��� �����̳�.
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

