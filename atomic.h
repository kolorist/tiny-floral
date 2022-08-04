#pragma once

#include "stdaliases.h"

namespace floral
{

const u32 interlocked_exchange(u32* io_target, const u32 i_value);

// The function compares the io_target value with the i_comperand value.
// If the io_target value is equal to the i_comperand value,
// the i_exchange value is stored in the address specified by io_target.
// Otherwise, no operation is performed.
// The function returns the initial value of the io_target parameter.
const u32 interlocked_compare_exchange(u32* io_target, const u32 i_exchange, const u32 i_comperand);

}
