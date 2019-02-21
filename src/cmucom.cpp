
//
//		MUCOM88 access class
//		(PC-8801��VM�����MUCOM88�̋@�\��񋟂��܂�)
//			MUCOM88 by Yuzo Koshiro Copyright 1987-2019(C) 
//			Windows version by onion software/onitama 2018/11
//

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cmucom.h"
#include "mucomvm.h"
#include "mucomerror.h"
#include "md5.h"

#include "bin_music2.h"

#include "bin_expand.h"
#include "bin_errmsg.h"
#include "bin_msub.h"
#include "bin_muc88.h"
#include "bin_ssgdat.h"
#include "bin_time.h"
#include "bin_voice.h"
#include "bin_smon.h"

#define PRINTF vm->Msgf

int CMucom::htoi_sub(char hstr)
{
	//	exchange hex to int

	char a1;
	a1 = tolower(hstr);
	if ((a1 >= '0') && (a1 <= '9')) return a1 - '0';
	if ((a1 >= 'a') && (a1 <= 'f')) return a1 - 'a' + 10;
	return 0;
}


int CMucom::htoi(char *str)
{
	//	16�i->10�i�ϊ�
	//
	char a1;
	int d;
	int conv;
	conv = 0;
	d = 0;
	while (1) {
		a1 = str[d++]; if (a1 == 0) break;
		conv = (conv << 4) + htoi_sub(a1);
	}
	return conv;
}

int CMucom::strpick_spc(char *target, char *dest, int strmax)
{
	//		str�̐擪����space�܂ł��������Ƃ��Ď��o��
	//
	unsigned char *p;
	unsigned char *dst;
	unsigned char a1;
	int len = 0;
	int mulchr;
	p = (unsigned char *)target;
	dst = (unsigned char *)dest;
	while (1) {
		if (len >= strmax) break;
		a1 = *p; if (a1 == 0) return 0;
		if (a1 == 32) break;

		mulchr = GetMultibyteCharacter(p);
		if (mulchr == 1) {
			*dst++ = tolower(a1);
			p++;
			len++;
			continue;
		}
		while (mulchr > 0) {
			*dst++ = *p++;
			len++;
		}
	}
	*dst++ = 0;
	return len;
}


/*------------------------------------------------------------*/
/*
		interface
*/
/*------------------------------------------------------------*/

CMucom::CMucom( void )
{
	flag = 0;
	vm = NULL;
	infobuf = NULL;
	edit_status = MUCOM_EDIT_STATUS_NONE;
	user_uuid[0] = 0;
}


CMucom::~CMucom( void )
{
	Stop(1);
	DeleteInfoBuffer();
	MusicBufferTerm();
	if (vm != NULL) delete vm;
}


void CMucom::Init(void *window, int option, int rate)
{
	//		MUCOM88�̏�����(���񂾂��Ăяo���Ă�������)
	//		window : 0   = �E�C���h�E�n���h��(HWND)
	//		               (NULL�̏ꍇ�̓A�N�e�B�u�E�C���h�E���I�������)
	//		option : 0   = 1:FM���~���[�g  2:SCCI���g�p
	//
	vm = new mucomvm;
	flag = 1;

	// ���[�g�ݒ�
	int myrate = rate;
	if (rate == 0) myrate = MUCOM_AUDIO_RATE;
	SetAudioRate(myrate);

	if (window != NULL) {
		vm->SetWindow(window);
	}
	vm->SetOption(option);
	vm->InitSoundSystem(myrate);
	MusicBufferInit();
	vm->SetMucomInstance(this);			// Mucom�̃C���X�^���X��ʒm����(�v���O�C���p)

	//		�I�[�g�Z�[�u��������(�f�t�H���g�͖���)
	edit_autosave = 0;
	edit_autosave_time = 0;
	edit_autosave_next = 0;

	//		�G�f�B�^���ێ�������̃��Z�b�g
	EditorReset();
	InitFMVoice();
}


int CMucom::AddPlugins(const char *filename, int bootopt)
{
	//		�v���O�C���ǉ�
	//		filename = DLL�t�@�C����(���̂Ƃ���Win�̂�)
	//		bootopt = �N���I�v�V����
	//
	return vm->AddPlugins(filename, bootopt);
}


void CMucom::NoticePlugins(int cmd, void *p1, void *p2)
{
	//		�v���O�C���ʒm
	//
	vm->NoticePlugins(cmd,p1,p2);
}


void CMucom::SetAudioRate(int rate)
{
	//		�O�������_�����O�p�̏o�̓��[�g�ݒ�
	//
	AudioStep = (double)1000.0 / rate;
	AudioLeftMs = 0.0;
}


void CMucom::Reset(int option)
{
	//		MUCOM88�̃��Z�b�g(���x�ł��Ăׂ܂�)
	//		option : 0   = �����̃v���C���[������
	//		         1   = �O���t�@�C���ɂ�鏉����
	//		         2,3 = �R���p�C����������
	//
	int devres;
	vm->Reset();
	PRINTF("#OpenMucom88 Ver.%s Copyright 1987-2019(C) Yuzo Koshiro\r\n",VERSION);
	pcmfilename[0] = 0;

	devres = vm->DeviceCheck();
	if (devres) {
		PRINTF("#Device error(%d)\r\n", devres);
	}

	if (option & MUCOM_CMPOPT_COMPILE) {
		if (option & MUCOM_CMPOPT_USE_EXTROM) {
			//	�R���p�C�����t�@�C������ǂ�
			vm->LoadMem("expand", 0xab00, 0);
			vm->LoadMem("errmsg", 0x8800, 0);
			vm->LoadMem("msub", 0x9000, 0);
			vm->LoadMem("muc88", 0x9600, 0);
			vm->LoadMem("ssgdat", 0x5e00, 0);
			vm->LoadMem("time", 0xe400, 0);
			vm->LoadMem("smon", 0xde00, 0);
			LoadFMVoice(MUCOM_DEFAULT_VOICEFILE,true);
		}
		else {
			//	�����̃R���p�C����ǂ�
			vm->SendMem(bin_expand, 0xab00, expand_size);
			vm->SendMem(bin_errmsg, 0x8800, errmsg_size);
			vm->SendMem(bin_msub, 0x9000, msub_size);
			vm->SendMem(bin_muc88, 0x9600, muc88_size);
			vm->SendMem(bin_ssgdat, 0x5e00, ssgdat_size);
			vm->SendMem(bin_time, 0xe400, time_size);
			vm->SendMem(bin_smon, 0xde00, smon_size);
			//vm->SendMem(bin_voice_dat, 0x6000, voice_dat_size);
			StoreFMVoice((unsigned char *)bin_voice_dat);
		}

		int i;
		// music���̃_�~�[���[�`��
		for (i = 0; i < 0x40; i++) {
			vm->Poke(0xb000 + i, 0xc9);
		}
		// basic rom���̃_�~�[���[�`��
		for (i = 0; i < 0x4000; i++) {
			vm->Poke(0x1000 + i, 0xc9);
		}
		// ���[�N�ݒ�p���[�`��
		i = 0xb036;
		vm->Poke(i++, 0xdd);	// LD IX,$c9bf
		vm->Poke(i++, 0x21);
		vm->Poke(i++, 0x0c);
		vm->Poke(i++, 0xbf);
		vm->Poke(i++, 0xc9);

		return;
	}
	if (option & MUCOM_CMPOPT_USE_EXTROM) {
		//	�v���C���[���t�@�C������ǂ�
		vm->LoadMem("music", 0xb000, 0);
	}
	else {
		//	�����̃v���C���[��ǂ�
		vm->SendMem(bin_music2, 0xb000, music2_size);
	}

	//	���s�p���������V���h�[�R�s�[�Ƃ��ĕۑ����Ă���
	vm->SendMemoryToShadow();

	DeleteInfoBuffer();

	int i,adr;
	vm->InitChData(MUCOM_MAXCH,MUCOM_CHDATA_SIZE);
	for (i = 0; i < MUCOM_MAXCH; i++){
		adr = vm->CallAndHaltWithA(0xb00c, i);
		vm->SetChDataAddress( i,adr );
	}

	NoticePlugins(MUCOM88IF_NOTICE_RESET);
}

