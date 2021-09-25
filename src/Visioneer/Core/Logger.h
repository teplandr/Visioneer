#pragma once

#include <memory>

#include <spdlog/spdlog.h>

namespace Visioneer
{

class Logger
{
public:
    static void init();
    static std::shared_ptr<spdlog::logger>& get() { return sInstance; }

    static spdlog::source_loc codeLocation(const char *filename, int line, const char *funcname);

private:
    static std::shared_ptr<spdlog::logger> sInstance;
};

}

#define VSR_CODE_LOCATION() ::Visioneer::Logger::codeLocation(__FILE__, __LINE__, __PRETTY_FUNCTION__)

#define VSR_LOG(level, ...) ::Visioneer::Logger::get()->log(VSR_CODE_LOCATION(), level, __VA_ARGS__)

#define VSR_TRACE(...)    VSR_LOG(spdlog::level::trace, __VA_ARGS__)
#define VSR_INFO(...)     VSR_LOG(spdlog::level::info, __VA_ARGS__)
#define VSR_WARN(...)     VSR_LOG(spdlog::level::warn, __VA_ARGS__)
#define VSR_ERROR(...)    VSR_LOG(spdlog::level::err, __VA_ARGS__)
#define VSR_CRITICAL(...) VSR_LOG(spdlog::level::critical, __VA_ARGS__)
