#include "Camera.h"
#include "Game.h"
Camera::Camera()
    :Transform()
{
    //Set the default camera orientation
    SetRotation(-XM_PIDIV2, 0, 0);       
    UpdateProjectionMatrix();
   
}

void Camera::UpdateViewMatrix()
{
    if (m_isDirty) {
        XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(0, m_yaw, m_pitch);
        XMStoreFloat4(&m_rotation, rotation);
        XMVECTOR pos = XMLoadFloat3(&m_position);
        XMVECTOR forward = XMVector3Rotate(XMVectorSet(1, 0, 0, 0), rotation);
        XMVECTOR up = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), rotation);

        XMMATRIX V = XMMatrixLookToLH(
            pos,     // The position of the "camera"
            forward,     // Direction the camera is looking
            up);     // "Up" direction in 3D space (prevents roll)
        XMStoreFloat4x4(&m_viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!
        m_isDirty = false;
    }

}

void Camera::UpdateProjectionMatrix()
{
    float fovRad =  XMConvertToRadians(m_fov);

    XMMATRIX P = XMMatrixPerspectiveFovLH(
        fovRad,		// Field of View Angle
        (float)Game::s_width / Game::s_height,		// Aspect ratio
        0.1f,						// Near clip plane distance
        100.0f);					// Far clip plane distance
    XMStoreFloat4x4(&m_projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

void Camera::Rotate(float yaw, float pitch, float roll)
{
    m_yaw += yaw;
    m_pitch += pitch;

    m_isDirty = true;
}

void Camera::SetCameraMatricies(SimpleVertexShader* vertexShader)
{
    UpdateViewMatrix();
    vertexShader->SetMatrix4x4("view", m_viewMatrix);
    vertexShader->SetMatrix4x4("projection", m_projectionMatrix);
    vertexShader->CopyAllBufferData();
    vertexShader->SetShader();

}
