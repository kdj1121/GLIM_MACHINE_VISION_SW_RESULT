#pragma once

#include "ImageBuffer.h"
#include <string>

namespace ip
{
	class FilterBase
	{
		public:
			virtual ~FilterBase() = default;

			virtual ImageBuffer apply(ImageBuffer image) const = 0;
			virtual std::string name() const = 0;
	};
}

