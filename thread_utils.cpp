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

thread_syncpoint_t create_syncpoint()
{
	thread_syncpoint_t newSp = {};

	newSp.signaled = false;
	newSp.mtx = floral::create_mutex();
	newSp.cv = floral::create_condition_variable();

	return newSp;
}

void wait_syncpoint(thread_syncpoint_t* i_syncPoint)
{
	floral::lock_guard_t guard(&i_syncPoint->mtx);
	while (!i_syncPoint->signaled)
	{
		floral::wait_for_condition(&i_syncPoint->cv, &i_syncPoint->mtx);
	}
}

void signal_syncpoint(thread_syncpoint_t* i_syncPoint)
{
	floral::lock_guard_t guard(&i_syncPoint->mtx);
	i_syncPoint->signaled = true;
	floral::notify_one(&i_syncPoint->cv);
}

void reset_syncpoint(thread_syncpoint_t* i_syncPoint)
{
	floral::lock_guard_t guard(&i_syncPoint->mtx);
	i_syncPoint->signaled = false;
}

// ----------------------------------------------------------------------------
}
