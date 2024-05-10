#include "pch.h"
#include "EditorCamera.h"
#include "Forge/Core/Input/Input.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Forge
{
    EditorCamera::EditorCamera(float fov, float viewportWidth, float viewportHeight, float nearClip, float farClip)
        : mFOV(fov), 
        mViewportWidth(viewportWidth), mViewportHeight(viewportHeight),
        mNearClip(nearClip), mFarClip(farClip)
    {
        UpdateView();
        UpdateProjection();
    }

    void EditorCamera::UpdateProjection()
    {
        /*mAspectRatio = mViewportWidth / mViewportHeight;
        mFOV = 30.0f;
        mNearClip = -1.0f;
        mFarClip = 100.0f;
        float orthoLeft = -mFOV * mAspectRatio * 0.5f;
        float orthoRight = mFOV * mAspectRatio * 0.5f;
        float orthoBottom = -mFOV * 0.5f;
        float orthoTop = mFOV * 0.5f;
        mProjection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, mNearClip, mFarClip);*/

        mProjection = glm::perspective(glm::radians(mFOV), mAspectRatio, mNearClip, mFarClip);
    }

    void EditorCamera::UpdateView()
    {
        mPosition = CalculatePosition();

        glm::quat orientation = GetOrientation();
        mViewMatrix = glm::translate(glm::mat4(1.0f), mPosition) * glm::toMat4(orientation);
        mViewMatrix = glm::inverse(mViewMatrix);
    }

    float EditorCamera::RotationSpeed() const
    {
        return 0.8f;
    }

    float EditorCamera::ZoomSpeed() const
    {
        float distance = mDistance * 0.2f;
        distance = std::max(distance, 0.0f);
        float speed = distance * distance;
        speed = std::min(speed, 100.0f);
        return speed;
    }

    void EditorCamera::Update(float dt)
    {
        const glm::vec2& mouse{ Input::GetMousePosition().first, Input::GetMousePosition().second };
        glm::vec2 delta = (mouse - mInitialMousePosition) * 0.003f;
        mInitialMousePosition = mouse;

        if (Input::IsMouseButtonPressed(MouseCode::ButtonLeft))
        {
            MouseRotate(delta);
        }

        glm::vec2 moveDir = { 0.0f, 0.0f };

        if (Input::IsKeyPressed(KeyCode::W))
        {
            moveDir.y += 1.0f;
        }
        if (Input::IsKeyPressed(KeyCode::A))
        {
            moveDir.x += 1.0f;
        }
        if (Input::IsKeyPressed(KeyCode::S))
        {
            moveDir.y -= 1.0f;
        }
        if (Input::IsKeyPressed(KeyCode::D))
        {
            moveDir.x -= 1.0f;
        }

        if (moveDir != glm::vec2(0.0f, 0.0f))
        {
            moveDir = glm::normalize(moveDir) * mSpeed * dt;
            MovePosition(moveDir);
        }

        UpdateView();
    }

    void EditorCamera::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(EditorCamera::OnMouseScroll));
    }

    bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
    {
        float delta = e.GetY_Offset() * 0.1f;
        MouseZoom(delta);
        UpdateView();
        return false;
    }

    void EditorCamera::MovePosition(const glm::vec2& delta)
    {
        mFocalPoint += -GetRightDirection() * delta.x * mDistance;
        mFocalPoint += GetForwardDirection() * delta.y * mDistance;
    }

    void EditorCamera::MouseRotate(const glm::vec2& delta)
    {
        float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
        mYaw += yawSign * delta.x * RotationSpeed();
        mPitch += delta.y * RotationSpeed();
    }

    void EditorCamera::MouseZoom(float delta)
    {
        mDistance -= delta * ZoomSpeed();
        if (mDistance < 1.0f)
        {
            mFocalPoint += GetForwardDirection();
            mDistance = 1.0f;
        }
    }

    glm::vec3 EditorCamera::GetUpDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetRightDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetForwardDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::vec3 EditorCamera::CalculatePosition() const
    {
        return mFocalPoint - GetForwardDirection() * mDistance;
    }

    glm::quat EditorCamera::GetOrientation() const
    {
        return glm::quat(glm::vec3(-mPitch, -mYaw, 0.0f));
    }
}