#pragma once

#include <memory>

#include <spdlog/spdlog.h>

namespace Visioneer
{

class Logger
{
public:
    static void init();
    static std::shared_ptr<spdlog::logger>& get();

    static spdlog::source_loc spdlogSourceLoc(const char *filename, int line, const std::string& funcsig);
    static std::string spdlogSourceLocHelper(std::string_view functionSignature, std::string_view functionName);

private:
    static std::shared_ptr<spdlog::logger> sInstance;
};

}

#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
        #define VSR_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
    #define VSR_FUNC_SIG __FUNCSIG__
#else
    #define VSR_FUNC_SIG "VSR_FUNC_SIG unknown!"
#endif

#define VSR_CODE_LOCATION() Visioneer::Logger::spdlogSourceLoc(__FILE__, __LINE__, Visioneer::Logger::spdlogSourceLocHelper(VSR_FUNC_SIG, __FUNCTION__))

#define VSR_LOG(level, ...) Visioneer::Logger::get()->log(VSR_CODE_LOCATION(), level, __VA_ARGS__)

#define VSR_TRACE(...)    VSR_LOG(spdlog::level::trace, __VA_ARGS__)
#define VSR_INFO(...)     VSR_LOG(spdlog::level::info, __VA_ARGS__)
#define VSR_WARN(...)     VSR_LOG(spdlog::level::warn, __VA_ARGS__)
#define VSR_ERROR(...)    VSR_LOG(spdlog::level::err, __VA_ARGS__)
#define VSR_CRITICAL(...) VSR_LOG(spdlog::level::critical, __VA_ARGS__)
