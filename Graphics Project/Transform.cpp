#include "Transform.h"

Transform::Transform()
{
    m_transform = XMFLOAT4X4();
    m_position = XMFLOAT3();
    m_scale = XMFLOAT3(1, 1, 1);
    m_isDirty = true;
    XMStoreFloat4(&m_rotation, XMQuaternionIdentity());
}

void Transform::LookAt(XMFLOAT3 position)
{

    XMFLOAT3 lineBetween = XMFLOAT3(position.x - m_position.x, position.y - m_position.y, position.z - m_position.z);


    float angle_x = -atan2f(lineBetween.z, lineBetween.x);
    float angle_y = atan2f(lineBetween.y, sqrt((lineBetween.z * lineBetween.z) + (lineBetween.x * lineBetween.x)));
    float angle_z = 0;
    SetRotation(angle_x, angle_y, angle_z);
}

XMFLOAT3 Transform::Lerp(XMFLOAT3 start, XMFLOAT3 end, float percentComplete)
{
    float xPos = start.x + ((end.x - start.x) * percentComplete);
    float yPos = start.y + ((end.y - start.y) * percentComplete);
    float zPos = start.z + ((end.z - start.z) * percentComplete);
    XMFLOAT3 pos = XMFLOAT3(xPos, yPos, zPos);

    return pos;
}

void Transform::Translate(XMFLOAT3 position)
{
    m_position = XMFLOAT3(m_position.x + position.x, m_position.y + position.y, m_position.z + position.z);
    m_isDirty = true;
}

void Transform::moveReltave(float x, float y, float z)
{
    XMVECTOR dirVector = XMVector3Rotate(XMVectorSet(x, y, z, 0), XMLoadFloat4(&m_rotation));
    XMStoreFloat3(&m_position, XMLoadFloat3(&m_position) + dirVector);

    m_isDirty = true;
}

void Transform::Rotate(float yaw, float pitch, float roll)
{

    XMVECTOR newRotation = XMQuaternionMultiply(XMQuaternionRotationRollPitchYaw(roll, yaw, pitch), XMLoadFloat4(&m_rotation));
   
    XMStoreFloat4(&m_rotation, newRotation);

    m_isDirty = true;
}

void Transform::UpdateTransform()
{
    if (m_isDirty) {
        m_isDirty = false;
        XMMATRIX newTransform = XMMatrixIdentity();
        XMMATRIX newScale = XMMatrixScalingFromVector(XMLoadFloat3(&m_scale));
        XMMATRIX newPosition = XMMatrixTranslationFromVector(XMLoadFloat3(&m_position));
        XMMATRIX newRotation = XMMatrixRotationQuaternion(XMLoadFloat4(&m_rotation));
        newTransform = newTransform * newScale * newRotation * newPosition;

        XMStoreFloat4x4(&m_transform, XMMatrixTranspose(newTransform));
    }
}
