#include "ImguiManager.h"
#include "Imgui/imgui_impl_dx11.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/curve.hpp"

#include <stdexcept>
#include <iostream>
#include "renderer.h"
#include <cmath>
#include <string>

#include "input.h"

//�J�����̏���������
const float CAMERA_DISTANCE = 8.0f;	//�J�����Ƃ̋���
const float PLAYER_TOP_THRESHOLD = 2.0f;	//�v���C���[�L�����N�^�[�̓��̈ʒu
const float CAMERA_HEIGHT = 4.0f;	//�ʏ탂�[�h�̎��̃J�����̍���
const float CAMERA_HEIGHT_CLOSERANGE = 2.2f;	//�ߋ������[�h�̎��̃J�����̍���
const float CAMERA_TARGET_HEIGHT = -6.8f;	//�J�����̒����_�̍���
const float CAMERA_RIGHT_OFFSET = 1.2f;	//�J�������E�Ɋ񂹂�
const float CAMERA_ROTATE_PITCHPOWER = 0.5f;	//�J������X��]�̕␳�{��

const float CAMERA_POS_DISTANCE_CORRECTION = 100.0f;		//�J�����̈ʒu�̋����␳
const float CAMERA_TARGET_DISTANCE_CORRECTION = 100.0f;		//�J�����̒����_�̋����␳

// Lerp �W���B�J�����̈ʒu�⒍���_�̕ύX���ɏ��X�Ɉړ����邽�߂Ɏg�p����܂��B
const float LERP_FACTOR = 0.07f;		//�J�����̒����_�̋����␳

//�����ɍ��킹�ăJ�����̑��x��␳
const float DISTANCE_FACTOR = 0.02f;		//�J�����̒����_�̋����␳

void ImguiManager::ImguiAddSystem()
{
	// �E�B���h�E����GUI�v�f��ǉ�
	FLOAT color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// �܂肽���ݓW�J�\�ȃw�b�_�[���쐬���A���̒���GUI�v�f��z�u
	//�V�[���`�F���W
	if (ImGui::CollapsingHeader("SceneChange", !ImGuiTreeNodeFlags_DefaultOpen))
	{
		// �`�F�b�N�{�b�N�X�̒ǉ�
		AddButton("TitleGo", &mapBool["TitleFlg"]);
		AddButton("ResultGo", &mapBool["ResultFlg"]);
	}

	//�v���C���[�ƓG�Ƃ̋���
	if (ImGui::CollapsingHeader("Distanse", !ImGuiTreeNodeFlags_DefaultOpen))
	{
		// �ϐ��̒l���e�L�X�g�ŕ\��
		float roundDistance = 0;//�l�̌ܓ�
		//�l�̌ܓ�
		roundDistance = std::round(m_distance);
		// ���l�𕶎���ɕϊ�
		std::string distanceStr = "Distance: " + std::to_string(static_cast<int>(roundDistance));
		// ImGui �ŕ\��
		ImGui::Text(distanceStr.c_str());
	}

	//Curve����
	if (ImGui::CollapsingHeader("Curve", !ImGuiTreeNodeFlags_DefaultOpen))
	{
		static ImVec2 foo[10];

		if (ImGui::Curve("Das editor", ImVec2(300, 100), 10, foo))
		{
			// curve changed
		}

		float value_you_care_about = ImGui::CurveValue(0.7f, 10, foo); // calculate value at position 0.7
	}
}

void ImguiManager::CameraAddSystem()
{
	// �܂肽���ݓW�J�\�ȃw�b�_�[���쐬���A���̒���GUI�v�f��z�u
	//�J�������[�N�n
	if (ImGui::CollapsingHeader("Camera", !ImGuiTreeNodeFlags_DefaultOpen))
	{
		// �`�F�b�N�{�b�N�X�̒ǉ�
		//�J�����Ƃ̋���
		AddSliderNum("CameraDistance", &mapCameraFloat["CameraDistance"], -30, 30);
		//�v���C���[�L�����N�^�[�̓��̈ʒu
		AddSliderNum("PlayerTopThreshold", &mapCameraFloat["PlayerTopThreshold"], -30, 30);

		//�ʏ탂�[�h�̎��̃J�����̍���
		AddSliderNum("CameraHeight", &mapCameraFloat["CameraHeight"], -30, 30);
		//�ߋ������[�h�̎��̃J�����̍���
		AddSliderNum("CameraHeightCloseRange", &mapCameraFloat["CameraHeightCloseRange"], -30, 30);

		//�J�����̒����_�̍���
		AddSliderNum("CameraTargetHeight", &mapCameraFloat["CameraTargetHeight"], -30, 30);
		//�J�������E�Ɋ񂹂�
		AddSliderNum("CameraRightOffset", &mapCameraFloat["CameraRightOffset"], -30, 30);
		//�J������X��]�̕␳�{��
		AddSliderNum("CameraRotatePitchPower", &mapCameraFloat["CameraRotatePitchPower"], -30, 30);
		//�J�����̈ʒu�̋����␳
		AddSliderNum("CameraPosDistanceCorrection", &mapCameraFloat["CameraPosDistanceCorrection"], -500, 500);
		//�J�����̒����_�̋����␳//�J�������E�Ɋ񂹂�
		AddSliderNum("CameraTargetDistanceCorrection", &mapCameraFloat["CameraTargetDistanceCorrection"], -500, 500);

		// Lerp �W���B�J�����̈ʒu�⒍���_�̕ύX���ɏ��X�Ɉړ����邽�߂Ɏg�p����܂��B
		AddSliderNum("lerpFactor", &mapCameraFloat["lerpFactor"], 0, 1);//�J�����̒����_�̋����␳
		//�����ɍ��킹�ăJ�����̑��x��␳
		AddSliderNum("distanceFactor", &mapCameraFloat["distanceFactor"], 0, 1);//�J�����̒����_�̋����␳
	}
}

