#pragma once

#include "Common.h"

#include "App.h"
#include "AppState.h"
#include "Event.h"
#include "EventManager.h"

// Components
#include "Component.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"
#include "MeshComponent.h"
#include "ModelComponent.h"
#include "RenderObjectComponent.h"
#include "SoundBankComponent.h"
#include "SoundEffectComponent.h"
#include "TransformComponent.h"
#include "UIComponent.h"
#include "UIButtonComponent.h"
#include "UISpriteComponent.h"

// Game Object
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "GameWorld.h"
#include "TypeIds.h"
#include "GameObjectHandle.h"

// Services
#include "Service.h"
#include "CameraService.h"
#include "RenderService.h"
#include "UIRenderService.h"

namespace PanicEngine
{
    App& MainApp();
}