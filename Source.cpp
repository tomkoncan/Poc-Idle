#include "windows.h"
#include "stdio.h"

int wmain(int argc, wchar_t** argv)
{
	if (argc != 3)
	{
		wprintf(L"Syntax error\nUsage: %s  <file_path_specification>  <seconds_of_inactivity>\n\n", argv[0]);
		return 1;
	}

	DWORD dwInactivity = _wtoi(argv[2])*1000;
	LASTINPUTINFO lii;
	lii.cbSize = sizeof(LASTINPUTINFO);

	HANDLE hFile = CreateFileW(
		argv[1],
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_DELETE_ON_CLOSE,
		NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		wprintf(L"CreateFile() failed with status %d - exiting\n", GetLastError());
		return 2;
	}

	for (;;)
	{
		if (!GetLastInputInfo(&lii))
		{
			wprintf(L"Error: GetLastInputInfo() failed with %d\n\r", GetLastError());
			return 1;
		}
		else
		{
			if ((GetTickCount() - lii.dwTime) > dwInactivity)
			{
				wprintf(L"Idleness detected\n");
				CloseHandle(hFile);
				return 0;
			}
			Sleep(1000);
		}
	}

	return 0;
}