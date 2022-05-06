#include "core/logger.h"

namespace Log
{
	Logger::Logger(std::string _name, const Behaviour& behaviour) :
		loggingBehaviour(behaviour),
		name(_name)
	{}

	Behaviour& Logger::getBehaviour() noexcept
	{
		return loggingBehaviour;
	}

	void Logger::operator<<(std::string_view str) const noexcept
	{
		write(str);
	}

	void Logger::write(std::string_view str, const Log::Level& level) const noexcept
	{
		if (level < loggingBehaviour.getLevel())
			return;

		for (auto& probe : loggingBehaviour.getProbes())
			probe->write(format(str, level));
	}

	std::string Logger::format(std::string_view str, const Log::Level& level) const noexcept
	{
		std::string ret{ loggingBehaviour.getFormatString() };
									  
		findAndReplaceAll(ret, "$(H)"        , getTime("H"));
		findAndReplaceAll(ret, "$(12H)"      , getTime("12H"));
		findAndReplaceAll(ret, "$(M)"        , getTime("M"));
		findAndReplaceAll(ret, "$(S)"        , getTime("S"));
		findAndReplaceAll(ret, "$(d)"        , getTime("d"));
		findAndReplaceAll(ret, "$(dn)"       , getTime("dn"));
		findAndReplaceAll(ret, "$(m)"        , getTime("m"));
		findAndReplaceAll(ret, "$(mn)"       , getTime("mn"));
		findAndReplaceAll(ret, "$(y)"        , getTime("y"));
		findAndReplaceAll(ret, "$(message)"  , str);
		findAndReplaceAll(ret, "$(level)"    , getLevel(level));
		findAndReplaceAll(ret, "$(name)"     , name);
		findAndReplaceAll(ret, "$(pid)"      , getPID());
		findAndReplaceAll(ret, "$(0xpid)"    , getPID(true));
		findAndReplaceAll(ret, "$(thread)"   , getThread());
		findAndReplaceAll(ret, "$(0xthread)" , getThread(true));

		return ret;
	}

	void Logger::findAndReplaceAll(std::string& data, std::string_view toSearch, std::string_view replaceStr)
	{
		size_t pos = data.find(toSearch);
		while (pos != std::string::npos)
		{
			data.replace(pos, toSearch.size(), replaceStr);
			pos = data.find(toSearch, pos + replaceStr.size());
		}
	}

	std::string Log::Logger::getTime(std::string_view requestedTime) const noexcept
	{
		auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::tm* time{ new std::tm };
		localtime_s(time, &t);			// I hate dealing with time in c++

		static const std::string days[]{
			"Sunday",
			"Monday",
			"Tuesday",
			"Wednesday",
			"Thursday",
			"Friday",
			"Saturday"
		};

		static const std::string months[]{
			"January",
			"February",
			"March",
			"April",
			"May",
			"June",
			"July",
			"August",
			"September",
			"October",
			"November",
			"December",
		};

		if (requestedTime.size() == 1)
		{
			float period{ static_cast<float>(time->tm_hour / 12.0) };

			switch (requestedTime.data()[0])
			{
			case 'H':	// Hour
				return std::to_string(time->tm_hour);
			case 'M':	// Min
				return std::to_string(time->tm_min);
			case 'S':	// Second
				return std::to_string(time->tm_sec);
			case 'p':	// Period (am/pm)
				return period > 1 ? "pm" : "am";
			case 'd':	// Day [0-31]
				return std::to_string(time->tm_mday);
			case 'm':	// Month
				return std::to_string(time->tm_mon);
			case 'y':	// Year
				return std::to_string(time->tm_year);
			default:
				return "";
			}
		}
		else
		{
			if (requestedTime == "12h") // 12 hour time
				return std::to_string(time->tm_hour % 12);
			if (requestedTime == "mn")	// Month name
				if(time->tm_mon <= 12)
					return months[time->tm_mon];
			if (requestedTime == "dn")	// Day name
				if (time->tm_wday <= 31)
					return days[time->tm_wday];
		}

		// Everything else
		return std::string("");
	}

	std::string Logger::getLevel(const Log::Level& level) const noexcept
	{
		static const std::string levelMap[]{
			"DEBUG",
			"INFO",
			"WARNING",
			"ERROR",
			"CRITICAL"
		};

		unsigned int index = static_cast<unsigned int>(level);
		if (index > levelMap->size())
			return "__ERRLVL__";

		return levelMap[index];
	}
	std::string Logger::getPID(const bool hex) const noexcept
	{
		return getInt(GetCurrentProcessId(), hex);
	}

	std::string Logger::getThread(const bool hex) const noexcept
	{
		return getInt( std::hash<std::thread::id>{}( std::this_thread::get_id() ), hex );
	}
	std::string Logger::getInt(const int& value, const bool hex) noexcept
	{
		std::stringstream buffer;

		if (hex)
			buffer << std::hex << value;
		else
			buffer << value;

		return buffer.str();
	}
}
