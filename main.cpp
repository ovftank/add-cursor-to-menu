#include <windows.h>
#include <lmcons.h>
#include <fstream>
#include <string>

void showMessage(const std::wstring& message, const std::wstring& title = L"Thông báo") {
    MessageBoxW(NULL, message.c_str(), title.c_str(), MB_OK | MB_ICONINFORMATION);
}

void createRegFile(const std::wstring& username) {
    std::wofstream regFile(L"add_cursor.reg");

    if (!regFile.is_open()) {
        showMessage(L"Vui lòng chạy lại chương trình với quyền admin", L"Lỗi");
        return;
    }
    regFile << L"Windows Registry Editor Version 5.00\n\n";
    regFile << L"[HKEY_CLASSES_ROOT\\directory\\background\\shell\\Cursor]\n";
    regFile << L"\"Icon\"=\"C:\\\\Users\\\\" << username << L"\\\\AppData\\\\Local\\\\Programs\\\\cursor\\\\Cursor.exe\"\n";
    regFile << L"@=\"Open w&ith Cursor\"\n\n";
    regFile << L"[HKEY_CLASSES_ROOT\\directory\\background\\shell\\Cursor\\command]\n";
    regFile << L"@=\"\\\"C:\\\\Users\\\\" << username << L"\\\\AppData\\\\Local\\\\Programs\\\\cursor\\\\Cursor.exe\\\" \\\"%V\\\"\"\n";
    regFile.close();

    if (system("regedit /s add_cursor.reg") != 0) {
        showMessage(L"Thiếu quyền truy cập, vui lòng chạy lại chương trình với quyền admin", L"Lỗi");
        return;
    }
    if (std::remove("add_cursor.reg") != 0) {
        showMessage(L"Không thể xoá tệp .reg", L"Lỗi");
    } else {
        showMessage(L"Đã thêm Cursor vào menu!");
    }
}

int main() {
    WCHAR username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    if (!GetUserNameW(username, &username_len)) {
        showMessage(L"Không thể lấy tên người dùng. Lỗi: " + std::to_wstring(GetLastError()), L"Lỗi");
        return 1;
    }
    createRegFile(username);
    return 0;
}
