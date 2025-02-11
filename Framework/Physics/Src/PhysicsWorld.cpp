#include "Precompiled.h"
#include "PhysicsWorld.h"
#include "PhysicsObject.h"

using namespace PanicEngine;
using namespace PanicEngine::Physics;

namespace
{
	std::unique_ptr<PhysicsWorld> sPhysicsWorld;
}

void PhysicsWorld::StaticInitialize(const Settings& settings)
{
	ASSERT(sPhysicsWorld == nullptr, "PhysicsWorld: is already initialized.");
	sPhysicsWorld = std::make_unique<PhysicsWorld>();
	sPhysicsWorld->Initialize(settings);
}

void PhysicsWorld::StaticTerminate()
{
	if (sPhysicsWorld != nullptr)
	{
		sPhysicsWorld->Terminate();
		sPhysicsWorld.reset();
	}
}

PhysicsWorld* PhysicsWorld::Get()
{
	ASSERT(sPhysicsWorld != nullptr, "PhysicsWorld: must be intialized");
	return sPhysicsWorld.get();
}

PhysicsWorld::~PhysicsWorld()
{
	ASSERT(mDynamicsWorld == nullptr, "PhysicsWorld: must be terminated");
}

void PhysicsWorld::Initialize(const Settings& settings)
{
	mSettings = settings;
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
	mInterface = new btDbvtBroadphase();
	mSolver = new btSequentialImpulseConstraintSolver();
	mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mInterface, mSolver, mCollisionConfiguration);
	mDynamicsWorld->setGravity(TobtVector3(settings.gravity));
	mDynamicsWorld->setDebugDrawer(&mPhysicsDebugDraw);
}

void PhysicsWorld::Terminate()
{
	SafeDelete(mDynamicsWorld);
	SafeDelete(mSolver);
	SafeDelete(mInterface);
	SafeDelete(mDispatcher);
	SafeDelete(mCollisionConfiguration);
}

void PhysicsWorld::Update(float deltaTime)
{
	mDynamicsWorld->stepSimulation(deltaTime, mSettings.simulationSteps, mSettings.fixedTimeStep);
	for (PhysicsObject* po : mPhysicsObjects)
	{
		po->SyncWithGraphics();
	}
}

void PhysicsWorld::DebugUI()
{
	if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Gravity", &mSettings.gravity.x, 0.1f))
		{
			mDynamicsWorld->setGravity(TobtVector3(mSettings.gravity));
		}
		ImGui::Checkbox("DebugDraw", &mDebugDraw);
		if (mDebugDraw)
		{
			ImGui::Indent();

			int debugMode = mPhysicsDebugDraw.getDebugMode();
			bool isEnabled = (debugMode & btIDebugDraw::DBG_DrawWireframe);
			if (ImGui::Checkbox("DrawWireFrame", &isEnabled))
			{
				debugMode = (isEnabled) ? debugMode | btIDebugDraw::DBG_DrawWireframe : debugMode & ~btIDebugDraw::DBG_DrawWireframe;
			}
			isEnabled = (debugMode & btIDebugDraw::DBG_DrawAabb);
			if (ImGui::Checkbox("DrawAABB", &isEnabled))
			{
				debugMode = (isEnabled) ? debugMode | btIDebugDraw::DBG_DrawAabb : debugMode & ~btIDebugDraw::DBG_DrawAabb;
			}

			mPhysicsDebugDraw.setDebugMode(debugMode);
			mDynamicsWorld->debugDrawWorld();
			ImGui::Unindent();
		}
	}
}

void PhysicsWorld::Register(PhysicsObject* physicsObject)
{
	auto iter = std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), physicsObject);
	if (iter == mPhysicsObjects.end())
	{
		mPhysicsObjects.push_back(physicsObject);
		if (physicsObject->GetRigidBody())
		{
			mDynamicsWorld->addRigidBody(physicsObject->GetRigidBody());
		}
	}
}

void PhysicsWorld::Unregister(PhysicsObject* physicsObject)
{
	auto iter = std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), physicsObject);
	if (iter != mPhysicsObjects.end())
	{
		mPhysicsObjects.push_back(physicsObject);
		if (physicsObject->GetRigidBody())
		{
			mDynamicsWorld->removeRigidBody(physicsObject->GetRigidBody());
		}
		mPhysicsObjects.erase(iter);
	}
}

void PhysicsWorld::SetGravity(const Math::Vector3& gravity)
{
	mSettings.gravity = gravity;
	mDynamicsWorld->setGravity(TobtVector3(gravity));
}
