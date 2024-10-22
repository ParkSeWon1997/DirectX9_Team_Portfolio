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

	const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0); // �ؽ�ó�� UV ��ǥ ���� FLOAT�� 3���� ǥ���ϰڴٴ� ��ũ��(��ȣ���� ���� 0�� �ǹ̴� ���� ���ؽ��� �ؽ��� UV���� �������� �� �� �ִµ� ���� 0��° ���� �����ϰڴٴ� �ǹ�)
	
	
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

	typedef struct tagObjStat		//������Ʈ���� �������� ������ ���� ���� ����(������Ʈ�� �ϳ��� ����)
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
		_int iInfoCount; // �ʿ��� ������ ��
		ITEMINFO tItemInfo[3]; // ������ key�� �ֱ�
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
