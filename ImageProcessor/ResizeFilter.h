#pragma once

#include "FilterBase.h"

namespace ip {
	class ResizeFilter : public FilterBase
	{
		public:
			ResizeFilter(int width, int height);

			ImageBuffer apply(ImageBuffer image) const override;
			std::string name() const override;

		private:
			int m_width, m_height;
	};
}

