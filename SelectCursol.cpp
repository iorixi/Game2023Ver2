#include "SelectCursol.h"
#include "input.h"
#include <algorithm>

void SelectCursol::Init(int num)
{
	// mSelectComandをデフォルトの値で初期化する
	mSelectComand = 0;
	mNumComnd = num;
}

int SelectCursol::Check()
{
	int Comand = -1;

	// Wキーと上矢印キーが押されたら
	if (Input::GetKeyTrigger('W') || Input::GetKeyTrigger(VK_UP))
	{
		// 選択されたコマンドをデクリメントし、0より小さい場合は最大値にラップアラウンドする
		mSelectComand = (mSelectComand - 1 + mNumComnd) % mNumComnd;
	}
	// 他のキーを処理するか、必要に応じて条件を追加します（例：'S'またはVK_DOWNの場合など）
	if (Input::GetKeyTrigger('S') || Input::GetKeyTrigger(VK_DOWN))
	{
		// 選択されたコマンドをデクリメントし、0より小さい場合は最大値にラップアラウンドする
		mSelectComand = (mSelectComand + 1 + mNumComnd) % mNumComnd;
	}
	// 現在の選択されたコマンドを返す
	return mSelectComand;
}