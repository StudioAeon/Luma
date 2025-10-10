#include "lmpch.hpp"
#include "SceneEnvironment.hpp"

#include "SceneRenderer.hpp"

namespace Luma {

	Environment Environment::Load(const std::string& filepath)
	{
		auto [radiance, irradiance] = SceneRenderer::CreateEnvironmentMap(filepath);
		return { filepath, radiance, irradiance };
	}
}