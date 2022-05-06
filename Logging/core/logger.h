#pragma once

#include <string>
#include <sstream>
#include <thread>
#include <iomanip>
#include <chrono>

#include <windows.h>

#include "behaviour.h"


namespace Log
{
	class Logger
	{
		Behaviour loggingBehaviour{};
		std::string name{ "" };

	public:
		Logger(std::string name = "", const Behaviour& behaviour = Behaviour());

		[[nodiscard]] Behaviour& getBehaviour() noexcept;

		void operator<<(std::string_view str) const noexcept;

		void write(std::string_view str, const Log::Level& level = Log::Level::Info) const noexcept
		{
			if (level < loggingBehaviour.getLevel())
				return;

			for (auto& probe : loggingBehaviour.getProbes())
				probe->write( format(str, level) );
		}

	private:
		// Before we use it as a formatting string we're going to
		// change extract special characters like date and time
		std::string format(std::string_view str, const Log::Level& level = Log::Level::Info) const noexcept;

		static void findAndReplaceAll(std::string& data, std::string_view toSearch, std::string_view replaceStr)
		{
			size_t pos = data.find(toSearch);
			while (pos != std::string::npos)
			{
				data.replace(pos, toSearch.size(), replaceStr);
				pos = data.find(toSearch, pos + replaceStr.size());
			}
		}
	};
}