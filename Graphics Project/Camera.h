#pragma once
#include "Transform.h"
class Camera :
    public Transform
{

private:
    XMFLOAT4X4 m_viewMatrix;
    XMFLOAT4X4 m_projectionMatrix;
    float m_yaw = 0;
    float m_pitch = 0;
    float m_fov = 80;
public:
    Camera();
    
    
    void SetRotation(float yaw, float pitch, float roll) {
        m_yaw = yaw;
        m_pitch = pitch;
        m_isDirty = true;
    }
    void SetFov(float fov) {
        m_fov = fov;
        UpdateProjectionMatrix();
    }

    void UpdateFov(float fov) {
        m_fov += fov;
        UpdateProjectionMatrix();
    }
    void UpdateViewMatrix();
    void UpdateProjectionMatrix();
    void SetRotation(XMFLOAT4 rotation) {
        m_yaw = 2 * acos(rotation.x);
        m_pitch = 2 * acos(rotation.y);
        m_isDirty = true;
    };




    void Rotate(float yaw, float pitch, float roll);
    void SetCameraMatricies(SimpleVertexShader* vertexShader);
};

