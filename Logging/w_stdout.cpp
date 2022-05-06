#include "core/wrappers/w_stdout.h"

void Log::Wrapper::StdOut::write(std::string_view output) noexcept
{
	std::cout << output << "\n";
}
