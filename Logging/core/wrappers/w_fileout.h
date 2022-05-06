#pragma once

#include <fstream>
#include <filesystem>

#include "../output_wrapper.h"

namespace Log
{
	namespace Wrapper
	{

		class FileOut : public OutputWrapper
		{
			const static inline std::filesystem::path DEFAULT_PATH{ "log.log" };
			std::ofstream fileout{};

		public:
			FileOut(const std::filesystem::path& path = DEFAULT_PATH);

			~FileOut();

			void open(const std::filesystem::path& path) noexcept;
			void write(std::string_view output) noexcept override;
		};
	}
}