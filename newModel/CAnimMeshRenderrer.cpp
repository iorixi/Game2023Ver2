#include	"CAnimMeshRenderer.h"
#include	"CAnimMesh.h"

void CAnimMeshRenderer::Init(CAnimMesh& mesh) {
	// ���_�o�b�t�@����
	m_VertexBuffer.Create(mesh.GetVertices());

	// �C���f�b�N�X�o�b�t�@����
	m_IndexBuffer.Create(mesh.GetIndices());

	// �}�e���A�������擾���}�e���A�����ɒ萔�o�b�t�@�𐶐�����
	const std::vector<MATERIAL>& materials = mesh.GetMaterials();

	for (auto& mtrl : materials) {
		CMaterial* m = new CMaterial();
		m->Init(mtrl);
		m_Materials.emplace_back(m);
	}

	// �f�B�t���[�Y�e�N�X�`���e�[�u�������b�V������擾
	m_DiffuseSRVtable = mesh.GetSRVtable();

	// SUBSET���擾
	m_Subsets = mesh.GetSubsets();

	// �萔�o�b�t�@�����i�{�[���R���r�l�[�V�����i�[�p�j
	bool sts = CreateConstantBufferWrite(											// 20231227
		Renderer::GetDevice(),					    // �f�o�C�X�I�u�W�F�N�g			// 20231227
		sizeof(CBBoneCombMatrix),					// �R���X�^���g�o�b�t�@�T�C�Y	// 20231227
		&m_BoneCombMtxCBuffer);						// �R���X�^���g�o�b�t�@			// 20231227

	assert(m_BoneCombMtxCBuffer);
}

void CAnimMeshRenderer::Uninit() {
	for (auto& mtrl : m_Materials) {
		mtrl->Uninit();
		delete mtrl;
	}
}

void CAnimMeshRenderer::Draw() {
	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	// ���_�o�b�t�@�ݒ�
	m_VertexBuffer.SetGPU();
	// �C���f�b�N�X�o�b�t�@�ݒ�
	m_IndexBuffer.SetGPU();

	// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v�j
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 20231227 ���W�X�^�T�ɃZ�b�g
	devicecontext->VSSetConstantBuffers(5, 1, &m_BoneCombMtxCBuffer);

	// �T�u�Z�b�g�̐������[�v�i�T�u�Z�b�g���̓}�e���A���̐��j
	for (auto& subset : m_Subsets) {
		// SRV���Z�b�g
		devicecontext->PSSetShaderResources(0, 1, &m_DiffuseSRVtable[subset.MaterialIdx]);

		// �}�e���A���萔�o�b�t�@��GPU�ɃZ�b�g
		m_Materials[subset.MaterialIdx]->SetGPU();

		// �h���[�R�[��
		devicecontext->DrawIndexed(
			subset.IndexNum,	// �`�悷��C���f�b�N�X��
			subset.IndexBase,	// �C���f�b�N�X�o�b�t�@�x�[�X�J�n�ʒu�i���̏ꏊ���C���f�b�N�X�O�ԖڂɂȂ�j
			subset.VertexBase);	// ���_�o�b�t�@�x�[�X�J�n�ʒu�i���̏ꏊ�����_�̂O�ԖڂɂȂ�j
	}
}

// �{�[���R���r�l�[�V�����s��i�[�p
void CAnimMeshRenderer::Update(CAnimMesh& mesh)
{
	// ���b�V������X�V���ꂽ�{�[���R���r�l�[�V�����s��Q���擾
	const std::vector<aiMatrix4x4> mtxcontainer = mesh.GetBoneCombinationMatrix();

	// �萔�o�b�t�@�X�V
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	CBBoneCombMatrix* pData = nullptr;

	HRESULT hr = Renderer::GetDeviceContext()->Map(
		m_BoneCombMtxCBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&MappedResource);

	if (SUCCEEDED(hr)) {
		pData = (CBBoneCombMatrix*)MappedResource.pData;
		memcpy(pData->BoneCombMtx,
			mtxcontainer.data(),
			sizeof(aiMatrix4x4) * mtxcontainer.size());
		Renderer::GetDeviceContext()->Unmap(m_BoneCombMtxCBuffer, 0);
	}
}