//
// Created by Filip on 03.10.2024.
//

#include "actionsProtector.h"

#include <stdio.h>

LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < 0) {
        printf("nCode less than 0\n");
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    if (actionFunction == NULL) {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT *)(lParam);
    if (wParam == WM_KEYDOWN) {
        printf("Pressed: %lu, %lu\n", p->vkCode, p->flags);

        if (p->vkCode == VK_LWIN || p->vkCode == VK_RWIN) {
            actionFunction(Super);
            return 1;
        } else if (p->vkCode == VK_SNAPSHOT) {
            actionFunction(ScreenShot);
            return 1;
        }
    } else if (wParam == WM_SYSKEYDOWN) {
        printf("Pressed: %lu, %lu\n", p->vkCode, p->flags);

        if (p->vkCode == VK_TAB && p->flags == LLKHF_ALTDOWN) {
            actionFunction(WindowSwitch);
            return 1;
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

__declspec(dllexport) void initKeyboardGuard() {
    hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookProc, NULL, 0);
    HHOOK test = hhkLowLevelKybd;
}

void setActionFunction(void(*action)(enum DetectedAction action)) {
    actionFunction = action;
}

__declspec(dllexport) void deinitKeyboardGuard() {
    UnhookWindowsHookEx(hhkLowLevelKybd);
    hhkLowLevelKybd = NULL;
}


