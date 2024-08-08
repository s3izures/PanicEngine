#include "Precompiled.h"
#include "Camera.h"
#include "GraphicsSystem.h"

using namespace PanicEngine;
using namespace PanicEngine::Graphics;

void Camera::SetMode(ProjectionMode mode)
{
    mProjectionMode = mode;
}

void Camera::SetPosition(const Math::Vector3& position)
{
    mPosition = position;
}

void Camera::SetDirection(const Math::Vector3& direction)
{
    Math::Vector3 dir = Math::Normalize(direction);
    if (Math::Abs(Math::Dot(dir, Math::Vector3::YAxis) < 0.995f)) //No directy up to avoid gimbal lock
    {
        mDirection = dir;
    }
}

void Camera::SetLookAt(const Math::Vector3& target)
{
    SetDirection(target - mPosition);
}

void Camera::SetFov(float fov)
{
    constexpr float kMinFov = 10.0f * Math::Constants::DegToRad;
    constexpr float kMaxFov = 170.0f * Math::Constants::DegToRad;
    mFov = Math::Clamp(fov, kMinFov, kMaxFov);
}

void Camera::SetAspectRatio(float ratio)
{
    mAspectRatio = ratio;
}

void Camera::SetSize(float width, float height)
{
    mWidth = width;
    mHeight = height;
}

void Camera::SetNearPlane(float nearPlane)
{
    mNearPlane = nearPlane;
}

void Camera::SetFarPlane(float farPlane)
{
    mFarPlane = farPlane;
}

const Math::Vector3& Camera::GetPosition() const
{
    return mPosition;
}

const Math::Vector3& Camera::GetDirection() const
{
    return mDirection;
}

Math::Matrix4 Camera::GetViewMatrix() const
{
    const Math::Vector3 l = mDirection;
    const Math::Vector3 r = Math::Normalize(Math::Cross(Math::Vector3::YAxis, l));
    const Math::Vector3 u = Math::Normalize(Math::Cross(l, r));
    const float a = -Math::Dot(r, mPosition);
    const float b = -Math::Dot(u, mPosition);
    const float c = -Math::Dot(l, mPosition);
    return
    {
        r.x, u.x, l.x, 0.0f,
        r.y, u.y, l.y, 0.0f,
        r.z, u.z, l.z, 0.0f,
        a  , b  , c  , 1.0f
    };
}

Math::Matrix4 Camera::GetProjectionMatrix() const
{
    return (mProjectionMode == ProjectionMode::Prespective) ? GetPerspectiveMatrix() : GetOrthographicMatrix();
}

Math::Matrix4 Camera::GetPerspectiveMatrix() const
{
    const float a = (mAspectRatio == 0.0f) ? GraphicsSystem::Get()->GetBackBufferAspectRatio() : mAspectRatio;
    const float h = 1.0f / tan(mFov * 0.5f);
    const float w = h / a;
    const float zf = mFarPlane;
    const float zn = mNearPlane;
    const float q = zf / (zf - zn);
    return
    {
        w   , 0.0f, 0.0f, 0.0f,
        0.0f, h   , 0.0f, 0.0f,
        0.0f, 0.0f, q   , 1.0f,
        0.0f, 0.0f, -zn * q, 0.0f
    };
}

Math::Matrix4 Camera::GetOrthographicMatrix() const
{
    const float w = (mWidth == 0.0f) ? GraphicsSystem::Get()->GetBackBufferWidth() : mWidth;
    const float h = (mHeight == 0.0f) ? GraphicsSystem::Get()->GetBackBufferWidth() : mHeight;
    const float f = mFarPlane;
    const float n = mNearPlane;
    return
    {
        2.0f / w, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / h, 0.0f, 0.0f,
        0.0f, 0.0f, 1/(f-n) ,0.0f,
        0.0f, 0.0f, n/(n-f) , 1.0f
    };
}
