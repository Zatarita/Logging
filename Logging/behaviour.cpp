#include "core/behaviour.h"

namespace Log
{
	Behaviour::~Behaviour()
	{
		for (auto* probe : probes)
			delete probe;
	}

	OutputWrapper* Behaviour::addProbe(OutputWrapper* probe) noexcept
	{
		probes.push_back(probe);
		return probe;
	}

	void Behaviour::deleteProbe(OutputWrapper* probe) noexcept
	{
		for (size_t i = 0; i < probes.size(); i++)
			if (probes[i] == probe)
				probes.erase(probes.begin() + i);
	}

	void Behaviour::setLevel(const Level& level) noexcept
	{
		listenLevel = level;
	}

	void Behaviour::setFormatString(std::string_view fmt_str) noexcept
	{
		formatString = fmt_str;
	}

	const std::string& Behaviour::getFormatString() const noexcept
	{
		return formatString;
	}

	const Level& Behaviour::getLevel() const noexcept
	{
		return listenLevel;
	}

	const std::vector<OutputWrapper*>& Behaviour::getProbes() const noexcept
	{
		return probes;
	}
}

