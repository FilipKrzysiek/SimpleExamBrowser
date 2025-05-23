//
// Created by Filip on 03.10.2024.
//

#ifndef GUARD_H
#define GUARD_H

#include <windows.h>

enum DetectedAction {
    Acceptable,
    Super,
    WindowSwitch,
    ScreenShot
};

HHOOK hhkLowLevelKybd = NULL;
void (*actionFunction)(enum DetectedAction action, int keyCode) = NULL;

__declspec(dllexport) void initKeyboardGuard();

__declspec(dllexport) void setActionFunction(void (*action)(enum DetectedAction action, int keyCode));

__declspec(dllexport) void deinitKeyboardGuard();

LRESULT CALLBACK keyboardLowLevelHookProc(int nCode, WPARAM wParam, LPARAM lParam);

#endif //GUARD_H
