#pragma once

namespace PanicEngine::Math
{
	struct RangeInt
	{
		int min = 0;
		int max = 0;

		RangeInt(int min, int max) : min(min), max(max) {}
		int GetRandom() const { return min + (rand() % (max - min)); }
		int GetRandomInc() const { return min + (rand() % ((max + 1) - min)); }
	};

	template<class T>
	struct Range
	{
		T min;
		T max;
		Range(const T& min, const T& max) : min(min), max(max) {}
		T GetRandom() const
		{
			const float t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			return min + ((max - min) * t);
		}
	};
}