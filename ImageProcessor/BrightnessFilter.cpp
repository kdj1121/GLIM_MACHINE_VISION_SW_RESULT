#include "BrightnessFilter.h"

#include <algorithm>

namespace ip
{
	BrightnessFilter::BrightnessFilter(int brightness, unsigned threadCount)
		: TiledFilter(threadCount), m_brightness(brightness)
	{
	}

	void BrightnessFilter::applyUnitsInPlace(ImageBuffer& image, int y0, int y1) const
	{
		for (int y = y0; y < y1; y++)
		{
			std::uint8_t* imagePtr = image.rowPtr(y);
			for (int x = 0; x < image.width(); x++)
			{
				imagePtr[x * ImageBuffer::CHANNELS] = static_cast<std::uint8_t>(std::clamp(imagePtr[x * ImageBuffer::CHANNELS] + m_brightness, 0, 255));
				imagePtr[x * ImageBuffer::CHANNELS + 1] = static_cast<std::uint8_t>(std::clamp(imagePtr[x * ImageBuffer::CHANNELS + 1] + m_brightness, 0, 255));
				imagePtr[x * ImageBuffer::CHANNELS + 2] = static_cast<std::uint8_t>(std::clamp(imagePtr[x * ImageBuffer::CHANNELS + 2] + m_brightness, 0, 255));
			}
		}
	}
	std::string BrightnessFilter::name() const
	{
		return "brightness";
	}
}
