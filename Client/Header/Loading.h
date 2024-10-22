#pragma once

#include "Base.h"
#include "Engine_Define.h"

class CLoading : public CBase
{
public:
	enum LOADINGID { LOADING_STAGE, LOADING_ROAD, LOADING_BOSS, LOADING_END };

public:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading();

public:
	LOADINGID			Get_LoadingID() const { return m_eID; }
	CRITICAL_SECTION*	Get_Crt() { return &m_Crt; }
	_bool				Get_Finish() const { return m_bFinish; }

public:
	HRESULT		Ready_Loading(LOADINGID eID);
	HRESULT		Loading_Player_Texture();
	HRESULT		Loading_Beefalo_Texture();
	HRESULT		Loading_UI_Texture();
	HRESULT		Loading_Item_Texture();
	HRESULT		Loading_Spider_Texture();
	HRESULT		Loading_Pig_Texture();
	HRESULT		Loading_TallBird_Texture();
	HRESULT		Loading_Boss_Texture();
	HRESULT		Loading_Anim_Texture();
	HRESULT		Loading_Effect_UI();
	const _tchar*		Get_String() const { return m_szLoading; }
public:

	_uint		Loading_ForStage();
	_uint		Loading_ForRoad();
	_uint		Loading_ForBoss();


public:
	HRESULT SaveLoadingTexture(const _tchar* _key,TEXTUREID _textureTag,const _tchar* _path, int num = 1);
public:
	static unsigned int CALLBACK Thread_Main(void* pArg);

private:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;
	LOADINGID			m_eID;
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	bool				m_bFinish;
	_tchar				m_szLoading[128];


public:
	static CLoading*		Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);

private:
	virtual void		Free();

};

