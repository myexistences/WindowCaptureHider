#include <windows.h>
#include <thread>
#include <atomic>
#include <unordered_set>

std::atomic<bool> running(true);
std::unordered_set<HWND> hiddenWindows;


void ResetWindowCapture(HWND hwnd) {
    if (IsWindow(hwnd)) {
        SetWindowDisplayAffinity(hwnd, WDA_NONE);
    }
}

void HideWindowFromCapture(HWND hwnd) {
    if (IsWindow(hwnd)) {
        ResetWindowCapture(hwnd);
        SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE);
        hiddenWindows.insert(hwnd);
    }
}

void HideAllWindowsFromCapture() {
    EnumWindows([](HWND hwnd, LPARAM) -> BOOL {
        HideWindowFromCapture(hwnd);
        return TRUE;
        }, 0);
}

void WindowMonitorLoop() {
    while (running.load()) {
        HideAllWindowsFromCapture();
        for (auto it = hiddenWindows.begin(); it != hiddenWindows.end(); ) {
            if (!IsWindow(*it)) { 
                ResetWindowCapture(*it);
                it = hiddenWindows.erase(it);
            }
            else {
                ++it;
            }
        }

        Sleep(1000); 
    }
}

std::thread monitorThread;

extern "C" __declspec(dllexport) void StartHidingWindows() {
    if (!monitorThread.joinable()) {
        running = true;
        monitorThread = std::thread(WindowMonitorLoop);
    }
}

extern "C" __declspec(dllexport) void StopHidingWindows() {
    running = false;
    if (monitorThread.joinable()) {
        monitorThread.join();
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        StartHidingWindows();
        break;
    case DLL_PROCESS_DETACH:
        StopHidingWindows();
        break;
    }
    return TRUE;
}
