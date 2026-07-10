/*
 * Shutdown.exe — one-click shutdown utility for Windows.
 *
 * GUI subsystem app (no console window). On launch it immediately runs
 * shutdown /s /t 0  with NO confirmation and no visible window.
 *
 * Built for Windows via mingw-w64:
 *   x86_64-w64-mingw32-gcc Shutdown.c resource.o -o Shutdown.exe -mwindows
 */

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    (void)hInstance; (void)hPrevInstance; (void)lpCmdLine; (void)nCmdShow;

    /* Run "shutdown /s /t 0" with no visible console window. */
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    /* Mutable buffer required by CreateProcessW's lpCommandLine. */
    wchar_t cmd[] = L"shutdown /s /t 0";

    if (CreateProcessW(NULL, cmd, NULL, NULL, FALSE,
                       CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    return 0;
}
