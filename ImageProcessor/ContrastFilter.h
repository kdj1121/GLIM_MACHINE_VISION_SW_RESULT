#pragma once

#include "TiledFilter.h"

namespace ip
{
	class ContrastFilter : public TiledFilter
	{
		public:
			ContrastFilter(double contrast, unsigned threadCount = 0);

			std::string name() const override;

		protected:
			void applyUnitsInPlace(ImageBuffer& image, int y0, int y1) const override;

		private:
			double m_contrast;
	};
}
