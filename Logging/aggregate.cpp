#include "core/aggregate.h"


namespace Log
{
	Aggregate::Aggregate(std::vector<Logger*> _loggers) : loggers(_loggers) 
	{}

	void Aggregate::addLog(Logger* newLog)
	{
		loggers.push_back(newLog);
	}

	void Aggregate::write(std::string_view str, Log::Level level)
	{
		for (auto& log : loggers)
			log->write(str, level);
	}

	void Aggregate::operator<<(std::string_view str)
	{
		write(str);
	}
}


