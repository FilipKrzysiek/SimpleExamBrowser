//
// Created by Filip on 16.09.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Browser.h" resolved

#include "browser.h"

#include <qscreen.h>

#include "ui_browser.h"


Browser::Browser(QWidget *parent) :
    QWidget(parent), ui(new Ui::Browser) {
    ui->setupUi(this);

    setupConnections();
    setupUi();
}

Browser::~Browser() {
    delete ui;
}

void Browser::setupWindowsGeometry() {
    //TODO macro to enable and disbale id in debug mode
    setAttribute(Qt::WA_NativeWindow);
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    auto primaryScreen = QApplication::primaryScreen();
    auto pScreenGeometry = primaryScreen->geometry();
    setGeometry(0, 0, pScreenGeometry.width()+1, pScreenGeometry.height());
    setContentsMargins(0,0,1,0);

    auto allScreens = QApplication::screens();

    //TODO macro to enable adn disable it
    // for (auto screen: allScreens) {
    //     if (screen == primaryScreen) {
    //         continue;
    //     }
    //
    //     auto *widgetScreen = new QWidget();
    //     widgetScreen->setGeometry(screen->geometry());
    //     widgetScreen->show();
    //     otherScreensWidget.push_back(widgetScreen);
    // }
}

void Browser::loadUrl(const QString &url) {
    ui->webView->load(QUrl(url));
}

void Browser::setBaseUrl(const QUrl &base_url) {
    baseUrl = base_url;
    openBaseUrlPage();
}

void Browser::setBaseUrl(const QString &base_url) {
    setBaseUrl(QUrl(base_url));
}

void Browser::openBaseUrlPage() {
    ui->webView->load(baseUrl);
}

void Browser::detectedForbiddenAction(enum DetectedAction action) {
    ++warningLevel;

    ui->warningLevel->setValue(warningLevel);
}

void Browser::closeEvent(QCloseEvent *event) {
    QWidget::closeEvent(event);

    for (auto *screen: otherScreensWidget) {
        screen->close();
    }
}

void Browser::setupUi() {
    updateTime.setInterval(1000);
    updateTime.start();

    ui->webView->show();

    updateDataTime();
    setupIcons();
}

void Browser::setupIcons() {
    ui->close->setIcon(QIcon(":/icons/close.png"));
    ui->reload->setIcon(QIcon(":/icons/refresh.png"));
    ui->homePage->setIcon(QIcon(":/icons/home.png"));
}

void Browser::setupConnections() {
    connect(&updateTime, &QTimer::timeout, this, &Browser::updateDataTime);

    connect(ui->reload, &QPushButton::clicked, ui->webView, &QWebEngineView::reload);
    connect(ui->homePage, &QPushButton::clicked, this, &Browser::openBaseUrlPage);
}

void Browser::updateDataTime() {
    const QDateTime now = QDateTime::currentDateTime();
    ui->dateTime->setText(now.toString("hh:mm:ss\ndd.MM.yyyy"));
}