void CMucom::SetUUID(char *uuid)
{
	if (uuid == NULL) {
		user_uuid[0] = 0;
		return;
	}
	strncpy(user_uuid,uuid,64);
}


const char *CMucom::GetMessageBuffer(void)
{
	return vm->GetMessageBuffer();
}

int CMucom::Play(int num)
{
	//		MUCOM88���y�Đ�
	//		num : 0   = ���yNo. (0�`15)
	//		(�߂�l��0�ȊO�̏ꍇ�̓G���[)
	//
	char *data;
	char *pcmdata;
	int datasize;
	int pcmsize;

	if ((num < 0) || (num >= MUCOM_MUSICBUFFER_MAX)) return -1;

	Stop();

	hedmusic = NULL;

	CMemBuf *buf = musbuf[num];
	if (buf == NULL) return -1;

	hedmusic = (MUBHED *)buf->GetBuffer();
	mubver = MUBGetHeaderVersion(hedmusic);
	if (mubver < 0) return -2;

	LoadTagFromMusic(num);

	data = MUBGetData(hedmusic, datasize);
	if (data == NULL) return -3;

	if (hedmusic->pcmdata) {
		int skippcm = 0;
		const char *pcmname = GetInfoBufferByName("pcm");
		if (pcmname[0] != 0) {
			//	���ɓ�����PCM���ǂݍ��܂�Ă��邩?
			if (strcmp(pcmname, pcmfilename) == 0) skippcm = 1;
		}
		if (skippcm==0) {
			//	���ߍ���PCM��ǂݍ���
			pcmdata = MUBGetPCMData(hedmusic, pcmsize);
			vm->LoadPcmFromMem(pcmdata, pcmsize);
		}
	}

	LoadFMVoiceFromTAG();

	vm->SendMem((const unsigned char *)data, 0xc200, datasize);

	StoreFMVoiceFromEmbed();

	NoticePlugins(MUCOM88IF_NOTICE_PREPLAY);

	PRINTF("#Play[%d]\r\n", num);
	vm->CallAndHalt(0xb000);
	//int vec = vm->Peekw(0xf308);
	//PRINTF("#INT3 $%x.\r\n", vec);

	NoticePlugins(MUCOM88IF_NOTICE_PLAY);

	int jcount = hedmusic->jumpcount;
	vm->SkipPlay(jcount);
	vm->StartINT3();

	playflag = true;

	return 0;
}


void CMucom::PlayLoop() {
	vm->PlayLoop();
}


// ���Ԃ�i�߂ă����_�����O���s��
void CMucom::RenderAudio(void *mix, int size) {
	AudioLeftMs += size * AudioStep;
	int ms = (int)AudioLeftMs;
	if (ms > 0) UpdateTime(ms);
	AudioLeftMs -= ms;

	memset(mix, 0, size * 2 * sizeof(int));
	vm->RenderAudio(mix, size);
}


// ���Ԃ̂ݍX�V
void CMucom::UpdateTime(int tick_ms) {
	vm->UpdateTime(tick_ms << TICK_SHIFT);
}


int CMucom::LoadTagFromMusic(int num)
{
	//		MUCOM88���y�f�[�^����^�O�ꗗ���擾����
	//		num : 0   = ���yNo. (0�`15)
	//		(�߂�l��0�ȊO�̏ꍇ�̓G���[)
	//
	MUBHED *hed;
	int ver;
	int tagsize;

	if ((num < 0) || (num >= MUCOM_MUSICBUFFER_MAX)) return -1;

	DeleteInfoBuffer();
	infobuf = new CMemBuf();

	CMemBuf *buf = musbuf[num];
	if (buf == NULL) {
		infobuf->Put((int)0);
		return -1;
	}

	hed = (MUBHED *)buf->GetBuffer();
	ver = MUBGetHeaderVersion(hed);
	if (ver < 0) {
		infobuf->Put((int)0);
		return -1;
	}

	if (hed->tagdata) {
		infobuf->PutStr(MUBGetTagData(hed, tagsize));
		infobuf->Put((int)0);
	}

	return 0;
}


int CMucom::Stop(int option)
{
	//		MUCOM88���y�Đ��̒�~
	//		(�߂�l��0�ȊO�̏ꍇ�̓G���[)
	//
	playflag = false;
	if (option & 1) {
		vm->StopINT3();
		vm->CallAndHalt(0xb003);
		vm->ResetFM();
	}
	else {
		vm->StopINT3();
		vm->CallAndHalt(0xb003);
	}
	NoticePlugins(MUCOM88IF_NOTICE_STOP);
	return 0;
}


int CMucom::Restart(void)
{
	//		MUCOM88���y�Đ��̍ĊJ(��~��)
	//		(�߂�l��0�ȊO�̏ꍇ�̓G���[)
	//
	NoticePlugins(MUCOM88IF_NOTICE_PLAY);
	vm->RestartINT3();
	playflag = true;
	return 0;
}


int CMucom::Fade(void)
{
	//		MUCOM88���y�t�F�[�h�A�E�g
	//		(�߂�l��0�ȊO�̏ꍇ�̓G���[)
	//
	if (playflag == false) return -1;
	vm->CallAndHalt(0xb006);
	return 0;
}


int CMucom::LoadPCM(const char * fname)
{
	//		ADPCM�f�[�^�ǂݍ���
	//		fname = PCM�f�[�^�t�@�C�� (�f�t�H���g��MUCOM_DEFAULT_PCMFILE)
	//		(�߂�l��0�ȊO�̏ꍇ�̓G���[)
	//
	if (strcmp(pcmfilename,fname)==0) return 0;			// ���ɓǂݍ���ł���ꍇ�̓X�L�b�v
	strncpy(pcmfilename, fname, MUCOM_FILE_MAXSTR-1);
	if (vm->LoadPcm(fname) == 0) return 0;
	PRINTF("#PCM file not found [%s].\r\n", fname);
	return -1;
}

int CMucom::LoadMusic(const char * fname, int num)
{
	//		���y�f�[�^�ǂݍ���
	//		fname = ���y�f�[�^�t�@�C��
	//		num : 0   = ���yNo. (0�`15)
	//		(�߂�l��0�ȊO�̏ꍇ�̓G���[)
	//
	if ((num < 0) || (num >= MUCOM_MUSICBUFFER_MAX)) return -1;

	CMemBuf *buf = new CMemBuf();
	if (buf->PutFile((char *)fname) < 0) {
		PRINTF("#MUSIC file not found [%s].\r\n", fname);
		delete buf;
		return -1;
	}

	if (musbuf[num] != NULL) {
		delete musbuf[num];
	}

	musbuf[num] = buf;
	//if (vm->LoadMem(fname, 0xc200, 0) >= 0) return 0;

	NoticePlugins(MUCOM88IF_NOTICE_LOADMUB);

	return 0;
}

