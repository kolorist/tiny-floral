#include "file_system.h"

#if defined(FLORAL_PLATFORM_WINDOWS)
#include <Windows.h>
#elif defined(FLORAL_PLATFORM_LINUX)
#endif
#include <stdio.h>
#include <string.h>

namespace floral
{
// ----------------------------------------------------------------------------
static tchar s_workingDir[256];
void set_working_directory(const_tcstr i_workingDir)
{
#if defined(UNICODE)
	wcscpy(s_workingDir, i_workingDir);
#else
	strcpy(s_workingDir, i_workingDir);
#endif
}

const size read_all_file_internal(FILE* f, p8 o_buffer)
{
	size bufferLen = 0;
	if (f != nullptr)
	{
		fseek(f, 0, SEEK_END);
		bufferLen = ftell(f);

		if (o_buffer == nullptr)
		{
			return bufferLen;
		}

		fseek(f, 0, SEEK_SET);
		fread(o_buffer, bufferLen, 1, f);
		fclose(f);
	}
	return bufferLen;
}

const size read_all_file(const_tcstr i_fileName, p8 o_buffer)
{
	FILE* f = nullptr;
#if defined(FLORAL_PLATFORM_WINDOWS)
#	if defined(UNICODE)
	_wfopen_s(&f, i_fileName, TEXT("rb"));
#	else
	fopen_s(&f, i_fileName, "rb");
#	endif
#else
	tchar filePath[256];
	sprintf(filePath, "%s/%s", s_workingDir, i_fileName);
	f = fopen(filePath, "rb");
#endif
	return read_all_file_internal(f, o_buffer);
}
// ----------------------------------------------------------------------------
}
