#pragma once

#include <string>
#include <string_view>
#include <map>
#include <vector>

#include "logger.h"
#include "aggregate.h"


namespace Log
{
    class Logger;

    namespace _core
    {
        static std::map<std::string, Logger> _logs;
    };

    void deleteLog(const std::string& key) noexcept
    {
        if (!_core::_logs.count(key))
            _core::_logs[key] = Logger(key);
        _core::_logs.erase(key);
    }

    [[nodiscard]] static Logger& getLog(const std::string& key) noexcept
    {
        if(!_core::_logs.count(key))
            _core::_logs[key] = Logger(key);

        return _core::_logs[ key ];
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
