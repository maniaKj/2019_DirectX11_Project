#include "Mesh.h"

Mesh::Mesh(ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::vector<Vertex3D>& vertices, std::vector<DWORD>& indices, std::vector<Texture> & textures, const DirectX::XMMATRIX & transformMatrix)
{
	this->deviceContext = deviceContext;
	this->textures = textures;
	this->transformMatrix = transformMatrix;

	try {
		HRESULT hr = this->vertexbuffer.Initialize(device, vertices.data(), vertices.size());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer for mesh.");

		hr = this->indexbuffer.Initialize(device, indices.data(), indices.size());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer for mesh.");
	}
	catch (COMException e) {
		ErrorLogger::Log(e);
		return ;
	}
	
}

Mesh::Mesh(const Mesh & mesh)
{
	this->deviceContext = mesh.deviceContext;
	this->indexbuffer = mesh.indexbuffer;
	this->vertexbuffer = mesh.vertexbuffer;
	this->textures = mesh.textures;
	this->transformMatrix = mesh.transformMatrix;
}

void Mesh::Draw()
{
	UINT offset = 0;

	for (int i = 0; i < textures.size(); i++) {//���� ���߿� ����ȭ
		if (textures[i].GetType() == aiTextureType::aiTextureType_DIFFUSE) {
			this->deviceContext->PSSetShaderResources(i, 1, textures[i].GetTextureResourceViewAddress()); //�ȼ� ���̴��� �ؽ��� ����
			break;
		}
	}

	this->deviceContext->IASetVertexBuffers(0, 1, this->vertexbuffer.GetAddressOf(), this->vertexbuffer.StridePtr(), &offset);
	/* ������ ������ �������� ������ �������������� �����ϱ� ���� ���۸� ��ġ�� �� �Է� ���Կ� ����.
	virtual void STDMETHODCALLTYPE IASetVertexBuffers( 
	_In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,	//���� ���۵��� ���̱� ������ �Է� ������ ����
		_In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumBuffers,	//�� ȣ�⿡�� �Է� ���Ե鿡 ���� ���۰���
		_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppVertexBuffers,	//���� ���� ���۵��� ���� �迭�� ù ���Ҹ� ����Ű�� ������
		_In_reads_opt_(NumBuffers)  const UINT *pStrides,	//�������� �迭�� ù ���Ҹ� ����Ű�� ������
		_In_reads_opt_(NumBuffers)  const UINT *pOffsets) = 0;	//�����µ��� �迭�� ù ���Ҹ� ����Ű�� ������. 
		//���⼭ �������� ���� ������ġ���� ��ǥ�ϴ� ���� ��ġ���� �Ÿ�
	*/
	this->deviceContext->IASetIndexBuffer(this->indexbuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	this->deviceContext->DrawIndexed(this->indexbuffer.IndexCount(), 0, 0);
	/*
	virtual void STDMETHODCALLTYPE DrawIndexed( 
		_In_  UINT IndexCount,				//�� �׸��� ȣ�⿡�� ����� �ε��� ����
		_In_  UINT StartIndexLocation,		//����� ù ������ ��ġ
		_In_  INT BaseVertexLocation) = 0;	//�������� �������� ����, �� �׸��� ȣ�⿡�� ����� ���ε鿡 �������� ���� ��
	*/

}

const DirectX::XMMATRIX & Mesh::GetTransformMatrix()
{
	return this->transformMatrix;
}
