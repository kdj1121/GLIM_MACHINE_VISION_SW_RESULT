#include "SharpenFilter.h"

#include <algorithm>
#include <cstdlib>

namespace ip
{
	namespace {
		constexpr int WEIGHT[3] = { 5, -1, 0 };
	}

	SharpenFilter::SharpenFilter(unsigned threadCount)
		: TiledFilter(threadCount)
	{
	}

	bool SharpenFilter::needsSeparateBuffer() const
	{
		return true;
	}

	void SharpenFilter::applyUnitsSeparate(const ImageBuffer& src, ImageBuffer& dst, int y0, int y1) const
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

				int weightSum = 0;
				int sum[3] = {};

				for (int ry = 0; ry < rowCount; ry++)
				{
					const int dy = std::abs(leftY + ry);
					for (int targetX = x + leftX; targetX <= x + rightX; targetX++)
					{
						const int dx = std::abs(targetX - x);
						const int w = WEIGHT[dy + dx];

						weightSum += w;
						for (int channel = 0; channel < ImageBuffer::CHANNELS; channel++)
						{
							sum[channel] += rows[ry][targetX * ImageBuffer::CHANNELS + channel] * w;
						}
					}
				}

				for (int channel = 0; channel < ImageBuffer::CHANNELS; channel++)
				{
					outPtr[x * ImageBuffer::CHANNELS + channel] = static_cast<std::uint8_t>(std::clamp(sum[channel] / weightSum, 0, 255));
				}
			}
		}
	}
	std::string SharpenFilter::name() const
	{
		return "sharpen";
	}
}
