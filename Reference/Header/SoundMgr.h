#pragma once

#include "Engine_Define.h"
#include "Base.h"
class CSoundMgr : public CBase
{
 DECLARE_SINGLETON(CSoundMgr)

private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Ready_Sound();
	void PlaySound(_tchar* pSoundKey, CHANNELID eID, float fVolume);
	void PlayBGM(TCHAR* pSoundKey, float fVolume);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);

private:
	void LoadSoundFile(); 

private:
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound; 
	
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL]; 

	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem; 

private:
	virtual void Free();
};

