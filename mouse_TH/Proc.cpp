#include "pch.h"

typedef struct EnumFunArg
{
    HWND      hWnd;
    DWORD    dwProcessId;
} EnumFunArg, *LPEnumFunArg;
BOOL CALLBACK lpEnumFunc(HWND hwnd, LPARAM lParam)
{

    EnumFunArg  *pArg = (LPEnumFunArg)lParam;
    DWORD  processId;
    GetWindowThreadProcessId(hwnd, &processId);
    if (processId == pArg->dwProcessId)
    {
        DWORD style = (DWORD)GetWindowLong(hwnd, GWL_STYLE);
        if (style & WS_VISIBLE) {
            pArg->hWnd = hwnd;
            return FALSE;
        }
    }
    return TRUE;
}
HWND ReturnWnd(DWORD processID)
{
    HWND retWnd = NULL;
    EnumFunArg wi;
    wi.dwProcessId = processID;
    wi.hWnd = NULL;
    EnumWindows(lpEnumFunc, (LPARAM)&wi);
    if (wi.hWnd) {
        retWnd = wi.hWnd;
    }
    return retWnd;
}

int FindProcessPid(const WCHAR* pszProcessName)
{
    DWORD id = 0;
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pInfo;
    pInfo.dwSize = sizeof(pInfo);

    Process32First(hSnapShot, &pInfo);
    do
    {
        if (wcscmp(_wcsdup(pInfo.szExeFile), pszProcessName) == 0)
        {
            id = pInfo.th32ProcessID;
            break;
        }
    }
    while (Process32Next(hSnapShot, &pInfo) != FALSE);
    return id;
}
void Clear()
{
    HANDLE                     hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD                      count;
    DWORD                      cellCount;
    COORD                      homeCoords = { 0, 0 };

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE)
        return;

    /* Get the number of cells in the current buffer */
    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi))
        return;
    cellCount = csbi.dwSize.X *csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(hStdOut, (TCHAR) ' ', cellCount, homeCoords, &count))
        return;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count))
        return;

    /* Move the cursor home */
    SetConsoleCursorPosition(hStdOut, homeCoords);
}
