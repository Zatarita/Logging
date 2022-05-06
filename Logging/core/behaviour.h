#pragma once

#include <string>
#include <vector>

#include "output_wrapper.h"

namespace Log
{
	enum class Level
	{
		Debug,
		Info,
		Warning,
		Error,
		Critical
	};

	class Behaviour
	{
		Level						listenLevel;
		std::vector<OutputWrapper*> probes;
		std::string formatString    { "$(name): $(message)" };

	public:
		Behaviour() : 
			listenLevel( Level::Info )
		{ }

		~Behaviour();

		OutputWrapper* addProbe(OutputWrapper* probe)	 noexcept;
		void		   deleteProbe(OutputWrapper* probe) noexcept;
		void		   setLevel(const Level& level)		 noexcept;

		void setFormatString(std::string_view fmt_str) noexcept;
		const std::string& getFormatString()     const noexcept;

		[[nodiscard]] const Level&						 getLevel()  const noexcept;
		[[nodiscard]] const std::vector<OutputWrapper*>& getProbes() const noexcept;
	};
}