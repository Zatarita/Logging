#pragma once

#include "core/core.h"
#include "core/aggregate.h"
#include "core/wrappers/w_fileout.h"
#include "core/wrappers/w_stdout.h"

#include <string>

// Kinda hackish; however, it works. shhh
namespace std
{
	std::string to_string(const std::string& str) { return str; }
}

#define VAR(var) std::string(#var) + std::string(": ") + std::to_string(var);