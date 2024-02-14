#pragma once

#include "gameObject.h"
#include "newModel/CMaterial.h"

class Transition : public GameObject
{
public:
	enum class State
	{
		Stop,			// ��~
		In,				// �t�F�[�h�C��
		Out,			// �t�F�[�h�A�E�g
		Finish			// ��ʑJ�ڏI��
	};

private:

	float m_Alpha = 0.0f;
	State m_State = State::Stop;

	class Sprite* m_Sprite{};
	CMaterial m_Material;

public:

	void Init() override;
	void Update() override;
	void PreDraw() override;

	State GetState() { return m_State; }

	void FadeIn()
	{
		m_Alpha = 1.0f;
		m_State = State::In;
	}

	void FadeOut() {
		m_Alpha = 0.0f;
		m_State = State::Out;
	}
};