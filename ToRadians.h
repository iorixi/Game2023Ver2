#pragma once
#include "renderer.h"

/// <summary>
/// �p�x�����W�A���ɕϊ�����֐�
/// </summary>

float ToRadians(float degrees)
{
	return degrees * DirectX::XM_PI / 180.0f;
}