#include "container.h"

namespace floral
{
// ----------------------------------------------------------------------------
command_buffer_mt_t create_command_buffer(voidptr i_memory, size i_size)
{
    command_buffer_mt_t newCmdBuff;
    newCmdBuff.bufferSize = i_size / 2;
    newCmdBuff.cmdData[0] = (p8)i_memory;
    newCmdBuff.cmdData[1] = (p8)i_memory + newCmdBuff.bufferSize;

    newCmdBuff.backBuffer = nullptr;
    newCmdBuff.backBufferIdx = 1;

    newCmdBuff.mtx = create_mutex();
    newCmdBuff.frontCv = create_condition_variable();
    newCmdBuff.backCv = create_condition_variable();

    return newCmdBuff;
}

void submit_buffer(command_buffer_mt_t* const i_cmdBuff)
{
    // wait for backBuffer finish processing
    floral::lock_guard_t guard(&i_cmdBuff->mtx);
    while (i_cmdBuff->backBuffer != nullptr)
    {
        floral::wait_for_condition(&i_cmdBuff->frontCv, &i_cmdBuff->mtx);
    }

    // finish waiting, swap front to back
    sidx backBufferIdx = i_cmdBuff->backBufferIdx;
    backBufferIdx = (backBufferIdx + 1) % 2;
    i_cmdBuff->backBuffer = i_cmdBuff->cmdData[backBufferIdx];
    i_cmdBuff->backBufferIdx = backBufferIdx;

    floral::notify_one(&i_cmdBuff->backCv);
}

void begin_buffer(command_buffer_mt_t* const i_cmdBuff)
{
    // wait for new backBuffer
    floral::lock_mutex(&i_cmdBuff->mtx);
    while (i_cmdBuff->backBuffer == nullptr)
    {
        floral::wait_for_condition(&i_cmdBuff->backCv, &i_cmdBuff->mtx);
    }
}

void end_buffer(command_buffer_mt_t* const i_cmdBuff)
{
    i_cmdBuff->backBuffer = nullptr;

    floral::notify_one(&i_cmdBuff->frontCv);
    floral::unlock_mutex(&i_cmdBuff->mtx);
}

// ----------------------------------------------------------------------------
}
