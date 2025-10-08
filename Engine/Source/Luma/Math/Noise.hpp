#pragma once

namespace Luma {

	class Noise
	{
	public:
		static float PerlinNoise(float x, float y);
		static float SimplexNoise(float x, float y);
		static float FractalNoise(float x, float y);

		static float PerlinNoiseNormalized(float x, float y)
		{
			return (PerlinNoise(x, y) + 1.0f) * 0.5f; // Convert [-1,1] to [0,1]
		}

		static float PerlinNoiseOctave(float x, float y, int octaves = 4, float persistence = 0.5f);
	};

}