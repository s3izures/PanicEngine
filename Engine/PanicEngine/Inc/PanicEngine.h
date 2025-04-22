#pragma once

#include "Common.h"
#include "App.h"
#include "AppState.h"
#include "Event.h"
#include "EventManager.h"

//Game Object
#include "GameWorld.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "TypeIds.h"
#include "GameObjectHandle.h"

//Components
#include "Component.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"
#include "MeshComponent.h"
#include "RenderObjectComponent.h" //no type id
#include "TransformComponent.h"

//Services
#include "Service.h"
#include "CameraService.h"
#include "RenderService.h"

namespace PanicEngine
{
    App& MainApp();
}