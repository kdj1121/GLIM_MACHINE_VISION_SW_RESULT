#include "FlipHorizontalFilter.h"

#include <algorithm>

namespace ip
{
	FlipHorizontalFilter::FlipHorizontalFilter(unsigned threadCount)
		: TiledFilter(threadCount)
	{
	}

	void FlipHorizontalFilter::applyUnitsInPlace(ImageBuffer& image, int y0, int y1) const
	{
		for (int y = y0; y < y1; y++)
		{
			std::uint8_t* imagePtr = image.rowPtr(y);
			for (int x = 0; x < image.width() / 2; x++)
			{
				std::uint8_t* left = imagePtr + x * ImageBuffer::CHANNELS;
				std::uint8_t* right = imagePtr + (image.width() - 1 - x) * ImageBuffer::CHANNELS;
				std::swap_ranges(left, left + ImageBuffer::CHANNELS, right);
			}
		}
	}
	std::string FlipHorizontalFilter::name() const
	{
		return "horizontal";
	}
}
