#include "file_system.h"

#include <Windows.h>
#include <stdio.h>

namespace floral
{
// ----------------------------------------------------------------------------

const size read_all_file_internal(FILE* f, p8* o_buffer)
{
	size bufferLen = 0;
    if (f != nullptr)
    {
        fseek(f, 0, SEEK_END);
        bufferLen = ftell(f);

        *o_buffer = new u8[bufferLen];
        fseek(f, 0, SEEK_SET);
        fread(*o_buffer, bufferLen, 1, f);
        fclose(f);
    }
    return bufferLen;
}

const size read_all_file(const_wcstr i_fileName, p8* o_buffer)
{
	FILE* f = nullptr;
    _wfopen_s(&f, i_fileName, TEXT("rb"));
	return read_all_file_internal(f, o_buffer);
}

const size read_all_file(const_cstr i_fileName, p8* o_buffer)
{
	FILE* f = nullptr;
    fopen_s(&f, i_fileName, "rb");
	return read_all_file_internal(f, o_buffer);
}

// ----------------------------------------------------------------------------
}