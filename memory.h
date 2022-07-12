#pragma once
#include "stdaliases.h"

#define SIZE_KB(X) (X * 1024u)
#define SIZE_MB(X) (SIZE_KB(X) * 1024u)
#define SIZE_GB(X) (SIZE_MB(X) * 1024u)
#define SIZE_TB(X) (SIZE_GB(X) * 1024u)

#define TO_KB(X) (X >> 10)
#define TO_MB(X) (TO_KB(X) >> 10)

namespace floral
{
// ----------------------------------------------------------------------------

struct platform_info_t
{
	size pageSize;
};

struct allocator_t
{
	platform_info_t platformInfo;

	c8 id[4];
	size realAllocatedBytes;
	voidptr baseAddress;
	size capacity;
};

struct alloc_header_t
{
	alloc_header_t* prevAlloc;
	alloc_header_t* nextAlloc;
	p8 marker;
	size frameSize;

	size alignment;
	size dataSize;
};

struct linear_allocator_t : public allocator_t
{
	p8 currentMarker;
	alloc_header_t* lastAlloc;

	size usedBytes;
};

struct freelist_allocator_t : public allocator_t
{
	alloc_header_t* firstFreeBlock;

	alloc_header_t* lastAlloc;
	size usedBytes;
};

struct pool_allocator_t : public allocator_t
{
	alloc_header_t* firstFreeBlock;

	alloc_header_t* lastAlloc;
	size usedBytes;

	size slotSize;
	size alignment;
	size slotCapacity;
	size usedSlot;
};

// ----------------------------------------------------------------------------

linear_allocator_t* create_linear_allocator(const_cstr i_id, const size i_bytes, voidptr i_userData = nullptr);
voidptr allocate(const size i_bytes, linear_allocator_t* i_allocator);
voidptr reallocate(voidptr i_data, const size i_newBytes, linear_allocator_t* i_allocator);
voidptr allocate(const size i_bytes, const size i_alignment, linear_allocator_t* i_allocator);
voidptr reallocate(voidptr i_data, const size i_newBytes, const size i_alignment, linear_allocator_t* i_allocator);
void free(voidptr i_data, linear_allocator_t* i_allocator);
void reset_allocator(linear_allocator_t* io_allocator);
void destroy_allocator(linear_allocator_t* io_allocator);

freelist_allocator_t* create_freelist_allocator(const_cstr i_id, const size i_bytes);
freelist_allocator_t* create_freelist_allocator(const_cstr i_id, voidptr i_buffer, const size i_bytes);
voidptr allocate(const size i_bytes, freelist_allocator_t* i_allocator);
voidptr reallocate(voidptr i_data, const size i_newBytes, freelist_allocator_t* i_allocator);
voidptr allocate(const size i_bytes, const size i_alignment, freelist_allocator_t* i_allocator);
voidptr reallocate(voidptr i_data, const size i_newBytes, const size i_alignment, freelist_allocator_t* i_allocator);
void free(voidptr i_data, freelist_allocator_t* i_allocator);
void reset_allocator(freelist_allocator_t* io_allocator);
void destroy_allocator(freelist_allocator_t* io_allocator);

pool_allocator_t* create_pool_allocator(const_cstr i_id, const size i_slotSize, const size i_slotCapacity);
pool_allocator_t* create_pool_allocator(const_cstr i_id, const size i_slotSize, const size i_slotCapacity, const size i_alignment);
voidptr allocate(pool_allocator_t* i_allocator);
void free(voidptr i_data, pool_allocator_t* i_allocator);
void destroy_allocator(pool_allocator_t* io_allocator);

const size get_allocated_size(voidptr i_data);

// ----------------------------------------------------------------------------
// helpers
struct linear_allocator_i
{
	linear_allocator_i();
	~linear_allocator_i();

	void initialize(const_cstr i_id, const size i_bytes);
	void destroy();

	voidptr allocate(const size i_bytes);
	voidptr allocate(const size i_bytes, const size i_alignment);
	voidptr reallocate(voidptr i_data, const size i_newBytes);
	voidptr reallocate(voidptr i_data, const size i_newBytes, const size i_alignment);

	void free(voidptr i_data);
	void reset();

	linear_allocator_t* allocator;
};

struct freelist_allocator_i
{
	freelist_allocator_i();
	~freelist_allocator_i();

	void initialize(const_cstr i_id, const size i_bytes);
	void destroy();

	voidptr allocate(const size i_bytes);
	voidptr allocate(const size i_bytes, const size i_alignment);
	voidptr reallocate(voidptr i_data, const size i_newBytes);
	voidptr reallocate(voidptr i_data, const size i_newBytes, const size i_alignment);

	void free(voidptr i_data);

	freelist_allocator_t* allocator;
	u64 allocCount;
	u64 freeCount;
};

// ----------------------------------------------------------------------------
}
