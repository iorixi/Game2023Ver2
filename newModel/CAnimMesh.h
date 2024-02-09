#pragma once

#include	"CStaticMesh.h"
#include	"CAnimationData.h"

class CAnimMesh : public CStaticMesh {
private:
	std::vector<aiMatrix4x4> m_bonecombmtxcontainer;		// �{�[���R���r�l�[�V�����s��̔z��
public:
	// �X�V
	void Update(CAnimationData& animdata, const char* animname, int Frame1);

	// �{�[���R���r�l�[�V�����s����K�w�\���ɏ]���čX�V����
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);

	// �{�[���R���r�l�[�V�����s��Q���擾
	const std::vector<aiMatrix4x4>& GetBoneCombinationMatrix() {
		return m_bonecombmtxcontainer;
	}
};