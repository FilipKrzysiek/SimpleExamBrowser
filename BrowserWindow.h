//
// Created by Filip on 22.10.2022.
//

#ifndef SIMPLEEXAMBROWSER_BROWSERWINDOW_H
#define SIMPLEEXAMBROWSER_BROWSERWINDOW_H

#include <QPushButton>
#include <QtWebEngineWidgets>
#include <QWidget>
#include <memory>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QSpacerItem>
#include <QStyle>
#include <QApplication>
#include <QMessageBox>

using namespace std;


class BrowserWindow : public QWidget {
    Q_OBJECT

public:
    BrowserWindow(QWidget *parent);

    virtual ~BrowserWindow();

    void setUrl(const QUrl &url);

public slots:
    void exitApp();

    void reloadPage();

    void detectedProhibitedAction();

    void detectedPrintScreen();

    void openHomePage();

signals:
    void signalDetectedProhibitedAction();
    void signalDetectedPrintScreen();

private:
    void setupUI();

    void setupUiTopBar();

    void asyncReadKeyboard();

    void emptyingClipboard();

protected:
//    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;


private:
    unique_ptr<QWebEngineView> pageView;
    unique_ptr<QVBoxLayout> mainWidgetLayout;
    unique_ptr<QHBoxLayout> topBar;
    unique_ptr<QSpacerItem> topBarHSpacer;

    unique_ptr<QToolButton> exitButton;
    unique_ptr<QToolButton> reloadButton;
    unique_ptr<QToolButton> homeButton;
    unique_ptr<QComboBox> comboBox;

    unique_ptr<QProgressBar> warningPBar;

    QUrl homePage;

    bool appWorking = true;
    bool openedWarning = false;
};


#endif //SIMPLEEXAMBROWSER_BROWSERWINDOW_H
