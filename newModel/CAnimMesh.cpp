#include	<iostream>
#include	"CAnimMesh.h"
#include "../renderer.h"

using namespace DirectX;

// �{�[���R���r�l�[�V�����s����X�V����
void CAnimMesh::Update(CAnimationData& animdata, const char* animname, int Frame1) {
	// ���[�g�s�񐶐�
	aiMatrix4x4 rootMatrix = aiMatrix4x4(
		aiVector3D(1.0f, 1.0f, 1.0f),						// scale
		aiQuaternion(0.0f, 0.0f, 0.0f),						// rotation
		aiVector3D(0.0f, 0.0f, 0.0f));						// transform

	//�A�j���[�V�����f�[�^����{�[���}�g���N�X�Z�o
	aiAnimation* animation1 = animdata.GetAnimation(animname, 0);

	for (unsigned int c = 0; c < animation1->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation1->mChannels[c];

		C_BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

		int f;

		f = Frame1 % nodeAnim->mNumRotationKeys;				//�ȈՎ���
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = Frame1 % nodeAnim->mNumPositionKeys;				//�ȈՎ���
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}

	//�ċA�I�Ƀ{�[���R���r�l�[�V�����s����X�V
	UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

	// �萔�o�b�t�@�ɏ������ނ��߂̏��𐶐�
	m_bonecombmtxcontainer.clear();
	m_bonecombmtxcontainer.resize(m_Bone.size());
	for (auto data : m_Bone) {
		m_bonecombmtxcontainer[data.second.idx] = data.second.Matrix;
	}

	// DirectX�p�ɓ]�u����
	for (auto& bcmtx : m_bonecombmtxcontainer)
	{
		// �]�u����
		bcmtx.Transpose();
	}
}

void CAnimMesh::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	// �����œn���ꂽ�m�[�h�����L�[�Ƃ��ă{�[�������擾����
	C_BONE* bone = &m_Bone[node->mName.C_Str()];

	//�}�g���N�X�̏�Z���Ԃɒ���
	aiMatrix4x4 worldMatrix;					// �����l�͒P�ʍs��

	//	worldMatrix *= matrix;

	worldMatrix = matrix;						// �e�̈ʒu�A�p����������ԁi�t�{�[���I�t�Z�b�g�s��j
	worldMatrix *= bone->AnimationMatrix;		// �����œn���ꂽ�s����|���Z�i���m�[�h�̃A�j���[�V�����𔽉f������j�i�{�[����Ԃł̂��́j

	bone->Matrix = worldMatrix;					// �v���O�������ɗp�ӂ��Ă���s��ɔ��f������
	bone->Matrix *= bone->OffsetMatrix;			// �I�t�Z�b�g�s��𔽉f������

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}