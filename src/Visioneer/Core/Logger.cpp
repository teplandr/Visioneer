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

std::shared_ptr<spdlog::logger> &Logger::get()
{
    return sInstance;
}

spdlog::source_loc Logger::spdlogSourceLoc(const char *filename, int line, const std::string &funcsig)
{
    return {filename, line, funcsig.c_str()};
}

std::string Logger::spdlogSourceLocHelper(std::string_view functionSignature, std::string_view functionName)
{
    functionSignature.remove_suffix(functionSignature.size() - functionSignature.find(functionName) - functionName.size());
    functionSignature.remove_prefix(functionSignature.find_last_of(' ') + 1);
    return std::string(functionSignature);
}

}
