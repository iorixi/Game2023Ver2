#include "main.h"
#include "input.h"
#include <future>

BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];
bool Input::m_IsInputEnabled = true;
POINT Input::mousePos;
POINT Input::oldMousePos;
Timer::ScheduledTask Input::mouseScheduledTask;

void Input::Init()
{
	memset(m_OldKeyState, 0, 256);
	memset(m_KeyState, 0, 256);
	mouseScheduledTask.SetTimer(0.01f);
}

void Input::Uninit()
{
}

void Input::Update()
{
	if (!m_IsInputEnabled) {
		return; // キー入力が無効の場合は何もしない
	}

	memcpy(m_OldKeyState, m_KeyState, 256);

	GetKeyboardState(m_KeyState);

	oldMousePos = mousePos;
	GetCursorPos(&mousePos);

	//if (mouseScheduledTask.GetFlg())
	//{
	//	SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	//	GetCursorPos(&oldMousePos);
	//}
}

void Input::UpdateAsync()
{
	if (!m_IsInputEnabled) {
		return; // キー入力が無効の場合は何もしない
	}

	// 同期処理を非同期にする
	auto future = std::async(std::launch::async, UpdateInternal);

	// 非同期処理の完了を待つ
	future.get();
}

void Input::UpdateInternal()
{
	memcpy(m_OldKeyState, m_KeyState, 256);

	GetKeyboardState(m_KeyState);
}

void Input::EnableInput()
{
	m_IsInputEnabled = true;
}

void Input::DisableInput()
{
	m_IsInputEnabled = false;
}

void Input::Reset()
{
	m_IsInputEnabled = false;
}

bool Input::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}

bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}

POINT Input::GetMousePos()
{
	return mousePos;
}

POINT Input::GetOldMousePos()
{
	return oldMousePos;
}