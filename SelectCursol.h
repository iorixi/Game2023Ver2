#pragma once
#pragma once

#include"GameObject.h"
#include <vector>
#include "renderer.h"

/// <summary>
/// ���ݑI������Ă���J�[�\���̔ԍ����擾����N���X
/// </summary>
class SelectCursol : public Component
{
private:
	int mNumComnd; //�R�}���h�̐�
	int mSelectComand;  // ���ǂ̃R�}���h�������ł��邩

	using Component::Component;

public:
	void Init(int num);
	//���݂̑I������Ă���ԍ����擾
	int Check();
	//���� �P�R�}���h���@�Q ��ԏ�̃R�}���h�̂����W�@�R ��ԏ�̃R�}���h�̂����W�@�S �R�}���h��
};