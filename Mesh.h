#pragma once

#include "DXCore.h"
#include "Vertex.h"
#include <DirectXMath.h>
#include <fstream>
#include <vector>
#include <d3d11.h>

class Mesh
{
public:
	Mesh();
	Mesh(Vertex* verts, int numVerts, unsigned int* indexes, int numIndices, ID3D11Device* device);
	Mesh(const char* filename, ID3D11Device* device);
	~Mesh();

	// helper functions
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();
private:
	void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);
	// the vertex and index buffers
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// how many indices are in the index buffer
	int numIndices;
};

