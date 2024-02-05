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
	float someVariable = 100;
	bool checkboxValue = false;
	bool titleFlg = false;
	std::map<std::string, bool> mapBool;
	std::map<std::string, float> mapFloat;
	bool imguiUpdateFlg = false;
	float m_distance = 0;
	float roundDistance = 0;
	//imgui���I���ɂ��邩
	bool imguiFlg = false;

public:
	ImguiManager();
	~ImguiManager();

	void Init(HWND hwnd);
	void Init()override;
	void Update() override;
	void Draw() override;
	void End();

	// �X���C�_�[�t���̐��l����UI��ǉ�
	void AddSliderNum(const char* label, float* value, float min, float max);
	// �`�F�b�N�{�b�N�X��ǉ�
	void AddCheckbox(const char* label, bool* value);

	//true�{�^����ǉ�
	void AddButton(const char* label, bool* flag);

	bool  GetMapBool(const std::string string);
	float GetMapFloat(const std::string string);
	void  SetImguiUpdateFlg(bool flg);
	void  SetDistance(float distance);

private:
};