void CMucom::MusicBufferInit(void)
{
	for (int i = 0; i < MUCOM_MUSICBUFFER_MAX; i++) {
		musbuf[i] = NULL;
	}
}

void CMucom::MusicBufferTerm(void)
{
	for (int i = 0; i < MUCOM_MUSICBUFFER_MAX; i++) {
		if (musbuf[i] != NULL) {
			delete musbuf[i];
			musbuf[i] = NULL;
		}
	}
}

int CMucom::GetStatus(int option)
{
	//		MUCOM�X�e�[�^�X�ǂݏo��
	//		option : 0  ��~=0/���t��=1
	//		         1  ���t�J�n����̊��荞�݃J�E���g
	//		         2  �X�g���[���Đ��ɂ�����������(ms)
	//		         3  ���W���[�o�[�W�����R�[�h
	//		         4  �}�C�i�[�o�[�W�����R�[�h
	//		(�߂�l��32bit����)
	//
	int i;
	switch (option) {
	case MUCOM_STATUS_PLAYING:
		if (playflag) return 1;
		break;
	case MUCOM_STATUS_INTCOUNT:
		return vm->GetIntCount();
	case MUCOM_STATUS_PASSTICK:
		return vm->GetMasterCount();
	case MUCOM_STATUS_MAJORVER:
		return MAJORVER;
	case MUCOM_STATUS_MINORVER:
		return MINORVER;

	case MUCOM_STATUS_COUNT:
		i = vm->GetIntCount();
		if (maxcount) {
			i = i % maxcount;
		}
		return i;
	case MUCOM_STATUS_MAXCOUNT:
		return maxcount;
	case MUCOM_STATUS_MUBSIZE:
		return mubsize;
	case MUCOM_STATUS_MUBRATE:
		return mubsize * 100 / MUCOM_MUBSIZE_MAX;
	case MUCOM_STATUS_BASICSIZE:
		return basicsize;
	case MUCOM_STATUS_BASICRATE:
		return basicsize * 100 / MUCOM_BASICSIZE_MAX;

	default:
		break;
	}
	return 0;
}


/*------------------------------------------------------------*/
/*
FM Voice file support
*/
/*------------------------------------------------------------*/

void CMucom::InitFMVoice(unsigned char *voice)
{
	//	FM���F�f�[�^������������
	//
	fmvoice_mode = MUCOM_FMVOICE_MODE_EXTERNAL;
	fmvoice_original = fmvoice_internal;
	StoreFMVoice(voice);
	voicefilename.clear();
	tempfilename.clear();
}


int CMucom::SaveFMVoice(bool sw)
{
	//	FM���F�f�[�^��ۑ�����
	//	(sw true=�ۑ��Afalse=�ۑ������Ɉꎞ�t�@�C��������)
	//
	int res;
	char curdir[MUCOM_FILE_MAXSTR];

	if (fmvoice_mode == MUCOM_FMVOICE_MODE_EXTERNAL) return 0;	// �ꎞ�t�@�C�����Ȃ���Εۑ��̕K�v�Ȃ�
	if (voicefilename.empty()) return -1;

	*curdir = 0;
	vm->GetDirectory(curdir, MUCOM_FILE_MAXSTR);			// ���݂̃f�B���N�g��
	vm->ChangeDirectory(voice_pathname.c_str());

	//	�ۑ�
	res = 0;
	if (sw) {
		vm->SendMem((unsigned char *)fmvoice_internal, MUCOM_FMVOICE_ADR, MUCOM_FMVOICE_SIZE);
		res = vm->SaveMem(voicefilename.c_str(), MUCOM_FMVOICE_ADR, MUCOM_FMVOICE_SIZE);
		//Alertf("[%s]%s", voice_pathname.c_str(), voicefilename.c_str());
	}

	//	FM���F�f�[�^�̈ꎞ�t�@�C����j������
	if (tempfilename.empty() == false) {
		if (res == 0) {
			vm->KillFile(tempfilename.c_str());
			tempfilename.clear();
		}
	}
	vm->ChangeDirectory(curdir);
	fmvoice_mode = MUCOM_FMVOICE_MODE_EXTERNAL;
	fmvoice_original = fmvoice_internal;
	return 0;
}


void CMucom::StoreFMVoice(unsigned char *voice)
{
	//	FM���F�f�[�^���������ɓ]������
	//
	if (voice == NULL) return;
	vm->SendMem(voice, MUCOM_FMVOICE_ADR, MUCOM_FMVOICE_SIZE);
	memcpy(fmvoice_internal, voice, MUCOM_FMVOICE_SIZE);
}


int CMucom::LoadFMVoice(const char *fname, bool sw)
{
	//		FM���F�f�[�^�ǂݍ���
	//		fname = FM���F�f�[�^�t�@�C�� (�󕶎���MUCOM_DEFAULT_VOICEFILE)
	//		sw = true�̏ꍇ�͋����I�ɓǂݍ��ށAfalse�̏ꍇ�͉��t�@�C������������
	//		(�߂�l��0�ȊO�̏ꍇ�̓G���[)
	//
	int voicesize;
	char *voicedata;
	char *voicedata_org;
	char dirname[MUCOM_FILE_MAXSTR];

	voicefilename.clear();
	voice_pathname.clear();
	tempfilename.clear();

	fmvoice_mode = MUCOM_FMVOICE_MODE_EXTERNAL;
	if (*fname != 0) {
		voicefilename = std::string(fname);
	}
	else {
		voicefilename = std::string(MUCOM_DEFAULT_VOICEFILE);
	}

	tempfilename = voicefilename + "_tmp";
	*dirname = 0;
	vm->GetDirectory(dirname, MUCOM_FILE_MAXSTR);
	voice_pathname = std::string(dirname);

	voicedata = NULL;
	voicedata_org = NULL;

	if (fmvoice_original != fmvoice_internal) {
		vm->LoadAllocFree((char *)fmvoice_original);		// �I���W�i���f�[�^������Δj������
		fmvoice_original = fmvoice_internal;
	}

	voicedata_org = vm->LoadAlloc(voicefilename.c_str(), &voicesize);
	if (voicedata_org == NULL) {
		PRINTF("#Voice file not found [%s].\r\n", fname);
		return -1;
	}
	fmvoice_original = (MUCOM88_VOICEFORMAT *)voicedata_org;

	if (sw == false) {
		voicedata = vm->LoadAlloc(tempfilename.c_str(), &voicesize);
	}

	if (voicedata == NULL) {
		// �I���W�i���t�@�C���݂̂̏ꍇ
		StoreFMVoice((unsigned char *)voicedata_org);
		//Alertf("[%s]%s", voice_pathname.c_str(), voicefilename.c_str());
	}
	else {
		// �ꎞ�t�@�C�����������ꍇ
		StoreFMVoice((unsigned char *)voicedata);
		vm->LoadAllocFree(voicedata);
		fmvoice_mode = MUCOM_FMVOICE_MODE_INTERNAL;
		//Alertf("[%s]%s", voice_pathname.c_str(), tempfilename.c_str());
	}
	return 0;
}


