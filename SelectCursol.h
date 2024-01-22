#pragma once
#pragma once

#include"GameObject.h"
#include <vector>
#include "renderer.h"

/// <summary>
/// 現在選択されているカーソルの番号を取得するクラス
/// </summary>
class SelectCursol : public Component
{
private:
	int mNumComnd; //コマンドの数
	int mSelectComand;  // 今どのコマンドをえらんでいるか

	using Component::Component;

public:
	void Init(int num);
	//現在の選択されている番号を取得
	int Check();
	//引数 １コマンド数　２ 一番上のコマンドのｘ座標　３ 一番上のコマンドのｙ座標　４ コマンド同
};