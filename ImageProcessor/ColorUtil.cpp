#include "ColorUtil.h"

#include <algorithm>
#include <cmath>

namespace ip {

	std::uint8_t toLuma(std::uint8_t b, std::uint8_t g, std::uint8_t r)
	{
		const float color = b * 0.0722f + g * 0.7152f + r * 0.2126f;
		return static_cast<std::uint8_t>(std::clamp(static_cast<int>(std::round(color)), 0, 255));
	}

}
