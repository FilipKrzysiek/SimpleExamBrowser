
#include <QApplication>
#include <QSettings>

#include "ui/browser.h"

extern "C" {
#include "src/guard/actionsProtector.h"
}

class CallbackForworder {
    static Browser *browser;

public:
    explicit CallbackForworder(Browser *browser) {
        CallbackForworder::browser = browser;
    }

    static void call(enum DetectedAction action) {
        browser->detectedForbiddenAction(action);
    }
};

Browser* CallbackForworder::browser = nullptr;


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
