#pragma once

#include "TiledFilter.h"

namespace ip
{
	class FlipHorizontalFilter : public TiledFilter
	{
		public:
			explicit FlipHorizontalFilter(unsigned threadCount = 0);

			std::string name() const override;

		protected:
			void applyUnitsInPlace(ImageBuffer& image, int y0, int y1) const override;
	};
}
