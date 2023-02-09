#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <wingdi.h>
#include <fstream>
#include <string>
#include <codecvt>
#include <locale>

void ctrl_v()
{
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

void alt_s()
{
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

void click(int x, int y)
{
    SetCursorPos(x, y);
    INPUT Inputs[2] = {0};
    Inputs[0].type = INPUT_MOUSE;
    Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    Inputs[1].type = INPUT_MOUSE;
    Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(2, Inputs, sizeof(INPUT));
}

bool find_window(LPCSTR name)
{
    HWND w = FindWindowA(nullptr, name);
    if (w == nullptr)
    {
        return false;
    }
    SetWindowPos(w, HWND_TOPMOST, 0, 0, 300, 500, SWP_SHOWWINDOW);
    SetForegroundWindow(w);
    return true;
}

void paste_contents_from_file(const char *fileName)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    std::string text;

    // Open the file and read its contents
    std::ifstream file(fileName);
    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            text += line + "\n";
        }
        file.close();
    }

    // Convert the contents to UTF-16
    std::wstring wtext = convert.from_bytes(text);

    // Copy the contents to the clipboard
    HGLOBAL hText = GlobalAlloc(GMEM_MOVEABLE, (wtext.size() + 1) * sizeof(wchar_t));
    memcpy(GlobalLock(hText), wtext.c_str(), (wtext.size() + 1) * sizeof(wchar_t));
    GlobalUnlock(hText);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_UNICODETEXT, hText);
    CloseClipboard();
    ctrl_v();
}

bool is_wechat()
{
    click(25, 110);
    return GetPixel(GetDC(GetActiveWindow()), 25, 110) == 6340871;
}

int send_wechat(const char *wechat, const char *text)
{
    if (find_window("WeChat"))
    {
        Sleep(300);
    }
    else
    {
        std::cout << "Can't find wechat";
        return 1;
    }
    if (!is_wechat())
    {
        std::cout << "This is not wechat";
        return 2;
    }
    click(143, 39);
    paste_contents_from_file(wechat);
    Sleep(1000);
    click(155, 120);
    paste_contents_from_file(text);
    alt_s();
    return 0;
}

int main()
{
    const char *wechat = "wechat.txt";
    const char *text = "text.txt";
    send_wechat(wechat, text);
    return 0;
}
