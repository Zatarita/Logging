#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>

#include <string>
#include <string_view>

namespace Log
{
	class OutputWrapper
	{
	public:
		virtual void write(std::string_view output) noexcept = 0;
		virtual ~OutputWrapper() = default;
	};
}