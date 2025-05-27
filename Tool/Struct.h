#pragma once

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

typedef struct tagVertex_Texture
{
	D3DXVECTOR4			vPosition;
	D3DXVECTOR2			vTexUV;
}VTXTEX;

typedef struct tagPolygon16
{
	unsigned short		_1, _2, _3;
}POLYGON16;
