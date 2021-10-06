#pragma once

#include "stdaliases.h"
#include "thread.h"
#include "memory.h"

namespace floral
{
// ----------------------------------------------------------------------------

template <typename t_value>
struct array_t
{
    ssize size;
    ssize capacity;

    t_value* data;
};

template <typename t_value>
array_t<t_value> create_array(ssize i_capacity, voidptr i_buffer);

template <typename t_value>
void array_push(array_t<t_value>* const i_arr, const t_value& i_value);

// ----------------------------------------------------------------------------

struct command_buffer_mt_t
{
    size bufferSize;
    p8 cmdData[2];

    // multithread control
    p8 backBuffer;
    uidx backBufferIdx;
    floral::mutex_t mtx;
    floral::condition_variable_t frontCv;
    floral::condition_variable_t backCv;
};

// ----------------------------------------------------------------------------

command_buffer_mt_t create_command_buffer(voidptr i_memory, size i_size);
void submit_buffer(command_buffer_mt_t* const i_cmdBuff);
void begin_buffer(command_buffer_mt_t* const i_cmdBuff);
void end_buffer(command_buffer_mt_t* const i_cmdBuff);

// ----------------------------------------------------------------------------
#if 0
template<typename t_item>
struct circular_queue_st_t
{

};

template<typename t_item>
struct circular_queue_mt_t
{
};

template<typename t_item>
queue_st_t<t_item> create_queue(pool_allocator_t* i_allocator);
#endif
// ----------------------------------------------------------------------------
}

#include "container.inl"