MUCOM88_VOICEFORMAT *CMucom::GetFMVoice(int no)
{
	//	�����ۑ����ꂽ���F�f�[�^���擾����
	//	(no=���F�ԍ�0�`255)
	//
	if ((no < 0) || (no >= MUCOM_FMVOICE_MAXNO)) return NULL;
	return &fmvoice_internal[no];
}


int CMucom::UpdateFMVoice(int no, MUCOM88_VOICEFORMAT *voice)
{
	//	���F�f�[�^���X�V����
	//	(no=���F�ԍ�0�`255)
	//
	char curdir[MUCOM_FILE_MAXSTR];

	if ((no < 0) || (no >= MUCOM_FMVOICE_MAXNO)) return -1;

	fmvoice_internal[no] = *voice;

	*curdir = 0;
	if (voicefilename.empty()) return -1;


	vm->GetDirectory(curdir, MUCOM_FILE_MAXSTR);			// ���݂̃f�B���N�g��
	vm->ChangeDirectory(voice_pathname.c_str());

	if (tempfilename.empty() == false) {
		fmvoice_mode = MUCOM_FMVOICE_MODE_INTERNAL;
		vm->SendMem((unsigned char *)fmvoice_internal, MUCOM_FMVOICE_ADR, MUCOM_FMVOICE_SIZE);
		return vm->SaveMem((char *)tempfilename.c_str(), MUCOM_FMVOICE_ADR, MUCOM_FMVOICE_SIZE);
	}
	vm->ChangeDirectory(curdir);

	return 0;
}


void CMucom::DumpFMVoice(int no)
{
	//	���F�f�[�^��\������
	//	(no=���F�ԍ�1�`255)
	//
	unsigned char name[8];
	unsigned char *p;
	unsigned char a1;
	MUCOM88_VOICEFORMAT *v = GetFMVoice(no);
	if (v==NULL) return;

	p = (unsigned char *)v->name;
	int len = 0;
	while (1) {
		if (len >= 6) break;
		a1 = *p++;
		if (a1 < 32) a1 = 32;		// �R���g���[���R�[�h�̓X�y�[�X�ɕϊ�
		if (a1 >= 128) a1 = 32;		// ���p�����̓X�y�[�X�ɕϊ�
		name[len++] = a1;
	}
	name[len++] = 0;

	PRINTF("  @%d:{\r\n", no);
	PRINTF("  %d, %d\r\n", v->fb, v->al);
	/*
	  AR,DR,SR,RR,SL,TL,KS,ML,DT
	*/
	PRINTF("  %02d, %02d, %02d, %02d, %02d, %02d, %02d, %02d, %02d\r\n", v->ar_op1, v->dr_op1, v->sr_op1, v->rr_op1, v->sl_op1, v->tl_op1, v->ks_op1, v->ml_op1, v->dt_op1);
	PRINTF("  %02d, %02d, %02d, %02d, %02d, %02d, %02d, %02d, %02d\r\n", v->ar_op2, v->dr_op2, v->sr_op2, v->rr_op2, v->sl_op2, v->tl_op2, v->ks_op2, v->ml_op2, v->dt_op2);
	PRINTF("  %02d, %02d, %02d, %02d, %02d, %02d, %02d, %02d, %02d\r\n", v->ar_op3, v->dr_op3, v->sr_op3, v->rr_op3, v->sl_op3, v->tl_op3, v->ks_op3, v->ml_op3, v->dt_op3);
	PRINTF("  %02d, %02d, %02d, %02d, %02d, %02d, %02d, %02d, %02d, \"%s\"}\r\n", v->ar_op4, v->dr_op4, v->sr_op4, v->rr_op4, v->sl_op4, v->tl_op4, v->ks_op4, v->ml_op4, v->dt_op4,name);
}


int CMucom::StoreFMVoiceFromEmbed(void)
{
	//	���t�f�[�^(MUB)�������F�f�[�^������ۑ��f�[�^�ɔ��f������
	//	(VM��$c200���牉�t�f�[�^��ǂݍ���ł�������)
	//		voicelist = �o�͂���鉹�F�ԍ��̃��X�g
	//		�Ԓl = voicelist�ɏo�͂��ꂽ���F�ԍ��̐�
	//
	int i,j,no;
	char *v;
	int vmmem;

	int vdata = vm->Peekw(0xc201) + 0xc200;
	int result = vm->Peek(vdata++);
	fmvoice_usemax = result;

	for (i = 0; i < result; i++) {
		no = (int)hedmusic->ext_fmvoice[i] - 1;
		fmvoice_use[i] = (unsigned char)no;
		v = (char *)GetFMVoice(no);
		vmmem = no * sizeof(MUCOM88_VOICEFORMAT) + MUCOM_FMVOICE_ADR;
		vm->SendMem( (unsigned char *)v, vmmem, sizeof(MUCOM88_VOICEFORMAT));
		v++;
		for (j = 0; j < 25; j++) {			// 25byte�̉��F�f�[�^���R�s�[
			*v++ = vm->Peek(vdata++);
		}
	}
	return result;
}


int CMucom::LoadFMVoiceFromTAG(void)
{
	//	(#voice)�^�O��񂩂特�F�f�[�^�����[�h����(���F�G�f�B�^�p�̏���)
	//

	//	voice�t�@�C�������[�h���Ă���
	char voicefile[MUCOM_FILE_MAXSTR];
	strncpy(voicefile, GetInfoBufferByName("voice"), MUCOM_FILE_MAXSTR);
	if (voicefile[0]) {
		LoadFMVoice(voicefile);
	}

	return 0;
}


/*------------------------------------------------------------*/
/*
Editor support
*/
/*------------------------------------------------------------*/

void CMucom::EditorReset(const char *mml, int option)
{
	//		�G�f�B�^�ҏW���ڂ̃��Z�b�g
	//		(mml=�ҏW�ǂݍ��ݎ���MML�o�b�t�@���e/NULL�̏ꍇ�͋�ɂȂ�)
	//		(option=MUCOM_EDIT_OPTION_*)
	//
	edit_status = MUCOM_EDIT_STATUS_NONE;
	edit_option = option;
	edit_notice = 0;
	edit_master.clear();
	edit_filename.clear();
	edit_pathname.clear();
	edit_request.clear();
	if (mml) {
		edit_master = std::string(mml);
		edit_status = MUCOM_EDIT_STATUS_SAVED;
	}
	edit_buffer = (char *)edit_master.c_str();
}


void CMucom::EditorSetFileName(const char *filename, const char *pathname,bool sessionstart)
{
	//		�G�f�B�^�ҏW�t�@�C�����E�p�X��ݒ肷��
	//		(EditorReset��ɐݒ肷�邱��)
	//		(filename+pathname���ۑ������t�@�C���ɂȂ�܂�)
	//		(sessionstart=true���̓t�@�C���̓��e���`�F�b�N���܂�)
	//
	if (filename) edit_filename = std::string(filename);
	if (pathname) edit_pathname = std::string(pathname);
	if (sessionstart) {
		int res;
		res = ProcessFile(filename);
		if (res == 0) {
			NoticePlugins(MUCOM88IF_NOTICE_SESSION);
		}
	}
}


