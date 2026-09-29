#include "BlurFilter.h"

#include <algorithm>

namespace ip
{
	BlurFilter::BlurFilter(unsigned threadCount)
		: TiledFilter(threadCount)
	{
	}

	bool BlurFilter::needsSeparateBuffer() const
	{
		return true;
	}

	void BlurFilter::applyUnitsSeparate(const ImageBuffer& src, ImageBuffer& dst, int y0, int y1) const
	{
		for (int y = y0; y < y1; y++)
		{
			const int leftY = (y - 1 < 0) ? 0 : -1;
			const int rightY = (y + 1 >= src.height()) ? 0 : 1;
			const int rowCount = rightY - leftY + 1;

			const std::uint8_t* rows[3] = {};
			for (int i = 0; i < rowCount; i++)
			{
				rows[i] = src.rowPtr(y + leftY + i);
			}

			std::uint8_t* outPtr = dst.rowPtr(y);

			for (int x = 0; x < src.width(); x++)
			{
				const int leftX = (x - 1 < 0) ? 0 : -1;
				const int rightX = (x + 1 >= src.width()) ? 0 : 1;
				const int colCount = rightX - leftX + 1;
				const int count = rowCount * colCount;

				int sum[3] = {};
				for (int ry = 0; ry < rowCount; ry++)
				{
					for (int targetX = x + leftX; targetX <= x + rightX; targetX++)
					{
						for (int channel = 0; channel < ImageBuffer::CHANNELS; channel++)
						{
							sum[channel] += rows[ry][targetX * ImageBuffer::CHANNELS + channel];
						}
					}
				}

				for (int channel = 0; channel < ImageBuffer::CHANNELS; channel++)
				{
					outPtr[x * ImageBuffer::CHANNELS + channel] = static_cast<std::uint8_t>(std::clamp(sum[channel] / count, 0, 255));
				}
			}
		}
	}
	std::string BlurFilter::name() const
	{
		return "blur";
	}
}
