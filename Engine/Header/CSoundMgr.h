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
		//�������� ����� ��ü�鸸 ���, ��� ������Ʈ �������
		void Update_Sound(_vec3 _SoundVel,_vec3 _PlayerPos, _vec3 _ObjPos,_vec3 _ObjFoward, _vec3 _ObjUp, CHANNELID eID, float _SoundFower=1);
		void Update_System() { FMOD_System_Update(m_pSystem); }

	private:
		void LoadSoundFile();

	private:
		// ���� ���ҽ� ������ ���� ��ü 
		map<TCHAR*, FMOD_SOUND*> m_mapSound;

		// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
		FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	
		// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
		FMOD_SYSTEM* m_pSystem;

	private:
		virtual void Free();
	};


}
