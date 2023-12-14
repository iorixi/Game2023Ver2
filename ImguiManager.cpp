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
	// someVariableの初期値を設定する
	someVariable = 100.0f;
}

ImguiManager::~ImguiManager()
{
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

	//mapBoolの初期化
	mapBool["TitleFlg"] = false;
	mapBool["ResultFlg"] = false;

	//mapFloatの初期化
}

void ImguiManager::Init()
{
}

void ImguiManager::Update()
{
	// ImGuiのフレームを開始
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//updateが読み込んだかどうか
	if (!imguiUpdateFlg)
	{
		imguiUpdateFlg = true;
	}

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

	AddButton("TitleGo", &mapBool["TitleFlg"]);
	AddButton("ResultGo", &mapBool["ResultFlg"]);
	AddCheckbox("imguiUpdateFlg", &imguiUpdateFlg);

	// 変数の値をテキストで表示
	roundDistance = std::round(m_distance);
	// 数値を文字列に変換
	std::string distanceStr = "Distance: " + std::to_string(static_cast<int>(roundDistance));

	static ImVec2 foo[10];

	if (ImGui::Curve("Das editor", ImVec2(300, 100), 10, foo))
	{
		// curve changed
	}

	float value_you_care_about = ImGui::CurveValue(0.7f, 10, foo); // calculate value at position 0.7

	// ImGui で表示
	ImGui::Text(distanceStr.c_str());

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
}

void ImguiManager::Draw()
{
	if (imguiUpdateFlg)
	{
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	// ImGuiの描画処理
	// ImGui::Render();
}

void ImguiManager::End()
{
	// ImGuiの終了処理
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
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

/// <summary>
// ImguiManager.cppファイル内のImguiManagerクラスの定義部分
/// </summary>
/// <param name="label">チェックボックスの前に表示されるテキストラベル。</param>
/// <param name="value">操作されるブール型の変数へのポインタ。</param>
void ImguiManager::AddButton(const char* label, bool* flag)
{
	// ボタンを追加
	if (ImGui::Button(label))
	{
		// ボタンが押されたときの処理
		*flag = true;
	}
}

/// <param name="string">取り出したい値の名前</param>
bool ImguiManager::GetMapBool(const std::string string)
{
	//もし値がなければ例外処理（プログラムを止める）
	try
	{
		return mapBool.at(string);
	}
	catch (const std::out_of_range& e)
	{
		// キーが存在しない場合のエラーハンドリング
		// 名前が違う可能性があるので直してね
		std::cerr << " 引数がないです　もう一度引数の名前を確認してください" << e.what() << std::endl;
		assert(false && "Key not found in GetMapFloat");
	}
}

/// <param name="string">取り出したい値の名前</param>
float ImguiManager::GetMapFloat(const std::string string)
{
	//もし値がなければ例外処理（プログラムを止める）
	try
	{
		return mapFloat.at(string);
	}
	catch (const std::out_of_range& e)
	{
		// キーが存在しない場合のエラーハンドリング
		// 名前が違う可能性があるので直してね
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