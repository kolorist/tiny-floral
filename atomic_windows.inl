#include <Windows.h>

namespace floral
{

const u32 interlocked_exchange(u32* io_target, const u32 i_value)
{
	return InterlockedExchange(io_target, i_value);
}

const u32 interlocked_compare_exchange(u32* io_target, const u32 i_exchange, const u32 i_comperand)
{
	return InterlockedCompareExchange(io_target, i_exchange, i_comperand);
}

}
