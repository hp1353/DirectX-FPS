#include "stdafx.h"
#include "cSoundManager.h"

cSoundManager::cSoundManager() : _system(NULL), _channel(NULL), _sound(NULL)
{
}

cSoundManager::~cSoundManager()
{
}

HRESULT cSoundManager::Setup(void)
{
	//사운드 시스템 생성
	System_Create(&_system);

	//maxchannels : fomd 에서 사용하는 최대 채널의 수를 뜻함
	//사운드 채널수 생성
	_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//사운드 , 채널 동적할당
	_sound = new Sound *[TOTALSOUNDBUFFER];
	_channel = new Channel *[TOTALSOUNDBUFFER];

	//메모리 초기화
	memset(_sound, 0, sizeof(Sound*) * (TOTALSOUNDBUFFER));
	memset(_channel, 0, sizeof(Channel*) * (TOTALSOUNDBUFFER));

	return S_OK;
}
void cSoundManager::Destroy(void)
{
	//사운드 및 채널 삭제
	if (_channel != NULL || _sound != NULL)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; i++)
		{
			if (_channel != NULL)
			{
				if (_channel[i]) _channel[i]->stop();
			}

			if (_sound != NULL)
			{
				if (_sound[i]) _sound[i]->release();
			}
		}
	}

	//메모리 지우기
	SAFE_DELETE(_channel);
	SAFE_DELETE(_sound);

	//시스템 닫기
	if (_system != NULL)
	{
		_system->release();
		_system->close();
	}
}

void cSoundManager::Update(void)
{
	/*
	사운드 시스템을 계속 업데이트 해줘야 볼륨이 바뀌거나
	재성이 끝난 사운드를 채널에서 빼내는 등 다양한 작업을 자동으로 해준다
	*/
	//메인게임에서 사운드매니져를 업데이트 해준다
	_system->update();
}

//사운드 추가(키값, 파일이름, BGM?, 루프냐?)
void cSoundManager::addSound(std::string keyName, std::string soundName, bool bgm, bool loop)
{
	if (loop)//브금 or 이펙트
	{
		if (bgm)
		{
			//1: 파일 URL의 이름 
			//2: 사운드를 열기 위한 옵션
			//3: 사운드를 재생하는 동안 유저에게 정보를 제공하는 FMOD_CREATESOUNDEXINFO의 포인터 , 이옵션을 무시하려면 0 또는 NULL을 지정하면 됨
			//4: FMOD::SOUND 오브젝트를 받는 변수의 주소
			_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
		else
		{
			_system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
	}

	else  //이펙트
	{
		_system->createSound(soundName.c_str(), FMOD_DEFAULT, 0, &_sound[_mTotalSounds.size()]);
	}

	//맵에 사운드를 키값과 함께 담아준다
	_mTotalSounds.insert(make_pair(keyName, &_sound[_mTotalSounds.size()]));
}

//사운드 플레이(키값, 볼륨) 0.0f~ 1.0f
void cSoundManager::play(std::string keyName, float volume)
{
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 플레이
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);
			//볼륨셋팅
			_channel[count]->setVolume(volume);
		}
	}
}
//사운드 정지
void cSoundManager::stop(std::string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 정지
			_channel[count]->stop();
			break;
		}
	}
}
//사운드 일시정지
void cSoundManager::pause(std::string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 정지
			_channel[count]->setPaused(true);
			break;
		}
	}
}
//사운드 다시 재생
void cSoundManager::resume(std::string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 일시정지(다시재생)
			_channel[count]->setPaused(false);
			break;
		}
	}
}

//플레이 중이냐?
bool cSoundManager::isPlaySound(std::string keyName)
{
	bool isPlay;
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//플레이 중이냐?
			_channel[count]->isPlaying(&isPlay);
			break;
		}
	}
	return isPlay;
}
//일시정지 중이냐?
bool cSoundManager::isPauseSound(std::string keyName)
{
	bool isPause;
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//플레이 중이냐? (일시정지)
			_channel[count]->isPlaying(&isPause);
			break;
		}
	}
	return isPause;
}
