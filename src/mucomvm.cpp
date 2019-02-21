
//
//		PC-8801 virtual machine
//		(PC-8801FA������CPU��OPNA�݂̂��G�~�����[�V�������܂�)
//			onion software/onitama 2018/11
//			Z80 emulation by Yasuo Kuwahara 2002-2018(C)
//			FM Sound Generator by cisc 1998, 2003(C)
//

#include <stdio.h>
#include <stdio.h>
#include "mucomvm.h"

#include "adpcm.h"
#include "mucomerror.h"

#define BUFSIZE 200			// Stream Buffer 200ms
#define baseclock 7987200		// Base Clock

#include "mucomvm_os.h"
#include "voiceformat.h"

/*------------------------------------------------------------*/
/*
interface
*/
/*------------------------------------------------------------*/

mucomvm::mucomvm(void)
{
	m_flag = 0;
	m_option = 0;
	m_fastfw = 4;
	m_fmvol = 0;
	m_ssgvol = -3;

	opn = NULL;
	membuf = NULL;
	osd = NULL;
	master_window = NULL;

	channel_max = 0;
	channel_size = 0;
	pchdata = NULL;

	playflag = false;
}

mucomvm::~mucomvm(void)
{
	FreePlugins();

	if (osd) {
		osd->FreeTimer();
		osd->FreeAudio();
		osd->FreeRealChip();
		osd = NULL;
	}

	if (opn) {
		delete opn;
		opn = NULL;
	}
	if (membuf) delete membuf;

	if (pchdata) {
		free(pchdata);
	}

}


void mucomvm::SetMucomInstance(CMucom *mucom)
{
	p_cmucom = mucom;
}


void mucomvm::SetOption(int option)
{
	m_flag = 0;
	m_option = option;
}


void mucomvm::SetVolume(int fmvol, int ssgvol)
{
	if (opn) {
		m_fmvol = fmvol;
		m_ssgvol = ssgvol;
		opn->SetVolumeFM(m_fmvol);
		opn->SetVolumePSG(m_ssgvol);
	}
}

void mucomvm::SetFastFW(int value)
{
	if (opn) {
		m_fastfw = value;
	}
}


static void RunAudioCallback(void *CallbackInstance, void *MethodInstance);
static void RunTimerCallback(void *CallbackInstance, void *MethodInstance);

// �I�[�f�B�I�R�[���o�b�N
static void RunAudioCallback(void *CallbackInstance, void *MethodInstance) {
	AudioCallback *acb = (AudioCallback *)CallbackInstance;
	((mucomvm *)MethodInstance)->AudioCallback(acb->mix, acb->size);
}

// �^�C�}�[�R�[���o�b�N
static void RunTimerCallback(void *CallbackInstance, void *MethodInstance) {
	TimerCallback *tcb = (TimerCallback *)CallbackInstance;
	((mucomvm *)MethodInstance)->UpdateCallback(tcb->tick);
}


void mucomvm::ResetFM(void)
{
	//	VM�̃��Z�b�g(FM���̂�)
	//
	if (playflag) {
		osd->WaitSendingAudio();
	}

	int i;
	for (i = 0; i < OPNACH_MAX; i++) {
		chmute[i] = 0; chstat[i] = 0;
	}
	memset(regmap, 0, OPNAREG_MAX);

	if (opn) {
		int3mask = 0;
		int3flag = false;
		opn->Reset();
		opn->SetRate(baseclock, Rate, false);
		opn->SetReg(0x2d, 0);
		opn->SetVolumeFM(m_fmvol);
		opn->SetVolumePSG(m_ssgvol);
	}

	if (m_option & VM_OPTION_SCCI) {
		osd->ResetRealChip();
		osd->OutputRealChip(0x2d, 0);
	}
}

void mucomvm::InitSoundSystem(int rate)
{
	// ���[�g�ݒ�
	Rate = rate;

	//	�T�E���h�̏�����(����݂̂�OK)
	//
	playflag = false;
	predelay = 0;

	// OS�ˑ�����
	osd = new OSDEP_CLASS();
	if (osd == NULL) return;

	//		COM������
	//
	osd->CoInitialize();

	//		SCCI�Ή�
	//
	if (m_option & VM_OPTION_SCCI) {
		osd->InitRealChip();
	}

	//		�I�[�f�B�I
	if (!(m_option & VM_OPTION_STEP)) {
		osd->UserAudioCallback->Set(this, &RunAudioCallback);
		osd->UserTimerCallback->Set(this, &RunTimerCallback);
		osd->InitAudio(master_window, rate, BUFSIZE);
	}

	//		�^�C�}�[������
	//
	ResetTimer();

	if (!(m_option & VM_OPTION_STEP)) {
		osd->InitTimer();
		osd->ResetTime();
	}

	opn = new FM::OPNA;
	if (opn) {
		opn->Init(baseclock, 8000, 0);
#if 0
		// PSG TEST
		opn->SetReg(0x07, 8 + 16 + 32);
		opn->SetReg(0x08, 0x8);
		opn->SetReg(0x00, 0x0);
		opn->SetReg(0x01, 0x4);
#endif
		ResetFM();
	}

	playflag = true;
	//printf("#Stream update %dms.\n", time_stream);
}

