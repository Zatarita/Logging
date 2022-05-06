#pragma once

#include <sstream>
#include <thread>
#include <chrono>
#include <map>

#include <windows.h>

#include "behaviour.h"


namespace Log
{
	class Logger
	{
		Behaviour loggingBehaviour{};
		std::string name{ "__ERROR_NAME__" };

	public:
		Logger(std::string name = "", const Behaviour& behaviour = Behaviour());

		[[nodiscard]] Behaviour& getBehaviour() noexcept;

		void operator<<(std::string_view str) const noexcept;
		void write(std::string_view str, const Log::Level& level = Log::Level::Info) const noexcept;

	private:
		// Before we use it as a formatting string we're going to
		// change extract special characters like date and time
		std::string format(std::string_view str, const Log::Level& level = Log::Level::Info) const noexcept;
		static void findAndReplaceAll(std::string& data, std::string_view toSearch, std::string_view replaceStr);

		/* Formatter functions */
		std::string getTime(std::string_view requestedTime) const noexcept;
		std::string getLevel(const Log::Level& level) const noexcept;
		std::string getPID(const bool hex = false) const noexcept;
		std::string getThread(const bool hex = false) const noexcept;

		static std::string getInt(const int& value, const bool hex = false) noexcept;
	};
}