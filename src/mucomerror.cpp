
//
//	MUCOM88 debug support
//	(�G���[����)
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mucomerror.h"

/*------------------------------------------------------------*/
/*
		system data
*/
/*------------------------------------------------------------*/


/*------------------------------------------------------------*/
/*
		interface
*/
/*------------------------------------------------------------*/

static const char *orgmsg[]={
	"",
	"����߳ � ���� �� ��Ͻ",
	"���Ұ�� ��� �� ��ޮ��޽",
	" ]  � ��� �� �����Ͻ",
	" [  � ��� �� �����Ͻ",
	"�ݼ�� � ��� �� �����Ͻ",
	"������ �� �ò�ݲ � ���Ͻ",
	"ؽ�� �� �ۯ� � �� ���Ͻ",
	"[ ] Ų � / � ����޹�޽",
	"���Ұ� �� ��Ͼ�",
	"������� ��� ¶�Ų ����޶ޱ�Ͻ",
	"[ ] � Ƚ�� 16�� ����޽",
	"�ݼ�� �ް� �� ײ���� � �ݻ޲�Ͼ�",
	"����� ���ް�۰",
	"Ӱ�� �װ",
	"��޼ު�� خ��� � ��ϼ�",
	"ò�޼�Ų ϸ����ް�ޱ�Ͻ",
	"ϸ۴��޺��� �� ��Ͼ�",
};

static const char *err[]={
	"",												// 0
	"Syntax error",									// 1 '����߳ � ���� �� ��Ͻ',0
	"Illegal parameter",							// 2 '���Ұ�� ��� �� ��ޮ��޽',0
	"Too many loop end ']'",						// 3 ' ]  � ��� �� �����Ͻ',0
	"Too many loops '['",							// 4 ' [  � ��� �� �����Ͻ',0
	"Too many FM voice '@'",						// 5 '�ݼ�� � ��� �� �����Ͻ',0
	"Octave out of range",							// 6 '������ �� �ò�ݲ � ���Ͻ',0
	"Rhythm too fast, Clock over",					// 7 'ؽ�� �� �ۯ� � �� ���Ͻ',0
	"Unexpected loop escape '/'",					// 8 '[ ] Ų � / � ����޹�޽',0
	"Need more parameter",							// 9 '���Ұ� �� ��Ͼ�',0
	"Command not supported at this CH",				// 10 '������� ��� ¶�Ų ����޶ޱ�Ͻ',0
	"Too many loop nest '['-']'",					// 11 '[ ] � Ƚ�� 16�� ����޽',0
	"FM Voice not found",							// 12 '�ݼ�� �ް� �� ײ���� � �ݻ޲�Ͼ�',0
	"Counter overflow",								// 13 '����� ���ް�۰',0
	"Mode error",									// 14 'Ӱ�� �װ',0
	"Music data overflow",							// 15 '��޼ު�� خ��� � ��ϼ�',0
	"Undefined macro number",						// 16 'ò�޼�Ų ϸ����ް�ޱ�Ͻ',0
	"Macro end code not found",						// 17 'ϸ۴��޺��� �� ��Ͼ�',0
	"*"
};

static const char *err_jpn[] = {
	"",
	"���@�Ɍ�肪����܂�",
	"�p�����[�^�[�̒l���ُ�ł�",
	" ]�̐����������܂�",
	" [�̐����������܂�",
	"���F�̐����������܂�",
	"�I�N�^�[�u���K��͈͂𒴂��Ă܂�",
	"���Y�����N���b�N�̒l�𒴂��Ă܂�",
	"[ ] ���� / ��1�����ł�",
	"�p�����[�^�[������܂���",
	"���̃`�����l���ł͎g���Ȃ��R�}���h������܂�",
	"[ ] �̃l�X�g��16��܂łł�",
	"���F�̃f�[�^�����C�u�����ɑ��݂��܂���",
	"�J�E���^�[�I�[�o�[�t���[",
	"���[�h�G���[",
	"�I�u�W�F�N�g�̈�𒴂��܂���",
	"��`���Ă��Ȃ��}�N���i���o�[������܂�",
	"�}�N���G���h�R�[�h������܂���",
};

const char *mucom_geterror(int error)
{
	if ((error<0)||(error>=MUCOMERR_MAX)) return err[0];
	return err[error];
}

const char *mucom_geterror_j(int error)
{
	if ((error<0) || (error >= MUCOMERR_MAX)) return err[0];
	return err_jpn[error];
}

int mucom_geterror(const char *orgerror)
{
	int i;
	for (i = 1; i < MUCOMERR_MAX; i++) {
		if (strcmp(orgmsg[i], orgerror) == 0) return i;
	}
	return 0;
}

