#include "container.h"

namespace floral
{
// ----------------------------------------------------------------------------
// ref: https://stackoverflow.com/questions/4062126/can-2-pthread-condition-variables-share-the-same-mutex
command_buffer_mt_t create_command_buffer_mt(voidptr i_memory, size i_size)
{
	command_buffer_mt_t newCmdBuff;
	newCmdBuff.bufferSize = i_size / 2;
	newCmdBuff.cmdData[0] = (p8)i_memory;
	newCmdBuff.cmdData[1] = (p8)i_memory + newCmdBuff.bufferSize;

	newCmdBuff.frontBuffer = nullptr;
	newCmdBuff.backBufferIdx = 0;
	newCmdBuff.writePtr = newCmdBuff.cmdData[0];
	newCmdBuff.readPtr = nullptr;

	newCmdBuff.mtx = create_mutex();
	newCmdBuff.frontCv = create_condition_variable();
	newCmdBuff.backCv = create_condition_variable();

	return newCmdBuff;
}

void create_command_buffer(voidptr i_memory, size i_size, command_buffer_t* io_cmdBuff)
{
	io_cmdBuff->bufferSize = i_size;
	io_cmdBuff->cmdData = (p8)i_memory;
	io_cmdBuff->writePtr = (p8)i_memory;
	io_cmdBuff->readPtr = (p8)i_memory;
}

command_buffer_t create_command_buffer(voidptr i_memory, size i_size)
{
	command_buffer_t newCmdBuff;
	create_command_buffer(i_memory, i_size, &newCmdBuff);
	return newCmdBuff;
}

void submit_buffer(command_buffer_mt_t* const i_cmdBuff)
{
	// wait for frontBuffer to finish processing
	floral::lock_guard_t guard(&i_cmdBuff->mtx);
	while (i_cmdBuff->frontBuffer != nullptr)
	{
		floral::wait_for_condition(&i_cmdBuff->backCv, &i_cmdBuff->mtx); // wait for front buffer to be processed
	}

	// finish waiting, swap back to front
	sidx frontBufferIdx = i_cmdBuff->backBufferIdx;
	i_cmdBuff->frontBuffer = i_cmdBuff->cmdData[frontBufferIdx];
	i_cmdBuff->readPtr = i_cmdBuff->frontBuffer;

	uidx backBufferIdx = (frontBufferIdx + 1) % 2;
	i_cmdBuff->backBufferIdx = backBufferIdx;
	i_cmdBuff->writePtr = i_cmdBuff->cmdData[backBufferIdx];

	floral::notify_one(&i_cmdBuff->frontCv); // notify that the front buffer is now ready
}

void begin_buffer(command_buffer_mt_t* const i_cmdBuff)
{
	// wait for new backBuffer
	floral::lock_mutex(&i_cmdBuff->mtx);
	while (i_cmdBuff->frontBuffer == nullptr)
	{
		floral::wait_for_condition(&i_cmdBuff->frontCv, &i_cmdBuff->mtx); // wait for front buffer
	}
}

void end_buffer(command_buffer_mt_t* const i_cmdBuff)
{
	i_cmdBuff->frontBuffer = nullptr;

	floral::notify_one(&i_cmdBuff->backCv); // notify that the front buffer was processed
	floral::unlock_mutex(&i_cmdBuff->mtx);
}

void reset_buffer(command_buffer_t* const io_cmdBuff)
{
	io_cmdBuff->writePtr = io_cmdBuff->cmdData;
	io_cmdBuff->readPtr = io_cmdBuff->cmdData;
}

void copy_buffer(command_buffer_t* o_to, const command_buffer_t* i_from)
{
	FLORAL_ASSERT(o_to->bufferSize >= i_from->bufferSize);
	size cmdSize = (aptr)i_from->writePtr - (aptr)i_from->cmdData;
	memcpy(o_to->cmdData, i_from->cmdData, cmdSize);
	o_to->writePtr = o_to->cmdData + cmdSize;
}

// ----------------------------------------------------------------------------
}
