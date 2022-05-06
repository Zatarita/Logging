#pragma once

#include <vector>
#include <string_view>

#include "logger.h"

namespace Log
{
	class Aggregate
	{
		std::vector<Logger*> loggers;

	public:
		Aggregate() = default;
		Aggregate(std::vector<Logger*> _loggers);

		void addLog(Logger* newLog);

		void write(std::string_view str, Log::Level level = Log::Level::Info);
		void operator<<(std::string_view str);
	};
}