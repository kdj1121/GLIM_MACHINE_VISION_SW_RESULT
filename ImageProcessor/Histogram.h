#pragma once

#include "FilterBase.h"

#include <cstdint>
#include <vector>

namespace ip {

	class Histogram : public FilterBase
	{
		public:
			static constexpr int BINS = 256;

			ImageBuffer apply(ImageBuffer image) const override;

			std::string name() const override;

		private:
			std::vector<std::uint32_t> loadImage(const ImageBuffer& image) const;

			void printReport(const std::vector<std::uint32_t>& hist) const;
	};
}
