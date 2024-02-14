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

//カメラの初期化処理
const float CAMERA_DISTANCE = 8.0f;	//カメラとの距離
const float PLAYER_TOP_THRESHOLD = 2.0f;	//プレイヤーキャラクターの頭の位置
const float CAMERA_HEIGHT = 4.0f;	//通常モードの時のカメラの高さ
const float CAMERA_HEIGHT_CLOSERANGE = 2.2f;	//近距離モードの時のカメラの高さ
const float CAMERA_TARGET_HEIGHT = -6.8f;	//カメラの中視点の高さ
const float CAMERA_RIGHT_OFFSET = 1.2f;	//カメラを右に寄せる
const float CAMERA_ROTATE_PITCHPOWER = 0.5f;	//カメラのX回転の補正倍率

const float CAMERA_POS_DISTANCE_CORRECTION = 100.0f;		//カメラの位置の距離補正
const float CAMERA_TARGET_DISTANCE_CORRECTION = 100.0f;		//カメラの中視点の距離補正

// Lerp 係数。カメラの位置や注視点の変更時に徐々に移動するために使用されます。
const float LERP_FACTOR = 0.07f;		//カメラの中視点の距離補正

//距離に合わせてカメラの速度を補正
const float DISTANCE_FACTOR = 0.02f;		//カメラの中視点の距離補正

void ImguiManager::ImguiAddSystem()
{
	// ウィンドウ内のGUI要素を追加
	FLOAT color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 折りたたみ展開可能なヘッダーを作成し、その中にGUI要素を配置
	//シーンチェンジ
	if (ImGui::CollapsingHeader("SceneChange", !ImGuiTreeNodeFlags_DefaultOpen))
	{
		// チェックボックスの追加
		AddButton("TitleGo", &mapBool["TitleFlg"]);
		AddButton("ResultGo", &mapBool["ResultFlg"]);
	}

	//プレイヤーと敵との距離
	if (ImGui::CollapsingHeader("Distanse", !ImGuiTreeNodeFlags_DefaultOpen))
	{
		// 変数の値をテキストで表示
		float roundDistance = 0;//四捨五入
		//四捨五入
		roundDistance = std::round(m_distance);
		// 数値を文字列に変換
		std::string distanceStr = "Distance: " + std::to_string(static_cast<int>(roundDistance));
		// ImGui で表示
		ImGui::Text(distanceStr.c_str());
	}

	//Curve処理
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
	// 折りたたみ展開可能なヘッダーを作成し、その中にGUI要素を配置
	//カメラワーク系
	if (ImGui::CollapsingHeader("Camera", !ImGuiTreeNodeFlags_DefaultOpen))
	{
		// チェックボックスの追加
		//カメラとの距離
		AddSliderNum("CameraDistance", &mapCameraFloat["CameraDistance"], -30, 30);
		//プレイヤーキャラクターの頭の位置
		AddSliderNum("PlayerTopThreshold", &mapCameraFloat["PlayerTopThreshold"], -30, 30);

		//通常モードの時のカメラの高さ
		AddSliderNum("CameraHeight", &mapCameraFloat["CameraHeight"], -30, 30);
		//近距離モードの時のカメラの高さ
		AddSliderNum("CameraHeightCloseRange", &mapCameraFloat["CameraHeightCloseRange"], -30, 30);

		//カメラの中視点の高さ
		AddSliderNum("CameraTargetHeight", &mapCameraFloat["CameraTargetHeight"], -30, 30);
		//カメラを右に寄せる
		AddSliderNum("CameraRightOffset", &mapCameraFloat["CameraRightOffset"], -30, 30);
		//カメラのX回転の補正倍率
		AddSliderNum("CameraRotatePitchPower", &mapCameraFloat["CameraRotatePitchPower"], -30, 30);
		//カメラの位置の距離補正
		AddSliderNum("CameraPosDistanceCorrection", &mapCameraFloat["CameraPosDistanceCorrection"], -500, 500);
		//カメラの中視点の距離補正//カメラを右に寄せる
		AddSliderNum("CameraTargetDistanceCorrection", &mapCameraFloat["CameraTargetDistanceCorrection"], -500, 500);

		// Lerp 係数。カメラの位置や注視点の変更時に徐々に移動するために使用されます。
		AddSliderNum("lerpFactor", &mapCameraFloat["lerpFactor"], 0, 1);//カメラの中視点の距離補正
		//距離に合わせてカメラの速度を補正
		AddSliderNum("distanceFactor", &mapCameraFloat["distanceFactor"], 0, 1);//カメラの中視点の距離補正
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
	mapCameraFloat["CameraDistance"] = CAMERA_DISTANCE;     //カメラとの距離
	mapCameraFloat["PlayerTopThreshold"] = PLAYER_TOP_THRESHOLD;		//プレイヤーキャラクターの頭の位置
	mapCameraFloat["CameraHeight"] = CAMERA_HEIGHT;		//通常モードの時のカメラの高さ
	mapCameraFloat["CameraHeightCloseRange"] = CAMERA_HEIGHT_CLOSERANGE;		//近距離モードの時のカメラの高さ
	mapCameraFloat["CameraTargetHeight"] = -CAMERA_TARGET_HEIGHT;		//カメラの中視点の高さ
	mapCameraFloat["CameraRightOffset"] = CAMERA_RIGHT_OFFSET;		//カメラを右に寄せる
	mapCameraFloat["CameraRotatePitchPower"] = CAMERA_ROTATE_PITCHPOWER;		//カメラのX回転の補正倍率
	mapCameraFloat["CameraPosDistanceCorrection"] = CAMERA_POS_DISTANCE_CORRECTION;		//カメラの位置の距離補正
	mapCameraFloat["CameraTargetDistanceCorrection"] = CAMERA_TARGET_DISTANCE_CORRECTION;		//カメラの中視点の距離補正

	// Lerp 係数。カメラの位置や注視点の変更時に徐々に移動するために使用されます。
	mapCameraFloat["lerpFactor"] = LERP_FACTOR;		//カメラの中視点の距離補正
	//距離に合わせてカメラの速度を補正
	mapCameraFloat["distanceFactor"] = DISTANCE_FACTOR;		//カメラの中視点の距離補正
}

void ImguiManager::Init()
{
}

void ImguiManager::Update()
{
	// imguiを付けるかどうか
	if (Input::GetKeyTrigger('P'))
	{
		imguiFlg = !imguiFlg;
	}

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
	ImGui::SetNextWindowSize(ImVec2(420, 400), ImGuiCond_Once);

	// ウィンドウを開始
	ImGui::Begin("Imgui Window");

	//system追加
	ImguiAddSystem();
	CameraAddSystem();

	// ImGuiの描画処理
	ImGui::Render();

	// 描画先のレンダーターゲットを設定
	ID3D11RenderTargetView* pRenderTargetView = Renderer::GetRenderTargetview();
	//Renderer::GetDeviceContext()->OMSetRenderTargets(1, &pRenderTargetView, NULL);

	// レンダーターゲットをクリア
	FLOAT color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	Renderer::GetDeviceContext()->ClearRenderTargetView(Renderer::GetRenderTargetview(), color);

	// ImGuiの描画データを描画
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
		assert(false && "Key not found in GetMapBool");
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

float ImguiManager::GetMapCameraFloat(const std::string string)
{
	//もし値がなければ例外処理（プログラムを止める）
	try
	{
		return mapCameraFloat.at(string);
	}
	catch (const std::out_of_range& e)
	{
		// キーが存在しない場合のエラーハンドリング
		// 名前が違う可能性があるので直してね
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