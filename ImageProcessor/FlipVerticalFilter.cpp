#include "FlipVerticalFilter.h"

#include <algorithm>

namespace ip
{
	FlipVerticalFilter::FlipVerticalFilter(unsigned threadCount)
		: TiledFilter(threadCount)
	{
	}

	int FlipVerticalFilter::unitCount(const ImageBuffer& image) const
	{
		return image.height() / 2;
	}

	void FlipVerticalFilter::applyUnitsInPlace(ImageBuffer& image, int u0, int u1) const
	{
		for (int y = u0; y < u1; y++)
		{
			std::uint8_t* imagePtr = image.rowPtr(y);
			std::uint8_t* imagePtr2 = image.rowPtr(image.height() - 1 - y);

			std::swap_ranges(imagePtr, imagePtr + image.rowStride(), imagePtr2);
		}
	}
	std::string FlipVerticalFilter::name() const
	{
		return "vertical";
	}
}
