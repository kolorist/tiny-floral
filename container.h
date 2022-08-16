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

template <typename t_value>
void array_empty(array_t<t_value>* const i_arr);

// ----------------------------------------------------------------------------

struct command_buffer_mt_t
{
	size bufferSize;
	p8 cmdData[2];

	// multithread control
	uidx backBufferIdx;
	p8 writePtr;
	p8 readPtr;

	p8 frontBuffer;
	floral::mutex_t mtx;
	floral::condition_variable_t frontCv;
	floral::condition_variable_t backCv;
};

struct command_buffer_t
{
	size bufferSize;
	p8 cmdData;

	p8 writePtr;
	p8 readPtr;
};

// ----------------------------------------------------------------------------

command_buffer_mt_t create_command_buffer_mt(voidptr i_memory, size i_size);
void create_command_buffer(voidptr i_memory, size i_size, command_buffer_t* io_cmdBuff);
command_buffer_t create_command_buffer(voidptr i_memory, size i_size);
void submit_buffer(command_buffer_mt_t* const i_cmdBuff);
void begin_buffer(command_buffer_mt_t* const i_cmdBuff);
void end_buffer(command_buffer_mt_t* const i_cmdBuff);
void reset_buffer(command_buffer_t* const io_cmdBuff);
void copy_buffer(command_buffer_t* o_to, const command_buffer_t* i_from);

template <typename t_commandBuffer, typename t_value>
const bool cmdbuff_read(t_commandBuffer* const i_cmdBuff, t_value* o_value);
template <typename t_commandBuffer, typename t_value>
void cmdbuff_write(t_commandBuffer* const io_cmdBuff, const t_value& i_value);

// ----------------------------------------------------------------------------

template<typename t_item>
struct circular_queue_mt_t
{
	t_item* data;
	size head;
	size tail;
	size capacity;

	floral::mutex_t mtx;
	floral::condition_variable_t cv;
};

template<typename t_item>
circular_queue_mt_t<t_item> create_circular_queue_mt(voidptr i_memory, const size i_capacity);

template<typename t_item>
void enqueue(circular_queue_mt_t<t_item>* i_queue, t_item& i_item);

template<typename t_item>
t_item dequeue_block(circular_queue_mt_t<t_item>* i_queue);

// ----------------------------------------------------------------------------

template <typename t_handle_type>
struct handle_pool_t
{
	t_handle_type* dense;
	t_handle_type* sparse;

	size capacity;
	size count;
};

template <typename t_handle_type>
handle_pool_t<t_handle_type> create_handle_pool(voidptr i_memory, const size i_memSize);

template <typename t_handle_type>
t_handle_type get_new_handle(handle_pool_t<t_handle_type>* i_handlePool);

template <typename t_handle_type>
void destroy_handle(handle_pool_t<t_handle_type>* i_handlePool, const t_handle_type i_handle);

template <typename t_handle_type>
const bool is_handle_valid(handle_pool_t<t_handle_type>* i_handlePool, const t_handle_type i_handle);

#if 0
template<typename t_item>
struct circular_queue_st_t
{

};

template<typename t_item>
queue_st_t<t_item> create_queue(pool_allocator_t* i_allocator);
#endif
// ----------------------------------------------------------------------------
}

#include "container.inl"
