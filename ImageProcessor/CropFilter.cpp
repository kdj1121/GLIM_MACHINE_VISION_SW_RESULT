#include "CropFilter.h"
#include "Exceptions.h"

#include <cmath>
#include <algorithm>

namespace ip {

	CropFilter::CropFilter(int x, int y, int w, int h)
		: m_x(x), m_y(y), m_w(w), m_h(h)
	{

	}

	ImageBuffer CropFilter::apply(ImageBuffer image) const
	{
		if (m_x < 0 || m_y < 0)
		{
			throw FilterError("Crop x/y must be non-negative (got x=" +
				std::to_string(m_x) + ", y=" + std::to_string(m_y) + ")");
		}

		if (m_w <= 0 || m_h <= 0)
		{
			throw FilterError("Crop w/h must be positive (got w=" +
				std::to_string(m_w) + ", h=" + std::to_string(m_h) + ")");
		}

		if (m_x + m_w > image.width())
		{
			throw FilterError("Crop rectangle exceeds image width (x+w=" +
				std::to_string(m_x + m_w) + ", image width=" + std::to_string(image.width()) + ")");
		}

		if (m_y + m_h > image.height())
		{
			throw FilterError("Crop rectangle exceeds image height (y+h=" +
				std::to_string(m_y + m_h) + ", image height=" + std::to_string(image.height()) + ")");
		}

		ImageBuffer canvas(m_w, m_h);

		for (int y = 0; y < m_h; y++)
		{
			std::uint8_t* canvasPtr = canvas.rowPtr(y);
			std::copy(image.rowPtr(m_y + y) + m_x * ImageBuffer::CHANNELS , image.rowPtr(m_y + y) + (m_x + m_w)* ImageBuffer::CHANNELS, canvasPtr);
		}

		return canvas;
	}

	std::string CropFilter::name() const
	{
		return "crop";
	}
}