#include "ImguiOperator.h"

//����
// ���Z�q - �̃I�[�o�[���[�h�̎���
ImVec2 operator-(const ImVec2& v1, const ImVec2& v2) {
	return ImVec2(v1.x - v2.x, v1.y - v2.y);
}

// ���Z�q - �̃I�[�o�[���[�h�̎���
ImVec2 operator+(const ImVec2& v1, const ImVec2& v2) {
	return ImVec2(v1.x + v2.x, v1.y + v2.y);
}

// ���Z�q - �̃I�[�o�[���[�h�̎���
ImVec2 operator/(const ImVec2& v1, const ImVec2& v2) {
	return ImVec2(v1.x / v2.x, v1.y / v2.y);
}

// ���Z�q - �̃I�[�o�[���[�h�̎���
ImVec2 operator*(const ImVec2& v1, const ImVec2& v2) {
	return ImVec2(v1.x * v2.x, v1.y * v2.y);
}