void mucomvm::Reset(void)
{
	//	VM�̃��Z�b�g(FMReset���܂�)
	//
	uint8_t *p;

	Z80::Reset();
	ResetFM();
	SetIntVec(0xf3);

	p = &mem[0];
	memset(p, 0, 0x10000);		// CLEAR
	ClearBank();
	bankprg = VMPRGBANK_MAIN;

	m_flag = VMFLAG_EXEC;
	sound_reg_select = 0;

#if 0
	*p++ = 0x3c;	    // LABEL1:INC A
	*p++ = 0xd3;
	*p++ = 0x01;		// OUT (0),A
	*p++ = 0x76;		// HALT
	*p++ = 0x18;
	*p++ = 0xfb;		// JR LABEL1
#endif

	if (membuf) delete membuf;
	membuf = new CMemBuf;
}


int mucomvm::DeviceCheck(void)
{
	//	�f�o�C�X�̐��퐫���`�F�b�N
	//
	if (m_option & VM_OPTION_SCCI) {
		return osd->CheckRealChip();
	}
	return 0;
}


int32_t mucomvm::loadpc(uint16_t adr)
{
	if (bankprg == VMPRGBANK_SHADOW) {
		if ((adr < 0xde00)||(adr >=0xe300)) {
			return (int32_t)mem[adr];
		}
		return (int32_t)memprg[adr];
	}
	return (int32_t)mem[adr];
}


int32_t mucomvm::load(uint16_t adr)
{
	return (int32_t)mem[adr];
}


void mucomvm::store(uint16_t adr, uint8_t data)
{
	mem[adr] = data;
}


int32_t mucomvm::input(uint16_t adr)
{
	//printf("input : %x\n", adr);
	int port = adr & 0xff;
	switch (port)
	{
	case 0x32:
		return int3mask;
	case 0x45:
		return FMInData();
	case 0x47:
		return FMInData2();
	default:
		break;
	}
	return 0;
}


void mucomvm::output(uint16_t adr, uint8_t data)
{
	//printf("output: %x %x\n", adr, data);
	int port = adr & 0xff;
	switch (port)
	{
	case 0x32:
		int3mask = (int)data;
		break;
	case 0x44:
		sound_reg_select = (int)data;
		break;
	case 0x45:
		FMOutData((int)data);
		break;
	case 0x46:
		sound_reg_select2 = (int)data;
		break;
	case 0x47:
		FMOutData2((int)data);
		break;
	case 0x5c:
	case 0x5d:
	case 0x5e:
	case 0x5f:
		ChangeBank( port - 0x5c );
		break;
	default:
		break;
	}
}

void mucomvm::Halt(void)
{
	//if (m_flag == VMFLAG_EXEC) printf("Halt.\n");
	m_flag = VMFLAG_HALT;
}


void mucomvm::SendMemoryToShadow(void)
{
	//	�������̃V���h�[�R�s�[���쐬����
	//
	memcpy(memprg, mem, 0x10000);
}


/*------------------------------------------------------------*/
/*
status
*/
/*------------------------------------------------------------*/

void mucomvm::ClearBank(void)
{
	int i;
	uint8_t *p;

	for (i = 0; i < VMBANK_MAX; i++) {
		p = &vram[i][0];
		memset(p, 0, VMBANK_SIZE);		// CLEAR
	}
	bankmode = VMBANK_MAX-1;
}


void mucomvm::ChangeBank(int bank)
{
	uint8_t *p;
	if (bankmode == bank) return;
	//	���݂̃��������e��ޔ�
	p = &vram[bankmode][0];
	memcpy(p, mem+0xc000, VMBANK_SIZE);
	//	�����炵���o���N�f�[�^���R�s�[
	p = &vram[bank][0];
	memcpy(mem + 0xc000,p, VMBANK_SIZE);
	bankmode = bank;
}


void mucomvm::BackupMem(uint8_t *mem_bak)
{
	memcpy( mem_bak, mem, 0x10000 );
}

