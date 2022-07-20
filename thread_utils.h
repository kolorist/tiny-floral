#pragma once

#include "stdaliases.h"
#include "thread.h"

namespace floral
{
// ----------------------------------------------------------------------------

struct frame_package_t
{
	bool dataReady;

	floral::mutex_t mtx;
	floral::condition_variable_t readiedCv;
	floral::condition_variable_t consumedCv;
};

void initialize_package(frame_package_t* i_package);
void submit_package(frame_package_t* i_package);
void consume_package(frame_package_t* i_package);
void wait_for_package_readied(frame_package_t* i_package);
void wait_for_package_consumed(frame_package_t* i_package);

struct thread_syncpoint_t
{
	bool signaled;
	floral::mutex_t mtx;
	floral::condition_variable_t cv;
};

thread_syncpoint_t create_syncpoint();
void wait_syncpoint(thread_syncpoint_t* i_syncPoint);
void signal_syncpoint(thread_syncpoint_t* i_syncPoint);
void reset_syncpoint(thread_syncpoint_t* i_syncPoint);

// ----------------------------------------------------------------------------
}
