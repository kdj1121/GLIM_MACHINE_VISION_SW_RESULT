#include "ThresholdFilter.h"
#include "ColorUtil.h"

namespace ip
{
	ThresholdFilter::ThresholdFilter(int threshold, unsigned threadCount)
		: TiledFilter(threadCount), m_threshold(threshold)
	{
	}

	void ThresholdFilter::applyUnitsInPlace(ImageBuffer& image, int y0, int y1) const
	{
		for (int y = y0; y < y1; y++)
		{
			std::uint8_t* imagePtr = image.rowPtr(y);
			for (int x = 0; x < image.width(); x++)
			{
				std::uint8_t* p = imagePtr + x * ImageBuffer::CHANNELS;
				const std::uint8_t result = (toLuma(p[0], p[1], p[2]) >= m_threshold) ? 255 : 0;

				p[0] = result;
				p[1] = result;
				p[2] = result;
			}
		}
	}

	std::string ThresholdFilter::name() const
	{
		return "threshold";
	}
}
