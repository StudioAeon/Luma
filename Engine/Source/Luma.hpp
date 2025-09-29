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

#include "Luma/Events/Event.hpp"
#include "Luma/Events/ApplicationEvent.hpp"
#include "Luma/Events/KeyEvent.hpp"
#include "Luma/Events/MouseEvent.hpp"

#include <imgui.h>

// === Luma Render API ======================
#include "Luma/Renderer/Renderer.hpp"
#include "Luma/Renderer/VertexBuffer.hpp"
#include "Luma/Renderer/IndexBuffer.hpp"
// ==========================================