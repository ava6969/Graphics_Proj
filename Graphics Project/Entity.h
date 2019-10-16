#pragma once

#include "Mesh.h"
#include "Material.h"
#include <DirectXMath.h>
#include "Transform.h"
using namespace DirectX;
class Entity :
    public Transform {


protected:

    Mesh* m_model;
    Material* m_material;


public:

    void SetMesh(Mesh* mesh) {
        m_model = mesh;
    }
    void setMaterial(Material* mat) {
        m_material = mat;
    }
    void setMaterialColor(XMFLOAT4 color) {
        m_material->matColor = color;
    }
    Mesh* GetMesh() {
        return m_model;
    }
    Material* GetMaterial() {
        return m_material;
    }

    Entity(Mesh* newModel, Material* mat);
    Entity();
    void Draw(ID3D11DeviceContext* context);
  
    void Draw(ID3D11DeviceContext* context, Material* matOverride);
};