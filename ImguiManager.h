#pragma once
#include "Imgui/imgui.h"
#include <Windows.h>
#include <vector>
#include <map>
#include <string>

class ImguiManager
{
	float someVariable = 100;
	bool checkboxValue = false;
	bool titleFlg = false;
	std::map<std::string, bool> mapBool;
	std::map<std::string, float> mapFloat;

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

	//true�{�^����ǉ�
	void AddButton(const char* label, bool* flag);

	bool  GetMapBool(const std::string string);
	float GetMapFloat(const std::string string);

private:
};