ImguiManager::ImguiManager()
{
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
	mapCameraFloat["CameraDistance"] = CAMERA_DISTANCE;     //�J�����Ƃ̋���
	mapCameraFloat["PlayerTopThreshold"] = PLAYER_TOP_THRESHOLD;		//�v���C���[�L�����N�^�[�̓��̈ʒu
	mapCameraFloat["CameraHeight"] = CAMERA_HEIGHT;		//�ʏ탂�[�h�̎��̃J�����̍���
	mapCameraFloat["CameraHeightCloseRange"] = CAMERA_HEIGHT_CLOSERANGE;		//�ߋ������[�h�̎��̃J�����̍���
	mapCameraFloat["CameraTargetHeight"] = -CAMERA_TARGET_HEIGHT;		//�J�����̒����_�̍���
	mapCameraFloat["CameraRightOffset"] = CAMERA_RIGHT_OFFSET;		//�J�������E�Ɋ񂹂�
	mapCameraFloat["CameraRotatePitchPower"] = CAMERA_ROTATE_PITCHPOWER;		//�J������X��]�̕␳�{��
	mapCameraFloat["CameraPosDistanceCorrection"] = CAMERA_POS_DISTANCE_CORRECTION;		//�J�����̈ʒu�̋����␳
	mapCameraFloat["CameraTargetDistanceCorrection"] = CAMERA_TARGET_DISTANCE_CORRECTION;		//�J�����̒����_�̋����␳

	// Lerp �W���B�J�����̈ʒu�⒍���_�̕ύX���ɏ��X�Ɉړ����邽�߂Ɏg�p����܂��B
	mapCameraFloat["lerpFactor"] = LERP_FACTOR;		//�J�����̒����_�̋����␳
	//�����ɍ��킹�ăJ�����̑��x��␳
	mapCameraFloat["distanceFactor"] = DISTANCE_FACTOR;		//�J�����̒����_�̋����␳
}

void ImguiManager::Init()
{
}

void ImguiManager::Update()
{
	// imgui��t���邩�ǂ���
	if (Input::GetKeyTrigger('P'))
	{
		imguiFlg = !imguiFlg;
	}

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
	ImGui::SetNextWindowSize(ImVec2(420, 400), ImGuiCond_Once);

	// �E�B���h�E���J�n
	ImGui::Begin("Imgui Window");

	//system�ǉ�
	ImguiAddSystem();
	CameraAddSystem();

	// ImGui�̕`�揈��
	ImGui::Render();

	// �`���̃����_�[�^�[�Q�b�g��ݒ�
	ID3D11RenderTargetView* pRenderTargetView = Renderer::GetRenderTargetview();
	//Renderer::GetDeviceContext()->OMSetRenderTargets(1, &pRenderTargetView, NULL);

	// �����_�[�^�[�Q�b�g���N���A
	FLOAT color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	Renderer::GetDeviceContext()->ClearRenderTargetView(Renderer::GetRenderTargetview(), color);

	// ImGui�̕`��f�[�^��`��
}

void ImguiManager::Draw()
{
	if (imguiUpdateFlg)
	{
		if (imguiFlg)
		{
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}
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
		assert(false && "Key not found in GetMapBool");
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

float ImguiManager::GetMapCameraFloat(const std::string string)
{
	//�����l���Ȃ���Η�O�����i�v���O�������~�߂�j
	try
	{
		return mapCameraFloat.at(string);
	}
	catch (const std::out_of_range& e)
	{
		// �L�[�����݂��Ȃ��ꍇ�̃G���[�n���h�����O
		// ���O���Ⴄ�\��������̂Œ����Ă�
		std::cerr << "Error: Key not found in GetMapCameraFloat - " << e.what() << std::endl;
		throw std::runtime_error("Key not found in GetMapCameraFloat");
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