#pragma once

#include "stdaliases.h"

#if defined(FLORAL_PLATFORM_WINDOWS)
#include <Windows.h>
#elif defined(FLORAL_PLATFORM_LINUX)
#else
// TODO
#endif

namespace floral
{
// ----------------------------------------------------------------------------

#if defined(FLORAL_PLATFORM_WINDOWS)
struct thread_platform_data_t
{
    DWORD id;
    HANDLE handle;
};
#elif defined(FLORAL_PLATFORM_LINUX)
struct thread_platform_data_t
{
};

#else
// TODO
#endif

typedef void (*thread_func_t)(voidptr i_data, voidptr i_allocator);

struct thread_desc_t
{
    voidptr allocator;
    voidptr data;
    thread_func_t func;
};

struct thread_t
{
    thread_platform_data_t platformData;
    thread_desc_t desc;
};

// ----------------------------------------------------------------------------

void sleep_thread(u32 i_durationMs);
void terminate_thread(s32 i_exitCode);

thread_t create_thread(const thread_desc_t* i_desc);
void start_thread(thread_t* const io_thread);
void join_thread(thread_t* const io_thread);

// ----------------------------------------------------------------------------

#if defined(FLORAL_PLATFORM_WINDOWS)
struct mutex_platform_data_t
{
    CRITICAL_SECTION handle;
};

struct cv_platform_data_t
{
    HANDLE notifyEvents[2];
    u32 waitersCount;
    CRITICAL_SECTION waitersCountLock;
};
#elif defined(FLORAL_PLATFORM_LINUX)
struct mutex_platform_data_t
{
};
struct cv_platform_data_t
{
};
#else
// TODO
#endif

struct mutex_t
{
    mutex_platform_data_t platformData;
};

struct condition_variable_t
{
    cv_platform_data_t platformData;
};

struct lock_guard_t
{
    lock_guard_t(mutex_t* const i_mtx);
    ~lock_guard_t();

    mutex_t* const mtx;
};

// ----------------------------------------------------------------------------

mutex_t create_mutex();
void destroy_mutex(mutex_t* const i_mtx);
void lock_mutex(mutex_t* const i_mtx);
void unlock_mutex(mutex_t* const i_mtx);

condition_variable_t create_condition_variable();
void destroy_condition_variable(condition_variable_t* const i_cv);
// will unlock the mutex
void wait_for_condition(condition_variable_t* const i_cv, mutex_t* const i_mtx);
void notify_one(condition_variable_t* const i_cv);
void notify_all(condition_variable_t* const i_cv);

// ----------------------------------------------------------------------------
}
