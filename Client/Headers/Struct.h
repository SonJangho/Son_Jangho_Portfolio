#pragma once

typedef struct tagPosition
{
	float		fX;
	float		fY;
}POSITION;

typedef struct tagTileInfo
{
	D3DXVECTOR3			vPosition;
	D3DXVECTOR3			vSize;
	unsigned int		iIndex;
}TILEINFO;

typedef struct tagObjectInfo
{
	D3DXVECTOR3			vPosition;
	D3DXVECTOR3			vSize;
	unsigned int		iIndex;
}OBJECTINFO;

typedef struct ItemInfo
{
	_tchar			szName[128];
	_int			iCount;
	_int			iNum;
	_bool			isHandle;
}ITEMINFO;

// 정점(Vertex)
typedef struct tagVertex_Color
{	
	D3DXVECTOR4		vPosition;
	DWORD			dwColor;		
}VTXCOL;

// 정점(Vertex)
typedef struct tagVertex_Texture
{
	D3DXVECTOR4		vPosition;
	D3DXVECTOR2		vTexUV;	
}VTXTEX;


typedef struct tagVertex_Texture_ViewPortPos
{
	D3DXVECTOR4		vPosition;
	D3DXVECTOR2		vTexUV;
	D3DXVECTOR3		vViewPortPos;
}VTXTEXPOS;

// 인덱스(폴리곤 하나를 그리기위한 인덱스 세개)
typedef struct tagPolygon16
{
	unsigned short		_1;
	unsigned short		_2;
	unsigned short		_3;
}POLYGON16;

typedef struct tagPolygon32
{
	unsigned long		_1;
	unsigned long		_2;
	unsigned long		_3;
}POLYGON32;




