#pragma once

#include "GameObject.h"

// 로고의 배경이다.
class CFarming final : public CGameObject
{
	enum FARMINGSTATE {BASICDIRT,WATERDIRT,PARSNIP,STRAWBERRY,POTATO,DEADCROP,END};
	enum DIRTSTATE {BASIC,WATER};
private:
	explicit CFarming(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CFarming() = default;
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	typedef struct tagState
	{
		FARMINGSTATE		eState;
		_uint				iNumTexture;
	}STATEDESC;
	typedef struct DirtInfo
	{
		_int			DirtX;
		_int			DirtY;
		DIRTSTATE	DirtState;
	}DIRTINFO;
	typedef struct CropInfo
	{
		DIRTINFO*		DirtInfo;
		FARMINGSTATE	Crop;
		_int			GrowthStage;
	}CROPINFO;
public:
	_bool	Get_DirtCheck() {
		return m_bCheck;
	}
	_bool	Get_HarvestCheck() {
		return m_bCheck2;
	}
	_bool	Get_HarvestOn() {
		return m_isClicked2;
	}
	_int	Get_Crop() {
		return m_iCrop;
	}
private:
	CTexture*				m_pTexture[END] = { nullptr };
	CTexture*				m_pWatering = nullptr;
	STATEDESC*				m_pStateDesc = nullptr;
	FARMINGSTATE			m_eCurrentState = FARMINGSTATE(0);
	CBuffer_RcTex*			m_pBuffer = nullptr;
	_float					m_fFrame = 0.f;
	CShader*				m_pShader = nullptr;
	D3DXMATRIX				TempMatrix;
	CGameObject*			m_pPlayer = nullptr;
	CGameObject*			m_pHoe = nullptr;
	CGameObject*			m_pWateringPot = nullptr;
	CGameObject*			m_pUI = nullptr;
	CGameObject*			m_pInventory = nullptr;
	CGameObject*			m_pCamera = nullptr;
	vector<DIRTINFO*>		m_vecDirt;
	vector<CROPINFO*>		m_vecCrop;
	vector<DIRTINFO*>		m_vecWater;
	ITEMINFO*				pItemInfo;
	_int					m_iDirtX = 0;
	_int					m_iDirtY = 0;
	_bool					m_bExist = false;
	_bool					m_bCheck = false;
	_bool					m_bCheck2 = false;
	_int					m_PlayerX = 0;
	_int					m_PlayerY = 0;
	_float					m_fTime = 0.f;
	_int					m_iDeleteIndex = 0;
	_bool					m_bErase = false;
	_bool					m_isClicked = false;
	_bool					m_isClicked2 = false;
	_int					m_iCrop = 0;
	_int					m_iDirt = 0;
	_bool					m_bCenter = false;
	_bool					m_bWateringStart = false;
	_int					m_iWaterX = 0, m_iWaterY = 0;
	_float					m_fWaterFrame = 0.f;
	_bool					m_bExist2 = false;
	_bool					m_bFirst = false;
	_bool					m_bFirst2 = false;
private:
	HRESULT SetUp_Resource();
	HRESULT SetUp_StateDesc();
	_bool	isDraw(CGameObject* pCamera);
	_matrix Render_TransForm(CGameObject* pCamera);
	HRESULT SetUp_RenderState();
	HRESULT Reset_RenderState();
	void Hoe_Dig();
	void Seed_Planting();
	void Watering_Crops();
	void Harvest_Crops();
	void Calculate_Dirt(DIRTINFO* pDirt);
	void Calculate_WaterDirt(DIRTINFO* pDirt);
public:
	static CFarming* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

