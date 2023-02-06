#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <wingdi.h>

void ctrl_v() {
    INPUT ip;
    ip.type = INPUT_KEYBOARD;

    // Press the "Ctrl" key
    ip.ki.wVk = VK_CONTROL;
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Press the "V" key
    ip.ki.wVk = 'V';
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "V" key
    ip.ki.wVk = 'V';
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "Ctrl" key
    ip.ki.wVk = VK_CONTROL;
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));
}

void alt_s() {
    INPUT ip;
    ip.type = INPUT_KEYBOARD;

    // Press the "Alt" key
    ip.ki.wVk = VK_MENU;
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Press the "S" key
    ip.ki.wVk = 'S';
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "S" key
    ip.ki.wVk = 'S';
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "Alt" key
    ip.ki.wVk = VK_MENU;
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));
}

void click(int x, int y) {
    SetCursorPos(x, y);
    INPUT Inputs[2] = {0};
    Inputs[0].type = INPUT_MOUSE;
    Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    Inputs[1].type = INPUT_MOUSE;
    Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(2, Inputs, sizeof(INPUT));
}

bool find_window(LPCSTR name) {
    HWND w = FindWindowA(nullptr, name);
    if (w == nullptr) {
        return false;
    }
    SetWindowPos(w, HWND_TOPMOST, 0, 0, 300, 500, SWP_SHOWWINDOW);
    SetForegroundWindow(w);
    Sleep(250);
    return true;
}

void set_clipboard_text(const char *text, size_t length) {
    OpenClipboard(nullptr);
    if (!EmptyClipboard()) {
        std::cout << "Cannot empty clipboard!";
    }

    HGLOBAL global = GlobalAlloc(GMEM_FIXED, (length + 1) * sizeof(char));
    if (!global) {
        std::cout << "Cannot allocate memory for copying text";
    }

#ifdef _MSC_VER
    strcpy_s((char *)global, (length + 1) * sizeof(char), text);
#else
    strncpy((char *) global, text, length + 1);
#endif

    SetClipboardData(CF_TEXT, global);
    CloseClipboard();
    ctrl_v();
}

bool is_wechat() {
    click(25, 110);
    return GetPixel(GetDC(GetActiveWindow()), 25, 110) == 6340871;
}

int send_wechat(const char* wechat, const char* text) {
    if (find_window("WeChat")) {
        Sleep(200);
    } else {
        std::cout << "Can't find wechat";
        return 1;
    }
    if (!is_wechat()) {
        std::cout << "This is not wechat";
        return 2;
    }
    click(143, 39);
    set_clipboard_text(wechat, strlen(wechat));
    Sleep(600);
    click(155, 120);
    set_clipboard_text(text, strlen(text));
    alt_s();
    return 0;
}


int main() {
    const char* wechat = "傳輸";
    const char* text = "中文";
    std::cout << wechat << text;
    send_wechat(wechat, text);
    return 0;
}
