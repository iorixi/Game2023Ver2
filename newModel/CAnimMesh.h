#pragma once

#include	"CStaticMesh.h"
#include	"CAnimationData.h"

class CAnimMesh : public CStaticMesh {
private:
	std::vector<aiMatrix4x4> m_bonecombmtxcontainer;		// ボーンコンビネーション行列の配列
public:
	// 更新
	void Update(CAnimationData& animdata, const char* animname, int Frame1);

	// ボーンコンビネーション行列を階層構造に従って更新する
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);

	// ボーンコンビネーション行列群を取得
	const std::vector<aiMatrix4x4>& GetBoneCombinationMatrix() {
		return m_bonecombmtxcontainer;
	}
};