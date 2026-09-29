#include "NumberParser.h"

#include <string>
#include <stdexcept>
#include <cmath>

namespace ip
{
	bool NumberParser::tryParseInt(const std::string& text, int& outValue, std::string& outError)
	{
		size_t pos = 0;
		int result = 0;
		
		try
		{
			result = std::stoi(text, &pos);
		}
		catch (const std::invalid_argument&)
		{
			outError = "Invalid Number(integer)";
			return false;
		}
		catch (const std::out_of_range&)
		{
			outError = "Out of Range(integer)";
			return false;
		}

		if (pos != text.size())
		{
			outError = "Invalid Argument";
			return false;
		}

		outValue = result;
		return true;
		
	}

	bool NumberParser::tryParseDouble(const std::string& text, double& outValue, std::string& outError)
	{
		size_t pos = 0;
		double result = 0;

		try
		{
			result = std::stod(text, &pos);
		}
		catch (const std::invalid_argument&)
		{
			outError = "Invalid Number(double)";
			return false;
		}
		catch (const std::out_of_range&)
		{
			outError = "Out of Range(double)";
			return false;
		}

		if (pos != text.size())
		{
			outError = "Invalid Argument";
			return false;
		}

		if (std::isnan(result) || std::isinf(result))
		{
			outError = "Invalid value";
			return false;
		}

		outValue = result;
		return true;
	}
}
