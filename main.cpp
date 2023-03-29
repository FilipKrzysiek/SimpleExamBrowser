#include <QApplication>
#include "BrowserWindow.h"
#include <Windows.h>
#include <iostream>

QUrl commandLineUrlArgument() {
    const QStringList args = QCoreApplication::arguments();
    for (const QString &arg: args.mid(1)) {
        if (!arg.startsWith(QLatin1Char('-')))
            return QUrl::fromUserInput(arg);
    }
    return QUrl(QStringLiteral("https://www.qt.io"));
}

void threadFun() {
    while(1) {
        cout << GetAsyncKeyState(VK_SNAPSHOT) << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);



    BrowserWindow browserWindow(nullptr);
    browserWindow.setUrl(commandLineUrlArgument());
    browserWindow.show();
    browserWindow.setAttribute(Qt::WA_NativeWindow);

    return QApplication::exec();
}