int CMucom::CheckEditorUpdate(void)
{
	//		�G�f�B�^�ҏW���ڂ��C������Ă��邩?
	//		return 0=���������A1=�ۑ��ς݁A2=�ҏW��
	//
	char *p = (char *)edit_master.c_str();
	if (edit_status == MUCOM_EDIT_STATUS_NONE) return MUCOM_EDIT_STATUS_NONE;
	if (strcmp(p, edit_buffer) == 0) {
		edit_status = MUCOM_EDIT_STATUS_SAVED;
		if (fmvoice_mode == MUCOM_FMVOICE_MODE_INTERNAL) {
			edit_status = MUCOM_EDIT_STATUS_VOICEEDIT;
		}
	}
	else {
		edit_status = MUCOM_EDIT_STATUS_CHANGED;
	}

	return edit_status;
}


int CMucom::SaveEditorMML(const char *filename)
{
	//		�G�f�B�^�ҏW���e��ۑ�����
	//		(filename��""�̏ꍇ�̓f�t�H���g���A�w�肳�ꂽ�ꍇ�͂��̖��O�ŕۑ�����)
	//
	SaveFMVoice();
	//
	if (*filename != 0) {
		return vm->SaveToFile(filename, (unsigned char *)edit_buffer, strlen(edit_buffer));
	}
	edit_master = std::string(edit_buffer);
	edit_status = MUCOM_EDIT_STATUS_SAVED;
	std::string savename = edit_pathname + edit_filename;
	return vm->SaveToFile(savename.c_str(), (unsigned char *)edit_buffer, strlen(edit_buffer));
}


int CMucom::UpdateEditorMML(const char *mml)
{
	//		�G�f�B�^�ҏW���e���X�V����
	//		(�O���̃G�f�B�^��MML���ω��������Ƃ�ʒm���܂�)
	//		(mml�̃|�C���^�̂ݍX�V���܂��A���e�͊O���ŕێ�����Ă���K�v������܂�)
	//
	edit_buffer = (char *)mml;
	edit_status = MUCOM_EDIT_STATUS_CHANGED;
	return 0;
}


int CMucom::GetEditorPosToLine(int pos)
{
	//		�G�f�B�^�̃J�[�\���ʒu����s���𓾂�
	//
	int line = 1;
	int myline = 1;
	int cur;
	const char *src = edit_buffer;

	while (1) {
		if (src == NULL) break;
		cur = src - edit_buffer;
		if (cur<=pos) myline = line;
		src = GetTextLine(src);
		line++;
	}
	return myline;
}


int CMucom::RequestEditorMML(const char *mml)
{
	//		�G�f�B�^��MML�t�@�C�����O������X�V���郊�N�G�X�g���o��
	//		(���N�G�X�g���G�f�B�^�����擾���Ȃ�����X�V����܂���)
	//
	edit_notice |= MUCOM_NOTICE_MMLCHANGE;
	edit_request = std::string(mml);
	return 0;
}


const char *CMucom::GetRequestMML(void)
{
	//		�O�����N�G�X�g�ɂ��MML���擾����
	//		(���N�G�X�g���Ȃ��ꍇ��NULL��Ԃ�)
	//
	if (edit_notice & MUCOM_NOTICE_MMLCHANGE) {
		edit_notice ^= MUCOM_NOTICE_MMLCHANGE;
		return edit_request.c_str();
	}
	return NULL;
}


int CMucom::UpdateEditor(void)
{
	//		�G�f�B�^�X�V
	//
	return edit_notice;
}


/*------------------------------------------------------------*/
/*
Compiler support
*/
/*------------------------------------------------------------*/

int CMucom::GetMultibyteCharacter(const unsigned char *text)
{
	//		�}���`�o�C�g�����̃T�C�Y�𓾂�
	//
	const unsigned char *p = text;
	unsigned char a1;
	int mulchr = 1;

	a1 = *p;

#ifndef MUCOM88UTF8
	if (a1 >= 129) {				// �S�p�����`�F�b�N(SJIS)
		if ((a1 <= 159) || (a1 >= 224)) {
			mulchr++;
		}
	}
#endif

#ifdef MUCOM88UTF8
	if (a1 >= 128) {				// �S�p�����`�F�b�N(UTF8)
		int utf8cnt = 0;
		if ((a1 >= 192) && (p[1] != 0)) utf8cnt++;
		if ((a1 >= 224) && (p[2] != 0)) utf8cnt++;
		if ((a1 >= 240) && (p[3] != 0)) utf8cnt++;
		if ((a1 >= 248) && (p[4] != 0)) utf8cnt++;
		if ((a1 >= 252) && (p[5] != 0)) utf8cnt++;
		mulchr += utf8cnt;
	}
#endif

	return mulchr;
}


const char *CMucom::GetTextLine(const char *text)
{
	//	1�s���̃f�[�^���i�[
	//
	const unsigned char *p = (const unsigned char *)text;
	unsigned char a1;
	int mptr = 0;
	int mulchr;

	while (1) {
		a1 = *p;
		if (a1 == 0) {
			p = NULL;  break;			// End of text
		}
		if (a1 == 10) {					// LF
			p++;
			break;
		}
		if (a1 == 13) {
			p++;
			if (*p == 10) p++;
			break;						// CR/LF
		}
		if (a1 == 9) {					// TAB->space
			a1 = 32;
			linebuf[mptr++] = a1;
			p++;
			continue;
		}

		mulchr = GetMultibyteCharacter(p);
		while (mulchr>0) {
			linebuf[mptr++] = *p++;
			mulchr--;
		}

	}
	linebuf[mptr++] = 0;
	return (const char *)p;
}


const char *CMucom::GetInfoBuffer(void)
{
	if (infobuf == NULL) return "";
	return infobuf->GetBuffer();
}


const char *CMucom::GetInfoBufferByName(const char *name)
{
	//		infobuf���̎w��^�O���ڂ��擾
	//		name = �^�O��(�p������)
	//		(���ʂ�""�̏ꍇ�͊Y�����ڂȂ�)
	//
	int len;

	if (infobuf == NULL) return "";

	const char *src = GetInfoBuffer();
	while (1) {
		if (src == NULL) break;
		src = GetTextLine(src);

		len = strpick_spc((char *)linebuf+1, infoname, 63);
		if (len > 0) {
			//printf("[%s]\n", infoname);
			if (strcmp(name, infoname) == 0) {
				return (char *)(linebuf+1+len+1);
			}
		}
	}
	return "";
}


void CMucom::DeleteInfoBuffer(void)
{
	if (infobuf) {
		delete infobuf; infobuf = NULL;
	}
}


void CMucom::PrintInfoBuffer(void)
{
	//		infobuf�̓��e���o��
	//
	PRINTF("%s", GetInfoBuffer());
}


int CMucom::ProcessFile(const char *fname)
{
	//		MUCOM88 MML�\�[�X�t�@�C�����̃^�O������
	//		fname = MML�����̃e�L�X�g�f�[�^�t�@�C��(UTF8)
	//		(���ʂ́Ainfobuf�ɓ���܂�)
	//		(�߂�l��0�ȊO�̏ꍇ�̓G���[)
	//
	int sz;
	char *mml;
	DeleteInfoBuffer();
	mml = vm->LoadAlloc(fname, &sz);
	if (mml == NULL) {
		PRINTF("#File not found [%s].\r\n", fname);
		return -1;
	}
	ProcessHeader( mml );
	vm->LoadAllocFree(mml);
	return 0;
}


