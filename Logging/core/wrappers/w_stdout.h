#pragma once

#include "../output_wrapper.h"

namespace Log
{
	namespace Wrapper
	{
		class StdOut : public OutputWrapper
		{
			void write(std::string_view output) noexcept override;
		};
	}
}