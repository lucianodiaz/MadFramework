#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

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
		return 1 - std::cos((t * M_PI) / 2);
	}

	inline float easeOutSine(float t)
	{
		return std::sin((t * M_PI) / 2);
	}

	inline float easeInOutSine(float t)
	{
		return -(std::cos(M_PI * t) - 1) / 2;
	}

	inline float easeInQuad(float t)
	{
		return t * t;
	}

	inline float easeOutQuad(float t)
	{
		return 1 - (1 - t) * (1 - t);
	}

	inline float easeInOutQuad(float t)
	{
		if (t < 0.5f)
			return 2 * t * t;
		else
			return 1 - std::pow(-2 * t + 2, 2) / 2;
	}

	inline float easeInCubic(float t)
	{
		return t * t * t;
	}

	inline float easeOutCubic(float t)
	{
		return 1 - std::pow(1 - t, 3);
	}

	inline float easeInOutCubic(float t)
	{
		if (t < 0.5f)
			return 4 * t * t * t;
		else
			return 1 - std::pow(-2 * t + 2, 3) / 2;
	}

	inline float easeInQuart(float t)
	{
		return t * t * t * t;
	}

	inline float easeOutQuart(float t)
	{
		return 1 - std::pow(1 - t, 4);
	}

	inline float easeInOutQuart(float t)
	{
		if (t < 0.5f)
			return 8 * t * t * t * t;
		else
			return 1 - std::pow(-2 * t + 2, 4) / 2;
	}

	inline float easeInQuint(float t)
	{
		return t * t * t * t * t;
	}

	inline float easeOutQuint(float t)
	{
		return 1 - std::pow(1 - t, 5);
	}

	inline float easeInOutQuint(float t)
	{
		if (t < 0.5f)
			return 16 * t * t * t * t * t;
		else
			return 1 - std::pow(-2 * t + 2, 5) / 2;
	}

	inline float easeInExpo(float t)
	{
		return (t == 0) ? 0 : std::pow(2, 10 * t - 10);
	}

	inline float easeOutExpo(float t)
	{
		return (t == 1) ? 1 : 1 - std::pow(2, -10 * t);
	}

	inline float easeInOutExpo(float t)
	{
		if (t == 0 || t == 1)
			return t;
		if (t < 0.5f)
			return std::pow(2, 20 * t - 10) / 2;
		else
			return (2 - std::pow(2, -20 * t + 10)) / 2;
	}

	inline float easeInCirc(float t)
	{
		return 1 - std::sqrt(1 - (t * t));
	}

	inline float easeOutCirc(float t)
	{
		return std::sqrt(1 - std::pow(t - 1, 2));
	}

	inline float easeInOutCirc(float t)
	{
		if (t < 0.5f)
			return (1 - std::sqrt(1 - std::pow(2 * t, 2))) / 2;
		else
			return (std::sqrt(1 - std::pow(-2 * t + 2, 2)) + 1) / 2;
	}

	inline float easeInBack(float t)
	{
		constexpr float c1 = 1.70158f;
		constexpr float c3 = c1 + 1;

		return c3 * t * t * t - c1 * t * t;
	}

	inline float easeOutBack(float t)
	{
		constexpr float c1 = 1.70158f;
		constexpr float c3 = c1 + 1;
		return 1 + c3 * std::pow(t - 1, 3) + c1 * std::pow(t - 1, 2);
	}

	inline float easeInOutBack(float t)
	{
		constexpr float c1 = 1.70158f;
		constexpr float c2 = c1 * 1.525f;
		if (t < 0.5f)
			return (std::pow(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) / 2;
		else
			return (std::pow(2 * t - 2, 2) * ((c2 + 1) * (t * 2 - 2) + c2) + 2) / 2;
	}


	inline float easeInElastic(float t)
	{
		constexpr float c4 = (2 * M_PI) / 3;

		return (t == 0) 
			? 0 
			: (t == 1) 
			? 1 
			: -std::pow(2, 10 * t - 10) * std::sin((t * 10 - 10.75f) * c4);
	}

	inline float easeOutElastic(float t)
	{
		constexpr float c4 = (2 * M_PI) / 3;
		return (t == 0) 
			? 0 
			: (t == 1) 
			? 1 
			: std::pow(2, -10 * t) * std::sin((t * 10 - 0.75f) * c4) + 1;
	}

	inline float easeInOutElastic(float t)
	{
		constexpr float c5 = (2 * M_PI) / 4.5f;
		if (t == 0 || t == 1)
			return t;
		if (t < 0.5f)
			return -(std::pow(2, 20 * t - 10) * std::sin((20 * t - 11.125f) * c5)) / 2;
		else
			return (std::pow(2, -20 * t + 10) * std::sin((20 * t - 11.125f) * c5)) / 2 + 1;
	}

	inline float easeInBounce(float t)
	{
		return 1 - easeOutBounce(1 - t);
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

	inline float easeInOutBounce(float t)
	{
		return t < 0.5f 
			? (1 - easeOutBounce(1 - 2 * t)) / 2 
			: (1 + easeOutBounce(2 * t - 1)) / 2;
	}



}