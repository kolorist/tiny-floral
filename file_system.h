#pragma once

#include "configs.h"
#include "stdaliases.h"

namespace floral
{
// ----------------------------------------------------------------------------

void set_working_directory(const_tcstr i_workingDir);
const size read_all_file(const_tcstr i_fileName, p8 o_buffer);

// ----------------------------------------------------------------------------
}
