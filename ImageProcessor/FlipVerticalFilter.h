#pragma once

#include "TiledFilter.h"

namespace ip
{
	class FlipVerticalFilter : public TiledFilter
	{
		public:
			explicit FlipVerticalFilter(unsigned threadCount = 0);

			std::string name() const override;

		protected:
			int unitCount(const ImageBuffer& image) const override;
			void applyUnitsInPlace(ImageBuffer& image, int u0, int u1) const override;
	};
}
