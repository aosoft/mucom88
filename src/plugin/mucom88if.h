
//
//	MUCOM88 plugin interface structures
//
#ifndef __CMucom88IF_h
#define __CMucom88IF_h

/*------------------------------------------------------------*/
//	for MUCOM88Win interface
/*------------------------------------------------------------*/

//		�t�@���N�V�����^
//
typedef int (*MUCOM88IF_COMMAND) (void *,int,int,int,void *,void *);
typedef int(*MUCOM88IF_CALLBACK)(void *, int);
typedef int(*MUCOM88IF_STARTUP)(void *);

#define MUCOM88IF_VERSION	0x100		// 1.0

#define MUCOM88IF_TYPE_NONE 0
#define MUCOM88IF_TYPE_SILENT 1			// �����I�ɓ��삷�����(�f�t�H���g)
#define MUCOM88IF_TYPE_TOOL 2			// �N��������^�C�v�̊O���c�[��

//	if_notice�Œʒm�����R�[�h
#define	MUCOM88IF_NOTICE_NONE 0
#define	MUCOM88IF_NOTICE_RESET 1		// VM���Z�b�g��(�R���p�C���A���t�J�n��)
#define MUCOM88IF_NOTICE_DRVINT 2		// VM�h���C�o���s��(���荞�݃^�C�~���O)
#define MUCOM88IF_NOTICE_TOOLSTART 3	// �v���O�C���c�[���N�����N�G�X�g
#define MUCOM88IF_NOTICE_PREPLAY 4		// ���t�J�n���O(MUB�ǂݍ��ݒ���)
#define MUCOM88IF_NOTICE_PLAY 5			// ���t�J�n
#define MUCOM88IF_NOTICE_STOP 6			// ���t��~
#define MUCOM88IF_NOTICE_MMLSEND 7		// �R���p�C��MML�m�莞
#define MUCOM88IF_NOTICE_COMPEND 8		// �R���p�C���I����
#define MUCOM88IF_NOTICE_LOADMUB 9		// MUB�ǂݍ��݌�

//	if_editor�Ŏg�p����R�}���h
#define	MUCOM88IF_EDITOR_CMD_NONE 0
#define	MUCOM88IF_EDITOR_CMD_PASTECLIP 1	// �G�f�B�^�ɃN���b�v�{�[�h�e�L�X�g��\��t��
#define	MUCOM88IF_EDITOR_CMD_GETTEXT 2		// �G�f�B�^�̃e�L�X�g���擾
#define	MUCOM88IF_EDITOR_CMD_UPDATETEXT 3	// �G�f�B�^�̃e�L�X�g���X�V
#define	MUCOM88IF_EDITOR_CMD_GETCURSOR 4	// �G�f�B�^�̃J�[�\���ʒu���擾
#define	MUCOM88IF_EDITOR_CMD_SETCURSOR 5	// �G�f�B�^�̃J�[�\���ʒu��ύX

//	if_mucomvm�Ŏg�p����R�}���h
#define	MUCOM88IF_MUCOMVM_CMD_NONE 0
#define	MUCOM88IF_MUCOMVM_CMD_FMWRITE 1		// FM���W�X�^�ɏ�������
#define	MUCOM88IF_MUCOMVM_CMD_FMREAD 2		// FM���W�X�^�̃e�[�u�����擾
#define	MUCOM88IF_MUCOMVM_CMD_GETCHDATA 2	// ch�̉��t�f�[�^���擾
#define	MUCOM88IF_MUCOMVM_CMD_CHDATA 3		// ch�̉��t�f�[�^���擾
#define	MUCOM88IF_MUCOMVM_CMD_TAGDATA 4		// TAG�f�[�^���擾


class mucomvm;
class CMucom;

#define MUCOM88IF_FILENAME_MAX 32

class Mucom88Plugin {
public:
	Mucom88Plugin();
	~Mucom88Plugin();

	//	���炩���ߐݒ肳�����
	//
	void *hwnd;							// ���C���E�B���h�E�n���h��
	int version;						// MUCOM88IF�o�[�W����
	char filename[MUCOM88IF_FILENAME_MAX];		// �v���O�C���t�@�C����
	void *instance;								// DLL�C���X�^���X

	//	Memory Data structure
	//	(*) = DLL���ŏ���������
	//
	int	type;							// �v���O�C���^�C�v(*)
	char *info;							// �v���O�C�����e�L�X�g�̃|�C���^(*)

	//	�R�[���o�b�N�t�@���N�V����
	MUCOM88IF_CALLBACK if_init;			// ������(�ŏ���1��̂�)
	MUCOM88IF_CALLBACK if_term;			// ���
	MUCOM88IF_CALLBACK if_notice;		// �R�}���h�ʒm

	//	�ėp�t�@���N�V����
	//
	MUCOM88IF_COMMAND if_mucomvm;	// MUCOM88 VM�̃A�N�Z�X
	MUCOM88IF_COMMAND if_editor;	// �G�f�B�^�n�̃T�[�r�X

	//	�N���X��� (�o�[�W�����œ��e�ύX�̉\��������܂�)
	mucomvm *vm;
	CMucom *mucom;

private:

};

#endif
