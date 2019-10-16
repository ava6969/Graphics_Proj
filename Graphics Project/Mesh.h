#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include "Vertex.h"
#include <fstream> 
using namespace std;

class Mesh {
public:
 
    Mesh(Vertex* vertices, int vertexCount, int* indecies, int indexCount, ID3D11Device *device);
    Mesh(const char* objFile, ID3D11Device* device);
    Mesh();
    ~Mesh();
    ID3D11Buffer* GetVertexBuffer();
    ID3D11Buffer* GetIndexBuffer();

    void DrawMesh(ID3D11DeviceContext* context, UINT meshIndex);

private:
    ID3D11Buffer* m_vertexBuffer;
    ID3D11Buffer* m_indexBuffer;

    void CreateVertexBuffer(Vertex* vertices, int vertexCount, ID3D11Device* device);
    void CreateIndexBuffer(UINT* indecies, ID3D11Device* device);

    int m_indexCount;
    
};