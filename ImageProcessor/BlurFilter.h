#pragma once

#include "TiledFilter.h"

namespace ip
{
	class BlurFilter : public TiledFilter
	{
		public:
			explicit BlurFilter(unsigned threadCount = 0);

			std::string name() const override;

		protected:
			bool needsSeparateBuffer() const override;
			void applyUnitsSeparate(const ImageBuffer& src, ImageBuffer& dst, int y0, int y1) const override;
	};
}