void mucomvm::RestoreMem(uint8_t *mem_bak)
{
	memcpy(mem, mem_bak, 0x10000);
}

int mucomvm::Peek(uint16_t adr)
{
	return (int)mem[adr];
}


int mucomvm::Peekw(uint16_t adr)
{
	int res;
	res = ((int)mem[adr+1])<<8;
	res += (int)mem[adr];
	return res;
}


void mucomvm::Poke(uint16_t adr, uint8_t data)
{
	mem[adr] = data;
}


void mucomvm::Pokew(uint16_t adr, uint16_t data)
{
	mem[adr] = data & 0xff;
	mem[adr+1] = (data>>8) & 0xff;
}


void mucomvm::PeekToStr(char *out, uint16_t adr, uint16_t length)
{
	int i;
	for (i = 0; i < 80; i++) {
		out[i] = Peek(adr + i);
	}
	out[i] = 0;
}


int mucomvm::ExecUntilHalt(int times)
{
	int cnt=0;
	int id = 0;
	msgid = 0;
	while (1) {
#ifdef DEBUGZ80_TRACE
		//membuf->Put( (int)pc );
		Msgf("%06x PC=%04x HL=%04x A=%02x\r\n", cnt, pc, GetHL(), GetA());
#endif
		//printf("PC=%04x HL=%04x (%d):\n", pc, GetHL(), Peek(0xa0f5));
		if (pc < 0x8000) {
			if (pc == 0x5550) {
				int i;
				uint8_t a1;
				uint16_t hl = GetHL();
				char stmp[256];
				i = 0;
				while (1) {
					if (i >= 250) break;
					a1 = mem[hl++];
					if (a1 == 0) break;
					stmp[i++] = (char)a1;
				}
				stmp[i++] = 0;
				Poke(pc, 0xc9);
				id = mucom_geterror(stmp);
				if (id > 0) {
					msgid = id;
					//Msgf("#%s\r\n", mucom_geterror(msgid),msgid);
				}
				else {
					Msgf("#Unknown message [%s].\r\n", stmp);
				}
			}
			else if (pc == 0x3b3) {
				Msgf("#Error trap at $%04x.\r\n", pc);
				//DumpBin(0, 0x100);
				return -1;
			}
			else {
				Msgf("#Unknown ROM called $%04x.\r\n", pc);
				//return -2;
			}
		}

		if (pc==0xaf80) {				// expand����CULC: ��u��������
			int amul = GetA();
			int val = Peekw(0xAF93);
			int frq = Peekw(0xAFFA);
			int ans,count;
			float facc;
			float frqbef = (float)frq;
			if (val == 0x0A1BB) {
				facc = 0.943874f;
			}
			else {
				facc = 1.059463f;
			}
			for (count = 0; count < amul; count++) {
				frqbef = frqbef * facc;
			}
			ans = int(frqbef);
			SetHL(ans);
			//Msgf("#CULC A=%d : %d * %f =%d.\r\n", amul, frq, facc, ans);

			pc = 0xafb3;				// ret�̈ʒu�܂Ŕ�΂�
		}

#if 0
		if (pc == 0x9000) {				// MWRITE(�R�}���h�������݃��C��)
			int mdata = Peekw(0x0f320);
			printf("#MWRITE MDATA$%04x.\r\n", mdata);
		}
#endif

#if 1
		if (pc == 0xde06) {				// CONVERT(���F��`�R���o�[�g)
										//	�o�̓f�[�^���傫���ꍇ�ADE06H�`�Ƃ��Ԃ�̂ő�փR�[�h�Ŏ��s����
										// $6001�` 38byte�̉��F�f�[�^��25byte�Ɉ��k����->$6001���珑������
										//
			MUCOM88_VOICEFORMAT *v = (MUCOM88_VOICEFORMAT *)(memprg + 0x6000);
			unsigned char *src = mem + 0x6001;
			v->ar_op1 = *src++;  v->dr_op1 = *src++; v->sr_op1 = *src++; v->rr_op1 = *src++; v->sl_op1 = *src++; v->tl_op1 = *src++; v->ks_op1 = *src++; v->ml_op1 = *src++; v->dt_op1 = *src++;
			v->ar_op2 = *src++;  v->dr_op2 = *src++; v->sr_op2 = *src++; v->rr_op2 = *src++; v->sl_op2 = *src++; v->tl_op2 = *src++; v->ks_op2 = *src++; v->ml_op2 = *src++; v->dt_op2 = *src++;
			v->ar_op3 = *src++;  v->dr_op3 = *src++; v->sr_op3 = *src++; v->rr_op3 = *src++; v->sl_op3 = *src++; v->tl_op3 = *src++; v->ks_op3 = *src++; v->ml_op3 = *src++; v->dt_op3 = *src++;
			v->ar_op4 = *src++;  v->dr_op4 = *src++; v->sr_op4 = *src++; v->rr_op4 = *src++; v->sl_op4 = *src++; v->tl_op4 = *src++; v->ks_op4 = *src++; v->ml_op4 = *src++; v->dt_op4 = *src++;
			v->fb = *src++; v->al = *src++;
			memcpy(mem + 0x6000, memprg + 0x6000, 26);

			SetHL(0x6001);
			pc = 0xafb3;				// ret�̈ʒu�܂Ŕ�΂�($c9�̃R�[�h�Ȃ牽�ł�����)
		}
#endif
#if 0
		if (pc == 0xde06) {				// CONVERT(���F��`�R���o�[�g)
			//	�o�̓f�[�^���傫���ꍇ�ADE06H�`�Ƃ��Ԃ�̂ŕʃo���N�Ŏ��s����
			bankprg = VMPRGBANK_SHADOW;
		}
		if (pc == 0xe132) {				// CONVERT(���F��`�R���o�[�g�I��)
			bankprg = VMPRGBANK_MAIN;
		}
#endif

#if 0
		if (pc==0x979c) {				// COMPST(�R���p�C�����C��)
			int ch = Peek(0x0f330);
			int line = Peekw(0x0f32b);
			int link = Peekw(0x0f354);
			int mdata = Peekw(0x0f320);
			printf("#CMPST $%04x LINKPT%04x CH%d LINE%04x MDATA%04x.\r\n", pc, link, ch, line,mdata);
		}
		if ((pc >= 0xAD86) && (pc < 0xb000)) {
				printf("#VOICECONV1 %04x.\r\n", pc);
		}
#endif

		Execute(times);
		if (m_flag == VMFLAG_HALT) break;
		cnt++;
#if 0
		if ( cnt>=0x100000) {
			Msgf( "#Force halted.\n" );
			break;
		}
#endif
	}
#ifdef DEBUGZ80_TRACE
	membuf->SaveFile("trace.txt");
#endif
	//Msgf( "#CPU halted.\n" );
	return 0;
}


