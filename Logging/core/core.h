#pragma once

#include <string>
#include <string_view>
#include <map>
#include <vector>

#include "logger.h"
#include "aggregate.h"

namespace Log
{
    namespace _core
    {
        static std::map<std::string, Logger> _logs;
        static std::map<std::string, Aggregate> _aggregates;
    };

    void deleteLog(const std::string& key) noexcept
    {
        if (!_core::_logs.count(key))
            _core::_logs[key] = Logger(key);
        _core::_logs.erase(key);
    }

    [[nodiscard]] static Logger& getLog(const std::string& key) noexcept
    {
        if (!_core::_logs.count(key))
            _core::_logs[key] = Logger(key);

        return _core::_logs[key];
    }

    [[nodiscard]] static Aggregate& getAggregate(const std::string& key, const std::vector<Logger*> _loggers = {}) noexcept
    {
        if (!_core::_aggregates.count(key))
            _core::_aggregates[key] = Aggregate(_loggers);

        return _core::_aggregates[key];
    }

    [[nodiscard]] static const std::vector<std::string_view> logNames() noexcept
    {
        std::vector<std::string_view> ret;
        ret.reserve(_core::_logs.size());

        for (auto& [name, _] : _core::_logs)
            ret.push_back(name);

        return ret;
    }

}
