#pragma once

#include <string>

namespace ip {

	class NumberParser
	{
		public:
			NumberParser() = delete;

			static bool tryParseInt(const std::string& text, int& outValue, std::string& outError);

			static bool tryParseDouble(const std::string& text, double& outValue, std::string& outError);

	};
}

