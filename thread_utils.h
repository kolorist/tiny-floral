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
    floral::condition_variable_t cv;
};

void initialize_package(frame_package_t* i_package);
void notify_package_ready(frame_package_t* i_package);
void wait_for_package(frame_package_t* i_package);

// ----------------------------------------------------------------------------
}
