#pragma once

#include "FilterBase.h"

namespace ip {

	class TiledFilter : public FilterBase
	{
		public:
			explicit TiledFilter(unsigned requestedThreads = 0);

			ImageBuffer apply(ImageBuffer image) const override final;

		protected:
			virtual int unitCount(const ImageBuffer& image) const;

			virtual bool needsSeparateBuffer() const;

			virtual void applyUnitsInPlace(ImageBuffer& image, int u0, int u1) const;

			virtual void applyUnitsSeparate(const ImageBuffer& src, ImageBuffer& dst, int u0, int u1) const;

		private:
			unsigned m_requestedThreads;
	};
}
