#include "lmpch.hpp"
#include "Noise.hpp"

#include <FastNoise/FastNoise.h>

namespace Luma {

	static auto s_PerlinGenerator = FastNoise::New<FastNoise::Perlin>();

	float Noise::PerlinNoise(float x, float y)
	{
		return s_PerlinGenerator->GenSingle2D(x, y, 1337);
	}

	float Noise::SimplexNoise(float x, float y)
	{
		static auto simplexGen = FastNoise::New<FastNoise::Simplex>();
		return simplexGen->GenSingle2D(x, y, 1337);
	}

	float Noise::FractalNoise(float x, float y)
	{
		static auto fractalGen = FastNoise::New<FastNoise::FractalFBm>();
		fractalGen->SetSource(FastNoise::New<FastNoise::Simplex>());
		fractalGen->SetOctaveCount(4);
		return fractalGen->GenSingle2D(x, y, 1337);
	}

}