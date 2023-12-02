#pragma

const int LEFTARROW = 37;
const int UPARROW = 38;
const int RIGHTARROW = 39;
const int DOWNARROW = 40;

class Input
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];

public:
	static void Init();
	static void Uninit();
	static void Update();

	static bool GetKeyPress(BYTE KeyCode);
	static bool GetKeyTrigger(BYTE KeyCode);
};
