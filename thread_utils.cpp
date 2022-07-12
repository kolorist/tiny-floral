#include "thread_utils.h"

namespace floral
{
// ----------------------------------------------------------------------------

void initialize_package(frame_package_t* i_package)
{
    i_package->dataReady = false;
    i_package->mtx = create_mutex();
    i_package->readiedCv = create_condition_variable();
    i_package->consumedCv = create_condition_variable();
}

void submit_package(frame_package_t* i_package)
{
    floral::lock_guard_t guard(&i_package->mtx);
    i_package->dataReady = true;
    floral::notify_one(&i_package->readiedCv);
}

void consume_package(frame_package_t* i_package)
{
    floral::lock_guard_t guard(&i_package->mtx);
    i_package->dataReady = false;
    floral::notify_one(&i_package->consumedCv);
}

void wait_for_package_readied(frame_package_t* i_package)
{
    floral::lock_guard_t guard(&i_package->mtx);
    while (!i_package->dataReady)
    {
        floral::wait_for_condition(&i_package->readiedCv, &i_package->mtx);
    }
}

void wait_for_package_consumed(frame_package_t* i_package)
{
    floral::lock_guard_t guard(&i_package->mtx);
    while (i_package->dataReady)
    {
        floral::wait_for_condition(&i_package->consumedCv, &i_package->mtx);
    }
}

// ----------------------------------------------------------------------------
}
