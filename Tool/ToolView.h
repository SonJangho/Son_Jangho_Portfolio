
// ToolView.h : CToolView 클래스의 인터페이스
//

#pragma once

#include "ToolDoc.h"

#include "Graphic_Device.h"

class CTile;
class CFarmObject;
class CPage_Object;
class CToolView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// 특성입니다.
public:
	CToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	HRESULT Ready_Tiles(const _uint& iCntX, const _uint& iCntY);
	HRESULT Ready_Object();
	void Set_WindowSize();
	HRESULT Save_TileInfo();
	HRESULT Load_TileInfo();
	HRESULT Save_ObjectInfo();
	HRESULT Load_ObjectInfo();
protected:
	CPage_Object*				m_Page = nullptr;
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	vector<CTile*>				m_vecTiles;
	vector<CFarmObject*>		m_vecObject;
	_uint						m_iNumTileX = 0;
	_uint						m_iNumTileY = 0;
	_uint						m_iNumTiles = 0;

	_uint						m_iNumX = 0;
	_uint						m_iNumY = 0;

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