void mucomvm::DumpBin(uint16_t adr, uint16_t length)
{
	int i;
	uint16_t p = adr;
	uint16_t ep = adr+length;
	while (1) {
		if (p >= ep) break;
		Msgf("#$%04x", p);
		for (i = 0; i < 16; i++) {
			Msgf(" %02x", Peek(p++));
		}
		Msgf("\r\n");
	}
}


void mucomvm::Msgf(const char *format, ...)
{
	char textbf[4096];
	va_list args;
	va_start(args, format);
	vsprintf(textbf, format, args);
	va_end(args);
	membuf->PutStr(textbf);
}


void mucomvm::SetPC(uint16_t adr)
{
	pc = adr;
	m_flag = VMFLAG_EXEC;
}


void mucomvm::CallAndHalt(uint16_t adr)
{
	uint16_t tempadr = 0xf000;
	uint8_t *p = mem + tempadr;
	*p++ = 0xcd;				// Call
	*p++ = (adr & 0xff);
	*p++ = ((adr>>8) & 0xff);
	*p++ = 0x76;				// Halt
	SetPC(tempadr);
	ExecUntilHalt();
}


int mucomvm::CallAndHalt2(uint16_t adr,uint8_t code)
{
	uint16_t tempadr = 0xf000;
	uint8_t *p = mem + tempadr;
	*p++ = 0x21;				// ld hl
	*p++ = 0x10;
	*p++ = 0xf0;
	*p++ = 0xcd;				// Call
	*p++ = (adr & 0xff);
	*p++ = ((adr >> 8) & 0xff);
	*p++ = 0x76;				// Halt
	p = mem + tempadr + 16;
	*p++ = code;
	*p++ = 0;
	*p++ = 0;

	SetPC(tempadr);
	return ExecUntilHalt(1);
}


int mucomvm::CallAndHaltWithA(uint16_t adr, uint8_t areg)
{
	uint16_t tempadr = 0xf000;
	uint8_t *p = mem + tempadr;
	*p++ = 0x3e;				// ld a
	*p++ = areg;
	*p++ = 0xcd;				// Call
	*p++ = (adr & 0xff);
	*p++ = ((adr >> 8) & 0xff);
	*p++ = 0x76;				// Halt
	SetPC(tempadr);
	ExecUntilHalt();
	return (int)GetIX();
}


