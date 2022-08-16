#include "assert.h"

#include <string.h>

namespace floral
{
// ----------------------------------------------------------------------------

template <typename t_value>
array_t<t_value> create_array(ssize i_capacity, voidptr i_buffer)
{
	array_t<t_value> newArr;

	newArr.size = 0;
	newArr.capacity = i_capacity;
	newArr.data = (t_value*)i_buffer;

	return newArr;
}

template <typename t_value>
void array_push(array_t<t_value>* const i_arr, const t_value& i_value)
{
	i_arr->data[i_arr->size] = i_value;
	i_arr->size++;
}

template <typename t_value>
void array_empty(array_t<t_value>* const i_arr)
{
	i_arr->size = 0;
}

// ----------------------------------------------------------------------------

template <typename t_commandBuffer, typename t_value>
const bool cmdbuff_read(t_commandBuffer* const i_cmdBuff, t_value* o_value)
{
	if (i_cmdBuff->readPtr == i_cmdBuff->writePtr)
	{
		return false;
	}

	p8 rpos = i_cmdBuff->readPtr;
	memcpy(o_value, rpos, sizeof(t_value));
	i_cmdBuff->readPtr = rpos + sizeof(t_value);

	return true;
}

template <typename t_commandBuffer, typename t_value>
void cmdbuff_write(t_commandBuffer* const io_cmdBuff, const t_value& i_value)
{
	p8 wpos = io_cmdBuff->writePtr;
	memcpy(wpos, &i_value, sizeof(t_value));
	io_cmdBuff->writePtr = wpos + sizeof(t_value);
}

// ----------------------------------------------------------------------------

template<typename t_item>
circular_queue_mt_t<t_item> create_circular_queue_mt(voidptr i_memory, const size i_capacity)
{
	circular_queue_mt_t<t_item> newQueue = {};
	newQueue.data = (t_item*)i_memory;
	newQueue.head = 0;
	newQueue.tail = 0;
	newQueue.capacity = i_capacity;

	newQueue.mtx = floral::create_mutex();
	newQueue.cv = floral::create_condition_variable();

	return newQueue;
}

template<typename t_item>
void enqueue(circular_queue_mt_t<t_item>* i_queue, t_item& i_item)
{
	floral::lock_guard_t guard(&i_queue->mtx);

	i_queue->data[i_queue->tail % i_queue->capacity] = i_item;
	i_queue->tail++;

	floral::notify_one(&i_queue->cv);
}

template<typename t_item>
t_item dequeue_block(circular_queue_mt_t<t_item>* i_queue)
{
	floral::lock_guard_t guard(&i_queue->mtx);
	while (i_queue->head == i_queue->tail)
	{
		floral::wait_for_condition(&i_queue->cv, &i_queue->mtx);
	}

	t_item item = i_queue->data[i_queue->head % i_queue->capacity];
	i_queue->head++;

	return item;
}

// ----------------------------------------------------------------------------

template <typename t_handle_type>
handle_pool_t<t_handle_type> create_handle_pool(voidptr i_memory, const size i_memSize)
{
	size capacity = (i_memSize >> 1) / sizeof(t_handle_type);

	handle_pool_t<t_handle_type> newPool;
	newPool.dense = (ssize*)i_memory;
	newPool.sparse = (ssize*)((aptr)i_memory + capacity * sizeof(ssize));
	newPool.capacity = capacity;
	newPool.count = 0;

	for (size i = 0; i < capacity; i++)
	{
		newPool.dense[i] = i;
	}

	return newPool;
}

template <typename t_handle_type>
t_handle_type get_new_handle(handle_pool_t<t_handle_type>* i_handlePool)
{
	if (i_handlePool->count < i_handlePool->capacity)
	{
		size index = i_handlePool->count;
		i_handlePool->count++;

		// new handle value is always located at the end of dense array
		ssize handle = i_handlePool->dense[index];

		// till now, we have dense[index] == handle
		// keep the relationship: sparse[handle] == index and dense[index] == handle
		i_handlePool->sparse[handle] = index;

		return (t_handle_type)handle;
	}
	else
	{
		return t_handle_type(-1);
	}
}

template <typename t_handle_type>
void destroy_handle(handle_pool_t<t_handle_type>* i_handlePool, const t_handle_type i_handle)
{
	ssize index = i_handlePool->sparse[(size)i_handle];
	ssize lastDenseIndex = i_handlePool->count - 1;

	// move the freed handle value to the last of dense array, it will be reused in the next get_new_handle call
	// also redirect the index of the last allocated handle into the freed handle's slot.
	ssize tmpHandle = i_handlePool->dense[lastDenseIndex];
	i_handlePool->dense[lastDenseIndex] = (size)i_handle;
	i_handlePool->sparse[tmpHandle] = index;
	i_handlePool->dense[index] = tmpHandle; // keep the relationship: sparse[handle] == index and dense[index] == handle

	i_handlePool->count--;
}

template <typename t_handle_type>
const bool is_handle_valid(handle_pool_t<t_handle_type>* i_handlePool, const t_handle_type i_handle)
{
	ssize index = i_handlePool->sparse[(size)i_handle];
	return index < i_handlePool->count && i_handlePool->dense[index] == (size)i_handle;
}

}
