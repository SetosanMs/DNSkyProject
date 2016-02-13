/* ReadFile */
extern BOOL(WINAPI *dReadFile)(HANDLE hFile, LPVOID lpBuffer, DWORD  nNumberOfBytesToRead, LPDWORD lpNumberOfByte, LPOVERLAPPED lpOverlapped);
extern BOOL WINAPI myReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD  nNumberOfBytesToRead, LPDWORD lpNumberOfByte, LPOVERLAPPED lpOverlapped);
