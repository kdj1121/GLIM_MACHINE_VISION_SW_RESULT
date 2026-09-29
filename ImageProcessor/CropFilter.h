#pragma once

#include "FilterBase.h"

namespace ip {

	class CropFilter : public FilterBase
	{
		public:
			CropFilter(int x, int y, int w, int h);

			ImageBuffer apply(ImageBuffer image) const override;

			std::string name() const override;

		private:
			int m_x, m_y, m_w, m_h;

	};
}

