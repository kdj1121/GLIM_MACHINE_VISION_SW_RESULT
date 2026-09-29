#include "Logger.h"

#include <chrono>
#include <ctime>
#include <iomanip>

namespace ip {

	Logger::Logger(const std::string& logPath)
		: m_enabled(!logPath.empty())
	{
		if (m_enabled)
		{
			m_file.open(logPath, std::ios::out | std::ios::app);
			m_enabled = m_file.is_open();
		}
	}

	void Logger::write(const char* level, const std::string& message) const
	{
		if (!m_enabled)
		{
			return;
		}

		std::lock_guard<std::mutex> lock(m_mutex);

		const auto now = std::chrono::system_clock::now();
		const std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
		std::tm localTm{};
		localtime_s(&localTm, &nowTime);

		m_file << std::put_time(&localTm, "%Y-%m-%d %H:%M:%S") << " [" << level << "] " << message << "\n";
		m_file.flush();
	}

	void Logger::info(const std::string& message) const
	{
		write("INFO", message);
	}

	void Logger::error(const std::string& message) const
	{
		write("ERROR", message);
	}
}
