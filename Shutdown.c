/*
 * Shutdown.exe — one-click shutdown utility for Windows.
 *
 * GUI subsystem app (no console window). On launch it asks the user to
 * confirm, and only on "Yes" does it run:  shutdown /s /t 0
 *
 * Built for Windows via mingw-w64:
 *   x86_64-w64-mingw32-gcc Shutdown.c resource.o -o Shutdown.exe -mwindows
 */

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    (void)hInstance; (void)hPrevInstance; (void)lpCmdLine; (void)nCmdShow;

    /* Confirmation dialog. Default button is "No" (MB_DEFBUTTON2) so a
       stray Enter keypress does not shut the machine down accidentally. */
    int response = MessageBoxW(
        NULL,
        L"Ești sigur că vrei să oprești calculatorul acum?\n"
        L"Orice lucru nesalvat se va pierde.",
        L"Confirmare oprire",
        MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 | MB_SETFOREGROUND | MB_TOPMOST);

    if (response != IDYES) {
        return 0;  /* user cancelled — do nothing */
    }

    /* Run "shutdown /s /t 0" with no visible console window.
       CREATE_NO_WINDOW keeps the console app fully hidden. */
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    /* Mutable buffer required by CreateProcessW's lpCommandLine. */
    wchar_t cmd[] = L"shutdown /s /t 0";

    BOOL ok = CreateProcessW(
        NULL,             /* look the executable up on PATH */
        cmd,              /* command line */
        NULL, NULL,       /* default security */
        FALSE,            /* do not inherit handles */
        CREATE_NO_WINDOW, /* no console window */
        NULL, NULL,       /* inherit environment & cwd */
        &si, &pi);

    if (!ok) {
        MessageBoxW(NULL,
            L"Nu am putut porni comanda de oprire (shutdown).",
            L"Eroare", MB_OK | MB_ICONERROR | MB_TOPMOST);
        return 1;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}
