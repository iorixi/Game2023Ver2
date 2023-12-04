#pragma
#include "ScheduledTask.h"

const int LEFTARROW = 37;
const int UPARROW = 38;
const int RIGHTARROW = 39;
const int DOWNARROW = 40;

class Input
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];
	static bool m_IsInputEnabled;
	static bool m_inputControl;
	static POINT mousePos;
	static POINT oldMousePos;
	static Timer::ScheduledTask mouseScheduledTask;

public:
	static void Init();
	static void Uninit();
	static void Update();

	//”ñ“¯Šúˆ—
	static void UpdateAsync();
	static void UpdateInternal();

	static void EnableInput();
	static void DisableInput();

	//“ü—Í•”•ª
	static void Reset();

	static bool GetKeyPress(BYTE KeyCode);
	static bool GetKeyTrigger(BYTE KeyCode);

	static POINT GetMousePos();
	static POINT GetOldMousePos();
};
