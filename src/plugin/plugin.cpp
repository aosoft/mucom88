
//
//		MUCOM88 plugin interface
//			Windows version by onion software/onitama 2018/12
//

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "plugin.h"

int dummyCallback(void *instance, int cmd, void *p1, void *p2)
{
	Mucom88Plugin *plg = (Mucom88Plugin *)instance;
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
	if_notice = dummyCallback;

	//	�ėp�t�@���N�V����
	//
	if_mucomvm = NULL;
	if_editor = NULL;

	//	�N���X��� (�o�[�W�����œ��e�ύX�̉\��������܂�)
	vm = NULL;
	mucom = NULL;

}

Mucom88Plugin::~Mucom88Plugin()
{
}

