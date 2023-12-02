#include "ImguiManager.h"
#include "renderer.h"
#include "Imgui/imgui_impl_dx11.h"
#include "Imgui/imgui_impl_win32.h"

ImguiManager::ImguiManager()
{
	// someVariable�̏����l��ݒ肷��
	someVariable = 100.0f;
}

ImguiManager::~ImguiManager()
{
	// ImGui�̏I������
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
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
}

void ImguiManager::Update()
{
	// ImGui�̃t���[�����J�n
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

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
	AddCheckbox("Enable Feature", &checkboxValue);

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
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImguiManager::Draw()
{
	// ImGui�̕`�揈��
	// ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
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