
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

#include "Defines.h"

class CToolView;
class CForm;
class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:
	RECT Get_ClientRect_FormView();
	_int Get_TileIndex();
	_int Get_ObjectIndex();
public:
	HRESULT Save_TileInfo();
	HRESULT Load_TileInfo();
	HRESULT Save_ObjectInfo();
	HRESULT Load_ObjectInfo();
// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual ~CMainFrame();



#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	HRESULT Ready_Tiles(const _uint& iNumCntX, const _uint& iNumCntY);
	HRESULT Ready_Object();
protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;
private: // 창분할을 관리해주는 객체를 생성한다.
	CSplitterWnd	  m_MainSplitter;

	CToolView*			m_pMainView = nullptr;
	CForm*				m_pForm = nullptr;



// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnDestroy();
};


