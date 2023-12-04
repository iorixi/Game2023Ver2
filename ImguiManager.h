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

	// スライダー付きの数値調整UIを追加
	void AddSliderNum(const char* label, float* value, float min, float max);
	// チェックボックスを追加
	void AddCheckbox(const char* label, bool* value);

	//trueボタンを追加
	void AddButton(const char* label, bool* flag);

	bool  GetMapBool(const std::string string);
	float GetMapFloat(const std::string string);

private:
};
