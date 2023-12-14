#include "ImguiOperator.h"

//自作
// 演算子 - のオーバーロードの実装
ImVec2 operator-(const ImVec2& v1, const ImVec2& v2) {
	return ImVec2(v1.x - v2.x, v1.y - v2.y);
}

// 演算子 - のオーバーロードの実装
ImVec2 operator+(const ImVec2& v1, const ImVec2& v2) {
	return ImVec2(v1.x + v2.x, v1.y + v2.y);
}

// 演算子 - のオーバーロードの実装
ImVec2 operator/(const ImVec2& v1, const ImVec2& v2) {
	return ImVec2(v1.x / v2.x, v1.y / v2.y);
}

// 演算子 - のオーバーロードの実装
ImVec2 operator*(const ImVec2& v1, const ImVec2& v2) {
	return ImVec2(v1.x * v2.x, v1.y * v2.y);
}