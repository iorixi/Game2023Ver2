#pragma once
#include "Imgui/imgui.h"
#include <Windows.h>

class ImguiManager
{
	float someVariable = 100;
	bool checkboxValue = false;

public:
	ImguiManager();
	~ImguiManager();

	void Init(HWND hwnd);
	void Update();
	void Draw();

	// �X���C�_�[�t���̐��l����UI��ǉ�
	void AddSliderNum(const char* label, float* value, float min, float max);
	// �`�F�b�N�{�b�N�X��ǉ�	
	void AddCheckbox(const char* label, bool* value);

private:

};

