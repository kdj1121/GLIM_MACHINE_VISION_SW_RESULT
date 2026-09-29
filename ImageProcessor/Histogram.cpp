#include "Histogram.h"
#include "ColorUtil.h"

#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>

namespace ip {

	std::vector<std::uint32_t> Histogram::loadImage(const ImageBuffer& image) const
	{
		std::vector<std::uint32_t> hist(BINS, 0);

		for (int y = 0; y < image.height(); y++)
		{
			const std::uint8_t* imagePtr = image.rowPtr(y);
			for (int x = 0; x < image.width(); x++)
			{
				const std::uint8_t* p = imagePtr + x * ImageBuffer::CHANNELS;
				hist[toLuma(p[0], p[1], p[2])]++;
			}
		}

		return hist;
	}

	void Histogram::printReport(const std::vector<std::uint32_t>& hist) const
	{
		std::uint64_t total = 0;
		std::uint64_t weightedSum = 0;
		int minValue = -1;
		int maxValue = -1;

		for (int i = 0; i < BINS; i++)
		{
			if (hist[i] == 0)
			{
				continue;
			}
			if (minValue < 0)
			{
				minValue = i;
			}
			maxValue = i;
			total += hist[i];
			weightedSum += static_cast<std::uint64_t>(i) * hist[i];
		}

		const double mean = (total == 0) ? 0.0 : static_cast<double>(weightedSum) / static_cast<double>(total);

		std::cout << "Histogram: min=" << minValue << " max=" << maxValue
			<< " mean=" << std::fixed << std::setprecision(2) << mean << "\n";

		constexpr int GROUP_SIZE = 16;
		constexpr int GROUP_COUNT = BINS / GROUP_SIZE;
		constexpr int BAR_WIDTH = 50;

		std::array<std::uint64_t, GROUP_COUNT> groupCounts{};
		for (int i = 0; i < BINS; i++)
		{
			groupCounts[i / GROUP_SIZE] += hist[i];
		}
		const std::uint64_t maxGroupCount = *std::max_element(groupCounts.begin(), groupCounts.end());

		for (int g = 0; g < GROUP_COUNT; g++)
		{
			const int rangeStart = g * GROUP_SIZE;
			const int rangeEnd = rangeStart + GROUP_SIZE - 1;
			const int barLen = (maxGroupCount == 0) ? 0
				: static_cast<int>(std::round(static_cast<double>(groupCounts[g]) / static_cast<double>(maxGroupCount) * BAR_WIDTH));

			std::cout << "[" << std::setw(3) << rangeStart << "-" << std::setw(3) << rangeEnd << "] "
				<< std::string(barLen, '#') << " (" << groupCounts[g] << ")\n";
		}
	}

	ImageBuffer Histogram::apply(ImageBuffer image) const
	{
		const std::vector<std::uint32_t> hist = loadImage(image);
		printReport(hist);
		return image;
	}

	std::string Histogram::name() const
	{
		return "hist";
	}
}