int mucomvm::LoadMem(const char *fname, int adr, int size)
{
	//	VM�������Ƀt�@�C�������[�h
	//
	FILE *fp;
	int flen,sz;
	fp = fopen(fname, "rb");
	if (fp == NULL) return -1;
	sz = size; if (sz == 0) sz = 0x10000;
	flen = (int)fread(mem+adr, 1, sz, fp);
	fclose(fp);
	Msgf("#load:%04x-%04x : %s (%d)\r\n", adr, adr + flen, fname, flen);
	return flen;
}


char *mucomvm::LoadAlloc(const char *fname, int *sizeout)
{
	//	�������Ƀt�@�C�������[�h
	//
	FILE *fp;
	char *buf;
	int sz;

	*sizeout = 0;
	fp = fopen(fname, "rb");
	if (fp == NULL) return NULL;
	fseek(fp, 0, SEEK_END);
	sz = (int)ftell(fp);			// normal file size
	if (sz <= 0) return NULL;
	buf = (char *)malloc(sz+16);
	if (buf) {
		fseek(fp, 0, SEEK_SET);
		fread(buf, 1, sz, fp);
		fclose(fp);
		buf[sz] = 0;
		Msgf("#load:%s (%d)\r\n", fname, sz);
		*sizeout = sz;
		return buf;
	}
	fclose(fp);
	return NULL;
}


void mucomvm::LoadAllocFree(char *ptr)
{
	//	LoadAlloc�����[�h������������ԋp����
	//
	free(ptr);
}


int mucomvm::LoadPcmFromMem(const char *buf, int sz, int maxpcm)
{
	//	PCM�f�[�^��OPNA��RAM�Ƀ��[�h(����������)
	//
	char *pcmdat;
	char *pcmmem;
	int infosize;
	int i;
	int pcmtable;
	int inftable;
	int adr, whl, eadr;
	char pcmname[17];

	infosize = 0x400;
	inftable = 0xd000;
	SendMem((const unsigned char *)buf, inftable, infosize);
	pcmtable = 0xe300;
	for (i = 0; i < maxpcm; i++) {
		adr = Peekw(inftable + 28);
		whl = Peekw(inftable + 30);
		eadr = adr + (whl >> 2);
		if (buf[i * 32] != 0) {
			Pokew(pcmtable, adr);
			Pokew(pcmtable + 2, eadr);
			Pokew(pcmtable + 4, 0);
			Pokew(pcmtable + 6, Peekw(inftable + 26));
			memcpy(pcmname, buf + i * 32, 16);
			pcmname[16] = 0;
			Msgf("#PCM%d $%04x $%04x %s\r\n", i + 1, adr, eadr, pcmname);
		}
		pcmtable += 8;
		inftable += 32;
	}
	pcmdat = (char *)buf + infosize;
	pcmmem = (char *)opn->GetADPCMBuffer();
	memcpy(pcmmem, pcmdat, sz - infosize);

	if (m_option & VM_OPTION_SCCI) {
		osd->OutputRealChipAdpcm(pcmdat, sz - infosize);
	}

	return 0;
}


int mucomvm::LoadPcm(const char *fname,int maxpcm)
{
	//	PCM�f�[�^��OPNA��RAM�Ƀ��[�h
	//	(�ʓr�AMUCOM88��PCM�f�[�^���p�b�L���O�����f�[�^���K�v)
	//
	int sz;
	char *buf;
	buf = LoadAlloc( fname, &sz );
	if (buf) {
		LoadPcmFromMem( buf,sz,maxpcm );
		LoadAllocFree(buf);
	}
	return 0;
}


int mucomvm::SendMem(const unsigned char *src, int adr, int size)
{
	//	VM�������Ƀf�[�^��]��
	//
	memcpy(mem + adr, src, size);
	return 0;
}


int mucomvm::SaveMem(const char *fname, int adr, int size)
{
	//	VM�������̓��e���t�@�C���ɃZ�[�u
	//
	return SaveToFile( fname, mem+adr, size );
}


int mucomvm::SaveToFile(const char *fname, const unsigned char *src, int size)
{
	//	�o�C�i���t�@�C����ۑ�
	//
	FILE *fp;
	int flen;
	fp = fopen(fname, "wb");
	if (fp == NULL) return -1;
	flen = (int)fwrite(src, 1, size, fp);
	fclose(fp);
	return 0;
}


