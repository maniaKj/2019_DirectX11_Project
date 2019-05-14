#pragma once
#include "Mesh.h"
#include <map>

struct ASSIMP_API aiNode;
struct aiScene;
struct aiMesh;
struct aiAnimation;
struct BoneChannel;
class AnimationClip;
class Animator;

class Model {
	friend class GraphicsManager;
public:
	bool Initialize(const std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_vertexshader> & cb_vs_vertexshader, ConstantBuffer<CB_VS_boneData>& cb_vs_boneData, std::vector<AnimationClip> * _animClipDestination);
	bool Initialize(const std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_vertexshader> & cb_vs_vertexshader);
	bool Initialize(std::vector<Vertex3D> * VertexBuffer, std::vector<DWORD> * IndexBuffer, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_vertexshader> & cb_vs_vertexshader);
	bool Initialize(Vertex3D * _VertexBuffer, const UINT _vertexSize, DWORD * _IndexBuffer, const UINT _indexSize, ID3D11Device * _device, ID3D11DeviceContext * _deviceContext, ConstantBuffer<CB_VS_vertexshader> & _cb_vs_vertexshader);
	void Draw(const DirectX::XMMATRIX & worldMatrix, const DirectX::XMMATRIX & viewProjectionMatrix);
	void Draw_WireFrame(const DirectX::XMMATRIX & worldMatrix, const DirectX::XMMATRIX & viewProjectionMatrix);
	void Draw_skinnedMesh(const DirectX::XMMATRIX & worldMatrix, const DirectX::XMMATRIX & viewProjectionMatrix, Animator * _animator);

protected:
	std::vector<Mesh> meshes;
	bool LoadModel(const std::string & filePath);
	bool LoadModel(const std::string & filePath, std::vector<AnimationClip> * _animClipDestination);
	void ProcessNode(aiNode * node, const aiScene * scene, const DirectX::XMMATRIX & parentTransformMatrix);
	Mesh ProcessMesh(aiMesh * mesh, const aiScene * scene, const DirectX::XMMATRIX & transformMatrix);
	Mesh ProcessMesh(std::vector<Vertex3D> * _vertexBuffer, std::vector<DWORD> * _indexBuffer);
	Mesh ProcessMesh(std::vector<Vertex3D_BoneWeight>* _vertexBuffer, std::vector<DWORD>* _indexBuffer);
	Mesh ProcessMesh(Vertex3D * _vertexBuffer, const int _vertexSize, DWORD * _indexBuffer, const int _indexSize);
	void ProcessAnimation(aiAnimation * _aiAnim, const aiScene * _aiScene, std::vector<AnimationClip> * _animClipDestination);
	void ProcessBone(aiMesh * _mesh, const aiScene * _scene, std::vector<MyCustom::Bone> & _boneBuffer, std::vector<MyCustom::Vertex_Bone_Data> & _VB_buffer);
	void ProcessBoneHierarchy(aiNode * _aiNode, AnimationClip * _animClip, BoneChannel * _parentBone, const DirectX::XMMATRIX & _parentTransform);
	TextureStorageType DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType);
	Mesh ProcessTerrain(std::vector<Vertex3D> * terrainVertexBuffer, std::vector<DWORD> * terrainIndexBuffer);


	std::vector<Texture> LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene);
	int GetTextureIndex(aiString * pStr);

	ID3D11Device * device = nullptr;
	ID3D11DeviceContext * deviceContext = nullptr;
	ConstantBuffer<CB_VS_vertexshader> * cb_vs_vertexshader = nullptr;
	ConstantBuffer<CB_VS_boneData> * cb_vs_boneData = nullptr;
	std::string directory = "";

#pragma region Animation Variable
	std::map<std::string, UINT> m_Bone_Name_Map;
	std::vector<MyCustom::Bone> mBoneBuffer;
	UINT mBoneCount;

	void nodeName(std::vector<std::string> * na, aiNode* node);
#pragma endregion
};

class SkinnedModel : public Model {
	friend class GraphicsManager;
public:

private:

};