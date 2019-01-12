
//
//		MUCOM88 plugin interface
//			Windows version by onion software/onitama 2018/12
//

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "plugin.h"

int dummyCommand(void *instance, int cmd, int prm1, int prm2, void *prm3, void *prm4)
{
	return 0;
}

int dummyCallback(void *instance, int cmd)
{
	return 0;
}

Mucom88Plugin::Mucom88Plugin()
{
	hwnd = NULL;
	version = MUCOM88IF_VERSION;		// MUCOM88IF�o�[�W����
	filename[0] = 0;					// �v���O�C���t�@�C����
	instance = NULL;					// DLL�C���X�^���X

	type = MUCOM88IF_TYPE_NONE;			// �v���O�C���^�C�v(*)
	info = "";							// �v���O�C�����e�L�X�g�̃|�C���^(*)

	//	�R�[���o�b�N�t�@���N�V����
	if_init = dummyCallback;
	if_term = dummyCallback;
	if_notice = dummyCallback;

	//	�ėp�t�@���N�V����
	//
	if_mucomvm = dummyCommand;
	if_editor = dummyCommand;

	//	�N���X��� (�o�[�W�����œ��e�ύX�̉\��������܂�)
	vm = NULL;
	mucom = NULL;

}

Mucom88Plugin::~Mucom88Plugin()
{
}