int mucomvm::SaveMemExpand(const char *fname, int adr, int size, char *header, int hedsize, char *footer, int footsize, char *pcm, int pcmsize)
{
	//	VM�������̓��e���t�@�C���ɃZ�[�u(�w�b�_�ƃt�b�^�t��)
	//
	FILE *fp;
	fp = fopen(fname, "wb");
	if (fp == NULL) return -1;
	if (header) fwrite(header, 1, hedsize, fp);
	fwrite(mem + adr, 1, size, fp);
	if (footer) fwrite(footer, 1, footsize, fp);
	if (pcm) fwrite(pcm, 1, pcmsize, fp);
	fclose(fp);
	return 0;
}


int mucomvm::KillFile(const char *fname)
{
	//		�t�@�C���̍폜
	//
	return osd->KillFile(fname);
}


int mucomvm::GetDirectory(char *buf, int size)
{
	//		�t�@�C���̍폜
	//
	return osd->GetDirectory(buf,size);
}


int mucomvm::ChangeDirectory(const char *dir)
{
	//		�t�@�C���̍폜
	//
	return osd->ChangeDirectory(dir);
}


void mucomvm::SkipPlay(int count)
{
	//		�Đ��̃X�L�b�v
	//
	if (count <= 0) return;

	int jcount = count;
	int vec = Peekw(0xf308);		// INT3 vector���擾

	SetChMuteAll(true);
	busyflag = true;				// Z80VM��2�d�N����h�~����
	while (1) {
		if (jcount <= 0) break;
		CallAndHalt(vec);
		jcount--;
		time_intcount++;
		if (m_option & VM_OPTION_SCCI) {
			if ((jcount & 3) == 0) osd->Delay(1);	// ������Ƒ҂��܂�
		}
	}
	busyflag = false;
	SetChMuteAll(false);
}


/*------------------------------------------------------------*/
/*
Timer
*/
/*------------------------------------------------------------*/

void mucomvm::ResetTimer(void)
{
	busyflag = false;
	predelay = 0;

	time_master = 0;
	time_scount = 0;
	time_intcount = 0;

	pass_tick = 0;
	last_tick = 0;

	osd->ResetTime();
}


void mucomvm::UpdateTime(int base)
{
	//		1ms���Ƃ̊��荞��
	//		base�l�́A1024=1ms�Ōo�ߎ��Ԃ�����
	//
	if (playflag == false) {
		return;
	}

	bool stream_event = false;
	bool int3_mode = int3flag;

	time_master += base;
	time_scount += base;

	if (tmflag) {
		return;
	}
	tmflag = true;

	if (int3mask & 128) int3_mode = false;		// ���荞�݃}�X�N

	if (opn->Count(base)) {
		if (int3_mode) {
			stream_event = true;
			if (predelay == 0) {
				int times = 1;
				if (m_option & VM_OPTION_FASTFW) times = m_fastfw;
				busyflag = true;				// Z80VM��2�d�N����h�~����
				while (1) {
					if (times <= 0) break;
					int vec = Peekw(0xf308);	// INT3 vector���Ăяo��
					CallAndHalt(vec);
					times--;
					time_intcount++;
				}
				busyflag = false;
				ProcessChData();
				NoticePlugins(MUCOM88IF_NOTICE_INTDONE, NULL, NULL);		// �v���O�C���ɒʒm����
			}
			else {
				predelay--;
			}
		}
	}

	if ((stream_event == false) && (int3_mode == false)) {
		// INT3�������ȏꍇ���X�g���[���Đ��͑�����
		if (time_scount > (10<< TICK_SHIFT)) {
			stream_event = true;
		}
	}

	if (stream_event) {
		stream_event = false;
		//pass_tick = ( opn->GetNextEvent() + 1023 ) >> TICK_SHIFT;
		pass_tick = time_scount >> TICK_SHIFT;
		time_scount = ( time_scount & ((int)TICK_FACTOR - 1) );
		osd->SendAudio(pass_tick);
	}
	tmflag = false;

}


void mucomvm::StartINT3(void)
{
	//		INT3���荞�݂��J�n
	//
	if (int3flag) {
		osd->WaitSendingAudio();
		checkThreadBusy();
	}
	osd->ResetTime();
	SetIntCount(0);
	predelay = 4;
	int3flag = true;
	tmflag = false;
}


void mucomvm::RestartINT3(void)
{
	//		INT3���荞�݂��ĊJ
	//
	if (int3flag) {
		return;
	}
	int3mask &= 0x7f;
	FMRegDataOut( 0x32, int3mask);
	osd->WaitSendingAudio();
	checkThreadBusy();
	osd->ResetTime();
	int3flag = true;
	tmflag = false;
}


