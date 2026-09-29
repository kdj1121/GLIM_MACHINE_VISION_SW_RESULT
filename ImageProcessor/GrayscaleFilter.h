#pragma once

#include "TiledFilter.h"

namespace ip {

	class GrayscaleFilter : public TiledFilter
	{
		public:
			explicit GrayscaleFilter(unsigned threadCount = 0);

			std::string name() const override;

		protected:
			void applyUnitsInPlace(ImageBuffer& image, int y0, int y1) const override;
	};

}
