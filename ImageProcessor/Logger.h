#pragma once

#include <fstream>
#include <mutex>
#include <string>

namespace ip {

	class Logger
	{
		public:
			explicit Logger(const std::string& logPath);

			void info(const std::string& message) const;
			void error(const std::string& message) const;

		private:
			void write(const char* level, const std::string& message) const;

			mutable std::mutex m_mutex;
			mutable std::ofstream m_file;
			bool m_enabled;
	};
}