void mucomvm::StopINT3(void)
{
	//		INT3���荞�݂��~
	//
	osd->WaitSendingAudio();
	int3flag = false;
	checkThreadBusy();
	//SetIntCount(0);
	predelay = 0;
}


void mucomvm::checkThreadBusy(void)
{
	//		�X���b�h��VM���g�p���Ă��邩�`�F�b�N
	//
	for (int i = 0; i < 300 && busyflag; i++) osd->Delay(10);
}


void mucomvm::PlayLoop() {
	while (1) {
		osd->Delay(20);
	}
}

void mucomvm::RenderAudio(void *mix, int size) {
	opn->Mix((FM::Sample *)mix, size);
}

void mucomvm::UpdateCallback(int tick) {
	if (m_option & VM_OPTION_STEP) return;
	UpdateTime(tick);
}


void mucomvm::AudioCallback(void *mix, int size) {
	if (m_option & VM_OPTION_FMMUTE || m_option & VM_OPTION_STEP) return;
	RenderAudio(mix, size);
}


/*------------------------------------------------------------*/
/*
FM synth
*/
/*------------------------------------------------------------*/

//	���W�X�^�f�[�^�o��
void mucomvm::FMRegDataOut(int reg, int data)
{
	regmap[reg] = (uint8_t)data;			// �������W�X�^�ێ��p
	opn->SetReg(reg, data);

	if (m_option & VM_OPTION_SCCI) {
		// ���A���`�b�v�o��
		osd->OutputRealChip(reg, data);
	}
}

//	���W�X�^�f�[�^�擾
int mucomvm::FMRegDataGet(int reg)
{
	return (int)regmap[reg];
}

void mucomvm::FMOutData(int data)
{
	//printf("FMReg: %04x = %02x\n", sound_reg_select, data);

	switch (sound_reg_select) {
	case 0x28:
	{
		//		FM KeyOn
		int ch = data & 7;
		int slot = data & 0xf0;
		if (chmute[ch]) return;
		if (slot) chstat[ch] = 1; else chstat[ch] = 0;
		break;
	}
	case 0x10:
	{
		//		Rhythm KeyOn
		if (chmute[OPNACH_RHYTHM]) return;
		if (data & 0x80) chstat[OPNACH_ADPCM] = 1; else chstat[OPNACH_ADPCM] = 0;
		break;
	}
	default:
		break;
	}

	FMRegDataOut(sound_reg_select, data);
}

//	�f�[�^�o��(OPNA��)
void mucomvm::FMOutData2(int data)
{
	//printf("FMReg2: %04x = %02x\n", sound_reg_select, data);

	if (sound_reg_select2 == 0x00) {
		if (chmute[OPNACH_ADPCM]) return;
		if (data & 0x80) chstat[OPNACH_ADPCM] = 1; else chstat[OPNACH_ADPCM] = 0;
	}

	FMRegDataOut(sound_reg_select2 | 0x100, data);
}

//	�f�[�^����
int mucomvm::FMInData(void)
{
	return (int)opn->GetReg(sound_reg_select);
}


//	�f�[�^����(OPNA��)
int mucomvm::FMInData2(void)
{
	return (int)opn->GetReg(sound_reg_select2|0x100);
}


void mucomvm::SetChMute(int ch, bool sw)
{
	chmute[ch] = (uint8_t)sw;
}


bool mucomvm::GetChMute(int ch)
{
	return (chmute[ch] != 0);
}


int mucomvm::GetChStatus(int ch)
{
	return (int)chstat[ch];
}


void mucomvm::SetChMuteAll(bool sw)
{
	int i;
	for (i = 0; i < OPNACH_MAX; i++) {
		SetChMute(i, sw);
	}
}


/*------------------------------------------------------------*/
/*
ADPCM
*/
/*------------------------------------------------------------*/

int mucomvm::ConvertWAVtoADPCMFile(const char *fname, const char *sname)
{
	Adpcm adpcm;
	DWORD dAdpcmSize;
	BYTE *dstbuffer;
	int sz, res;

	char *buf;
	buf = LoadAlloc(fname, &sz);
	if (buf == NULL) return -1;


	dstbuffer = adpcm.waveToAdpcm(buf, sz, dAdpcmSize, 16000 );
	LoadAllocFree(buf);

	if (dstbuffer == NULL) return -1;
	res = (int)dAdpcmSize;

	FILE *fp;
	fp = fopen(sname, "wb");
	if (fp != NULL) {
		fwrite(dstbuffer, 1, res, fp);
		fclose(fp);
	}
	else {
		res = -1;
	}
	delete[] dstbuffer;
	return res;
}


/*------------------------------------------------------------*/
/*
Channel Data
*/
/*------------------------------------------------------------*/

