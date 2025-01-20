
#include <QApplication>
#include <QSettings>
#include <chrono>

constexpr int acceptableCallTimeGap = 500;

#include "ui/browser.h"

extern "C" {
#include "src/guard/actionsProtector.h"
}

class CallbackForworder {
    static Browser *browser;
    static std::chrono::time_point<std::chrono::system_clock> lastCall;
    static int lastKeyCode;

public:
    explicit CallbackForworder(Browser *browser) {
        CallbackForworder::browser = browser;
    }

    static void call(enum DetectedAction action, int keyCode) {
        auto now = std::chrono::system_clock::now();
        if (!(lastKeyCode == keyCode &&
            std::chrono::duration_cast<std::chrono::milliseconds>(now - lastCall).count() < acceptableCallTimeGap)) {
            printf("%lld\n", std::chrono::duration_cast<std::chrono::milliseconds>(now - lastCall).count());
            browser->detectedForbiddenAction(action);
        }
        lastKeyCode = keyCode;
        lastCall = now;
    }
};

//Maybe create tab where keep which keys are pressed and on keyup release it

Browser* CallbackForworder::browser = nullptr;
std::chrono::time_point<std::chrono::system_clock> CallbackForworder::lastCall = std::chrono::system_clock::now();
int CallbackForworder::lastKeyCode = 0;


int main(int argc, char *argv[]) {
    initKeyboardGuard();

    QApplication a(argc, argv);
    QSettings settings("./config.ini", QSettings::IniFormat);

    Browser browser;

    CallbackForworder cf(&browser);
    setActionFunction(CallbackForworder::call);

    browser.setupWindowsGeometry();
    browser.show();

    browser.setBaseUrl(settings.value("baseUrl", "https://www.pk.edu.pl/").toString());
    browser.openBaseUrlPage();
    auto res = QApplication::exec();

    deinitKeyboardGuard();

    return res;
}
