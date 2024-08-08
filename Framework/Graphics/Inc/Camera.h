#pragma once

namespace PanicEngine::Graphics
{
    class Camera
    {
    public:
        enum class ProjectionMode
        {
            Prespective,
            Orthographic
        };

        void SetMode(ProjectionMode mode);
        void SetPosition(const Math::Vector3& position);
        void SetDirection(const Math::Vector3& direction);
        void SetLookAt(const Math::Vector3& target);

        void SetFov(float fov);
        void SetAspectRatio(float ratio);

        void SetSize(float width, float height);
        void SetNearPlane(float nearPlane);
        void SetFarPlane(float farPlane);

        //TBA: GIVE MOVEMENT CALLS

        const Math::Vector3& GetPosition() const;
        const Math::Vector3& GetDirection() const;
        Math::Matrix4 GetViewMatrix() const;
        Math::Matrix4 GetProjectionMatrix() const;

        //ProjectionMode transforms
        Math::Matrix4 GetPerspectiveMatrix() const;
        Math::Matrix4 GetOrthographicMatrix() const;

    private:
        ProjectionMode mProjectionMode = ProjectionMode::Prespective;
        Math::Vector3 mPosition = Math::Vector3::Zero;
        Math::Vector3 mDirection = Math::Vector3::ZAxis;

        float mFov = 60.0f * Math::Constants::DegToRad;
        float mAspectRatio = 0.0f;
        float mWidth = 0.0f;
        float mHeight = 0.0f;

        float mNearPlane = 0.01f;
        float mFarPlane = 10000.0f;
    };
}