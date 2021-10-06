#include "thread_utils.h"

namespace floral
{
// ----------------------------------------------------------------------------

void initialize_package(frame_package_t* i_package)
{
    i_package->dataReady = false;
    i_package->mtx = create_mutex();
    i_package->cv = create_condition_variable();
}

void notify_package_ready(frame_package_t* i_package)
{
    floral::lock_guard_t guard(&i_package->mtx);
    i_package->dataReady = true;
    floral::notify_one(&i_package->cv);
}

void wait_for_package(frame_package_t* i_package)
{
    floral::lock_guard_t guard(&i_package->mtx);
    while (!i_package->dataReady)
    {
        floral::wait_for_condition(&i_package->cv, &i_package->mtx);
    }
}

// ----------------------------------------------------------------------------
}
