#include "ContrastFilter.h"

#include <cmath>
#include <algorithm>

namespace ip
{
	ContrastFilter::ContrastFilter(double contrast, unsigned threadCount)
		: TiledFilter(threadCount), m_contrast(contrast)
	{
	}

	void ContrastFilter::applyUnitsInPlace(ImageBuffer& image, int y0, int y1) const
	{
		for (int y = y0; y < y1; y++)
		{
			std::uint8_t* imagePtr = image.rowPtr(y);
			for (int x = 0; x < image.width(); x++)
			{
				imagePtr[x * ImageBuffer::CHANNELS] = static_cast<std::uint8_t>(std::clamp(static_cast<int>(std::round((imagePtr[x * ImageBuffer::CHANNELS] - 128) * m_contrast)) + 128, 0, 255));
				imagePtr[x * ImageBuffer::CHANNELS + 1] = static_cast<std::uint8_t>(std::clamp(static_cast<int>(std::round((imagePtr[x * ImageBuffer::CHANNELS + 1] - 128) * m_contrast)) + 128, 0, 255));
				imagePtr[x * ImageBuffer::CHANNELS + 2] = static_cast<std::uint8_t>(std::clamp(static_cast<int>(std::round((imagePtr[x * ImageBuffer::CHANNELS + 2] - 128) * m_contrast)) + 128, 0, 255));
			}
		}
	}

	std::string ContrastFilter::name() const
	{
		return "contrast";
	}
}
