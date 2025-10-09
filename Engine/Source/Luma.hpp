//==============================================================================
// Luma Engine - Client Application Header
//
// This header is meant to be included **only** in client applications using the Luma Engine.
//
// **DO NOT** include this file in the engine's internal codebase.
//
// Including this file provides access to core engine functionality.
//
//==============================================================================

#pragma once

#include "Luma/Core/Application.hpp"
#include "Luma/Core/Log.hpp"
#include "Luma/Core/Input.hpp"
#include "Luma/Core/TimeStep.hpp"
#include "Luma/Core/Timer.hpp"
#include "Luma/Core/Version.hpp"

#include "Luma/Events/Event.hpp"
#include "Luma/Events/ApplicationEvent.hpp"
#include "Luma/Events/KeyEvent.hpp"
#include "Luma/Events/MouseEvent.hpp"

#include "Luma/Math/AABB.hpp"
#include "Luma/Math/Ray.hpp"

#include <imgui.h>

// === Luma Render API ======================
#include "Luma/Renderer/Buffer.hpp"
#include "Luma/Renderer/Camera.hpp"
#include "Luma/Renderer/Framebuffer.hpp"
#include "Luma/Renderer/Mesh.hpp"
#include "Luma/Renderer/Renderer.hpp"
#include "Luma/Renderer/Shader.hpp"
#include "Luma/Renderer/Texture.hpp"
#include "Luma/Renderer/Material.hpp"
#include "Luma/Renderer/VertexArray.hpp"
#include "Luma/Renderer/RenderPass.hpp"
#include "Luma/Renderer/SceneRenderer.hpp"
// ==========================================

// Scenes
#include "Luma/Scene/Entity.hpp"
#include "Luma/Scene/Scene.hpp"
#include "Luma/Scene/SceneCamera.hpp"
#include "Luma/Scene/SceneSerializer.hpp"
#include "Luma/Scene/Components.hpp"