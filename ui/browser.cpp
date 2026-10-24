//
// Created by Filip on 16.09.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Browser.h" resolved

#include "browser.h"

#include <qscreen.h>
#include <QWebEngineProfile>

#include "ui_browser.h"
#include <QClipboard>
#include <QKeyEvent>


Browser::Browser(QWidget *parent) : QWidget(parent), ui(new Ui::Browser) {
    ui->setupUi(this);

    setupConnections();
    setupUi();
}

Browser::~Browser() {
    delete ui;
}

void Browser::setupWindowsGeometry() {
    //TODO macro to enable and disbale id in debug mode
    setAttribute(Qt::WA_NativeWindow, false);   // ważne
    setAttribute(Qt::WA_TranslucentBackground, false);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setWindowState(Qt::WindowFullScreen);

    auto primaryScreen = QApplication::primaryScreen();
    auto pScreenGeometry = primaryScreen->geometry();
    setGeometry(0, 0, pScreenGeometry.width() + 1, pScreenGeometry.height());
    setContentsMargins(0, 0, 1, 0);

    auto allScreens = QApplication::screens();

    //TODO macro to enable adn disable it
    for (auto screen: allScreens) {
        if (screen == primaryScreen) {
            continue;
        }

        createBlackScreen(screen->geometry());
    }
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

    warningThread = std::thread(&Browser::showWarningWIndow, this);
    warningThread.detach();
}

void Browser::closeEvent(QCloseEvent *event) {
    QWidget::closeEvent(event);

    for (auto *screen: otherScreensWidget) {
        screen->close();
    }
}

void Browser::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_F2 &&event->modifiers() == Qt::ControlModifier) {
        if (!isAlternateDispMode) {
            auto pScreenGeom = QApplication::primaryScreen()->geometry();
            createBlackScreen(pScreenGeom);
            connect(qApp, &QApplication::focusChanged, [this]() {
                raise();
            });

            setContentsMargins(0, 0, 0, 0);
            setWindowState((windowState() & ~Qt::WindowFullScreen) | Qt::WindowActive);
            setWindowState((windowState() ^ Qt::WindowMaximized) | Qt::WindowActive);
            setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
            show();

            isAlternateDispMode = true;
        }
    }

    if (event->key() == Qt::Key_F3 &&event->modifiers() == Qt::ControlModifier) {
        if (isAlternateDispMode) {
            setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
            setWindowFlags(windowFlags()
                     & ~Qt::WindowMinimizeButtonHint
                     & ~Qt::WindowMaximizeButtonHint);
            show();
        }
    }

    if (event->key() == Qt::Key_F4 &&event->modifiers() == Qt::ControlModifier) {
        if (isAlternateDispMode) {
            setWindowFlag(Qt::WindowStaysOnTopHint, false);
            if (isAlternateDispMode) {
                otherScreensWidget.last()->setWindowFlag(Qt::WindowStaysOnTopHint, false);
                otherScreensWidget.last()->show();
            }
            show();
        }
    }

    QWidget::keyPressEvent(event);
}

void Browser::setupUi() {
    updateTime.setInterval(1000);
    updateTime.start();

    ui->webView->show();

    updateDataTime();
    setupIcons();

    setupWarningWindow();
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

    connect(ui->close, &QPushButton::clicked, this, &Browser::closeApplication);
}

void Browser::setupWarningWindow() {
    warningMessageBox.setWindowFlag(Qt::WindowStaysOnTopHint);
    warningMessageBox.setText("Wykryto zabronioną akcję");
    warningMessageBox.setWindowTitle("Wykryto zabronioną akcję");
    warningMessageBox.setIcon(QMessageBox::Icon::Critical);
}

void Browser::showWarningWIndow() {
    QMetaObject::invokeMethod(&warningMessageBox, &QMessageBox::show);
}

void Browser::updateDataTime() {
    const QDateTime now = QDateTime::currentDateTime();
    ui->dateTime->setText(now.toString("hh:mm:ss\ndd.MM.yyyy"));
}

void Browser::closeApplication() {
    for (auto otherScreen: otherScreensWidget) {
        otherScreen->hide();
    }
    hide();

    emit disableGuarder();

    closeMessageBox = new QMessageBox(QMessageBox::Information, tr("Zamykanie"),
                                      tr("Trwa zamykanie aplikacji. Możesz robić inne rzeczy."), QMessageBox::NoButton,
                                      this);
    closeMessageBox->setStandardButtons(QMessageBox::NoButton);
    closeMessageBox->show();

    closeActionCounter = 0;

    connect(&closeActionTimer, &QTimer::timeout, [this] {
        if (closeActionCounter > 10) {
            closeActionTimer.stop();

            QApplication::clipboard()->clear();
            close();
            QApplication::quit();
        } else {
            QApplication::clipboard()->setText("Cleaning: " + QString::number(closeActionCounter));
            ++closeActionCounter;
        }
    });

    closeActionTimer.setSingleShot(false);
    closeActionTimer.start(300);
}

void Browser::createBlackScreen(const QRect &geom) {
    auto *widgetScreen = new QWidget();
    widgetScreen->setGeometry(geom);
    widgetScreen->setStyleSheet("background-color:#000");
    widgetScreen->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    widgetScreen->show();
    otherScreensWidget.push_back(widgetScreen);

    //TODO close main app on close
}
