
#include "Engine_Define.h"
#include "CSoundMgr.h"

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
{
	m_pSystem = nullptr;
}


CSoundMgr::~CSoundMgr()
{
	Free();
}

void Engine::CSoundMgr::Ready_Sound()
{
	// ���带 ����ϴ� ��ǥ��ü�� �����ϴ� �Լ�
	FMOD_System_Create(&m_pSystem, FMOD_VERSION);
	// 1. �ý��� ������, 2. ����� ����ä�� �� , �ʱ�ȭ ���) 
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);


	LoadSoundFile();

}

void CSoundMgr::PlaySound_W(TCHAR* pSoundKey, CHANNELID eID, float fVolume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;

	/*if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	{*/
	FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[eID]);
	/*}*/

	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void Engine::CSoundMgr::PlaySound_NonStop(TCHAR* pSoundKey, CHANNELID eID, float fVolume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;

	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[eID]);
	}

	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void Engine::CSoundMgr::PlayTorch(TCHAR* pSoundKey, float fVolume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[SOUND_TORCH]);
	FMOD_Channel_SetMode(m_pChannelArr[SOUND_TORCH], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[SOUND_TORCH], fVolume);
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlayBGM(TCHAR* pSoundKey, float fVolume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, iter->second, nullptr,  FALSE, &m_pChannelArr[SOUND_BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[SOUND_BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[SOUND_BGM], fVolume);
	FMOD_System_Update(m_pSystem);
}

void Engine::CSoundMgr::PlayEffectContinue(TCHAR* pSoundKey, float fVolume, CHANNELID eID)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[eID]);
	FMOD_Channel_SetMode(m_pChannelArr[eID], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
	FMOD_System_Update(m_pSystem);

}

void CSoundMgr::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSoundMgr::StopAll()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundMgr::SetChannelVolume(CHANNELID eID, float fVolume)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void Engine::CSoundMgr::SpatialPlay_Sound(TCHAR* pSoundKey, CHANNELID eID)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;


	FMOD_BOOL bPlay = FALSE;
	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[eID]);
	}
	FMOD_Channel_SetMode(m_pChannelArr[eID], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[eID], 1.0f);
	FMOD_System_Update(m_pSystem);

}

void Engine::CSoundMgr::Update_Sound(_vec3 _SoundVel,_vec3 _PlayerPos, _vec3 _ObjPos, _vec3 _ObjFoward, _vec3 _ObjUp, CHANNELID eID, float _SoundFower)
{



	//����� ����ȯ �ʿ���,  FMOD_VECTOR�� c�� �̷���� ���� �׷��� c++������ ����� ����ȯ �ʿ�

	//���� ����� ��ġ
	FMOD_VECTOR vPos = { _PlayerPos.x,_PlayerPos.y,_PlayerPos.z };

	//�ʴ� �־��� �Ҹ��� �ӵ�
	FMOD_VECTOR vVel = { _SoundVel.x,_SoundVel.y,_SoundVel.z };

	//�Ҹ��� �� ����� ��ġ
	FMOD_VECTOR vObjPos = { _ObjPos.x,_ObjPos.y,_ObjPos.z };
	//�Ҹ��� �� ����� ���� ����
	FMOD_VECTOR vFoward = { _ObjFoward.x,_ObjFoward.y,_ObjFoward.z };
	//�Ҹ��� �� ����� �� ����
	FMOD_VECTOR vUp = { _ObjUp.x,_ObjUp.y,_ObjUp.z };



	//https://www.fmod.com/docs/2.00/api/core-api-channelcontrol.html#channelcontrol_set3dattributes
	FMOD_Channel_Set3DAttributes(m_pChannelArr[eID], &vObjPos, &vVel);

	//https://www.fmod.com/docs/2.00/api/core-api-system.html#system_set3dlistenerattributes
	FMOD_System_Set3DListenerAttributes(m_pSystem, 0, &vPos, &vVel, &vFoward, &vUp);


	float Distance = D3DXVec3Length(&(_PlayerPos - _ObjPos));
	float minDistance = 1.0f; // ������ �ּ� �Ÿ�
	float maxDistance = 10.0f; // ������ �ִ� �Ÿ�
	float volume = (Distance - minDistance) / (maxDistance - minDistance);


	FMOD_Channel_Set3DMinMaxDistance(m_pChannelArr[eID], minDistance, maxDistance);
	volume = 1.0f - volume; // ������ ����
	volume = max(0.0f, min(1.0f, volume)); // ������ 0.0�� 1.0 ���̷� Ŭ����
	FMOD_Channel_SetVolume(m_pChannelArr[eID], volume* _SoundFower);


	
	////0~1������ ������ ��ȯ, 1�� �Ѿ�� Ŭ������ �� ���� �ִٰ���
	//float volume = 1.0f - (Distance - minDistance) / (maxDistance - minDistance);
//	FMOD_Channel_SetVolume(m_pChannelArr[eID], volume);

	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::LoadSoundFile()
{
	_wfinddata64_t fd;
	__int64 handle = _wfindfirst64(L"../../Client/Bin/Resource/Sound/*.*", &fd);
	if (handle == -1 || handle == 0)
		return;

	int iResult = 0;

	char szCurPath[128] = "../../Client/Bin/Resource/Sound/";
	char szFullPath[128] = "";
	char szFilename[MAX_PATH];
	while (iResult != -1)
	{
		WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, szFilename);
		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_3D|0x00000020 , 0, &pSound);
		if (eRes == FMOD_OK)
		{
			int iLength = strlen(szFilename) + 1;

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
			MultiByteToWideChar(CP_ACP, 0, szFilename, iLength, pSoundKey, iLength);

			m_mapSound.emplace(pSoundKey, pSound);
		}
		iResult = _wfindnext64(handle, &fd);
	}
	FMOD_System_Update(m_pSystem);
	_findclose(handle);
}

void Engine::CSoundMgr::Free()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}
