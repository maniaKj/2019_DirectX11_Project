#pragma once
#include "Component.h"
#include <stdio.h>
#include <fstream>

#include <crtdbg.h>
#if _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif

struct TERRAIN_INIT_DESC {
	std::vector<Vertex3D> vertexBuffer;
	std::vector<DWORD> indexBuffer;
};

class Terrain : public Component {
private:

	struct HeightMapType {
		float x, y, z;
		float nx, ny, nz;
		float tu, tv;
	};
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
	};
	struct VectorType
	{
		float x, y, z;
	};

public:
	Terrain();
	Terrain(const Terrain &);
	~Terrain();

	virtual void initialize(const COMPONENT_INIT_DESC & desc) override;
	TERRAIN_INIT_DESC TerrainProcess(const char * HeightMapPath);
	void Shutdown();
	void Redner();

	bool LoadHeightMap(const char* filename);
	void NormalizeHeightMap();
	bool CalculateNormals();
	void CalculateTextureCoordinates();
	TERRAIN_INIT_DESC CreateTerrainModel();

	int GetIndexCount();

	Transform* transform;
	Renderer * renderer;
	int Texture_Repeat_Count = 8;
	const char * heightFilePath;
	const COMPONENT_TYPE mType = COMPONENT_TERRAIN;

private:
	int m_terrainWidth = 0;
	int m_terrainHeight = 0;
	int m_vertexCount = 0;
	int m_indexCount = 0;
	HeightMapType * m_heightMap = nullptr;
};