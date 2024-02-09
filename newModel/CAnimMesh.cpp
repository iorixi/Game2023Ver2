#include	<iostream>
#include	"CAnimMesh.h"
#include "../renderer.h"

using namespace DirectX;

// ボーンコンビネーション行列を更新する
void CAnimMesh::Update(CAnimationData& animdata, const char* animname, int Frame1) {
	// ルート行列生成
	aiMatrix4x4 rootMatrix = aiMatrix4x4(
		aiVector3D(1.0f, 1.0f, 1.0f),						// scale
		aiQuaternion(0.0f, 0.0f, 0.0f),						// rotation
		aiVector3D(0.0f, 0.0f, 0.0f));						// transform

	//アニメーションデータからボーンマトリクス算出
	aiAnimation* animation1 = animdata.GetAnimation(animname, 0);

	for (unsigned int c = 0; c < animation1->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation1->mChannels[c];

		C_BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

		int f;

		f = Frame1 % nodeAnim->mNumRotationKeys;				//簡易実装
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = Frame1 % nodeAnim->mNumPositionKeys;				//簡易実装
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}

	//再帰的にボーンコンビネーション行列を更新
	UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

	// 定数バッファに書き込むための情報を生成
	m_bonecombmtxcontainer.clear();
	m_bonecombmtxcontainer.resize(m_Bone.size());
	for (auto data : m_Bone) {
		m_bonecombmtxcontainer[data.second.idx] = data.second.Matrix;
	}

	// DirectX用に転置する
	for (auto& bcmtx : m_bonecombmtxcontainer)
	{
		// 転置する
		bcmtx.Transpose();
	}
}

void CAnimMesh::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	// 引数で渡されたノード名をキーとしてボーン情報を取得する
	C_BONE* bone = &m_Bone[node->mName.C_Str()];

	//マトリクスの乗算順番に注意
	aiMatrix4x4 worldMatrix;					// 初期値は単位行列

	//	worldMatrix *= matrix;

	worldMatrix = matrix;						// 親の位置、姿勢が初期状態（逆ボーンオフセット行列）
	worldMatrix *= bone->AnimationMatrix;		// 引数で渡された行列を掛け算（自ノードのアニメーションを反映させる）（ボーン空間でのもの）

	bone->Matrix = worldMatrix;					// プログラム内に用意している行列に反映させる
	bone->Matrix *= bone->OffsetMatrix;			// オフセット行列を反映させる

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}