#include "FilterFactory.h"
#include "Exceptions.h"
#include "NumberParser.h"

#include "GrayscaleFilter.h"
#include "ThresholdFilter.h"
#include "BrightnessFilter.h"
#include "ContrastFilter.h"
#include "FlipHorizontalFilter.h"
#include "FlipVerticalFilter.h"
#include "BlurFilter.h"
#include "SharpenFilter.h"
#include "Histogram.h"
#include "CropFilter.h"
#include "ResizeFilter.h"

#include <climits>
#include <vector>

namespace ip {

	namespace {

		std::vector<std::string> splitByColon(const std::string& token)
		{
			std::vector<std::string> parts;
			std::size_t start = 0;

			while (true) {
				const std::size_t end = token.find(':', start);
				parts.push_back(token.substr(start, end == std::string::npos ? std::string::npos : end - start));

				if (end == std::string::npos) {
					break;
				}
				start = end + 1;
			}

			return parts;
		}

		void expectArgCount(const std::string& token, const std::vector<std::string>& parts, std::size_t expected)
		{
			if (parts.size() - 1 != expected) {
				throw FilterError("'" + token + "': expected " + std::to_string(expected)
					+ " argument(s), got " + std::to_string(parts.size() - 1));
			}
		}

		int parseIntArg(const std::string& token, const std::string& text, int minValue, int maxValue)
		{
			int value = 0;
			std::string parseError;
			if (!NumberParser::tryParseInt(text, value, parseError)) {
				throw FilterError("'" + token + "': '" + text + "' is not a valid integer (" + parseError + ")");
			}
			if (value < minValue || value > maxValue) {
				throw FilterError("'" + token + "': " + text + " is out of range [" + std::to_string(minValue) + ", " + std::to_string(maxValue) + "]");
			}
			return value;
		}

		double parseDoubleArg(const std::string& token, const std::string& text, double minValue)
		{
			double value = 0;
			std::string parseError;
			if (!NumberParser::tryParseDouble(text, value, parseError)) {
				throw FilterError("'" + token + "': '" + text + "' is not a valid number (" + parseError + ")");
			}
			if (value < minValue) {
				throw FilterError("'" + token + "': " + text + " must be at least " + std::to_string(minValue));
			}
			return value;
		}

	}

	std::unique_ptr<FilterBase> FilterFactory::create(const std::string& token, unsigned threadCount)
	{
		const std::vector<std::string> parts = splitByColon(token);
		const std::string& name = parts[0];

		if (name == "grayscale") {
			expectArgCount(token, parts, 0);
			return std::make_unique<GrayscaleFilter>(threadCount);
		}
		if (name == "horizontal") {
			expectArgCount(token, parts, 0);
			return std::make_unique<FlipHorizontalFilter>(threadCount);
		}
		if (name == "vertical") {
			expectArgCount(token, parts, 0);
			return std::make_unique<FlipVerticalFilter>(threadCount);
		}
		if (name == "blur") {
			expectArgCount(token, parts, 0);
			return std::make_unique<BlurFilter>(threadCount);
		}
		if (name == "sharpen") {
			expectArgCount(token, parts, 0);
			return std::make_unique<SharpenFilter>(threadCount);
		}
		if (name == "hist") {
			expectArgCount(token, parts, 0);
			return std::make_unique<Histogram>();
		}
		if (name == "threshold") {
			expectArgCount(token, parts, 1);
			const int value = parseIntArg(token, parts[1], 0, 255);
			return std::make_unique<ThresholdFilter>(value, threadCount);
		}
		if (name == "brightness") {
			expectArgCount(token, parts, 1);
			const int value = parseIntArg(token, parts[1], -255, 255);
			return std::make_unique<BrightnessFilter>(value, threadCount);
		}
		if (name == "contrast") {
			expectArgCount(token, parts, 1);
			const double value = parseDoubleArg(token, parts[1], 0.0);
			return std::make_unique<ContrastFilter>(value, threadCount);
		}
		if (name == "crop") {
			expectArgCount(token, parts, 4);
			const int x = parseIntArg(token, parts[1], 0, INT_MAX);
			const int y = parseIntArg(token, parts[2], 0, INT_MAX);
			const int w = parseIntArg(token, parts[3], 1, INT_MAX);
			const int h = parseIntArg(token, parts[4], 1, INT_MAX);
			return std::make_unique<CropFilter>(x, y, w, h);
		}
		if (name == "resize") {
			expectArgCount(token, parts, 2);
			const int w = parseIntArg(token, parts[1], 1, INT_MAX);
			const int h = parseIntArg(token, parts[2], 1, INT_MAX);
			return std::make_unique<ResizeFilter>(w, h);
		}

		throw FilterError("Unknown filter: '" + name + "'");
	}
}
