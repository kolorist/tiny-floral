#pragma once

#include "stdaliases.h"

namespace floral
{
// ----------------------------------------------------------------------------

const size read_all_file(const_wcstr i_fileName, p8* o_buffer);
const size read_all_file(const_cstr i_fileName, p8* o_buffer);

// ----------------------------------------------------------------------------
}