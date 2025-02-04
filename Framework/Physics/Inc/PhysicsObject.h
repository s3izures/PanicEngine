#pragma once

namespace PanicEngine::Physics
{
	class PhysicsObject
	{
	public:
		PhysicsObject() = default;
		virtual ~PhysicsObject() = default;

	protected:
		friend class PhysicsWorld;

		virtual void SyncWithGraphics() = 0;
		virtual btRigidBody* GetRigidBody() { return nullptr; }
	};
}