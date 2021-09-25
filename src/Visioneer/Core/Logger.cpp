#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Visioneer
{

std::shared_ptr<spdlog::logger> Logger::sInstance;

void Logger::init()
{
    spdlog::sink_ptr logSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    logSink->set_pattern("%^[%T] [%!:%#] [%v]%$");

    sInstance = std::make_shared<spdlog::logger>("Visioneer", logSink);
    spdlog::register_logger(sInstance);
    sInstance->set_level(spdlog::level::trace);
    sInstance->flush_on(spdlog::level::trace);
}

spdlog::source_loc Logger::codeLocation(const char *filename, int line, const char *funcname)
{
    return {filename, line, funcname};
}

}
