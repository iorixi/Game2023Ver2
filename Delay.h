#pragma once
#include "Time.h"
#include "component.h"


/// <summary>
/// 
/// 時間を遅延させるクラス
/// 
/// </summary>

namespace Timer
{

	class Delay : public Time
	{

		bool delayProgress = false;//指定時間が経過したか
		bool loop = false;//ループさせるか
		float delayTime = 0.0f;//遅延時間

	public:

		using Time::Time;

		void Init() override;
		void Update() override;

		/// <summary>
		/// 経過時間が経過したかどうか
		/// 戻り値：設定した時間に到達したか
		/// </summary>
		bool DelayProgrees();

		/// <summary>
		/// 遅延時間を入力
		/// 引数：遅延時間
		/// </summary>
		void SetdelayTime(float _delayTime);

		/// <summary>
		/// ループさせるか
		/// 引数：ループ機能をありにするか（何秒間に一回などはあり）
		/// </summary>
		void SetLoop(bool _loop);


	private:

	};
}

