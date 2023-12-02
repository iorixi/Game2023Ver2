#include "ImguiManager.h"
#include "renderer.h"
#include "Imgui/imgui_impl_dx11.h"
#include "Imgui/imgui_impl_win32.h"

ImguiManager::ImguiManager()
{
	// someVariableの初期値を設定する
	someVariable = 100.0f;
}

ImguiManager::~ImguiManager()
{
	// ImGuiの終了処理
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImguiManager::Init(HWND hwnd)
{
	// ImGuiの初期化処理
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// フォントの読み込み
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	// デフォルトのフォントを設定
	io.FontDefault = io.Fonts->Fonts.back();

	// ImGuiの設定（必要に応じて）
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// ImGui Win32およびDX11の初期化
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(Renderer::GetDevice(), Renderer::GetDeviceContext());
}

void ImguiManager::Update()
{
	// ImGuiのフレームを開始
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// ウィンドウサイズを設定
	ImGui::SetNextWindowSize(ImVec2(320, 200), ImGuiCond_Once);

	// ウィンドウを開始
	ImGui::Begin("My Window");

	// ウィンドウ内のGUI要素を追加
	ImGui::Text("Hello, world");
	FLOAT color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//スライダーを追加
	AddSliderNum("aaa", &someVariable, 0.0f, 100.0f);
	// チェックボックスの追加
	AddCheckbox("Enable Feature", &checkboxValue);

	// ウィンドウを終了
	ImGui::End();

	// ImGuiの描画処理
	ImGui::Render();

	// 描画先のレンダーターゲットを設定
	ID3D11RenderTargetView* pRenderTargetView = Renderer::GetRenderTargetview();
	//Renderer::GetDeviceContext()->OMSetRenderTargets(1, &pRenderTargetView, NULL);

	// レンダーターゲットをクリア
	Renderer::GetDeviceContext()->ClearRenderTargetView(Renderer::GetRenderTargetview(), color);

	// ImGuiの描画データを描画
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImguiManager::Draw()
{
	// ImGuiの描画処理
	// ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

/// <summary>
/// ImGuiに指定された浮動小数点数型の変数に対するスライダーを追加。
/// </summary>
/// <param name="label">スライダーの前に表示されるテキストラベル。</param>
/// <param name="value">操作される浮動小数点数の変数へのポインタ。</param>
/// <param name="min">変数が取りうる最小値（デフォルトは0.0f）。</param>
/// <param name="max">変数が取りうる最大値（デフォルトは100.0f）。</param>
void ImguiManager::AddSliderNum(const char* label, float* value, float min /*= 0.0f*/, float max /*= 200.0f*/)
{
	// SliderFloatの範囲を修正
	ImGui::SliderFloat(label, value, min, max);
}

/// <summary>
/// ImGuiに指定されたブール型の変数に対するチェックボックスを追加。
/// </summary>
/// <param name="label">チェックボックスの前に表示されるテキストラベル。</param>
/// <param name="value">操作されるブール型の変数へのポインタ。</param>
void ImguiManager::AddCheckbox(const char* label, bool* value)
{
	ImGui::Checkbox(label, value);
}