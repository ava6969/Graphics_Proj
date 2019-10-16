#include "Entity.h"



Entity::Entity(Mesh* newModel, Material* mat)
{
    //LookAt(XMFLOAT3(m_position.x, m_position.y, m_position.z + 1));
    m_model = newModel;
    m_material = mat;
}

Entity::Entity()
    :Transform()
{
    m_model = 0;
    m_material = 0;

}

void Entity::Draw(ID3D11DeviceContext* context)
{
    UpdateTransform();

    SimpleVertexShader* vertexShader = m_material->GetVertex();
    SimplePixelShader* pixelShader = m_material->GetPixel();
    

    pixelShader->SetSamplerState("basicSampler", m_material->GetSampler());
    pixelShader->SetShaderResourceView("diffuseTexture", m_material->GetSRV());
    pixelShader->SetFloat4("surfaceColor", m_material->matColor);
    pixelShader->CopyAllBufferData();
    pixelShader->SetShader();
    vertexShader->SetMatrix4x4("world", m_transform);
    vertexShader->CopyAllBufferData();
    vertexShader->SetShader();

    m_model->DrawMesh(context, 0);
}



void Entity::Draw(ID3D11DeviceContext* context, Material* matOverride)
{
    UpdateTransform();

    SimpleVertexShader* vertexShader = matOverride->GetVertex();
    SimplePixelShader* pixelShader = matOverride->GetPixel();

    pixelShader->SetShader();
    vertexShader->SetMatrix4x4("world", m_transform);
    vertexShader->CopyAllBufferData();
    vertexShader->SetShader();

    m_model->DrawMesh(context, 0);
}
