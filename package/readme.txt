------------------------------------------------------------------------------
MUCOM88 Windows document (Japanese Shift_JIS text)

OpenMucom88 Ver.1.7a Copyright 1987-2019(C) Yuzo Koshiro
Z80 emulation by Yasuo Kuwahara 2002-2018(C)
FM Sound Generator by cisc 1998, 2003(C)
Windows version by ONION software/onitama 2018-2019(C)
------------------------------------------------------------------------------

�E�͂��߂�

	MUCOM88 Windows�́ANEC�̃p�\�R��PC-8801�V���[�Y�œ��삵�Ă����A
	�Ñ�S�O���ɂ��FM�����̃c�[��/�h���C�o�[�ł���AMUCOM88��
	Windows��œ��삳���邽�߂̃V�X�e���ł��B
	�R�}���h���C���ŁAGUI�ł̗������p�ӂ���Ă��܂��B

		mucom88win.exe         Windows GUI�ŃG�f�B�^
		FmToneEditor.exe       Windows GUI�ŉ��F�G�f�B�^
		mucom88DatToTxt.exe    Windows GUI�ŉ��F�ϊ��c�[��
		mucom88.exe            Windows �R�}���h���C����

	MUCOM88 Windows���g�p���邱�Ƃɂ��A���y�L�q�ɓ�����������
	MML(Music Macro Language)�Ƃ��ċL�q���ꂽ���y���A
	PC-8801�̉����Ɠ��l�ɉ��t�����邱�Ƃ��ł��܂��B
	�܂��ASCCI(Sound Chip Common Interface)���o�R���邱�Ƃ�
	���ۂ�FM�����`�b�v(YM2608)�ɂ�鉉�t���T�|�[�g���Ă��܂��B

	MUCOM88 Windows�̍ŐV���y�сA�t�@�C���̃_�E�����[�h��
	�ȉ��̃T�C�g�ɂčs���Ă��܂��B

	MUCOM88 Windows
	https://onitama.tv/mucom88/

	�I���W�i����PC-8801��MUCOM88���A������ЃG�C���V�����g�l�̃T�C�g��
	�f�B�X�N�C���[�W�����J����Ă��܂��B

	OPEN MUCOM88 for PC-8801
	https://www.ancient.co.jp/~mucom88/


�E�����

	MUCOM88 Windows�́A�T�E���h�Đ����\��Windows XP�ȍ~�̃V�X�e�����
	���삵�܂��B
	Windows�̃T�E���h�Đ������ɂ́ADirectX8�ȍ~��DirectSound��
	�g�p���Ă��܂��BWindows XP�ȍ~�̊��ł́A�ǉ��̃R���|�[�l���g��
	�p�ӂ��邱�ƂȂ����삳���邱�Ƃ��ł��܂��B

	FM�����APSG�T�E���h�̃G�~�����[�V������32bit/55KHz�ō������A
	�o�͂���Ă��܂��B�Â��}�V���ł́A���ׂ��傫���ꍇ������܂��B

	MUCOM88 Windows�́A�I���W�i����MUCOM88�Ŏg�p����Ă����R�[�h(Z80)��
	���z�I�ɃG�~�����[�V�������AFM�����W�F�l���[�^�[(fmgen)�����
	���t���܂��B�I���W�i���Ɠ��l�̓���ŁAMML�̉��߁A���t�������邱�Ƃ�
	�\�ɂȂ��Ă��܂��B
	����ɍۂ��āAPC-8801��BIOS��ROM�A�f�B�X�N�C���[�W�͕K�v����܂���B

	�A�v���P�[�V�����̍X�V�����ɂ��ẮA��������Ă���uhistory.txt�v��
	�Q�Ƃ��Ă��������B


�E�t�@�C���`���̊T�v

	MUCOM88 Windows�ň������Ƃ̂ł���t�H�[�}�b�g�́A�ȉ��̒ʂ�ł��B

	�E.MUC�`��      PC-8801��œ��삷��MUCOM88(MUSIC LALF)�̏�����
	                �L�q���ꂽMML(�s�ԍ�������)SJIS�`���̃e�L�X�g�t�@�C��
	                �Ƃ��ĕۑ��������́B
	                FM����(YM2608)6���AADPCM1���ƃ��Y������6���APSG3����
			�R���g���[�����\�ł��B
	                MML�̏ڍׂ�MUCOM88�̃}�j���A�����Q�Ɖ������B

	�E.MUB�`��      MUC�`�����o�C�i���ɕϊ������t�@�C���`���B
	                ���t�f�[�^�y��FM���F�AADPCM�f�[�^�����܂��B

	�E.dat�`��      FM�����̃v���Z�b�g���F��ۑ����Ă���t�@�C���ł��B
	                voice.dat�t�@�C����W���I�ȃv���Z�b�g���F�Ƃ���
	                �g�p���܂��B����́APC-8801�ł�MUCOM88(MUSIC LALF)
	                �Ŏg�p����Ă����t�@�C���ƌ݊���������܂��B

	�E.bin�`��      ADPCM�����g�`�f�[�^��ۑ����Ă���t�@�C���ł��B
			mucompcm.bin�t�@�C����W���Ŏg�p���܂��B
			PC-8801�ł�MUCOM88(MUSIC LALF)�Ŏg�p����Ă���
			�f�[�^��ϊ����Ďg�p���邱�Ƃ��\�ł��B


�E�g�����̊T�v

	Windows GUI�x�[�X��MML�̕ҏW�Ɖ��t���\�ł��B
	�umucom88win.exe�v���N������ƁA�G�f�B�^�̉�ʂƂȂ�MML�̕ҏW��
	�ł���悤�ɂȂ�܂��B��{�I�ȑ���́A�ȉ��̃L�[���{�^���ōs���܂��B

		���j���[�X�N���[���Ăяo�� ([F1]�L�[)
		�ҏW����MML��ۑ� ([ctrl]+[S]�L�[)
		�ҏW����MML�����t ([F5]�܂���[F12]�L�[)
		���t�̒�~ ([ESC]�L�[)
		���t�̑�����([ctrl]+[F1]�L�[)
		���F�G�f�B�^�̋N��(V.EDIT�{�^��)

	MML�̕ҏW��ʂł́A

	A t190@30v15 cdefgab>c

	�Ȃǁu�`�����l��(A�`K)�v�u�X�y�[�X�v�uMML�L�q�v�Ƃ����g�ݍ��킹��
	�e�s�ɏ��������̂��A���̂܂܉��t����܂��B

	��{�I�ɂ́A���R��MML���L�q��[F5]�ŉ��t(MML�R���p�C��)�����Ȃ���
	�Ȃ��쐬����X�^�C���ɂȂ�܂��B��ʉ��ɂ��鐔�s�̃E�C���h�E�ɁA
	MML�R���p�C���̌��ʂ⃁�b�Z�[�W���\������܂��B
	MML�̋L�q�ɃG���[���������ꍇ���A�����ɕ\������܂��B

	MML�́A�e�L�X�g�t�@�C���Ƃ��ēǂݍ��݁E�ۑ����ł���̂�
	�C�y�ȋC�����œǂݏ������\�ł��B

	MML�L�q�ɂ��Ă̏ڍׂ́A�t���̃e�L�X�g�uMUCOM88_REF.txt�v��
	�Q�l�ɂ��Ă��������B�܂��A�T���v��MML(sampl1�`sampl3)���A���ۂ�
	�쐬�ɖ𗧂͂��ł��B

	MML�ҏW�ȊO�̑���́A[F1]�L�[�ŌĂяo����郁�j���[�X�N���[������
	�s���܂��B


�E�I�[�g�v���C���[

	�I�[�g�v���C���[�ɂ��A�����̊y�Ȃ������I�ɉ��t�����邱�Ƃ��\�ł��B
	���j���[�X�N���[����[Home]->[�v���C���[...]�{�^���������ƁA
	���̎��_�őI������Ă����t�@�C�����X�g�̋Ȃ����Ԃɉ��t���܂��B
	�I�[�g�v���C���[�́A2���Ԍo�߂��邩�A�܂���1.5���[�v�Ȃ��Đ������
	���̋ȂɈڍs���܂��B

		[ESC]     �A�v���P�[�V�����I��
		[Enter]   �Ȃ̍X�V

	��SCCI���[�h���쎞�́AMUCOM88 Windows�Ƃ̓����N�����ł��܂���B
	  MUCOM88 Windows���I����������ŁA�I�[�g�v���C���[�݂̂�
	�@���s���Ă�������(aplayer.exe)


�E���j���[�X�N���[��

	[F1]�L�[�Ő؂�ւ�郁�j���[�X�N���[���ł́A��ʍ��ɕ��񂾃{�^���ɂ��
	�@�\��I�Ԃ��Ƃ��ł��܂��B
	���ꂼ��̃��j���[�ڍׂ́A�ȉ��̒ʂ�ł��B

	��<<

	���j���[�X�N���[�����I�����āAMML�ҏW��ʂɖ߂�܂��B
	[F1]�L�[�ł��߂�܂��B

	��Home

	���j���[�X�N���[���ōŏ��ɕ\�������z�[����ʂɂȂ�܂��B
	�t�@�C���u���E�U�ɂ��AMUC�t�@�C����I�����邱�Ƃ��ł��܂��B
	�J�[�\���L�[�Ńt�@�C����I���A[Enter]�L�[�ŕҏW��ʂɓǂݍ��݂܂��B
	�u�t�H���_���Q�Ɓv�{�^���ɂ��ʂȃt�H���_���Q�Ƃ��邱�Ƃ��ł��܂��B
	�܂��A�t�@�C���u���E�U�ォ�璼��MUC�t�@�C�����Đ�������A�o�C�i��
	�f�[�^(MUB�t�@�C��)���쐬���邱�Ƃ��ł��܂��B
	�u�v���C���[�v�{�^���ɂ��I�[�g�v���C���[���N�����܂��B

	��Load

	�t�@�C���_�C�A���O����MUC�t�@�C����I�����āA�ҏW���܂��B

	��Save

	�ҏW����MML��MUC�t�@�C���ɏ㏑���ۑ����܂��B

	��Save as...

	�ҏW����MML�ɐV�������O��t����MUC�t�@�C���ɕۑ����܂��B

	��New

	�ҏW���̓��e���������āA�V�K��MML�t�@�C���ҏW���J�n���܂��B

	��About

	MUCOM88 Windows�̃o�[�W��������\�����܂��B

	��Web

	MUCOM88 Windows��web�y�[�W���J���܂��B

	��Share

	�ҏW����MML��SNS���ŋ��L���܂��B
	(���̋@�\�͌��݊J�����̂��ߎg�p�ł��܂���B)

	��Tool

	MML��f�[�^�Ɋւ���c�[�������s���܂��B

	�ҏW����MML�𐮌`���܂�

		PC-8801�̃t�@�C����ϊ�����ۂɎg�p���܂�

	PCM�f�[�^�t�@�C�����쐬���܂�

		PC-8801�̃t�@�C����ϊ�������Awav�`����
		�g�`���g����PCM�f�[�^�t�@�C�����쐬���܂��B
		�ڂ����́A�uADPCM�f�[�^�t�@�C���ɂ��āv�̍��ڂ�
		�Q�Ƃ��Ă��������B

	�ҏW����MML��N88�G�N�X�|�[�g

		�ҏW����MML�ɍs�ԍ���t�����āA�Ă�PC-8801��
		�����čs�����Ƃ̂ł���t�@�C���𐶐����܂��B


	��Option

	MUCOM88 Windows�̐ݒ��ύX���܂��B

	���[�U�[��          : �^�O�ݒ莞�Ɏg�p��������̃��[�U�[��
	�W�����F�t�@�C��    : �^�O�ݒ莞�Ɏg�p��������̉��F�t�@�C��
	�W��ADPCM�t�@�C��   : �^�O�ݒ莞�Ɏg�p���������ADPCM�t�@�C��

	�E�C���h�E�T�C�Y    : �A�v���P�[�V�����̃E�C���h�E�T�C�Y��ݒ肵�܂�
	�t�H���g�ݒ�        : �t�H���g��ύX���܂�
	�����F�̕ύX        : �G�f�B�^�����F���_�C�A���O�Őݒ肵�܂�
	�w�i�F�̕ύX        : �G�f�B�^�w�i�F���_�C�A���O�Őݒ肵�܂�

	������Đ����x      : ������(ctrl+F1)���̑��x��ύX���܂�

	SCCI���g�p          : SCCI�ɂ����`�b�v���t��L���ɂ��܂�
	                      (���̐ݒ�ύX�̓c�[���ċN����ɗL���ɂȂ�܂�)
	SCCI�ݒ�            : SCCI�ݒ�c�[��(scciconfig.exe)���N�����܂�


�E�ꎞ�t�@�C���ɂ���

	mucom88win.exe�ł́AMML���t(�R���p�C��)���Ɉꎞ�t�@�C����
	MUC�t�@�C��������t�H���_�Ɠ����ꏊ�ɍ쐬���܂��B

		mucommml      �ҏW����MML���o�͂����ꎞ�t�@�C��
		mucombin      �ҏW����MML���o�C�i���o�͂����ꎞ�t�@�C��

	�����̈ꎞ�t�@�C���́A�ʏ펞�͕K�v����܂���̂�
	�폜���Ă���肠��܂���B�����A�G�f�B�^���G���[�Ȃǂŋ����I��
	����āA�t�@�C���̕ҏW���ł��Ȃ������ۂɂ́A�Ō�ɉ��t����
	�f�[�^��mucommml���畜�����邱�Ƃ��ł��܂��B

	�܂��A�G�f�B�^�̐ݒ�́A�ȉ��̃t�H���_�ɕۑ�����Ă��܂��B

	/User/���[�U�[��/AppData/Roaming/MUCOM88/mucom88win.ini

	�A���C���X�g�[������A�ݒ������������ۂ́A�Y���t�H���_��
	�폜���Ă��������B


�E���F�f�[�^�t�@�C���ɂ���

	MUCOM88 Windows�ɂ́A���F�f�[�^�t�@�C���Ƃ��āA�uvoice.dat�v��
	�t�����Ă��܂��B�uvoice.dat�v�ł́APC-8801�ł�MUCOM88�Ɠ��l��
	FM���F���w�肷�邱�Ƃ��ł��܂��B

	PC-8801�Ŏg�p���Ă���FM���F�t�@�C����Windows��Ɏ����Ă��邱�Ƃ�
	�uvoice.dat�v�ȊO�̉��F���g�p���邱�Ƃ��\�ł��B

	�W���I�Ɏg�p�����FM���F�t�@�C���́Amucom88win�̃��j���[(F1)����
	�I�v�V����(Option)->�u�W�����F�t�@�C���v�Ŏw�肳��Ă��鍀�ڂ�
	�C�����邱�ƂŕύX���\�ł��B

	�܂��AMML�̒��ɁA�^�O�Ƃ��āu#voice�v�Ŏn�܂�s��ǉ����邱�ƂŁA
	�����I��FM���F�t�@�C�����w�肷�邱�Ƃ��ł��܂��B

		#voice voice.dat

	��̗�ł́AMML���uvoice.dat�v��FM���F�f�[�^�t�@�C�����g�p���邱�Ƃ�
	�����Ă��܂��B����ɂ��A���t���Ɏ����I�ɊY������FM���F�f�[�^��
	�ǂݍ��܂�܂��B

	���[�U�[���Ǝ��ɍ쐬����FM���F�f�[�^�t�@�C���́AMML�f�[�^�t�@�C����
	�����t�H���_�ɔz�u����悤�ɂ��Ă��������B


�E���F�G�f�B�^�ɂ���

	���F�f�[�^�t�@�C����FM�����̃p�����[�^�[���m�F���Ȃ��特�F��
	�ҏW���邱�Ƃ��ł���A���F�G�f�B�^���t������Ă��܂��B
	MML�ҏW��ʂ́uV.EDIT�v�{�^�����������ƂŁA���F�G�f�B�^���N�����܂��B
	���F�G�f�B�^�̃E�C���h�E�ɁAvoice.dat�Ȃǂ̉��F�f�[�^�t�@�C����
	�h���b�O&�h���b�v���邱�ƂŁA�t�@�C���ɕۑ����ꂽ���F���m�F����
	���Ƃ��ł��܂��B
	�܂��A�ҏW�������F�́A�N���b�v�{�[�h���o�R���āAMUCOM88�̃G�f�B�^��
	�����Ă��邱�Ƃ��ł��܂��B

	���F�G�f�B�^��ctrl+C�ɂ��N���b�v�{�[�h�ɃR�s�[������A
	MUCOM88 Windows��MML�G�f�B�^��ʂŁActrl+V�������ăy�[�X�g�����
	�ȉ��̂悤�ȉ��F��`�e�L�X�g���\��t�����܂��B

	  @0:{
	   7,  0
	  31, 18,  0,  6,  2, 36,  0, 10,  3
	  31, 14,  4,  6,  2, 45,  0,  0,  3
	  31, 10,  4,  6,  2, 18,  1,  0,  3
	  31, 10,  3,  6,  2,  0,  1,  0,  3,""}

	���F�G�f�B�^�̏ڂ����g�p���@�́A�h�L�������g�uFmToneEditoe.txt�v��
	���ǂ݂��������B


�EADPCM�f�[�^�t�@�C���ɂ���

	MUCOM88 Windows�ł́AADPCM�����p�̃T���v�����O�f�[�^���A
	�W���Łumucompcm.bin�v�Ƃ����t�@�C���ɏW�񂵂ĊǗ����Ă��܂��B
	�umucompcm.bin�v�ɂ́A�W���I��ADPCM�̃T���v�����܂܂�Ă��܂��B

	        No. �T�E���h
	    -----------------------------------------------
		@0  kick(�o�X�h����)
		@1  snare(�X�l�A�h����)
		@2  kick+snare(�o�X+�X�l�A�h����)
		@3  crashcymbal(�N���b�V���V���o��)
		@4  kick+cymbal(�o�X�h����+�V���o��)
		@5  ac.tam(�A�R�[�X�e�B�b�N�E�^��)
		@6  e.tam(�G���N�g���b�N�E�^��)
		@7  snare2(�X�l�A�h����2)
		@8  o.hit(�I�[�P�X�g���q�b�g)
		@9  909kick(909�o�X�h����)
		@10 909snare(909�X�l�A�h����)
		@11 808openhihat(808�I�[�v���n�C�n�b�g)
		@12 timbal(�e�B���o���X)
		@13 hand clap(�n���h�N���b�v)
		@14 timpani(�e�B���p�j�[)

	���[�U�[���Ǝ���ADPCM(�T���v�����O)�f�[�^��ϊ�����ADPCM�f�[�^�t�@�C��
	�Ƃ��Ďg�p���邱�Ƃ��\�ł��B
	���炩���߁A�e�L�X�g�t�@�C���ɂ܂Ƃ߂�ΏۂƂȂ�t�@�C�������L�q
	�������̂�ۑ����Ă����܂��B���Ƃ��΁A�upcmlist.txt�v�Ƃ����t�@�C����

		voice1.wav
		voice2.wav
		voice3.wav

	�Ƃ���3�s���L�q���āAmucom88win�̃��j���[(F1)����c�[��(Tool)->
	�uPCM�f�[�^�t�@�C�����쐬���܂��v���ɂ���u�t�@�C�����X�g���w��v
	�{�^������A�upcmlist.txt�v���w�肷�邱�ƂŁA�����I��ADPCM�f�[�^
	�t�@�C�����쐬����܂��B
	�쐬�����t�@�C�����́A�f�t�H���g�Łumypcm.bin�v�ƂȂ��Ă��܂����A
	�_�C�A���O�ŕύX���邱�Ƃ��\�ł��B

	�t�@�C�����X�g���L�q�����e�L�X�g�t�@�C���Ɠ����t�H���_�ɁA���ۂ�
	ADPCM�t�@�C�����ۑ�����Ă���K�v������܂��B
	�����Ŏg�p����WAV�t�@�C���́A16bit���m������PCM�f�[�^�ł���
	�K�v������܂��B�T���v�����O���g���́AADPCM�ϊ�����16KHz�ɕ␳����܂��B
	(WAV�`���ȊO�̊g���q�����t�@�C�������X�g�ɂ���ꍇ�́AADPCM�`����
	�o�C�i���t�@�C���Ƃ��ĔF������܂��̂ŁAPC-8801�ō쐬���ꂽADPCM��
	�o�C�i���t�@�C�����w�肷�邱�Ƃ��\�ł��B)

	���[�U�[��MUCOM88(MUSIC LALF)�œƎ��Ɏg�p���Ă���ADPCM(�T���v�����O)
	�f�[�^���ۂ��ƕϊ����Ďg�p���邱�Ƃ��\�ł��B
	�܂��APC-8801��Ŏg�p���Ă����f�B�X�N��̃t�@�C�������ׂ�Windows���
	�����Ă���K�v������܂��Bn88 DISK BASIC�`���̃f�B�X�N(D88�`��)����
	�t�@�C�������o�����߂̃c�[���Ƃ��āA�`�j�`�s�s�`�����쐬����d88edj��
	�g�p���Č��؂��Ă��܂��B

	d88edj�́A�ȉ��̃T�C�g�ɂĔz�z����Ă��܂��B

	d88edj   -D88 Editor for Java-
	http://www.cug.net/~akatta/

	MUCOM88�ł́AADPCM�̃f�[�^�Ƃ��āuDATA�v�Ƃ����o�C�i���t�@�C���A
	�uVOICE._1�v�uVOICE._2�v�Ȃǂ̃f�[�^�t�@�C�����g�p���Ă��܂��B
	������1�̃t�H���_�ɂ��ׂēW�J���āAmucom88win�̃��j���[(F1)����
	�c�[��(Tool)->�uPCM�f�[�^�t�@�C�����쐬���܂��v���ɂ���uDATA�t�@�C���t�H���_���w��v�{�^����
	�����ăt�H���_���w�肷�邱�ƂŁAPCM�f�[�^�t�@�C�����쐬����܂��B

	�W���I�Ɏg�p�����PCM�f�[�^�t�@�C���́Amucom88win�̃��j���[(F1)����
	�I�v�V����(Option)->�u�W��ADPCM�t�@�C���v�Ŏw�肳��Ă��鍀�ڂ�
	�C�����邱�ƂŕύX���\�ł��B

	�܂��AMML�̒��ɁA�^�O�Ƃ��āu#pcm�v�Ŏn�܂�s��ǉ����邱�ƂŁA
	�����I��ADPCM�f�[�^�t�@�C�����w�肷�邱�Ƃ��ł��܂��B

		#pcm mucompcm.bin

	��̗�ł́AMML���umucompcm.bin�v��ADPCM�f�[�^�t�@�C�����g�p���邱�Ƃ�
	�����Ă��܂��B����ɂ��A���t���Ɏ����I�ɊY������PCM�f�[�^�t�@�C����
	�ǂݍ��܂�܂��B�܂��A�o�C�i���f�[�^�t�@�C��(.mub)���o�͂����ۂɁA
	ADPCM�f�[�^��������`�ŏo�͂���܂��B

	���[�U�[���Ǝ��ɍ쐬����ADPCM�f�[�^�t�@�C���́AMML�f�[�^�t�@�C����
	�����t�H���_�ɔz�u����悤�ɂ��Ă��������B


�E���Y�������ɂ���

	MUCOM88 Windows�Ɠ����t�H���_�ɁA�ȉ��̃t�@�C�����������ꍇ�́A
	���Y�������p�̃T���v���t�@�C���Ƃ��Ďg�p����܂��B
	���Y�����������g�p����ꍇ�́A�ʓr�t�@�C�������p�Ӓ����K�v������܂��B

	2608_BD.WAV
	2608_HH.WAV
	2608_RIM.WAV
	2608_SD.WAV
	2608_TOM.WAV
	2608_TOP.WAV

	���Y�������p�̃T���v���t�@�C���́A���ۂ̃{�[�h����T���v�����O����
	�쐬����Ȃǂ̑��ɁA�T�C�g����_�E�����[�h���邱�Ƃ��\�ł��B
	https://sites.google.com/site/ym2608rhythm/


�E���`�b�v�̎g�p�ɂ���

	MUCOM88 Windows�ł́ASCCI(Sound Chip Common Interface)���o�R����
	���ۂ�FM�����`�b�v(YM2608)�ɂ�鉉�t���T�|�[�g���Ă��܂��B
	��������Ă���SCCI�̃V�X�e��DLL�́A�ȉ��̃n�[�h�E�F�A�Ɏ��`�b�v
	(YM2608)�𓋍ڂ������̂ɑΉ����Ă��܂��B

	�ESPFM FM�̓�
	�ESPFM Light
	�ERE:Birth 
	�EG.I.M.I.C
	�EC86BOX

	���`�b�v�̎g�p�ɂ��ẮA�ŐV��SCCI�֘A�����Q�Ƃ��������B

	SCCI(Sound Chip Common Interface)
	http://www.pyonpyon.jp/~gasshi/fm/scci.html

	MUCOM88 Windows����SCCI���g�p����ۂ́A���j���[���I�v�V������
	�uSCCI�g�p�v�̃`�F�b�N��ON�ɂ��āA�c�[�����ċN�����Ă��������B
	(SCCI�I�v�V�����́A�c�[�����ēx�N�������ۂɗL���ɂȂ�܂�)

	���ۂɂɎg�p����ۂɂ́APC�Ƀn�[�h�E�F�A��ڑ�������ԂŁA�ŏ���
	SCCI�̐ݒ���s���Ă����K�v������܂��B
	���j���[���I�v�V�����uSCCI�ݒ�v�{�^������ASCCI�̐ݒ胁�j���[
	���N�����邱�Ƃ��ł��܂��B(scciconfig.exe������N���ł��܂�)

	SCCI�g�p���́AWindows��ł�FM�����Đ��͍s�킸�A�O���̃`�b�v�݂̂�
	�Đ����s���܂��B


�E�R�}���h���C���łɂ���

	�R�}���h���C������umucom88.exe�v���Ăяo���Ďg�p���邱�Ƃ��\�ł��B
	�ȉ��̏����ŃI�v�V�������w�肷�邱�Ƃ��ł��܂��B

	mucom88 [options] [filename]

	�I�v�V����

	       -p [filename] 	�ǂݍ��܂��PCM�t�@�C�����w�肷��
	       -v [filename]	�ǂݍ��܂�鉹�F�t�@�C�����w�肷��
	       -o [filename]	�o�͂����o�C�i���t�@�C�������w�肷��
	       -c [filename]	MML�t�@�C�����o�C�i���t�@�C���ɕϊ�����
	       -i [filename]	MML�t�@�C���̊T�v���o�͂���
	       -e           	MUCOM88�V�X�e���t�@�C�����O������ǂݍ���
	       -s           	SCCI���o�R���Ď��`�b�v�ł̉��t���s��
	       -k           	PCM�t�@�C���̓ǂݍ��݂��X�L�b�v����


�EHSP�v���O�C���ɂ���

	��������Ă���Ahspmucom.dll�̓X�N���v�g����HSP(HotSoupProcessor)����
	�Ăяo���Ďg�p���邱�Ƃ��\�ȃv���O�C���ɂȂ��Ă��܂��B
	MUCOM88 Windows�A�v���P�[�V�������AHSP�ɂ��L�q����Ă��܂��B
	hsplugin�t�H���_�ɃX�N���v�g�̃\�[�X�y�уv���O�C���h�L�������g��
	�܂܂�Ă��܂��B
	HSP�ɂ��Ă̏ڍׂ́A�ȉ����Q�Ƃ��Ă��������B

	Hot Soup Processor����HP
	https://hsp.tv/


�EOPEN MUCOM88�v���W�F�N�g�ɂ���

	MUCOM88 Windows�́AOPEN MUCOM88�v���W�F�N�g�̈ꕔ�Ƃ��Č��J����Ă��܂��B

	MUCOM88�́A���Ƃ���1987�N�E�Ñ�S�O���ɂ���ĊJ���E���\���ꂽ
	NEC PC-8801�v���b�g�t�H�[���p��MML�`���ɂ�鉹�y�쐬�c�[���A�y��
	�Đ��p�̃v���O����(�h���C�o�[)���ł��B

	OPEN MUCOM88�v���W�F�N�g�́A�I���W�i����MUCOM88�\�[�X�R�[�h�����J����
	���ƂŁA���L�����p�E�p�����邱�Ƃ�ړI�Ƃ��Ă��܂��B
	�����Ō��J���ꂽ�\�[�X�R�[�h�⎑�Y�Ȃǂ́A�I�[�v���ȃ��C�Z���X�ɂ��
	���R�Ɋ��p���邱�Ƃ��\�ł��B
	���C�Z���X�̏ڍׂ́A�u���C�Z���X����јA����v���ڋy�сulicense.txt�v
	�ɂ܂Ƃ߂��Ă��܂��B


�E���C�Z���X����јA����

	MUCOM88 Windows�́A�ȉ��̃T�C�g�ɂ�1���z�z����Ă��܂��B

	MUCOM88 Windows
	https://onitama.tv/mucom88/

	OPEN MUCOM88 Windows github repository
	https://github.com/onitama/mucom88


	MUCOM88 Windows�́A�ȉ��̃��C�u�����y�у\�[�X�R�[�h�E�f�[�^�ɂ��쐬����Ă��܂��B

	�EPortable Z80 emulation��� Yasuo Kuwahara��
	  http://www.geocities.jp/kwhr0/
	�Efmgen��� cisc��
	  http://www.retropc.net/cisc/
	�EMUCOM88��� �Ñ�S�O��
	  https://twitter.com/yuzokoshiro
	�EHSPMUCOM��� ���ɂ���(ONION software)
	  https://www.onionsoft.net/
	�Eadpcm converter/SCCI/FmToneEditor/mucom88DatToTxt���
	  �����R(gasshi)��
	  http://www.pyonpyon.jp/~gasshi/fm/

	�쐬�ɂ�����A�����͒������a�c��(�G�C���V�����g)�l�A
	WING���l�AOXYGEN�l�A�����R(gasshi)�l�A@MUCOM88�l�AUME-3�l������
	�I���W�i����PC-8801�ł��쐬�����Ñ�S�O�l�A�{���ɂ��肪�Ƃ��������܂����B

	MUCOM88 Windows�y�у\�[�X�R�[�h�́A�N���G�C�e�B�u�R�����Y�ŋK�肳�ꂽ
	CC BY-NC-SA 4.0���C�Z���X�Ō��J����Ă��܂��B
	https://creativecommons.org/licenses/by-nc-sa/4.0/deed.ja

	����(��c��)�ł������͎��R�ɏЉ�A�����A�Ĕz�z���\�ł��B
	���̍ۂɂ͕K���h�L�������g�ƃ��C�Z���X�\�L(license.txt)���܂߂�悤��
	���Ă��������B

	�T���v���y��(sampl1.muc�Asampl2.muc�Asampl3.muc)�y�ѕt������f�[�^
	(mucompcm.bin�Avoice.dat)�͊�����ЃG�C���V�����g�̌Ñ�S�O���ɂ��
	�񋟂���Ă��܂��B
	https://www.ancient.co.jp/yuzo.html

	�Ñ�S�O���̃T���v���y�Ȃɂ��ẮA���쌠�̃��C�Z���X��K����������
	�悤���肢�v���܂��B

	���C�Z���X�\�L�̗�:
	�u�y�Ȗ�(�܂��̓t�@�C����) / Copyright(C) by Yuzo Koshiro�v


	MUCOM88 Windows�́A���ɂ���(onion software)�����S�ƂȂ�쐬����Ă��܂��B

	ONION software Homepage
	https://www.onionsoft.net/

	���[�U�[��MUCOM88���g���č쐬�����I���W�i���̊y�ȁAMML�t�@�C���̌�����
	������쐬�������[�U�[�ɑ����܂��B
	onion software�͖{�v���O�����ɂ���Đ������A�����Ȃ鑹�Q�ɂ��Ă�
	�ۏ؂������܂���B���Ȃ̐ӔC�͈̔͂Ŏg�p���Ă��������B
	�܂��A�t����HSP�X�N���v�g�����R�ɉ��ρA���J���Ă��������č\���܂���B


	fmgen�\�[�X�R�[�h�Ɋւ���z�z�K��́A��҂ł���cisc���̃��C�Z���X��
	�]���Ă��������Bfmgen�\�[�X�R�[�h�̔z�z�K��͈ȉ��̒ʂ�ł��B

	�Efmgen�\�[�X�R�[�h�͍��(cisc@retropc.net) �����쌠�����L���Ă��܂��B

	�E�{�\�[�X�R�[�h�͂��邪�܂܂ɒ񋟂������̂ł���C
	  �Öًy�і����I�ȕۏ؂���؊܂݂܂���D

	�E�{�\�[�X�R�[�h�𗘗p�������ƁC���p���Ȃ��������ƁC
	  ���p�ł��Ȃ��������ƂɊւ��Đ��������邢�͐�����Ɨ\�������
	  ���Q�ɂ��āC��҂͈�ؐӔC�𕉂��܂���D

	�E�{�\�[�X�R�[�h�́C�ȉ��̐����𖞂������莩�R�ɉ��ρE�g�ݍ��݁E
	  �z�z�E���p���邱�Ƃ��ł��܂��D

	  1. �{�\�t�g�̗R��(���, ���쌠)�𖾋L���邱��.
	  2. �z�z����ۂɂ̓t���[�\�t�g�Ƃ��邱�ƁD
	  3. ���ς����\�[�X�R�[�h��z�z����ۂ͉��ϓ��e�𖾎����邱��.
	  4. �\�[�X�R�[�h��z�z����ۂɂ͂��̃e�L�X�g����؉��ς�����
	     ���̂܂ܓY�t���邱�ƁD

	�E���J�̍ۂɍ�҂ւ̘A���𒸂���΍K���ł��D

	�E���p�\�t�g(�V�F�A�E�F�A�܂�) �ɖ{�\�[�X�R�[�h�̈ꕔ�C�܂���
	  �S����g�ݍ��ލۂɂ́C���O�ɍ�҂̍��ӂ𓾂�K�v������܂��D

	�e���C�u�����ɂ��Ă̏ڍׂ́A���ꂼ��̃\�[�X�R�[�h�y�уh�L�������g��
	�Q�Ƃ��������B
	�L���E���p�ł̔z�z�ɂ��ẮA�ʓr��҂܂ł��₢���킹���������B


-------------------------------------------------------------------------------
                                                HSP users manual / end of file 
-------------------------------------------------------------------------------
