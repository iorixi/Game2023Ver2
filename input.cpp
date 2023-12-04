#include "main.h"
#include "input.h"
#include <future>

BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];
bool Input::m_IsInputEnabled = true;
POINT Input::mousePos;
POINT Input::oldMousePos;
Timer::ScheduledTask Input::mouseScheduledTask;
bool Input::m_inputControl = false;

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

	if (GetKeyTrigger('P'))
	{
		m_inputControl = !m_inputControl;
		// マウスカーソルを表示非表示にする
		ShowCursor(!m_inputControl);
	}

	//inputを無効化
	if (m_inputControl)
	{
		if (mouseScheduledTask.GetFlg())
		{
			SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
			GetCursorPos(&oldMousePos);
		}
	}
}

void Input::UpdateAsync()
{
	if (!m_IsInputEnabled) {
		return; // キー入力が無効の場合は何もしない
	}
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