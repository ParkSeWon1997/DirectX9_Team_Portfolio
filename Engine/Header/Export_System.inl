#include "Export_System.h"
// GraphicDev

HRESULT			Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _ulong& dwSizeX, const _ulong& dwSizeY, CGraphicDev** ppGraphicDev)
{
	return CGraphicDev::GetInstance()->Ready_GraphicDev(hWnd, eMode, dwSizeX, dwSizeY, ppGraphicDev);
}
void			Render_Begin(D3DXCOLOR Color)
{
	CGraphicDev::GetInstance()->Render_Begin(Color);
}
void			Render_End(void)
{
	CGraphicDev::GetInstance()->Render_End();
}

_float		Get_TimeDelta(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Get_TimeDelta(pTimerTag);
}
void		Update_TimeDelta(const _tchar* pTimerTag)
{
	CTimerMgr::GetInstance()->Update_TimeDelta(pTimerTag);
}
HRESULT		Ready_Timer(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Ready_Timer(pTimerTag);
}

// FrameMgr

_bool			IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	return CFrameMgr::GetInstance()->IsPermit_Call(pFrameTag, fTimeDelta);
}
HRESULT			Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	return CFrameMgr::GetInstance()->Ready_Frame(pFrameTag, fCallLimit);
}

// FontMgr
HRESULT		Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar* pFontType,
	const _tchar* pFontTag,
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight)
{
	return CFontMgr::GetInstance()->Ready_Font(pGraphicDev, pFontType, pFontTag, iWidth, iHeight, iWeight);
}

void		Render_Font(const _tchar* pFontTag,
	const _tchar* pString,
	const _vec2* pPos,
	D3DXCOLOR Color)
{
	CFontMgr::GetInstance()->Render_Font(pFontTag, pString, pPos, Color);
}

//InputDev

_byte	Get_DIKeyState(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->Get_DIKeyState(byKeyID);
}
_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->Get_DIMouseState(eMouse);
}
_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return CInputDev::GetInstance()->Get_DIMouseMove(eMouseState);
}
HRESULT	Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	return CInputDev::GetInstance()->Ready_InputDev(hInst, hWnd);
}
void		Update_InputDev(void)
{
	CInputDev::GetInstance()->Update_InputDev();
}
eKEY_STATE GetKeyState(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->GetKeyState(byKeyID);
}

eKEY_STATE GetMouseState(MOUSEKEYSTATE eMouseState)
{
	return CInputDev::GetInstance()-> GetMouseState(eMouseState);
}

void Update_MouseState(void)
{
	return CInputDev::GetInstance()->Update_MouseState();
}

// PathMgr

inline HRESULT Ready_PathMgr()
{
	return CPathMgr::GetInstance()->Ready_Path();
}

inline wstring GetRelativePath(const wchar_t* filePath) 
{
	return CPathMgr::GetInstance()->GetRelativePath(filePath);
}

inline const _tchar* GetResourcePath()
{
	return CPathMgr::GetInstance()->GetResourcePath();
}

//Collsion
_bool Collision_Item(_vec3 pPlayerPos, _vec3 pItemPos, _vec3 vPlayerScale, _vec3 vItemScale)
{
	return CCollisionMgr::GetInstance()->Collision_Item(pPlayerPos, pItemPos, vPlayerScale, vItemScale);
}

_bool Collision_Monster(
	_vec3 vPlayerPos,
	_vec3* vPlayerAxis,
	_vec3 vMonsterPos,
	_vec3* vMonsterAxis,
	_vec3 vPlayerScale,
	_vec3 vMonsterScale)
{
	return CCollisionMgr::GetInstance()->Collision_Monster(vPlayerPos, vPlayerAxis, vMonsterPos, vMonsterAxis, vPlayerScale, vMonsterScale);
}

_bool Collision_Mouse(_vec2 vMousePos, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	return CCollisionMgr::GetInstance()->Collision_Mouse(vMousePos, fX, fY, fSizeX, fSizeY);
}

_bool Collision_Mouse_Object(_vec3 vRayPos, _vec3 vRayDir, _vec3 vObjPos, _vec3 vObjScale)
{
	return CCollisionMgr::GetInstance()->Collision_Mouse_Object(vRayPos, vRayDir, vObjPos, vObjScale);
}



//Resource
inline void Save_TexturePath(wstring _fileName, wstring _key, TEXTUREID _textureTag, wstring _subPath, int num)
{
	CResMgr::GetInstance()->Save_TexturePath(_fileName,_key, _textureTag, _subPath, num);
}

//
////Sound
void Ready_Sound()
{
	return CSoundMgr::GetInstance()->Ready_Sound();
}

void PlaySound_W(_tchar* pSoundKey, CHANNELID eID, float fVolume)
{
	return CSoundMgr::GetInstance()->PlaySound_W(pSoundKey, eID, fVolume);
}

void PlaySound_NonStop(TCHAR* pSoundKey, CHANNELID eID, float fVolume)
{
	return CSoundMgr::GetInstance()->PlaySound_NonStop(pSoundKey, eID, fVolume);
}

void PlayTorch(TCHAR* pSoundKey, float fVolume)
{
	return CSoundMgr::GetInstance()->PlayTorch(pSoundKey, fVolume);
}

void PlayBGM(TCHAR* pSoundKey, float fVolume)
{
	return CSoundMgr::GetInstance()->PlayBGM(pSoundKey, fVolume);
}

void PlayEffectContinue(TCHAR* pSoundKey, float fVolume, CHANNELID eID)
{
	return CSoundMgr::GetInstance()->PlayEffectContinue(pSoundKey, fVolume, eID);

}

void SpatialPlay_Sound(TCHAR* pSoundKey, CHANNELID eID)
{
	return CSoundMgr::GetInstance()->SpatialPlay_Sound(pSoundKey, eID);
}

void Update_Sound(_vec3 _SoundVel, _vec3 _PlayerPos, _vec3 _ObjPos, _vec3 _ObjFoward, _vec3 _ObjUp, CHANNELID eID, float _SoundFower)
{
	return CSoundMgr::GetInstance()->Update_Sound(_SoundVel, _PlayerPos, _ObjPos, _ObjFoward, _ObjUp, eID, _SoundFower);
}

void Update_System()
{
	return CSoundMgr::GetInstance()->Update_System();
}


void StopSound(CHANNELID eID)
{
	return CSoundMgr::GetInstance()->StopSound(eID);
}

void StopAll()
{
	return CSoundMgr::GetInstance()->StopAll();
}

void SetChannelVolume(CHANNELID eID, float fVolume)
{
	return CSoundMgr::GetInstance()->SetChannelVolume(eID, fVolume);
}


// Destroy
void			Release_System()
{
	CInputDev::DestroyInstance();
	CFontMgr::DestroyInstance();
	CFrameMgr::DestroyInstance();
	CTimerMgr::DestroyInstance();
	CGraphicDev::DestroyInstance();
	CPathMgr::DestroyInstance();
	CResMgr::DestroyInstance();
	CCollisionMgr::DestroyInstance();
	CSoundMgr::DestroyInstance();
}