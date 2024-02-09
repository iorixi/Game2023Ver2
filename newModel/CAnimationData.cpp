#include	<cassert>
#include	"CAnimationData.h"

void CAnimationData::LoadAnimation(const std::string filename,const std::string name) {

	// importer��������ہ@�V�[������������̂ł������������ɂ��Ă�
	static Assimp::Importer importer;

	// �V�[�������\�z
	m_Animation[name] = importer.ReadFile(
		filename.c_str(),
		aiProcess_ConvertToLeftHanded);	
	assert(m_Animation[name]);

	if (m_Animation[name] == nullptr) {
		printf("load error %s ( %s ) \n ", filename.c_str(), importer.GetErrorString());
	}
}

// �w�肵�����O�̃A�j���[�V�������擾����
aiAnimation* CAnimationData::GetAnimation(const char* name,int idx) {

	aiAnimation* animation = m_Animation[name]->mAnimations[idx];

	return animation;
}
