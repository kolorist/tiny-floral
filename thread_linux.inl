#include "thread.h"

namespace floral
{
// ----------------------------------------------------------------------------

void sleep_thread(u32 i_durationMs)
{
}

void terminate_thread(s32 i_exitCode)
{
}

// ----------------------------------------------------------------------------

thread_t create_thread(const thread_desc_t* i_desc)
{
    thread_t newThread;
    return newThread;
}

void start_thread(thread_t* const io_thread)
{
}

void join_thread(thread_t* const io_thread)
{
}

// ----------------------------------------------------------------------------

mutex_t create_mutex()
{
    mutex_t newMutex;
    return newMutex;
}

void destroy_mutex(mutex_t* const i_mtx)
{
}

void lock_mutex(mutex_t* const i_mtx)
{
}

void unlock_mutex(mutex_t* const i_mtx)
{
}

condition_variable_t create_condition_variable()
{
    condition_variable_t newCv;
    return newCv;
}

void destroy_condition_variable(condition_variable_t* const i_cv)
{
}

void wait_for_condition(condition_variable_t* const i_cv, mutex_t* const i_mtx)
{
}

void notify_one(condition_variable_t* const i_cv)
{
}

void notify_all(condition_variable_t* const i_cv)
{
}

lock_guard_t::lock_guard_t(mutex_t* const i_mtx)
    : mtx(i_mtx)
{
    lock_mutex(mtx);
}

lock_guard_t::~lock_guard_t()
{
    unlock_mutex(mtx);
}

// ----------------------------------------------------------------------------
}
