
#include <QApplication>
#include <QSettings>
#include <chrono>

constexpr int acceptableCallTimeGap = 500;

#include "ui/browser.h"

extern "C" {
#include "src/guard/actionsProtector.h"
}

class CallbackForwarder {
    static Browser *browser;
    static std::chrono::time_point<std::chrono::system_clock> lastCall;
    static int lastKeyCode;

public:
    explicit CallbackForwarder(Browser *browser) {
        CallbackForwarder::browser = browser;
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

Browser* CallbackForwarder::browser = nullptr;
std::chrono::time_point<std::chrono::system_clock> CallbackForwarder::lastCall = std::chrono::system_clock::now();
int CallbackForwarder::lastKeyCode = 0;


int main(int argc, char *argv[]) {
    QSettings settings("./config.ini", QSettings::IniFormat);

    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL, settings.value("allowSoftwareOpenGl", false).toBool());

    if (settings.value("forceUseOpenGl", false).toBool())
        QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    initKeyboardGuard();

    QApplication a(argc, argv);



    Browser browser;

    QObject::connect(&browser, &Browser::disableGuarder, [] {
        deinitKeyboardGuard();
    });

    CallbackForwarder cf(&browser);
    setActionFunction(CallbackForwarder::call);

    browser.setupWindowsGeometry();
    browser.show();

    browser.setBaseUrl(settings.value("baseUrl", "https://www.pk.edu.pl/").toString());
    browser.openBaseUrlPage();
    auto res = QApplication::exec();

    deinitKeyboardGuard();

    return res;
}