void mucomvm::InitChData(int chmax, int chsize)
{
	if (pchdata) {
		free(pchdata);
	}
	channel_max = chmax;
	channel_size = chsize;
	pchdata = (uint8_t *)malloc(channel_max*channel_size);
	memset(pchdata, 0, channel_max*channel_size);
}


void mucomvm::SetChDataAddress(int ch, int adr)
{
	if ((ch < 0) || (ch >= channel_max)) return;
	pchadr[ch] = (uint16_t)adr;
}


uint8_t *mucomvm::GetChData(int ch)
{
	uint8_t *p = pchdata;
	if ((ch < 0) || (ch >= channel_max)) return NULL;
	if (pchdata) {
		return p + (ch*channel_size);
	}
	return NULL;
}


uint8_t mucomvm::GetChWork(int index)
{
	if ((index < 0) || (index >= 16)) return 0;
	return pchwork[index];
}


void mucomvm::ProcessChData(void)
{
	int i;
	uint8_t *src;
	uint8_t *dst;
	uint8_t code;
	uint8_t keyon;
	if (pchdata==NULL) return;
	if (channel_max == 0) return;
	for (i = 0; i < channel_max; i++){
		src = mem + pchadr[i];
		dst = pchdata + (channel_size*i);
		if (src[0] > dst[0]) {						// lebgth���X�V���ꂽ
			keyon = 0;
			code = src[32];
			if (i == 10) {
				keyon = src[1];						// PCMch�͉��FNo.������
			}
			else if ( i == 6) {
				keyon = mem[ pchadr[10] + 38 ];		// rhythm�̃��[�N�l������
			}
			else {
				if (code != dst[32]) {
					keyon = ((code >> 4) * 12) + (code & 15);	// ���K�������L�[No.
				}

			}
			src[37] = keyon;						// keyon����t������
		}
		else {
			if (src[0] < src[18]) {				// quantize�؂�
				src[37] = 0;						// keyon����t������
			}
		}
		memcpy(dst, src , channel_size);
	}
	memcpy( pchwork, mem + pchadr[0] - 16, 16 );	// CHDATA�̑O�Ƀ��[�N������
}


/*------------------------------------------------------------*/
/*
plugin interface
*/
/*------------------------------------------------------------*/

int MUCOM88IF_VM_COMMAND(void *ifptr, int cmd, int prm1, int prm2, void *prm3, void *prm4);
int MUCOM88IF_EDITOR_COMMAND(void *ifptr, int cmd, int prm1, int prm2, void *prm3, void *prm4);

int mucomvm::AddPlugins(const char *filename, int bootopt)
{
	//		�v���O�C����ǉ�����
	//		filename = �v���O�C��DLL��
	//		bootopt = �N���I�v�V����(���g�p)
	//		�I���R�[�h : 0=OK
	//
#ifndef USE_SDL
	int res;
	Mucom88Plugin *plg = new Mucom88Plugin;
	plg->if_mucomvm = (MUCOM88IF_COMMAND)MUCOM88IF_VM_COMMAND;
	plg->if_editor = (MUCOM88IF_COMMAND)MUCOM88IF_EDITOR_COMMAND;
	plg->mucom = p_cmucom;
	plg->vm = this;
	plg->hwnd = master_window;
	strncpy( plg->filename, filename, MUCOM88IF_FILENAME_MAX-1 );
	plugins.push_back(plg);
	res = osd->InitPlugin(plg, filename, bootopt);
	if (res) return res;
	plg->if_notice(plg, MUCOM88IF_NOTICE_BOOT,NULL,NULL);				// ��������ʒm����
#endif
	return 0;
}


void mucomvm::FreePlugins(void)
{
	//		�v���O�C�������ׂĔj������
	//
// C++11�ł�OS X 10.6�p�r���h���ʂ�Ȃ��̂Łc�B
#ifndef USE_SDL
	Mucom88Plugin *plg;
	for (auto it = begin(plugins); it != end(plugins); ++it) {
		plg = *it;
		plg->if_notice(plg, MUCOM88IF_NOTICE_TERMINATE, NULL, NULL);	// �j������O�ɒʒm����
		osd->FreePlugin(plg);
	}
	plugins.clear();			// ���ׂč폜
#endif
}


void mucomvm::NoticePlugins(int cmd, void *p1, void *p2)
{
#ifndef USE_SDL
	Mucom88Plugin *plg;
	for (auto it = begin(plugins); it != end(plugins); ++it) {
		plg = *it;
		plg->if_notice(plg, cmd, p1, p2);
	}
#endif
}


