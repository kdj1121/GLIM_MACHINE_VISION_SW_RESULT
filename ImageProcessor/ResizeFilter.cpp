#include "ResizeFilter.h"
#include "Exceptions.h"

#include <cmath>
#include <algorithm>

namespace ip {

    ResizeFilter::ResizeFilter(int width, int height)
        : m_width(width), m_height(height)
    {
    }

    ImageBuffer ResizeFilter::apply(ImageBuffer image) const
    {
        if (m_width <= 0 || m_height <= 0)
        {
            throw FilterError("Resize width/height must be positive (got " +
                std::to_string(m_width) + "x" + std::to_string(m_height) + ")");
        }
        
        ImageBuffer canvas(m_width, m_height);

        const double scaleX = image.width() / static_cast<double>(m_width);
        const double scaleY = image.height() / static_cast<double>(m_height);

        for (int targetY = 0; targetY < canvas.height(); targetY++)
        {
            std::uint8_t* canvasPtr = canvas.rowPtr(targetY);

            const double srcY = (targetY + 0.5) * scaleY - 0.5;
            const int y1 = std::clamp(static_cast<int>(std::floor(srcY)), 0, image.height() - 1);
            const int y2 = std::clamp(y1 + 1, 0, image.height() - 1);
            const double fy = srcY - y1;

            const std::uint8_t* imageTop = image.rowPtr(y1);
            const std::uint8_t* imageBottom = image.rowPtr(y2);

            for (int targetX = 0; targetX < canvas.width(); targetX++)
            {
                const double srcX = (targetX + 0.5) * scaleX - 0.5;

                const int x1 = std::clamp(static_cast<int>(std::floor(srcX)), 0, image.width() - 1);
                const int x2 = std::clamp(x1 + 1, 0, image.width() - 1);

                const double fx = srcX - x1;

                for (int c = 0; c < ImageBuffer::CHANNELS; c++)
                {
                    const int leftTop = imageTop[x1 * ImageBuffer::CHANNELS + c];
                    const int rightTop = imageTop[x2 * ImageBuffer::CHANNELS + c];
                    const int leftBottom = imageBottom[x1 * ImageBuffer::CHANNELS + c];
                    const int rightBottom = imageBottom[x2 * ImageBuffer::CHANNELS + c];

                    const double top = leftTop * (1 - fx) + rightTop * fx;
                    const double bottom = leftBottom * (1 - fx) + rightBottom * fx;

                    const double result = std::clamp((top * (1 - fy) + bottom * fy), 0.0, 255.0);

                    canvasPtr[targetX * ImageBuffer::CHANNELS + c] = static_cast<std::uint8_t>(std::round(result));
                }

            }

        }

        return canvas;
    }

    std::string ResizeFilter::name() const
    {
        return "resize";
    }

}