bool CMucom::hasMacro(char *text)
{
	//		MML�\�[�X�̍s�Ƀ}�N����`(*nn{})���܂܂�邩���ׂ�
	//		(�^�O��`�Ƃ̐؂蕪��������)
	//		(�߂�l��true�̏ꍇ�̓}�N����`)
	//
	const unsigned char *p = (const unsigned char *)text;
	unsigned char a1;
	int mulchr;
	bool flag_marco = false;

	if (p[0] != '#') return flag_marco;

	while (1) {
		a1 = *p;
		if (a1 == 0) {
			return flag_marco;
		}
		if ( a1=='*' ) {
			p++;
			break;
		}
		mulchr = GetMultibyteCharacter(p);
		while (mulchr > 0) {
			p++;
			mulchr--;
		}
	}
	a1 = *p++;
	if ((a1 >= '0') && (a1 <= '9')) flag_marco = true;
	return flag_marco;
}


int CMucom::ProcessHeader(char *text)
{
	//		MUCOM88 MML�\�[�X���̃^�O������
	//		text         = MML�����̃e�L�X�g�f�[�^(UTF8)(�I�[=0)
	//		(�߂�l��0�ȊO�̏ꍇ�̓G���[)
	//
	//		#mucom88 1.5
	//		#voice voice.dat
	//		#pcm mucompcm.bin
	//		#composer name
	//		#author name
	//		#title name
	//		#date xxxx/xx/xx
	//		#comment ----
	//		#url ----
	//
	const char *src = text;
	DeleteInfoBuffer();
	infobuf = new CMemBuf();
	while (1) {
		if (src == NULL) break;
		src = GetTextLine(src);
		if (linebuf[0] == '#'){							// �^�O��?
			if (hasMacro((char *)linebuf)==false) {		// �}�N����?
				//printf("[%s]\n", linebuf);
				infobuf->PutStr((char *)linebuf);
				infobuf->PutCR();
			}
		}
	}
	infobuf->Put((char)0);
	return 0;
}


int CMucom::StoreBasicSource(char *text, int line, int add)
{
	//	BASIC�\�[�X�̌`���Ń��X�g���쐬
	//
	const char *src = text;
	int ln = line;
	int mptr = 1;
	int linkptr;
	int mulchr;
	int i;
	unsigned char a1;

	while (1) {
		if (src == NULL) break;

		linkptr = mptr;
		mptr += 2;
		vm->Pokew(mptr,ln);
		mptr += 2;
		vm->Poke(mptr, 0x3a);
		vm->Poke(mptr+1, 0x8f);
		vm->Poke(mptr+2, 0xe9);
		mptr += 3;

		src = GetTextLine(src);
		i = 0;
		while (1) {
			a1 = linebuf[i];
			if (a1 == 0) {
				vm->Poke(mptr++, 0);
				break;
			}
			mulchr = GetMultibyteCharacter(linebuf+i);
			i += mulchr;
			if ( mulchr==1) {
				vm->Poke(mptr++, a1);	// ���p�̕����̂ݓo�^����
			}
		}
		vm->Pokew(linkptr, mptr);		// ���̃|�C���^��ۑ�����
		ln += add;
	}

	vm->Pokew(mptr, 0);					// End pointer
	return mptr;
}


int CMucom::Compile(char *text, const char *filename, int option)
{
	//		MUCOM88�R���p�C��(Reset���K�v)
	//		text         = MML�����̃e�L�X�g�f�[�^(UTF8)(�I�[=0)
	//		filename     = �o�͂���鉹�y�f�[�^�t�@�C��
	//		option : 1   = #�^�O�ɂ��voice�ݒ�𖳎�
	//		         2   = PCM���ߍ��݂��X�L�b�v
	//		         4   = ���F�̈ꎞ�o�b�t�@�쐬��������
	//		(�߂�l��0�ȊO�̏ꍇ�̓G���[)
	//
	int i, res;
	int start, length;
	char *adr_start;
	char *adr_length;
	int workadr;
	int pcmflag;

	maxch = MUCOM_MAXCH;				// ���[�N�̎擾��������ߌŒ�l

	AddExtraInfo(text);

	//		voice�^�O�̉��
	if ((option & MUCOM_COMPILE_IGNOREVOICE) == 0) {
		char voicefile[MUCOM_FILE_MAXSTR];
		strncpy(voicefile, GetInfoBufferByName("voice"), MUCOM_FILE_MAXSTR);
		if (voicefile[0]) {
			LoadFMVoice(voicefile);
		}
	}

	NoticePlugins(MUCOM88IF_NOTICE_MMLSEND);

	basicsize = StoreBasicSource( text, 1, 1 );

	vm->CallAndHalt(0x9600);
	int vec = vm->Peekw(0x0eea8);
	PRINTF("#poll a $%x.\r\n", vec);

	int loopst = 0xf25a;
	vm->Pokew( loopst, 0x0101 );		// ���[�v���X�^�b�N������������(���[�v�O��'/'�ŃG���[���o������)

	res = vm->CallAndHalt2(vec, 'A');
	if (res){
		int line = vm->Peekw(0x0f32e);
		int msgid = vm->GetMessageId();
		if (msgid>0) {
			PRINTF("#error %d in line %d.\r\n-> %s (%s)\r\n", msgid, line, mucom_geterror_j(msgid), mucom_geterror(msgid));
		}
		else {
			PRINTF("#unknown error in line %d.\r\n", line);
		}
		return -1;
	}
	
	char stmp[128];
	vm->PeekToStr(stmp,0xf3c8,80);		// ��ʍŏ�i�̃��b�Z�[�W
	PRINTF("%s\r\n", stmp);

	workadr = 0xf320;
	fmvoice = vm->Peek(workadr + 50);
	pcmflag = 0;
	maxcount = 0;
	mubsize = 0;

	jumpcount = vm->Peekw(0x8c90);		// JCLOCK�̒l(J�R�}���h�̃^�O�ʒu)
	jumpline = vm->Peekw(0x8c92);		// JPLINE�̒l(J�R�}���h�̍s�ԍ�)

	PRINTF("Used FM voice:%d", fmvoice);

	if (jumpcount > 0) {
		PRINTF("  Jump to line:%d", jumpline);
	}

	PRINTF("\r\n");
	PRINTF("[ Total count ]\r\n");

	for (i = 0; i < maxch; i++){
		int tcnt,lcnt;
		tcnt = vm->Peekw(0x8c10 + i * 4);
		lcnt = vm->Peekw(0x8c12 + i * 4);
		if (lcnt) { lcnt = tcnt - (lcnt - 1); }
		if (tcnt > maxcount) maxcount = tcnt;
		tcount[i] = tcnt;
		lcount[i] = lcnt;
		PRINTF("%c:%d ", 'A' + i, tcount[i]);
	}
	PRINTF("\r\n");

	PRINTF("[ Loop count  ]\r\n");

	for (i = 0; i < maxch; i++){
		PRINTF("%c:%d ", 'A' + i, lcount[i]);
	}
	PRINTF("\r\n");

	adr_start = stmp + 31;
	adr_start[4] = 0;
	start = htoi(adr_start);

	adr_length = stmp + 41;
	adr_length[4] = 0;
	length = htoi(adr_length);

	mubsize = length;

	PRINTF("#Data Buffer $%04x-$%04x ($%04x)\r\n", start, start + length - 1,length);
	PRINTF("#MaxCount:%d Basic:%04x Data:%04x\r\n", maxcount, basicsize, mubsize);

	if (tcount[maxch-1]==0) pcmflag = 2;	// PCM ch���g���ĂȂ����PCM���ߍ��݂̓X�L�b�v

	NoticePlugins(MUCOM88IF_NOTICE_COMPEND);

	return SaveMusic(filename,start,length,option| pcmflag);
}


