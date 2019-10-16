#pragma once

#include "Mesh.h"
#include <DirectXMath.h>

using namespace DirectX;

class Transform
{


protected:
    XMFLOAT3 m_position;
    XMFLOAT4X4 m_transform;
    XMFLOAT3 m_scale;
    XMFLOAT4 m_rotation;
    bool m_isDirty = false;
    //TODO: Later modify children by parrent tranform
    Transform* children;

public:
    Transform();
    XMFLOAT3 GetPosition() { return m_position; };
    XMFLOAT4X4 GetTransform() {
        return m_transform;
    };
    XMFLOAT3 GetScale() {
        return m_scale;
    };
    XMFLOAT4 GetRotation() {
        return m_rotation;
    };



    void SetPosition(XMFLOAT3 position) {
        m_isDirty = true;
        m_position = position;
    };

    void SetPosition(float x, float y, float z) {

        m_position = XMFLOAT3(x, y, z);
        m_isDirty = true;
    };


    void SetScale(float scale) {
        m_scale = XMFLOAT3(scale, scale, scale);
        m_isDirty = true;
    }

    void SetRotation(XMFLOAT4 rotation) {
        m_rotation = rotation;
        m_isDirty = true;

    };

    void SetRotation(float yaw, float pitch, float roll) {
        XMStoreFloat4(&m_rotation, XMQuaternionRotationRollPitchYaw(roll, yaw, pitch));
        m_isDirty = true;
    }

    //Rotates the gameobject so it is facing another gameobject
    void LookAt(XMFLOAT3 position);


    void LookAt(Transform object) {
        LookAt(object.GetPosition());
    }

    //returns the lerp between two points 
    static XMFLOAT3 Lerp(XMFLOAT3 start, XMFLOAT3 end, float percentComplete);

    //lerps from the objects current location
    XMFLOAT3 LerpFromSelf(XMFLOAT3 end, float percentComplete) {
        return Transform::Lerp(m_position, end, percentComplete);
    }

    //moves the position x units in the specified direction
    void Translate(XMFLOAT3 position);

    void Translate(float x, float y, float z) {
        Translate(XMFLOAT3(x, y, z));
    }


    //translates in relation to the current axis
    void moveReltave(float x, float y, float z);

    //Rotates an object x radians in the direction
    void Rotate(float yaw, float pitch, float roll);


    void UpdateTransform();

};

