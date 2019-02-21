// OsDependent 
// BouKiCHi 2019

#ifndef _OS_DEP_H_
#define _OS_DEP_H_

#include <vector>
#include <string>
#include "callback.h"
#include "plugin/mucom88if.h"

#define TICK_SHIFT 10
#define TICK_FACTOR 1024.0

//	Debug�p
void Alertf(const char *format, ...);

class OsDependent  {
public:
	OsDependent();
	virtual ~OsDependent();

	// COM�̏�����(�ŏ��̂P��̂�)
	virtual bool CoInitialize()=0;

	// �T�E���h
	AudioCallback *UserAudioCallback;
	virtual bool InitAudio(void *hwnd, int Rate, int BufferSize) = 0;
	virtual void FreeAudio() = 0;
	virtual bool SendAudio(int ms) = 0;
	virtual void WaitSendingAudio() = 0;
	bool MuteAudio;
	
	// ���`�b�v
	virtual bool InitRealChip() = 0;
	virtual void FreeRealChip() = 0;
	virtual void ResetRealChip() = 0;
	virtual int CheckRealChip() = 0;
	virtual void OutputRealChip(unsigned int Register, unsigned int Data) = 0;
	virtual void OutputRealChipAdpcm(void *pData, int size) = 0;

	// �^�C�}�[
	TimerCallback *UserTimerCallback;
	virtual bool InitTimer() = 0;
	virtual void FreeTimer() = 0;
	virtual void UpdateTimer() = 0;

	virtual void ResetTime() = 0;
	virtual int GetElapsedTime() = 0;

	// ����
	virtual int GetMilliseconds() = 0;
	virtual void Delay(int ms)=0;

	// �v���O�C���g��
	virtual int InitPlugin(Mucom88Plugin *plg, const char *filename, int bootopt) = 0;
	virtual void FreePlugin(Mucom88Plugin *plg) = 0;
	virtual int ExecPluginVMCommand(Mucom88Plugin *plg, int, int, int, void *, void *) = 0;
	virtual int ExecPluginEditorCommand(Mucom88Plugin *plg, int, int, int, void *, void *) = 0;

	// �t�@�C������֘A
	virtual int GetDirectory(char *buf, int size) = 0;
	virtual int ChangeDirectory(const char *dir) = 0;
	virtual int KillFile(const char *filename) = 0;


protected:

};

#endif
