#pragma once

#include "TiledFilter.h"

namespace ip
{
	class BrightnessFilter : public TiledFilter
	{
		public:
			BrightnessFilter(int brightness, unsigned threadCount = 0);

			std::string name() const override;

		protected:
			void applyUnitsInPlace(ImageBuffer& image, int y0, int y1) const override;

		private:
			int m_brightness;
	};
}
