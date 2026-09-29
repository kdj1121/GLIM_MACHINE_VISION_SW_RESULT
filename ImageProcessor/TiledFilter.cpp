#include "TiledFilter.h"

#include <algorithm>
#include <exception>
#include <stdexcept>
#include <thread>
#include <vector>

namespace ip {

	namespace {
		unsigned resolveThreadCount(unsigned requested, int totalUnits)
		{
			if (totalUnits <= 1)
			{
				return 1;
			}
			const unsigned hw = (requested > 0) ? requested : std::max(1u, std::thread::hardware_concurrency());
			return std::min(hw, static_cast<unsigned>(totalUnits));
		}
	}

	TiledFilter::TiledFilter(unsigned requestedThreads)
		: m_requestedThreads(requestedThreads)
	{
	}

	int TiledFilter::unitCount(const ImageBuffer& image) const
	{
		return image.height();
	}

	bool TiledFilter::needsSeparateBuffer() const
	{
		return false;
	}

	void TiledFilter::applyUnitsInPlace(ImageBuffer&, int, int) const
	{
		throw std::logic_error("TiledFilter::applyUnitsInPlace: not overridden");
	}

	void TiledFilter::applyUnitsSeparate(const ImageBuffer&, ImageBuffer&, int, int) const
	{
		throw std::logic_error("TiledFilter::applyUnitsSeparate: not overridden");
	}

	ImageBuffer TiledFilter::apply(ImageBuffer image) const
	{
		const int totalUnits = unitCount(image);
		const bool separate = needsSeparateBuffer();
		const unsigned threadCount = resolveThreadCount(m_requestedThreads, totalUnits);

		ImageBuffer dst = separate ? ImageBuffer(image.width(), image.height()) : ImageBuffer();

		auto runRange = [&](int u0, int u1) {
			if (separate)
			{
				applyUnitsSeparate(image, dst, u0, u1);
			}
			else
			{
				applyUnitsInPlace(image, u0, u1);
			}
		};

		if (threadCount <= 1 || totalUnits <= 0)
		{
			runRange(0, totalUnits);
		}
		else
		{
			std::vector<std::thread> threads;
			std::vector<std::exception_ptr> errors(threadCount);
			threads.reserve(threadCount);

			const int baseSize = totalUnits / static_cast<int>(threadCount);
			const int remainder = totalUnits % static_cast<int>(threadCount);

			int u0 = 0;
			for (unsigned i = 0; i < threadCount; i++)
			{
				const int size = baseSize + (static_cast<int>(i) < remainder ? 1 : 0);
				const int u1 = u0 + size;

				threads.emplace_back([&, i, u0, u1]() {
					try
					{
						runRange(u0, u1);
					}
					catch (...)
					{
						errors[i] = std::current_exception();
					}
				});

				u0 = u1;
			}

			for (auto& t : threads)
			{
				t.join();
			}

			for (auto& e : errors)
			{
				if (e)
				{
					std::rethrow_exception(e);
				}
			}
		}

		return separate ? dst : image;
	}
}
