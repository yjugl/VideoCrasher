#include <windows.h>
#include <stdio.h>

static int gCount = 0;

BOOL WINAPI MatchWindow(HWND window, LPARAM indent)
{
    DWORD processId = 0;
    auto threadId = GetWindowThreadProcessId(window, &processId);

    wchar_t* data = 0;
    auto thread = OpenThread(THREAD_QUERY_LIMITED_INFORMATION, false, threadId);

    if (thread) {
        auto result = GetThreadDescription(thread, &data);
        CloseHandle(thread);

        if (SUCCEEDED(result))
        {
            if (!lstrcmpW(data, L"VideoCapture")) {
                ++gCount;
                printf("Found VideoCapture thread window [pid = %ld, tid = %ld], sending WM_QUIT...\n", processId, threadId);
                auto result = PostMessageW(window, WM_QUIT, 0, 0);
                printf("Result: %ld\n", result);
            }
            LocalFree(data);
        }
    }

    return TRUE;
}

int main()
{
    EnumWindows(MatchWindow, 0);
    if (gCount == 0) {
        printf("Did not find any VideoCapture thread window.\n");
    }
}
