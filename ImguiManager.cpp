#include "ImguiManager.h"
#include "Imgui/imgui_impl_dx11.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/curve.hpp"

#include <stdexcept>
#include <iostream>
#include "renderer.h"
#include <cmath>
#include <string>

ImguiManager::ImguiManager()
{
	// someVariable�̏����l��ݒ肷��
	someVariable = 100.0f;
}

ImguiManager::~ImguiManager()
{
}

void ImguiManager::Init(HWND hwnd)
{
	// ImGui�̏���������
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// �t�H���g�̓ǂݍ���
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	// �f�t�H���g�̃t�H���g��ݒ�
	io.FontDefault = io.Fonts->Fonts.back();

	// ImGui�̐ݒ�i�K�v�ɉ����āj
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// ImGui Win32�����DX11�̏�����
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(Renderer::GetDevice(), Renderer::GetDeviceContext());

	//mapBool�̏�����
	mapBool["TitleFlg"] = false;
	mapBool["ResultFlg"] = false;

	//mapFloat�̏�����
}

void ImguiManager::Init()
{
}

void ImguiManager::Update()
{
	// ImGui�̃t���[�����J�n
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//update���ǂݍ��񂾂��ǂ���
	if (!imguiUpdateFlg)
	{
		imguiUpdateFlg = true;
	}

	// �E�B���h�E�T�C�Y��ݒ�
	ImGui::SetNextWindowSize(ImVec2(320, 200), ImGuiCond_Once);

	// �E�B���h�E���J�n
	ImGui::Begin("My Window");

	// �E�B���h�E����GUI�v�f��ǉ�
	ImGui::Text("Hello, world");
	FLOAT color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//�X���C�_�[��ǉ�
	AddSliderNum("aaa", &someVariable, 0.0f, 100.0f);
	// �`�F�b�N�{�b�N�X�̒ǉ�

	AddButton("TitleGo", &mapBool["TitleFlg"]);
	AddButton("ResultGo", &mapBool["ResultFlg"]);
	AddCheckbox("imguiUpdateFlg", &imguiUpdateFlg);

	// �ϐ��̒l���e�L�X�g�ŕ\��
	roundDistance = std::round(m_distance);
	// ���l�𕶎���ɕϊ�
	std::string distanceStr = "Distance: " + std::to_string(static_cast<int>(roundDistance));

	static ImVec2 foo[10];

	if (ImGui::Curve("Das editor", ImVec2(300, 100), 10, foo))
	{
		// curve changed
	}

	float value_you_care_about = ImGui::CurveValue(0.7f, 10, foo); // calculate value at position 0.7

	// ImGui �ŕ\��
	ImGui::Text(distanceStr.c_str());

	// �E�B���h�E���I��
	ImGui::End();

	// ImGui�̕`�揈��
	ImGui::Render();

	// �`���̃����_�[�^�[�Q�b�g��ݒ�
	ID3D11RenderTargetView* pRenderTargetView = Renderer::GetRenderTargetview();
	//Renderer::GetDeviceContext()->OMSetRenderTargets(1, &pRenderTargetView, NULL);

	// �����_�[�^�[�Q�b�g���N���A
	Renderer::GetDeviceContext()->ClearRenderTargetView(Renderer::GetRenderTargetview(), color);

	// ImGui�̕`��f�[�^��`��
}

void ImguiManager::Draw()
{
	if (imguiUpdateFlg)
	{
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	// ImGui�̕`�揈��
	// ImGui::Render();
}

void ImguiManager::End()
{
	// ImGui�̏I������
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

/// <summary>
/// ImGui�Ɏw�肳�ꂽ���������_���^�̕ϐ��ɑ΂���X���C�_�[��ǉ��B
/// </summary>
/// <param name="label">�X���C�_�[�̑O�ɕ\�������e�L�X�g���x���B</param>
/// <param name="value">���삳��镂�������_���̕ϐ��ւ̃|�C���^�B</param>
/// <param name="min">�ϐ�����肤��ŏ��l�i�f�t�H���g��0.0f�j�B</param>
/// <param name="max">�ϐ�����肤��ő�l�i�f�t�H���g��100.0f�j�B</param>
void ImguiManager::AddSliderNum(const char* label, float* value, float min /*= 0.0f*/, float max /*= 200.0f*/)
{
	// SliderFloat�͈̔͂��C��
	ImGui::SliderFloat(label, value, min, max);
}

/// <summary>
/// ImGui�Ɏw�肳�ꂽ�u�[���^�̕ϐ��ɑ΂���`�F�b�N�{�b�N�X��ǉ��B
/// </summary>
/// <param name="label">�`�F�b�N�{�b�N�X�̑O�ɕ\�������e�L�X�g���x���B</param>
/// <param name="value">���삳���u�[���^�̕ϐ��ւ̃|�C���^�B</param>
void ImguiManager::AddCheckbox(const char* label, bool* value)
{
	ImGui::Checkbox(label, value);
}

/// <summary>
// ImguiManager.cpp�t�@�C������ImguiManager�N���X�̒�`����
/// </summary>
/// <param name="label">�`�F�b�N�{�b�N�X�̑O�ɕ\�������e�L�X�g���x���B</param>
/// <param name="value">���삳���u�[���^�̕ϐ��ւ̃|�C���^�B</param>
void ImguiManager::AddButton(const char* label, bool* flag)
{
	// �{�^����ǉ�
	if (ImGui::Button(label))
	{
		// �{�^���������ꂽ�Ƃ��̏���
		*flag = true;
	}
}

/// <param name="string">���o�������l�̖��O</param>
bool ImguiManager::GetMapBool(const std::string string)
{
	//�����l���Ȃ���Η�O�����i�v���O�������~�߂�j
	try
	{
		return mapBool.at(string);
	}
	catch (const std::out_of_range& e)
	{
		// �L�[�����݂��Ȃ��ꍇ�̃G���[�n���h�����O
		// ���O���Ⴄ�\��������̂Œ����Ă�
		std::cerr << " �������Ȃ��ł��@������x�����̖��O���m�F���Ă�������" << e.what() << std::endl;
		assert(false && "Key not found in GetMapFloat");
	}
}

/// <param name="string">���o�������l�̖��O</param>
float ImguiManager::GetMapFloat(const std::string string)
{
	//�����l���Ȃ���Η�O�����i�v���O�������~�߂�j
	try
	{
		return mapFloat.at(string);
	}
	catch (const std::out_of_range& e)
	{
		// �L�[�����݂��Ȃ��ꍇ�̃G���[�n���h�����O
		// ���O���Ⴄ�\��������̂Œ����Ă�
		std::cerr << "Error: Key not found in GetMapFloat - " << e.what() << std::endl;
		throw std::runtime_error("Key not found in GetMapFloat");
	}
}

void ImguiManager::SetImguiUpdateFlg(bool flg)
{
	imguiUpdateFlg = flg;
}

void ImguiManager::SetDistance(float distance)
{
	m_distance = distance;
}