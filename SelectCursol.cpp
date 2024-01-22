#include "SelectCursol.h"
#include "input.h"
#include <algorithm>

void SelectCursol::Init(int num)
{
	// mSelectComand���f�t�H���g�̒l�ŏ���������
	mSelectComand = 0;
	mNumComnd = num;
}

int SelectCursol::Check()
{
	int Comand = -1;

	// W�L�[�Ə���L�[�������ꂽ��
	if (Input::GetKeyTrigger('W') || Input::GetKeyTrigger(VK_UP))
	{
		// �I�����ꂽ�R�}���h���f�N�������g���A0��菬�����ꍇ�͍ő�l�Ƀ��b�v�A���E���h����
		mSelectComand = (mSelectComand - 1 + mNumComnd) % mNumComnd;
	}
	// ���̃L�[���������邩�A�K�v�ɉ����ď�����ǉ����܂��i��F'S'�܂���VK_DOWN�̏ꍇ�Ȃǁj
	if (Input::GetKeyTrigger('S') || Input::GetKeyTrigger(VK_DOWN))
	{
		// �I�����ꂽ�R�}���h���f�N�������g���A0��菬�����ꍇ�͍ő�l�Ƀ��b�v�A���E���h����
		mSelectComand = (mSelectComand + 1 + mNumComnd) % mNumComnd;
	}
	// ���݂̑I�����ꂽ�R�}���h��Ԃ�
	return mSelectComand;
}