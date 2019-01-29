
//
//	cmucomvm.cpp structures
//
#ifndef __mucomvm_h
#define __mucomvm_h

#include <stdio.h>
#include "Z80/Z80.h"
#include "fmgen/opna.h"

#include "mucom88config.h"
#include "osdep.h"
#include "membuf.h"

//#define DEBUGZ80_TRACE

enum {
	VMFLAG_NONE, VMFLAG_EXEC, VMFLAG_HALT
};

#define VM_OPTION_FMMUTE 1
#define VM_OPTION_SCCI 2
#define VM_OPTION_FASTFW 4
#define VM_OPTION_STEP 8

#define VMBANK_MAX 4
#define VMBANK_SIZE 0x4000

#define VMPRGBANK_MAIN 0
#define VMPRGBANK_SHADOW 1

#define OPNACH_MAX 16
#define OPNACH_FM 0
#define OPNACH_PSG 6
#define OPNACH_RHYTHM 9
#define OPNACH_ADPCM 10
#define OPNAREG_MAX 0x200

class mucomvm : public Z80 {
public:
	mucomvm();
	~mucomvm();

	//		Z80�R���g���[��
	void InitSoundSystem(int Rate);
	void SetOption(int option);
	int GetOption(void) { return m_option; }
	void SetPC(uint16_t adr);
	void CallAndHalt(uint16_t adr);
	int CallAndHalt2(uint16_t adr, uint8_t code);
	int CallAndHaltWithA(uint16_t adr, uint8_t areg);
	int ExecUntilHalt(int times = 0x10000);
	void SendMemoryToShadow(void);

	//		���z�}�V���R���g���[��
	void Reset(void);
	void ResetFM(void);
	int LoadPcm(const char *fname, int maxpcm = 32);
	int LoadPcmFromMem(const char *buf, int sz, int maxpcm = 32);
	int LoadMem(const char *fname, int adr, int size);
	int SendMem(const unsigned char *src, int adr, int size);
	int SaveMem(const char *fname,int adr, int size);
	int SaveMemExpand(const char *fname, int adr, int size, char *header, int hedsize, char *footer, int footsize, char *pcm, int pcmsize);
	int KillFile(const char *fname);
	char *LoadAlloc(const char *fname, int *sizeout);
	void LoadAllocFree(char *ptr);
	int SaveToFile(const char *fname, const unsigned char *src, int size);
	void SetVolume(int fmvol, int ssgvol);
	void SetFastFW(int value);
	void SkipPlay(int count);
	void PlayLoop(void);

	//		���z�}�V���X�e�[�^�X
	uint8_t *GetMemoryMap(void) { return mem; }
	int GetFlag(void) { return m_flag; }
	int Peek(uint16_t adr);
	int Peekw(uint16_t adr);
	void Poke(uint16_t adr, uint8_t data);
	void Pokew(uint16_t adr, uint16_t data);
	void PeekToStr(char *out, uint16_t adr, uint16_t length);
	void BackupMem(uint8_t *mem_bak);
	void RestoreMem(uint8_t *mem_bak);

	void ResetTimer(void);
	void StartINT3(void);
	void StopINT3(void);
	void RestartINT3(void);
	void SetWindow(void *window) { master_window = window; }
	void SetIntCount(int value) { time_intcount = value; }
	int GetIntCount(void) { return time_intcount; }
	int GetMasterCount(void) { return time_master; }
	int GetPassTick(void) { return pass_tick; }

	//		YM2608�X�e�[�^�X
	void FMRegDataOut(int reg, int data);
	int FMRegDataGet(int reg);
	void SetChMuteAll(bool sw);
	void SetChMute(int ch, bool sw);
	bool GetChMute(int ch);
	int GetChStatus(int ch);
	uint8_t *GetRegisterMap(void) { return regmap;  }

	//		�f�o�b�O�p
	void Msgf(const char *format, ...);
	char *GetMessageBuffer(void) { return membuf->GetBuffer(); }
	void DumpBin(uint16_t adr, uint16_t length);
	int DeviceCheck(void);
	int GetMessageId() { return msgid; }

	//		ADPCM�p
	int ConvertWAVtoADPCMFile(const char *fname, const char *sname);

	//		BANK�؂�ւ�
	void ClearBank(void);
	void ChangeBank(int bank);

	//		CHDATA�p
	void InitChData(int chmax, int chsize);
	void SetChDataAddress(int ch, int adr);
	uint8_t *GetChData(int ch);
	uint8_t GetChWork(int index);
	void ProcessChData(void);

	//		�I�[�f�B�I��������
	void RenderAudio(void *mix, int size);
	void AudioCallback(void *mix, int size);
	void UpdateTime(int tick);
	void UpdateCallback(int tick);

	//		�v���O�C���R���g���[��
	void SetMucomInstance(CMucom *mucom);
	int AddPlugins(const char *filename, int bootopt);
	void FreePlugins(void);
	void NoticePlugins(int cmd, void *p1 = NULL, void *p2=NULL);

private:
	//		Z80
	int32_t load(uint16_t adr);
	int32_t loadpc(uint16_t adr);
	void store(uint16_t adr, uint8_t data);
	int32_t input(uint16_t adr);
	void output(uint16_t adr, uint8_t data);
	void Halt(void);

	//		������(64K)
	int m_flag;
	int m_option;						// ����I�v�V����
	int m_fastfw;						// ������J�E���g
	int m_fmvol, m_ssgvol;				// �{�����[��
	int bankmode;						// BANK(3=MAIN/012=BGR)
	int bankprg;						// BANK(0=MAIN/1=�V���h�[�R�s�[��)
	uint8_t mem[0x10000];				// ���C��������
	uint8_t vram[VMBANK_MAX][0x4000];	// GVRAM(3:�ޔ�/012=BRG)
	uint8_t memprg[0x10000];			// ���C��������(�v���O�������s�p�̃V���h�[�R�s�[)

	//		����
	FM::OPNA *opn;

	int sound_reg_select;
	int sound_reg_select2;
	void FMOutData(int data);
	void FMOutData2(int data);
	int FMInData();
	int FMInData2();

	//		OPNA���X�^�b�N
	int Rate;
	uint8_t chmute[OPNACH_MAX];
	uint8_t chstat[OPNACH_MAX];
	uint8_t regmap[OPNAREG_MAX];

	//		CH���X�^�b�N
	int channel_max, channel_size;
	uint16_t pchadr[64];
	uint8_t *pchdata;
	uint8_t pchwork[16];

	//		���荞�݃^�C�}�[�֘A
	int time_master;
	int time_scount;
	int time_intcount;
	int pass_tick;
	int last_tick;

	bool playflag;
	bool busyflag;
	bool int3flag;
	int predelay;
	int int3mask;
	int msgid;

	void checkThreadBusy(void);

	//		���b�Z�[�W�o�b�t�@
	CMemBuf *membuf;

	//		OS�ˑ�����
	OsDependent *osd;
	void *master_window;

	// �v���O�C���g��(�����p)
	std::vector<Mucom88Plugin *> plugins;

	//		�e�̃C���X�^���X(�Q��)
	CMucom *p_cmucom;

};



#endif
