#pragma once

#include "Engine_Define.h"
#include "Base.h"

class CSound;

namespace Engine
{
	class ENGINE_DLL CSoundMgr :
		public CBase
	{
		DECLARE_SINGLETON(CSoundMgr)

	private:
		explicit CSoundMgr();
		virtual ~CSoundMgr();

	public:
		void Ready_Sound();
		void PlaySound_W(TCHAR* pSoundKey, CHANNELID eID, float fVolume);
		void PlaySound_NonStop(TCHAR* pSoundKey, CHANNELID eID, float fVolume);
		void PlayTorch(TCHAR* pSoundKey, float fVolume);
		void PlayBGM(TCHAR* pSoundKey, float fVolume);
		void PlayEffectContinue(TCHAR* pSoundKey, float fVolume, CHANNELID eID);
		void StopSound(CHANNELID eID);
		void StopAll();
		void SetChannelVolume(CHANNELID eID, float fVolume);


	//PSW
	// 
		void SpatialPlay_Sound(TCHAR* pSoundKey, CHANNELID eID);
		//공간음향 사용할 객체들만 사용, 계속 업데이트 해줘야함
		void Update_Sound(_vec3 _SoundVel,_vec3 _PlayerPos, _vec3 _ObjPos,_vec3 _ObjFoward, _vec3 _ObjUp, CHANNELID eID, float _SoundFower=1);
		void Update_System() { FMOD_System_Update(m_pSystem); }

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


}
