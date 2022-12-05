#pragma once
#include "stdaliases.h"

namespace floral
{
// ----------------------------------------------------------------------------

enum class log_level_e : u8
{
    verbose = 0,
    debug,
    info,
    warning,
    error
};

void initialize_logger(const_cstr i_name, log_level_e i_logLevel = log_level_e::verbose);
void tick_logger();
void logf(log_level_e i_logLevel, const_cstr i_fmt, ...);

// ----------------------------------------------------------------------------
}

#define LOG_VERBOSE(...)                        floral::logf(floral::log_level_e::verbose, __VA_ARGS__)
#define LOG_DEBUG(...)                          floral::logf(floral::log_level_e::debug, __VA_ARGS__)
#define LOG_INFO(...)                           floral::logf(floral::log_level_e::info, __VA_ARGS__)
#define LOG_WARNING(...)                        floral::logf(floral::log_level_e::warning, __VA_ARGS__)
#define LOG_ERROR(...)                          floral::logf(floral::log_level_e::error, __VA_ARGS__)
