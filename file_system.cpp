#include "file_system.h"

#if defined(FLORAL_PLATFORM_WINDOWS)
#include <Windows.h>
#elif defined(FLORAL_PLATFORM_LINUX)
#endif
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

		// TODO: remove new
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
#if defined(FLORAL_PLATFORM_WINDOWS)
	_wfopen_s(&f, i_fileName, TEXT("rb"));
#endif
	return read_all_file_internal(f, o_buffer);
}

const size read_all_file(const_cstr i_fileName, p8* o_buffer)
{
	FILE* f = nullptr;
	f = fopen(i_fileName, "rb");
	return read_all_file_internal(f, o_buffer);
}

// ----------------------------------------------------------------------------
}
