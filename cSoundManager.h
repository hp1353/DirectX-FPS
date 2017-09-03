#pragma once
#include "./inc\fmod.hpp"		
#pragma comment(lib, "./lib/fmodex_vc.lib")	//lib ��ũ
using namespace FMOD;		//FMOD ���ӽ����̽� ���

							//ä�ι���, �������
#define EXTRACHANNERBUFFER 5
#define SOUNDBUFFER 20
							//�ѻ�� ���ۼ���: ���۰� �۾Ƽ� ������ ������ �ذ����ֱ� ����
#define TOTALSOUNDBUFFER SOUNDBUFFER + EXTRACHANNERBUFFER
#define g_pSoundManager cSoundManager::GetInstance()
class cSoundManager
{

private:
	typedef std::map<std::string, Sound**> arrSounds;
	typedef std::map<std::string, Sound**>::iterator arrSoundIter;
private:
	System* _system;		//fmod�� �ý��� Ŭ����
	Sound** _sound;			//fmod�� ���� Ŭ����
	Channel** _channel;		//fmod�� ä�� Ŭ����

	arrSounds _mTotalSounds;//�ʿ� ��� ���� ���

	SYNTHESIZE(float, m_fSoundVol, SoundVol);

public:
	SINGLETONE(cSoundManager);
	HRESULT Setup(void);
	void Destroy(void);
	void Update(void);

	//���� �߰�(Ű��, �����̸�, BGM? ������?)
	void addSound(std::string keyName, std::string soundName, bool bgm = FALSE, bool loop = FALSE);

	//���� �÷���(Ű��, �ҷ�) 0.0f ~ 1.0f
	void play(std::string keyName, float volume = 1.0f);
	//���� ����
	void stop(std::string keyName);
	//���� �Ͻ�����
	void pause(std::string keyName);
	//���� �ٽ� ���
	void resume(std::string ketName);

	//�÷��� ���̳�?
	bool isPlaySound(std::string keyName);
	//�Ͻ����� ���̳�?
	bool isPauseSound(std::string KeyName);

};

