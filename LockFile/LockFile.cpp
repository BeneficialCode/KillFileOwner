// LockFile.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <Windows.h>
#include <stdio.h>

int wmain(int argc,wchar_t* argv[])
{
	HANDLE hFile = ::CreateFile(argv[1], GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
	if (hFile == INVALID_HANDLE_VALUE) {
		return 1;
	}

	do
	{
		printf("Hey! I held the %ws file\n", argv[1]);
		Sleep(5000);
	} while (true);

	::CloseHandle(hFile);

	return 0;
}


