#pragma once

#include "FilterBase.h"

#include <memory>
#include <string>

namespace ip {

	class FilterFactory
	{
		public:
			FilterFactory() = delete;

			static std::unique_ptr<FilterBase> create(const std::string& token, unsigned threadCount = 0);
	};
}
