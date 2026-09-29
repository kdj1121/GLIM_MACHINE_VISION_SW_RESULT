#pragma once

#include "TiledFilter.h"

namespace ip
{
	class SharpenFilter : public TiledFilter
	{
		public:
			explicit SharpenFilter(unsigned threadCount = 0);

			std::string name() const override;

		protected:
			bool needsSeparateBuffer() const override;
			void applyUnitsSeparate(const ImageBuffer& src, ImageBuffer& dst, int y0, int y1) const override;
	};
}