int CMucom::CompileFile(const char *fname, const char *sname, int option)
{
	//		MUCOM88�R���p�C��(Reset���K�v)
	//		fname     = MML�����̃e�L�X�g�t�@�C��(UTF8)
	//		sname     = �o�͂���鉹�y�f�[�^�t�@�C��
	//		option : 1   = #�^�O�ɂ��voice�ݒ�𖳎�
	//		(�߂�l��0�ȊO�̏ꍇ�̓G���[)
	//
	int res;
	int sz;
	char *mml;
	PRINTF("#Compile[%s] to %s.\r\n", fname, sname);
	mml = vm->LoadAlloc(fname, &sz);
	if (mml == NULL) {
		PRINTF("#File not found [%s].\r\n", fname);
		return -1;
	}

	ProcessHeader(mml);

	res = Compile(mml, sname, option);
	vm->LoadAllocFree(mml);

	if (res) return res;

	return 0;
}


void CMucom::AddExtraInfo(char *mmlsource)
{
	//	infobuf�ɒǉ�������������
	//
	char mml_md5[128];
	if (infobuf == NULL) return;

	GetMD5(mml_md5, mmlsource, strlen(mmlsource));
	infobuf->PutStr("#mucver " VERSION );
	infobuf->PutCR();
	infobuf->PutStr("#mmlhash ");
	infobuf->PutStr(mml_md5);
	infobuf->PutCR();
	if (user_uuid[0] != 0) {
		infobuf->PutStr("#uuid ");
		infobuf->PutStr(user_uuid);
		infobuf->PutCR();
	}
}


int CMucom::SaveMusic(const char *fname,int start, int length, int option)
{
	//		���y�f�[�^�t�@�C�����o��(�R���p�C�����K�v)
	//		filename     = �o�͂���鉹�y�f�[�^�t�@�C��
	//		option : 1   = #�^�O�ɂ��voice�ݒ�𖳎�
	//		         2   = PCM���ߍ��݂��X�L�b�v
	//		(�߂�l��0�ȊO�̏ꍇ�̓G���[)
	//
	int res;
	MUBHED hed;
	char *header;
	char *footer;
	char *pcmdata;
	const char *pcmname;
	int hedsize;
	int footsize;
	int pcmsize;
	int i;
	if (fname == NULL) return -1;

	header = (char *)&hed;
	hedsize = sizeof(MUBHED);
	memset(header,0,hedsize);
	header[0] = 'M';
	header[1] = 'U';
	header[2] = 'B';	// 'C'=1.0 Header, 'B'=2.0 Header
	header[3] = '8';

	footer = NULL;
	pcmdata = NULL;
	footsize = 0;
	pcmsize = 0;
	if (infobuf) {
		infobuf->Put((int)0);
		footer = infobuf->GetBuffer();
		footsize = infobuf->GetSize();
	}

	hed.dataoffset = hedsize;
	hed.datasize = length;
	hed.tagdata = hedsize + length;
	hed.tagsize = footsize;
	hed.jumpcount = jumpcount;
	hed.jumpline = jumpline;

	//	2.0 Header option
#ifdef MUCOM88UTF8
	hed.ext_flags = MUCOM_FLAG_UTF8TAG;
#else
	hed.ext_flags = MUCOM_FLAG_SJISTAG;
#endif	

	hed.ext_system = MUCOM_SYSTEM_PC88;
	hed.ext_target = MUCOM_TARGET_YM2608;
	hed.ext_channel_num = maxch;
	hed.ext_fmvoice_num = fmvoice;
	hed.ext_player = 0;					// not use (reserved)

	for (i = 0; i < fmvoice; i++) {
		hed.ext_fmvoice[i] = (unsigned char)vm->Peek(0x8c50+i);
	}

	if ((option & MUCOM_COMPILE_IGNOREPCM) == 0) {
		pcmname = GetInfoBufferByName("pcm");
		if (pcmname[0] != 0) {
			pcmdata = vm->LoadAlloc(pcmname, &pcmsize);
			if (pcmdata != NULL) {
				hed.pcmdata = hed.tagdata + footsize;
				hed.pcmsize = pcmsize;
			}
		}
	}

	//res = vm->SaveMem(filename, start, length);
	res = vm->SaveMemExpand(fname, start, length, header, hedsize, footer, footsize, pcmdata, pcmsize);
	if (res){
		PRINTF("#File write error [%s].\r\n", fname);
		return -2;
	}

	if (pcmdata != NULL) vm->LoadAllocFree(pcmdata);

	PRINTF("#Saved [%s].\r\n", fname);
	return 0;
}


int CMucom::MUBGetHeaderVersion(MUBHED *hed)
{
	char *p;
	p = (char *)hed;
	if ((p[0] == 'M') && (p[1] == 'U') && (p[2] == 'C') && (p[3] == '8')) return MUCOM_HEADER_VERSION1;
	if ((p[0] == 'M') && (p[1] == 'U') && (p[2] == 'B') && (p[3] == '8')) return MUCOM_HEADER_VERSION2;
	return -1;
}


char *CMucom::MUBGetData(MUBHED *hed, int &size)
{
	char *p;
	p = (char *)hed;
	p += hed->dataoffset;
	size = hed->datasize;
	return p;
}


char *CMucom::MUBGetTagData(MUBHED *hed, int &size)
{
	char *p;
	p = (char *)hed;
	p += hed->tagdata;
	if (hed->tagdata == 0) return NULL;
	size = hed->tagsize;
	if (size == 0) return NULL;
	return p;
}


char *CMucom::MUBGetPCMData(MUBHED *hed, int &size)
{
	char *p;
	p = (char *)hed;
	p += hed->pcmdata;
	if (hed->pcmdata == 0) return NULL;
	size = hed->pcmsize;
	if (size == 0) return NULL;
	return p;
}


int CMucom::ConvertADPCM(const char *fname, const char *sname)
{
	int res;
	res = vm->ConvertWAVtoADPCMFile(fname, sname);
	if (res < 0) return -1;
	return 0;
}


void CMucom::GetMD5(char *res, char *buffer, int size)
{
	int di;
	md5_state_t state;
	md5_byte_t digest[16];
	char hex_output[16 * 2 + 1];

	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)buffer, size);
	md5_finish(&state, digest);

	for (di = 0; di < 16; ++di) {
		sprintf(hex_output + di * 2, "%02x", digest[di]);
	}
	strcpy(res, hex_output);
}


void CMucom::SetVMOption(int option, int mode)
{
	switch (mode) {
	case 0:
		vm->SetOption(option);
		break;
	case 1:
		vm->SetOption( vm->GetOption() | option);
		break;
	case 2:
		vm->SetOption(vm->GetOption() & ~option);
		break;
	default:
		break;
	}
}


void CMucom::SetVolume(int fmvol, int ssgvol)
{
	vm->SetVolume(fmvol, ssgvol);
}


void CMucom::SetFastFW(int value)
{
	vm->SetFastFW(value);
}


