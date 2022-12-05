#include "memory.h"

#include "assert.h"
#include "misc.h"

#include <stdio.h>
#include <string.h>

#if defined(FLORAL_PLATFORM_WINDOWS)
#	include <Windows.h>
#	if defined(FLORAL_PLATFORM_XBOX_SCARLETT)
#		include <xmem.h>
#	elif defined(FLORAL_PLATFORM_XBOX_ONE)
#	endif
#elif defined(FLORAL_PLATFORM_LINUX)
#	include <stdlib.h>
#endif

namespace floral
{
// ----------------------------------------------------------------------------

#define FILL_MEMORY 0
#define CHECK_ALIGNMENT(addr, alignment) FLORAL_ASSERT(((aptr)addr & (alignment - 1)) == 0)

// alignment of a memory allocation buffer (usually big, in KBytes)
static constexpr size k_defaultSizeAligment = SIZE_KB(16); // use 16KB because allocation size for PS5 is a multiply of 16KB

// alignment of a single allocated memory region in the allocator (usually small, in Bytes)
#if defined(FLORAL_PLATFORM_ANDROID)
static constexpr size k_defaultAlignment = 4;
#else
static constexpr size k_defaultAlignment = 8;
#endif

static voidptr internal_malloc(const size i_bytes, voidptr i_userData = nullptr)
{
	voidptr addr = nullptr;
#if defined(FLORAL_PLATFORM_WINDOWS)
	addr = (voidptr)VirtualAlloc(nullptr, i_bytes, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#elif defined(FLORAL_PLATFORM_ANDROID)
	addr = ::malloc(i_bytes);
#elif defined(FLORAL_PLATFORM_XBOX_SCARLETT)
	addr = (voidptr)XMemVirtualAlloc(nullptr, i_bytes, MEM_COMMIT | MEM_RESERVE | MEM_64K_PAGES, XMEM_CPU, PAGE_READWRITE);
#endif
	return addr;
}

static void internal_free(voidptr i_data, size i_bytes)
{
#if defined(FLORAL_PLATFORM_WINDOWS)
	VirtualFree((LPVOID)i_data, 0, MEM_RELEASE);
#elif defined(FLORAL_PLATFORM_ANDROID)
	::free(i_data);
#elif defined(FLORAL_PLATFORM_XBOX_SCARLETT)
	VirtualFree((LPVOID)i_data, 0, MEM_RELEASE);
#endif
}

static inline aptr align_aptr(aptr i_aptr, const size i_alignment)
{
	if ((i_aptr & (i_alignment - 1)) == 0)
	{
		return i_aptr;
	}

	return i_aptr + (i_alignment - (i_aptr & (i_alignment - 1)));
}

static inline voidptr align_address(voidptr i_addr, const size i_alignment)
{
	return (voidptr)(align_aptr((aptr)i_addr, i_alignment));
}

static void fill_platform_info(platform_info_t* o_platformInfo)
{
	o_platformInfo->pageSize = SIZE_KB(4);
}

static void reset_block(alloc_header_t* i_block)
{
#if FILL_MEMORY
	p8 fillAddr0 = (p8)i_block->marker;
	size fc0 = (aptr)i_block - (aptr)fillAddr0;
	memset(fillAddr0, 0xfe, fc0);
	p8 fillAddr = (p8)i_block + sizeof(alloc_header_t);
	size fc1 = i_block->frameSize - fc0 - sizeof(alloc_header_t);
	memset(fillAddr, 0xfe, fc1);
#endif
}

// ----------------------------------------------------------------------------

static allocator_t* create_allocator(const_cstr i_id, const size i_headerSize, const size i_bytes, voidptr i_userData = nullptr)
{
	const size headerSize = i_headerSize;
	const size headerSizeAligned = align_size(headerSize, k_defaultAlignment);
	const size realBytes = align_size(i_bytes + headerSizeAligned, k_defaultSizeAligment);
	voidptr pMem = internal_malloc(realBytes, i_userData);

	allocator_t* allocator = (allocator_t*)pMem;

	fill_platform_info(&(allocator->platformInfo));
	allocator->id[0] = i_id[0];
	allocator->id[1] = i_id[1];
	allocator->id[2] = i_id[2];
	allocator->id[3] = i_id[3];

	voidptr pAllocatorEnd = (voidptr)((aptr)pMem + headerSize);
	voidptr pAllocatorEndAligned = align_address(pAllocatorEnd, k_defaultAlignment);

	FLORAL_ASSERT((aptr)pAllocatorEndAligned == ((aptr)pMem + headerSizeAligned));
	FLORAL_ASSERT((aptr)pMem + realBytes - (aptr)pAllocatorEndAligned >= i_bytes);

	allocator->realAllocatedBytes = realBytes;
	allocator->baseAddress = pAllocatorEndAligned;
	allocator->capacity = i_bytes;

	return allocator;
}

static allocator_t* create_allocator(const_cstr i_id, voidptr i_buffer, const size i_headerSize, const size i_bytes, voidptr i_userData = nullptr)
{
	const size headerSize = i_headerSize;
	const size headerSizeAligned = (size)align_aptr((aptr)headerSize, k_defaultAlignment);
	const size realBytes = align_size(i_bytes + headerSizeAligned, k_defaultSizeAligment);
	voidptr pMem = i_buffer;

	allocator_t* allocator = (allocator_t*)pMem;

	fill_platform_info(&(allocator->platformInfo));
	allocator->id[0] = i_id[0];
	allocator->id[1] = i_id[1];
	allocator->id[2] = i_id[2];
	allocator->id[3] = i_id[3];

	voidptr pAllocatorEnd = (voidptr)((aptr)pMem + headerSize);
	voidptr pAllocatorEndAligned = align_address(pAllocatorEnd, k_defaultAlignment);

	FLORAL_ASSERT((aptr)pAllocatorEndAligned == ((aptr)pMem + headerSizeAligned));
	FLORAL_ASSERT((aptr)pMem + realBytes - (aptr)pAllocatorEndAligned >= i_bytes);

	allocator->realAllocatedBytes = realBytes;
	allocator->baseAddress = pAllocatorEndAligned;
	allocator->capacity = i_bytes;

	return allocator;
}

linear_allocator_t* create_linear_allocator(const_cstr i_id, const size i_bytes, voidptr i_userData /* = nullptr */)
{
	linear_allocator_t* allocator = (linear_allocator_t*)create_allocator(i_id, sizeof(linear_allocator_t), i_bytes, i_userData);
	reset_allocator(allocator);
	return allocator;
}

freelist_allocator_t* create_freelist_allocator(const_cstr i_id, const size i_bytes)
{
	freelist_allocator_t* allocator = (freelist_allocator_t*)create_allocator(i_id, sizeof(freelist_allocator_t), i_bytes);
	reset_allocator(allocator);
	return allocator;
}

freelist_allocator_t* create_freelist_allocator(const_cstr i_id, voidptr i_buffer, const size i_bytes)
{
	freelist_allocator_t* allocator = (freelist_allocator_t*)create_allocator(i_id, i_buffer, sizeof(freelist_allocator_t), i_bytes);
	reset_allocator(allocator);
	return allocator;
}

pool_allocator_t* create_pool_allocator(const_cstr i_id, const size i_slotSize, const size i_slotCapacity)
{
	return create_pool_allocator(i_id, i_slotSize, i_slotCapacity, k_defaultAlignment);
}

pool_allocator_t* create_pool_allocator(const_cstr i_id, const size i_slotSize, const size i_slotCapacity, const size i_alignment)
{
	FLORAL_ASSERT(i_alignment >= k_defaultAlignment);
	const size allocatorHeaderSize = align_aptr(sizeof(pool_allocator_t), k_defaultAlignment);
	const size headerSize = sizeof(alloc_header_t) + sizeof(voidptr); // the voidptr is the pointer to the header
	const size frameSlotSize = align_aptr(headerSize + i_alignment + i_slotSize, k_defaultAlignment);
	size bytes = frameSlotSize * i_slotCapacity;
	size realBytes = align_size(allocatorHeaderSize + bytes, k_defaultSizeAligment);

	voidptr pMem = internal_malloc(realBytes);
	pool_allocator_t* allocator = (pool_allocator_t*)pMem;
	fill_platform_info(&(allocator->platformInfo));
	allocator->id[0] = i_id[0];
	allocator->id[1] = i_id[1];
	allocator->id[2] = i_id[2];
	allocator->id[3] = i_id[3];

	voidptr pAllocatorEnd = (voidptr)((aptr)pMem + allocatorHeaderSize);
	voidptr pAllocatorEndAligned = align_address(pAllocatorEnd, k_defaultAlignment);

	FLORAL_ASSERT((aptr)pAllocatorEndAligned == ((aptr)pMem + allocatorHeaderSize));
	FLORAL_ASSERT((aptr)pMem + realBytes - (aptr)pAllocatorEndAligned == bytes);

	allocator->realAllocatedBytes = realBytes;
	allocator->baseAddress = pAllocatorEndAligned;
	allocator->capacity = bytes;

	allocator->firstFreeBlock = nullptr;
	allocator->lastAlloc = nullptr;
	allocator->usedBytes = 0;
	allocator->alignment = i_alignment;
	allocator->slotSize = i_slotSize;
	allocator->slotCapacity = i_slotCapacity;
	allocator->usedSlot = 0;

	p8 addr = (p8)allocator->baseAddress;
	alloc_header_t* prevSlot = nullptr;
	for (size i = 0; i < i_slotCapacity; i++)
	{
		alloc_header_t* slot = (alloc_header_t*)addr;
		slot->prevAlloc = prevSlot;
		slot->nextAlloc = nullptr;
		slot->marker = addr;
		slot->frameSize = frameSlotSize;
		slot->alignment = i_alignment;
		slot->dataSize = i_slotSize;
		reset_block(slot);

		if (allocator->firstFreeBlock == nullptr)
		{
			allocator->firstFreeBlock = slot;
		}

		if (prevSlot != nullptr)
		{
			prevSlot->nextAlloc = slot;
		}
		prevSlot = slot;
		addr += frameSlotSize;
	}

	FLORAL_ASSERT((aptr)addr == ((aptr)pMem + realBytes));

	return allocator;
}

static void destroy_allocator(allocator_t* io_allocator)
{
	voidptr addr = (voidptr)io_allocator;
	internal_free(addr, io_allocator->realAllocatedBytes);
}

// linear_allocator

voidptr allocate(const size i_bytes, linear_allocator_t* i_allocator)
{
	return allocate(i_bytes, k_defaultAlignment, i_allocator);
}

voidptr reallocate(voidptr i_data, const size i_newBytes, linear_allocator_t* i_allocator)
{
	return reallocate(i_data, i_newBytes, k_defaultAlignment, i_allocator);
}

voidptr allocate(const size i_bytes, const size i_alignment, linear_allocator_t* i_allocator)
{
	FLORAL_ASSERT(i_bytes > 0);
	const size alignment = max(i_alignment, k_defaultAlignment);
	// the whole stack frame size, count all headers, displacement, data, ...
	// ....[A..A][H..H][ptrToHeader][D..D]
	// A: aligning-bytes (use k_defaultAlignment)
	// H: header
	// D: data
	// sizeof([A..A]) + sizeof([A'..A']) == alignment
	size allocHeaderSize = sizeof(alloc_header_t) + sizeof(voidptr);

	// start address of the frame
	p8 orgAddr = i_allocator->currentMarker;
	p8 headerAddr = (p8)align_address(orgAddr, k_defaultAlignment); // forward align, this is address of the header
	p8 dataAddr = (p8)align_address(headerAddr + allocHeaderSize, alignment); // align forward, this is the start of the data
	voidptr* ptrToHeader = (voidptr*)((aptr)dataAddr - sizeof(voidptr));

	size frameSize = (aptr)dataAddr - (aptr)orgAddr + i_bytes;

	if (i_allocator->usedBytes + frameSize > i_allocator->capacity)
	{
		FLORAL_ASSERT_MSG(false, "Out of memory!");
		return nullptr;
	}

	// save info about displacement and allocated frame size
	alloc_header_t* header = (alloc_header_t*)headerAddr;
	header->prevAlloc = i_allocator->lastAlloc;
	header->nextAlloc = nullptr;
	header->marker = orgAddr;
	header->frameSize = frameSize;
	header->alignment = alignment;
	header->dataSize = i_bytes;

	*ptrToHeader = (voidptr)header;

	if (i_allocator->lastAlloc != nullptr)
	{
		i_allocator->lastAlloc->nextAlloc = header;
	}
	i_allocator->lastAlloc = header;

	// increase marker
	i_allocator->currentMarker += frameSize;

	i_allocator->usedBytes += frameSize;
	CHECK_ALIGNMENT(dataAddr, alignment);
	return (voidptr*)dataAddr;
}

voidptr reallocate(voidptr i_data, const size i_newBytes, const size i_alignment, linear_allocator_t* i_allocator)
{
	FLORAL_ASSERT(i_data != nullptr);
	alloc_header_t** ppHeader = (alloc_header_t**)((aptr)i_data - sizeof(voidptr));
	alloc_header_t* header = *ppHeader;

	voidptr newData = allocate(i_newBytes, i_alignment, i_allocator);
	size copySize = floral::min(i_newBytes, header->dataSize);
	memcpy(newData, i_data, copySize);
	return newData;
}

void free(voidptr i_data, linear_allocator_t* i_allocator)
{
	if (i_data == nullptr)
	{
		return;
	}

	alloc_header_t** ppHeader = (alloc_header_t**)((aptr)i_data - sizeof(voidptr));
	alloc_header_t* header = *ppHeader;
	size frameSize = header->frameSize;

	p8 lastMarker = i_allocator->currentMarker - header->frameSize;
	if (lastMarker != header->marker)
	{
		FLORAL_ASSERT_MSG(false, "Invalid free: not in allocation order");
		return;
	}

	// adjust header
	if (header->prevAlloc)
	{
		header->prevAlloc->nextAlloc = nullptr;
	}
	i_allocator->lastAlloc = header->prevAlloc;

	i_allocator->currentMarker -= frameSize;

	FLORAL_ASSERT(i_allocator->usedBytes >= frameSize);
	i_allocator->usedBytes -= frameSize;
}

void reset_allocator(linear_allocator_t* io_allocator)
{
	io_allocator->currentMarker = (p8)io_allocator->baseAddress;
	io_allocator->lastAlloc = nullptr;
	io_allocator->usedBytes = 0;
}

void destroy_allocator(linear_allocator_t* io_allocator)
{
	destroy_allocator((allocator_t*)io_allocator);
}

// freelist_allocator

voidptr allocate(const size i_bytes, freelist_allocator_t* i_allocator)
{
	return allocate(i_bytes, k_defaultAlignment, i_allocator);
}

voidptr reallocate(voidptr i_data, const size i_newBytes, freelist_allocator_t* i_allocator)
{
	return reallocate(i_data, i_newBytes, 0, i_allocator);
}

static const bool can_fit(alloc_header_t* i_header, const size i_bytes, const size i_alignment)
{
	const size allocHeaderSize = sizeof(alloc_header_t) + sizeof(voidptr);

	// start address of the frame
	aptr headerAddr = (aptr)i_header;
	aptr dataAddr = align_aptr(headerAddr + allocHeaderSize, i_alignment); // align forward, this is the start of the data

	size frameSizeNeeded = dataAddr - (aptr)i_header->marker + i_bytes;
	return (frameSizeNeeded <= i_header->frameSize);
}

static const bool can_create_new_block(alloc_header_t* i_header, const size i_bytes, const size i_alignment)
{
	const size allocHeaderSize = sizeof(alloc_header_t) + sizeof(voidptr);

	// start address of the frame
	aptr headerAddr = (aptr)i_header;
	aptr dataAddr = align_aptr(headerAddr + allocHeaderSize, i_alignment); // align forward, this is the start of the data

	size frameSizeNeeded = dataAddr - (aptr)i_header->marker + i_bytes;

	aptr endFrameAddr = (aptr)(i_header->marker) + i_header->frameSize;
	aptr nextHeaderAddr = align_aptr((aptr)i_header->marker + frameSizeNeeded, k_defaultAlignment);
	aptr nextDataAddr = align_aptr(nextHeaderAddr + allocHeaderSize, k_defaultAlignment);
	return (nextDataAddr <= endFrameAddr);
}

voidptr allocate(const size i_bytes, const size i_alignment, freelist_allocator_t* i_allocator)
{
	FLORAL_ASSERT(i_bytes > 0);
	const size alignment = max(i_alignment, k_defaultAlignment);
	// first-fit strategy
	alloc_header_t* currBlock = i_allocator->firstFreeBlock;
	// search
	while (currBlock && !can_fit(currBlock, i_bytes, alignment))
	{
		currBlock = currBlock->nextAlloc;
	}

	const size allocHeaderSize = sizeof(alloc_header_t) + sizeof(voidptr);
	if (currBlock) // found it!
	{
		voidptr dataAddr = align_address((p8)currBlock + allocHeaderSize, alignment);
		voidptr* ptrToHeader = (voidptr*)((aptr)dataAddr - sizeof(voidptr));
		// C1: a new free block needs to be created
		if (can_create_new_block(currBlock, i_bytes, alignment))
		{
			p8 orgAddr = currBlock->marker;
			p8 dataAddr = (p8)align_aptr((aptr)currBlock + allocHeaderSize, alignment); // align forward, this is the start of the data
			size frameSize = (aptr)dataAddr - (aptr)orgAddr + i_bytes;

			size oldFrameSize = currBlock->frameSize;
			// update frame_size of currBlock
			currBlock->frameSize = frameSize;

			// create new free block
			aptr endFrameAddr = (aptr)(currBlock->marker) + oldFrameSize;
			alloc_header_t* newBlock = (alloc_header_t*)align_address(currBlock->marker + frameSize, k_defaultAlignment);
			// update pointers of new free block
			newBlock->nextAlloc = currBlock->nextAlloc;
			newBlock->prevAlloc = currBlock->prevAlloc;
			newBlock->marker = currBlock->marker + frameSize;
			newBlock->frameSize = endFrameAddr - (aptr)newBlock->marker;
			newBlock->alignment = 0;
			newBlock->dataSize = 0;

			// delete pointers on currBlock as it's already occupied
			currBlock->nextAlloc = nullptr;
			currBlock->prevAlloc = nullptr;

			// update linked list
			if (newBlock->prevAlloc)
			{
				newBlock->prevAlloc->nextAlloc = newBlock;
			}
			if (newBlock->nextAlloc)
			{
				newBlock->nextAlloc->prevAlloc = newBlock;
			}

			// update first free block
			if (currBlock == i_allocator->firstFreeBlock)
			{
				i_allocator->firstFreeBlock = newBlock;
			}
		}
		else
		{
			// C2: we can use all of this block
			if (currBlock->prevAlloc)
			{
				currBlock->prevAlloc->nextAlloc = currBlock->nextAlloc;
			}
			if (currBlock->nextAlloc)
			{
				currBlock->nextAlloc->prevAlloc = currBlock->prevAlloc;
			}
			// update first free block?
			if (currBlock == i_allocator->firstFreeBlock)
			{
				i_allocator->firstFreeBlock = currBlock->nextAlloc;
			}

			// delete pointers
			currBlock->nextAlloc = nullptr;
			currBlock->prevAlloc = nullptr;
		}

		currBlock->nextAlloc = nullptr;
		currBlock->prevAlloc = i_allocator->lastAlloc;
		currBlock->alignment = alignment;
		currBlock->dataSize = i_bytes;

		*ptrToHeader = (voidptr)currBlock;

#if FILL_MEMORY
		memset(dataAddr, 0, i_bytes);
#endif

		if (i_allocator->lastAlloc != nullptr)
		{
			i_allocator->lastAlloc->nextAlloc = currBlock;
		}
		i_allocator->lastAlloc = currBlock;
		i_allocator->usedBytes += currBlock->frameSize;

		return dataAddr;
	}

	FLORAL_ASSERT_MSG(false, "Out of free memory!");
	// nothing found, cannot allocate anything
	return nullptr;
}

voidptr reallocate(voidptr i_data, const size i_newBytes, const size i_alignment, freelist_allocator_t* i_allocator)
{
	FLORAL_ASSERT(i_data != nullptr);
	alloc_header_t** ppHeader = (alloc_header_t**)((aptr)i_data - sizeof(voidptr));
	alloc_header_t* header = *ppHeader;

	size alignment = i_alignment;
	if (i_alignment == 0)
	{
		alignment = header->alignment;
	}

	voidptr newData = allocate(i_newBytes, alignment, i_allocator);
	size copySize = floral::min(i_newBytes, header->dataSize);
	memcpy(newData, i_data, copySize);
	free(i_data, i_allocator);
	return newData;
}

// free a block, update the free list
static void free_block(alloc_header_t* i_block, alloc_header_t* i_prevFree, alloc_header_t* i_nextFree)
{
	i_block->nextAlloc = nullptr;
	i_block->prevAlloc = nullptr;
	i_block->alignment = 0;
	i_block->dataSize = 0;

	reset_block(i_block);

	// adjust pointers
	if (i_prevFree)
	{
		i_block->prevAlloc = i_prevFree;
		i_prevFree->nextAlloc = i_block;
	}
	if (i_nextFree)
	{
		i_block->nextAlloc = i_nextFree;
		i_nextFree->prevAlloc = i_block;
	}
}

// check if 2 blocks can be joined
static const bool can_join(alloc_header_t* i_leftBlock, alloc_header_t* i_rightBlock)
{
	aptr leftEnd = ((aptr)i_leftBlock->marker + i_leftBlock->frameSize);
	aptr rightStart = (aptr)i_rightBlock->marker;
	return (leftEnd == rightStart);
}

// join 2 *free* blocks together
static const bool join_blocks(alloc_header_t* i_leftBlock, alloc_header_t* i_rightBlock)
{
	if (!i_leftBlock || !i_rightBlock)
	{
		return false;
	}

	if (can_join(i_leftBlock, i_rightBlock))
	{
		// adjust left block's pointers
		i_leftBlock->nextAlloc = i_rightBlock->nextAlloc;
		// update frame_size of left block
		i_leftBlock->frameSize += i_rightBlock->frameSize;
		if (i_rightBlock->nextAlloc)
		{
			i_rightBlock->nextAlloc->prevAlloc = i_leftBlock;
		}
		reset_block(i_leftBlock);
		return true;
	}
	else
	{
		return false;
	}
}

void free(voidptr i_data, freelist_allocator_t* i_allocator)
{
	if (i_data == nullptr)
	{
		return;
	}

	alloc_header_t** ppReleaseBlock = (alloc_header_t**)((aptr)i_data - sizeof(voidptr));
	alloc_header_t* releaseBlock = *ppReleaseBlock;
	size frameSize = releaseBlock->frameSize;

	if (releaseBlock->nextAlloc)
	{
		releaseBlock->nextAlloc->prevAlloc = releaseBlock->prevAlloc;
	}
	if (releaseBlock->prevAlloc)
	{
		releaseBlock->prevAlloc->nextAlloc = releaseBlock->nextAlloc;
	}
	if (releaseBlock == i_allocator->lastAlloc)
	{
		i_allocator->lastAlloc = releaseBlock->prevAlloc;
	}

	// search for nearest-after free block
	alloc_header_t* nextFree = i_allocator->firstFreeBlock;
	while (nextFree && ((aptr)nextFree <= (aptr)releaseBlock))
	{
		nextFree = nextFree->nextAlloc;
	}

	alloc_header_t* prevFree = nullptr;
	if (nextFree != nullptr)
	{
		prevFree = nextFree->prevAlloc;
	}

	// free releaseBlock
	free_block(releaseBlock, prevFree, nextFree);

	// update first free block
	if ((aptr)releaseBlock < (aptr)i_allocator->firstFreeBlock || i_allocator->firstFreeBlock == nullptr)
	{
		i_allocator->firstFreeBlock = releaseBlock;
	}
	// join blocks if possible
	if (join_blocks(prevFree, releaseBlock))
	{
		join_blocks(prevFree, nextFree);
	}
	else
	{
		join_blocks(releaseBlock, nextFree);
	}

	FLORAL_ASSERT(i_allocator->usedBytes >= frameSize);
	i_allocator->usedBytes -= frameSize;
}

void reset_allocator(freelist_allocator_t* io_allocator)
{
	aptr endAddr = (aptr)io_allocator + io_allocator->realAllocatedBytes;
	const size allocHeaderSize = sizeof(alloc_header_t) + sizeof(voidptr);
	aptr nextHeaderAddr = align_aptr((aptr)io_allocator->baseAddress, k_defaultAlignment);
	aptr nextDataAddr = align_aptr(nextHeaderAddr + allocHeaderSize, k_defaultAlignment);
	if (nextDataAddr > endAddr)
	{
		FLORAL_ASSERT_MSG(false, "Capacity too small");
		return;
	}

	alloc_header_t* firstFreeBlock = (alloc_header_t*)align_address(io_allocator->baseAddress, k_defaultAlignment);
	firstFreeBlock->prevAlloc = nullptr;
	firstFreeBlock->nextAlloc = nullptr;
	firstFreeBlock->marker = (p8)io_allocator->baseAddress;
	firstFreeBlock->frameSize = endAddr - (aptr)firstFreeBlock->marker;
	firstFreeBlock->alignment = 0;
	firstFreeBlock->dataSize = 0;
	reset_block(firstFreeBlock);
	io_allocator->firstFreeBlock = firstFreeBlock;
	io_allocator->lastAlloc = nullptr;
	io_allocator->usedBytes = 0;
}

void destroy_allocator(freelist_allocator_t* io_allocator)
{
	destroy_allocator((allocator_t*)io_allocator);
}

// ----------------------------------------------------------------------------

voidptr allocate(pool_allocator_t* i_allocator)
{
	FLORAL_ASSERT_MSG(i_allocator->firstFreeBlock != nullptr, "Out of memory slot!");

	const size headerSize = sizeof(alloc_header_t) + sizeof(voidptr);

	alloc_header_t* freeBlock = i_allocator->firstFreeBlock;
	i_allocator->firstFreeBlock = freeBlock->nextAlloc;
	freeBlock->nextAlloc = nullptr;
	freeBlock->prevAlloc = i_allocator->lastAlloc;
	if (i_allocator->lastAlloc != nullptr)
	{
		i_allocator->lastAlloc->nextAlloc = freeBlock;
	}

	i_allocator->lastAlloc = freeBlock;

	p8 headerEndAddr = freeBlock->marker + headerSize;
	p8 dataAddr = (p8)align_address(headerEndAddr, i_allocator->alignment);
	voidptr* ptrToHeader = (voidptr*)((aptr)dataAddr - sizeof(voidptr));
#if FILL_MEMORY
	memset(dataAddr, 0, i_allocator->slotSize);
#endif
	freeBlock->dataSize = i_allocator->slotSize;
	*ptrToHeader = (voidptr)freeBlock;

	i_allocator->usedBytes += freeBlock->frameSize;

	return dataAddr;
}

void free(voidptr i_data, pool_allocator_t* i_allocator)
{
	if (i_data == nullptr)
	{
		return;
	}

	alloc_header_t** ppHeader = (alloc_header_t**)((aptr)i_data - sizeof(voidptr));
	alloc_header_t* header = *ppHeader;

	header->dataSize = 0;
	if (header->nextAlloc)
	{
		header->nextAlloc->prevAlloc = header->prevAlloc;
	}
	if (header->prevAlloc)
	{
		header->prevAlloc->nextAlloc = header->nextAlloc;
	}

	// are we freeing the last allocation?
	if (header == i_allocator->lastAlloc)
	{
		// yes, then update the list's tail element
		i_allocator->lastAlloc = header->prevAlloc;
	}

	// update this slot's next free slot to next free slot
	header->nextAlloc = i_allocator->firstFreeBlock;
	i_allocator->firstFreeBlock = header;
	reset_block(header);

	i_allocator->usedBytes -= header->frameSize;
}

void destroy_allocator(pool_allocator_t* io_allocator)
{
	destroy_allocator((allocator_t*)io_allocator);
}

const size get_allocated_size(voidptr i_data)
{
	if (i_data == nullptr)
	{
		return 0 ;
	}

	alloc_header_t** ppHeader = (alloc_header_t**)((aptr)i_data - sizeof(voidptr));
	alloc_header_t* header = *ppHeader;
	return header->frameSize;
}

// ----------------------------------------------------------------------------
// helpers
linear_allocator_i::linear_allocator_i()
	: allocator(nullptr)
{
	// nothing :)
}

linear_allocator_i::~linear_allocator_i()
{
	destroy();
}

void linear_allocator_i::initialize(const_cstr i_id, const size i_bytes)
{
	FLORAL_ASSERT(allocator == nullptr);
	allocator = create_linear_allocator(i_id, i_bytes);
}

void linear_allocator_i::destroy()
{
	if (allocator != nullptr)
	{
		destroy_allocator(allocator);
		allocator = nullptr;
	}
}

voidptr linear_allocator_i::allocate(const size i_bytes)
{
	return allocate(i_bytes, k_defaultAlignment);
}

voidptr linear_allocator_i::allocate(const size i_bytes, const size i_alignment)
{
	return floral::allocate(i_bytes, i_alignment, allocator);
}

voidptr linear_allocator_i::reallocate(voidptr i_data, const size i_newBytes)
{
	return reallocate(i_data, i_newBytes, 0);
}

voidptr linear_allocator_i::reallocate(voidptr i_data, const size i_newBytes, const size i_alignment)
{
	return floral::reallocate(i_data, i_newBytes, i_alignment, allocator);
}

void linear_allocator_i::free(voidptr i_data)
{
	floral::free(i_data, allocator);
}

void linear_allocator_i::reset()
{
	floral::reset_allocator(allocator);
}

freelist_allocator_i::freelist_allocator_i()
	: allocator(nullptr)
	, allocCount(0)
	, freeCount(0)
{
	// nothing :)
}

freelist_allocator_i::~freelist_allocator_i()
{
	destroy();
}

void freelist_allocator_i::initialize(const_cstr i_id, const size i_bytes)
{
	FLORAL_ASSERT(allocator == nullptr);
	allocator = create_freelist_allocator(i_id, i_bytes);
	allocCount = 0;
	freeCount = 0;
}

void freelist_allocator_i::destroy()
{
	if (allocator != nullptr)
	{
		destroy_allocator(allocator);
		allocator = nullptr;
	}
}

voidptr freelist_allocator_i::allocate(const size i_bytes)
{
	return allocate(i_bytes, k_defaultAlignment);
}

voidptr freelist_allocator_i::allocate(const size i_bytes, const size i_alignment)
{
	allocCount++;
	return floral::allocate(i_bytes, i_alignment, allocator);
}

voidptr freelist_allocator_i::reallocate(voidptr i_data, const size i_newBytes)
{
	return reallocate(i_data, i_newBytes, k_defaultAlignment);
}

voidptr freelist_allocator_i::reallocate(voidptr i_data, const size i_newBytes, const size i_alignment)
{
	allocCount++;
	freeCount++;
	return floral::reallocate(i_data, i_newBytes, i_alignment, allocator);
}

void freelist_allocator_i::free(voidptr i_data)
{
	if (i_data != nullptr)
	{
		freeCount++;
	}
	floral::free(i_data, allocator);
}

// ----------------------------------------------------------------------------
}
