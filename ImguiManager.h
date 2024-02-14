#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "Imgui/imgui.h"
#include <Windows.h>
#include <vector>
#include <map>
#include <string>
#include "gameObject.h"

class ImguiManager : public GameObject
{
	//imguiをオンにするか
	bool imguiFlg = false;
	//ImguiUpdate処理を実行するか
	bool imguiUpdateFlg = false;

	//AddSystem変数
	bool titleFlg = false;
	std::map<std::string, bool> mapBool;
	std::map<std::string, float> mapFloat;
	float m_distance = 0;//距離

	//AddCameraSystem変数
	std::map<std::string, float> mapCameraFloat;

public:

	ImguiManager();
	~ImguiManager();

	void Init(HWND hwnd);
	void Init()override;
	void Update() override;
	void Draw() override;
	void End();

	// スライダー付きの数値調整UIを追加
	void AddSliderNum(const char* label, float* value, float min, float max);
	// チェックボックスを追加
	void AddCheckbox(const char* label, bool* value);

	//trueボタンを追加
	void AddButton(const char* label, bool* flag);

	bool  GetMapBool(const std::string string);
	float GetMapFloat(const std::string string);
	float GetMapCameraFloat(const std::string string);

	void  SetImguiUpdateFlg(bool flg);
	void  SetDistance(float distance);

private:

	//システムの追加処理
	void ImguiAddSystem();
	void CameraAddSystem();
};