int CMucom::GetChannelData(int ch, PCHDATA *result)
{
	//		�w�肳�ꂽ�`�����l���̃��A���^�C�����t�����擾����
	//			ch = �`�����l��No (0�`maxch)(A�`K�`�����l���̏�)
	//			result = ���ʂ��o�͂���PCHDATA�`���̃|�C���^(���炩���ߊm�ۂ��K�v)
	//
	int i;
	int size;
	int *ptr;
	unsigned char *src;
	unsigned char *srcp;
	bool ready = playflag;
	if ((ch < 0) || (ch >= MUCOM_MAXCH)) ready = false;
	if (!ready){
		memset(result, 0, sizeof(PCHDATA));
		return -1;
	}

	size = MUCOM_PCHDATA_PC88_SIZE;
	src = (unsigned char *)vm->GetChData(ch);
	srcp = src;
	ptr = (int *)result;

	if (src == NULL) return -1;

	for (i = 0; i < size; i++){
		*ptr++ = (int)*src++;
	}

	result->wadr = srcp[2] + (srcp[3] << 8);
	result->tadr = srcp[4] + (srcp[5] << 8);
	result->detune = srcp[9] + (srcp[10] << 8);
	result->lfo_diff = srcp[23] + (srcp[24] << 8);

	//	Check data (�`�����l�����Ƃɒl�̉��H���K�v)
	int pan = 0;
	int v_orig = 0;
	int vol_org = 0;
	unsigned char chwork;
	switch (ch) {
	case MUCOM_CH_PSG:
	case MUCOM_CH_PSG+1:
	case MUCOM_CH_PSG+2:
		vol_org = result->volume & 15;
		pan = 3;
		break;
	case MUCOM_CH_RHYTHM:
		vol_org = result->volume;
		pan = 3;
		break;
	case MUCOM_CH_ADPCM:
		chwork = vm->GetChWork(15);
		pan = chwork & 3;
		vol_org = result->volume - 4;
		if (vol_org < 0) vol_org = 0;
		break;
	default:
		if (ch >= MUCOM_CH_FM2) {
			chwork = vm->GetChWork(8 + 4 + (ch - MUCOM_CH_FM2));
		}
		else {
			chwork = vm->GetChWork(8 + ch);
		}
		vol_org = result->volume - 4;
		if (vol_org < 0) vol_org = 0;
		pan = (int)(chwork & 0xc0);
		pan = pan >> 6;
		if (mubver == MUCOM_HEADER_VERSION2) {
			v_orig = (int)hedmusic->ext_fmvoice[result->vnum] - 1;
			if (v_orig < 0) v_orig = 0;
		}
		break;
	}
	result->pan = pan;
	result->vnum_org = v_orig;
	result->vol_org = vol_org;

	return 0;
}

/*------------------------------------------------------------*/
/*
plugin interface
*/
/*------------------------------------------------------------*/

int MUCOM88IF_VM_COMMAND(void *ifptr, int cmd, int prm1, int prm2, void *prm3, void *prm4)
{
	//		VM�p�v���O�C���R�}���h
	Mucom88Plugin *plg = (Mucom88Plugin *)ifptr;
	mucomvm *vm = plg->vm;				// vm�C���X�^���X�𓾂�
	CMucom *mucom = plg->mucom;			// mucom�C���X�^���X�𓾂�
	switch (cmd) {
	case MUCOM88IF_MUCOMVM_CMD_FMWRITE:			// prm1=reg, prm2=data
		vm->FMRegDataOut( prm1, prm2 );
		return 0;
	case MUCOM88IF_MUCOMVM_CMD_FMREAD:
		break;
	case MUCOM88IF_MUCOMVM_CMD_GETCHDATA:		// prm1=ch, prm3=PCHDATA�o�͐�
		return mucom->GetChannelData(prm1, (PCHDATA *)prm3);
	case MUCOM88IF_MUCOMVM_CMD_CHDATA:
		break;
	case MUCOM88IF_MUCOMVM_CMD_TAGDATA:			// prm3=�^�O��, prm4=�o�͐�(max255chr)
		{
		char *result= (char *)prm4;
		const char *src = (const char *)prm3;
		const char *p = mucom->GetInfoBufferByName( src );
		if (p == NULL) return -1;
		strncpy(result,p,255 );
		return 0;
		}
	case MUCOM88IF_MUCOMVM_CMD_VOICEUPDATE:		// prm1=���FNo. , prm3=MUCOM88_VOICEFORMAT�|�C���^
		{
		return mucom->UpdateFMVoice(prm1, (MUCOM88_VOICEFORMAT *)prm3);
		}
	case MUCOM88IF_MUCOMVM_CMD_VOICESAVE:		// �p�����[�^�[�Ȃ�
		{
		return mucom->SaveFMVoice();
		}
	case MUCOM88IF_MUCOMVM_CMD_GETVOICENUM:		// (�Ԓl�̓e�[�u���̗v�f��) , prm3=�ԍ��o�͐�(int*)
		{
		int *pp = (int *)prm3;
		int max = mucom->GetUseVoiceMax();
		for (int i = 0; i < max; i++) {
			// �x�N�^�[�ɒǉ�����
			pp[i] = mucom->GetUseVoiceNum(i);
		}
		return max;
		}
	case MUCOM88IF_MUCOMVM_CMD_GETVOICEDATA:	// prm3,prm4=MUCOM88_VOICEFORMAT�|�C���^�o�͗p�̃|�C���^ (prm3�̓I���W�i���Aprm4�͕ҏW���̉��F)
		{
		MUCOM88_VOICEFORMAT **pp3 = (MUCOM88_VOICEFORMAT **)prm3;
		MUCOM88_VOICEFORMAT **pp4 = (MUCOM88_VOICEFORMAT **)prm4;
		*pp3 = mucom->GetVoiceDataOrg();
		*pp4 = mucom->GetVoiceData();
		return 0;
		}
	case MUCOM88IF_MUCOMVM_CMD_GETVOICENAME:	// ���F�t�@�C�������擾 prm3=char *�|�C���^�o�͗p�̃|�C���^
		{
		char **pp = (char **)prm3;
		*pp = (char *)mucom->GetVoiceFileName();
		return 0;
		}
	case MUCOM88IF_MUCOMVM_CMD_GETVMMEMMAP:		// VM�̂�Z80�������}�b�v���擾 prm3=char *�|�C���^�o�͗p�̃|�C���^
		{
		char **pp = (char **)prm3;
		*pp = (char *)vm->GetMemoryMap();
		return 0;
		}
	}
	return -1;
}


int MUCOM88IF_EDITOR_COMMAND(void *ifptr, int cmd, int prm1, int prm2, void *prm3, void *prm4)
{
	//		�G�f�B�^�p�v���O�C���R�}���h
	Mucom88Plugin *plg = (Mucom88Plugin *)ifptr;
	CMucom *mucom = plg->mucom;			// mucom�C���X�^���X�𓾂�
	switch (cmd) {
	case MUCOM88IF_EDITOR_CMD_GETTEXTSIZE:	// �p�����[�^�[�Ȃ�/�T�C�Y��Ԃ�
		return strlen(mucom->GetEditorMML());
	case MUCOM88IF_EDITOR_CMD_GETTEXT:	// prm3=�e�L�X�g�������ݐ�
		strcpy( (char *)prm3, mucom->GetEditorMML());
		return 0;
	case MUCOM88IF_EDITOR_CMD_UPDATETEXT:	// prm3=���������e�L�X�g�f�[�^
		return mucom->RequestEditorMML( (const char *)prm3 );
	}
	return -1;
}

