#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	typedef struct tagVertexColor
	{
		_vec3		vPosition;	
		_ulong		dwColor;

	}VTXCOL;

	const _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTexture
	{
		_vec3		vPosition;
		_vec3		vNormal;
		_vec2		vTexUV;

	}VTXTEX;

	const _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexCubeTexture
	{
		_vec3		vPosition;
		_vec3		vTexUV;

	}VTXCUBE;

	const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0); // 텍스처의 UV 좌표 값을 FLOAT형 3개로 표현하겠다는 매크로(괄호안의 숫자 0의 의미는 본래 버텍스에 텍스쳐 UV값이 여러개가 올 수 있는데 그중 0번째 값을 지정하겠다는 의미)
	
	
	typedef struct tagVertexParticle
	{
		_vec3		vPosition;
		D3DCOLOR	_color;
		_float		_size;
		static	const DWORD FVF;

	}VTXPARTICLE;

	const _ulong	FVF_PARTICLE = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	typedef struct tagIndex16
	{
		_ushort  _0;
		_ushort  _1;
		_ushort  _2;

	}INDEX16;

	typedef struct tagIndex32
	{
		_ulong	_0;
		_ulong	_1;
		_ulong	_2;

	}INDEX32;	

	typedef struct tagObjStat		//오브젝트들이 공통으로 가지고 있을 스탯 정보(컴포넌트에 하나씩 보유)
	{
		_tchar* strObjName;
		_float fHP;
		_float fMxHP;
		_float fSpeed;
		_float fATK;
		_float fAggroRange;
		_float fATKRange;
		_float fHungry;
		_float fMaxHungry;
		_float fMental;
		_float fMaxMental;
		_bool bDead;
	}OBJSTAT;

	struct tEvent
	{
		eEVENT_TYPE		eEven;
		DWORD_PTR		lParam;
		DWORD_PTR		wParam;
		DWORD_PTR		pParam;
	};

	typedef struct tagItemInfo
	{
		_tchar* strItemName;
		_int iCount;
	}ITEMINFO;

	typedef struct tagCreateInfo
	{
		_tchar* strKeyName;
		_tchar* strName;
		_tchar* strInfo;
		_int iInfoCount; // 필요한 아이템 수
		ITEMINFO tItemInfo[3]; // 아이템 key값 넣기
	} CREATEINFO;

	struct tFrameInfo
	{
		IDirect3DBaseTexture9* Texture;
		_float					Duration;
	};

	struct Attribute
	{
		_vec3 m_vPos;
		_vec3	m_vVelocity;
		_vec3	m_vAcceleration;
		_float	m_fLifeTime;
		_float	m_fAge;
		D3DXCOLOR	m_cColor;
		D3DXCOLOR	m_cColorFade;
		_bool	m_bAlive;
	};

}
#endif // Engine_Struct_h__
