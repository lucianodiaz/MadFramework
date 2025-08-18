#pragma once
#include <cmath>
#include <corecrt_math_defines.h>

// Easing functions for animations
namespace MAD::Easings
{

	//linear easing function
	inline float Linear(float t)
	{
		return t;
	}

	inline float easeInSine(float t)
	{
		return 1.0f - static_cast<float>(std::cos((t * M_PI) / 2.0f));
	}

	inline float easeOutSine(float t)
	{
		return static_cast<float>(std::sin((t * M_PI) / 2.0f));
	}

	inline float easeInOutSine(float t)
	{
		return -(static_cast<float>(std::cos(M_PI * t)) - 1.0f) / 2.0f;
	}

	inline float easeInQuad(float t)
	{
		return t * t;
	}

	inline float easeOutQuad(float t)
	{
		return 1.0f - (1.0f - t) * (1.0f - t);
	}

	inline float easeInOutQuad(float t)
	{
		if (t < 0.5f)
			return 2.0f * t * t;
		else
			return 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
	}

	inline float easeInCubic(float t)
	{
		return t * t * t;
	}

	inline float easeOutCubic(float t)
	{
		return 1.0f - std::pow(1.0f - t, 3.0f);
	}

	inline float easeInOutCubic(float t)
	{
		if (t < 0.5f)
			return 4.0f * t * t * t;
		else
			return 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
	}

	inline float easeInQuart(float t)
	{
		return t * t * t * t;
	}

	inline float easeOutQuart(float t)
	{
		return 1.0f - std::pow(1.0f - t, 4.0f);
	}

	inline float easeInOutQuart(float t)
	{
		if (t < 0.5f)
			return 8.0f * t * t * t * t;
		else
			return 1.0f - std::pow(-2.0f * t + 2.0f, 4.0f) / 2.0f;
	}

	inline float easeInQuint(float t)
	{
		return t * t * t * t * t;
	}

	inline float easeOutQuint(float t)
	{
		return 1.0f - std::pow(1.0f - t, 5.0f);
	}

	inline float easeInOutQuint(float t)
	{
		if (t < 0.5f)
			return 16.0f * t * t * t * t * t;
		else
			return 1.0f - std::pow(-2.0f * t + 2.0f, 5.0f) / 2.0f;
	}

	inline float easeInExpo(float t)
	{
		return (t == 0.0f) ? 0.0f : std::pow(2.0f, 10.0f * t - 10.0f);
	}

	inline float easeOutExpo(float t)
	{
		return (t == 1.0f) ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
	}

	inline float easeInOutExpo(float t)
	{
		if (t == 0.0f || t == 1.0f)
			return t;
		if (t < 0.5f)
			return std::pow(2.0f, 20.0f * t - 10.0f) / 2.0f;
		else
			return (2.0f - std::pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
	}

	inline float easeInCirc(float t)
	{
		return 1.0f - std::sqrt(1.0f - (t * t));
	}

	inline float easeOutCirc(float t)
	{
		return std::sqrt(1.0f - std::pow(t - 1.0f, 2.0f));
	}

	inline float easeInOutCirc(float t)
	{
		if (t < 0.5f)
			return (1.0f - std::sqrt(1.0f - std::pow(2.0f * t, 2.0f))) / 2.0f;
		else
			return (std::sqrt(1.0f - std::pow(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f;
	}

	inline float easeInBack(float t)
	{
		constexpr float c1 = 1.70158f;
		constexpr float c3 = c1 + 1.0f;

		return c3 * t * t * t - c1 * t * t;
	}

	inline float easeOutBack(float t)
	{
		constexpr float c1 = 1.70158f;
		constexpr float c3 = c1 + 1.0f;
		return 1.0f + c3 * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f);
	}

	inline float easeInOutBack(float t)
	{
		constexpr float c1 = 1.70158f;
		constexpr float c2 = c1 * 1.525f;
		if (t < 0.5f)
			return (std::pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f;
		else
			return (std::pow(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
	}


	inline float easeInElastic(float t)
	{
		constexpr float c4 = static_cast<float>((2.0f * M_PI) / 3.0f);

		return (t == 0.0f)
			? 0.0f 
			: (t == 1.0f)
			? 1.0f 
			: static_cast<float>( - std::pow(2.0f, 10.0f * t - 10.0f)) * static_cast<float>(std::sin((t * 10.0f - 10.75f) * c4));
	}

	inline float easeOutElastic(float t)
	{
		constexpr float c4 = static_cast<float>((2.0f * M_PI) / 3.0f);
		return (t == 0.0f)
			? 0.0f
			: (t == 1.0f)
			? 1.0f
			: static_cast<float>(std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f);
	}

	inline float easeInOutElastic(float t)
	{
		constexpr float c5 = static_cast<float>((2.0f * M_PI) / 4.5f);
		if (t == 0 || t == 1.0f)
			return t;
		if (t < 0.5f)
			return -(std::pow(2.0f, 20.0f * t - 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f;
		else
			return (std::pow(2.0f, -20.0f * t + 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f + 1.0f;
	}


	inline float easeOutBounce(float t)
	{
		constexpr float n1 = 7.5625f;
		constexpr float d1 = 2.75f;


		if (t < (1.0f / d1))
		{

			return n1 * t * t;
		}
		else if (t < (2.0f / d1))
		{
			t -= (1.5f / d1);
			return n1 * t * t + 0.75f;
		}
		else if (t < (2.5f / d1))
		{
			t -= (2.25f / d1);
			return n1 * t * t + 0.9375f;
		}
		else
		{
			t -= (2.625f / d1);
			return n1 * t * t + 0.984375f;
		}
	}


	inline float easeInBounce(float t)
	{
		return 1.0f - MAD::Easings::easeOutBounce(1.0f - t);
	}


	inline float easeInOutBounce(float t)
	{
		return t < 0.5f
			? (1.0f - MAD::Easings::easeOutBounce(1.0f - 2.0f * t)) / 2.0f
			: (1.0f + MAD::Easings::easeOutBounce(2.0f * t - 1.0f)) / 2.0f;
	}



}