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

	// スライダー付きの数値調整UIを追加
	void AddSliderNum(const char* label, float* value, float min, float max);
	// チェックボックスを追加	
	void AddCheckbox(const char* label, bool* value);

private:

};

