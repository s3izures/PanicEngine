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

//Components
#include "Component.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"
#include "TransformComponent.h"

//Services
#include "Service.h"
#include "CameraService.h"

namespace PanicEngine
{
    App& MainApp();
}