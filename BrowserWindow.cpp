//
// Created by Filip on 22.10.2022.
//

#include "BrowserWindow.h"

BrowserWindow::BrowserWindow(QWidget *parent) : QWidget(parent) {
    setupUI();

//    RegisterHotKey(HWND(winId()), IDHOT_SNAPDESKTOP, 0, VK_SNAPSHOT);
//    RegisterHotKey(HWND(winId()), IDHOT_SNAPWINDOW, MOD_ALT, VK_SNAPSHOT);
//    RegisterHotKey(HWND(winId()), IDHOT_SNAPDESKTOP, MOD_WIN, VK_SNAPSHOT);

    std::thread th(&BrowserWindow::asyncReadKeyboard, this);
    th.detach();
}

void BrowserWindow::setupUI() {
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
//    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
//    showFullScreen();

    mainWidgetLayout = make_unique<QVBoxLayout>(this);
    mainWidgetLayout->setContentsMargins(0, -1, 0, 0);
//    mainWidgetLayout->setStretch(1, 10);
    topBar = make_unique<QHBoxLayout>(this);
    topBar->setContentsMargins(15, 15, 15, 15);
    setupUiTopBar();

    pageView = make_unique<QWebEngineView>(this);
    pageView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
//    pageView->setAttribute(Qt::WA_AlwaysStackOnTop, true);
//    pageView->setWindowFlag(Qt::WindowStaysOnTopHint);


    mainWidgetLayout->addLayout(topBar.get());
    mainWidgetLayout->addWidget(pageView.get());

    auto screen = QApplication::primaryScreen()->geometry();
    setGeometry(0, 0, screen.width()+1, screen.height());
    setContentsMargins(0,0,1,0);
}

void BrowserWindow::setupUiTopBar() {
    reloadButton = make_unique<QToolButton>(this);
    reloadButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    reloadButton->setText("Odśwież");
    reloadButton->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    connect(reloadButton.get(), SIGNAL(clicked()), this, SLOT(reloadPage()));

    exitButton = make_unique<QToolButton>(this);
    exitButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    exitButton->setText("Zamknij");
    exitButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    connect(exitButton.get(), SIGNAL(clicked()), this, SLOT(exitApp()));

    homeButton = make_unique<QToolButton>(this);
    homeButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    homeButton->setText("Strona startowa");
    homeButton->setIcon(style()->standardIcon(QStyle::SP_CommandLink));
    connect(homeButton.get(), SIGNAL(clicked()), this, SLOT(openHomePage()));

    warningPBar = make_unique<QProgressBar>(this);
    warningPBar->setTextVisible(false);
    warningPBar->setValue(0);
    warningPBar->setMaximum(4);
    warningPBar->setStyleSheet(QString::fromUtf8("QProgressBar::chunk {background-color: #F00;}"));
    warningPBar->setVisible(false);
    warningPBar->setMinimumHeight(50);
    warningPBar->setMinimumWidth(300);

//    comboBox = make_unique<QComboBox>(this);
//    comboBox->addItem("Item1");
//    comboBox->addItem("Item2");
//    comboBox->addItem("Item3");


    topBarHSpacer = make_unique<QSpacerItem>(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    topBar->addWidget(reloadButton.get());
    topBar->addWidget(homeButton.get());
    topBar->addSpacerItem(topBarHSpacer.get());
    topBar->addWidget(warningPBar.get());
//    topBar->addWidget(comboBox.get());
    topBar->addWidget(exitButton.get());
}

void BrowserWindow::exitApp() {
    appWorking = false;
//    UnregisterHotKey(HWND(winId()), IDHOT_SNAPDESKTOP);
//    UnregisterHotKey(HWND(winId()), IDHOT_SNAPWINDOW);
//    UnregisterHotKey(HWND(winId()), 1);

    pageView->close();
    QApplication::quit();
}

void BrowserWindow::setUrl(const QUrl &url) {
    pageView->setUrl(url);
    homePage = url;
}

void BrowserWindow::reloadPage() {
//    pageView->reload();
    pageView->load(pageView->url());
}

BrowserWindow::~BrowserWindow() {
    appWorking = false;
//    UnregisterHotKey(HWND(winId()), IDHOT_SNAPDESKTOP);
//    UnregisterHotKey(HWND(winId()), IDHOT_SNAPWINDOW);
}

//bool BrowserWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result) {
//    Q_UNUSED(eventType);
//    Q_UNUSED(result);
//    MSG *msg = static_cast<MSG*>(message);
//    if(msg->message == WM_HOTKEY) // check if key is pressed
//    {
//        printf("Catch native event!\n");
//    }
//    return QWidget::nativeEvent(eventType, message, result);
//}

void BrowserWindow::asyncReadKeyboard() {
    connect(this, &BrowserWindow::signalDetectedProhibitedAction, this, &BrowserWindow::detectedProhibitedAction);
    connect(this, &BrowserWindow::signalDetectedPrintScreen, this, &BrowserWindow::detectedPrintScreen);
    while(appWorking) {
        if(GetAsyncKeyState(VK_SNAPSHOT) != 0) {
            emit signalDetectedPrintScreen();
        } else if ((GetAsyncKeyState(VK_LMENU) != 0 || GetAsyncKeyState(VK_RMENU) != 0 ) && GetAsyncKeyState(VK_TAB) != 0) {
            emit signalDetectedProhibitedAction();
        } else if (GetAsyncKeyState(VK_LWIN) != 0 || GetAsyncKeyState(VK_RWIN) != 0) {
            emit signalDetectedProhibitedAction();
        }

        QClipboard *clipboard = QApplication::clipboard();
        if(clipboard->mimeData()->imageData().isValid()) {
            emptyingClipboard();
        }

        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

void BrowserWindow::detectedProhibitedAction() {
    if(!openedWarning) {
        openedWarning = true;
        this_thread::sleep_for(chrono::milliseconds(200));
        emptyingClipboard();

        auto mBox = QMessageBox("Mam Cie!",
                                "Co Ty chciałeś zrobić!?",
                                QMessageBox::Critical, QMessageBox::Ok, QMessageBox::NoButton,
                                QMessageBox::NoButton);
        mBox.setWindowFlag(Qt::WindowStaysOnTopHint);
        mBox.exec();

        if(warningPBar->value() == 0) {
            warningPBar->setVisible(true);
        }

        warningPBar->setValue(warningPBar->value() + 1);

        openedWarning = false;
    }
}

void BrowserWindow::emptyingClipboard() {
    OpenClipboard(NULL);
    EmptyClipboard();
    CloseClipboard();

    QPixmap image(500,500);
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->clear();
    clipboard->setPixmap(image);
    clipboard->clear();
    clipboard->setText("Ups nie udało się!");
}

void BrowserWindow::openHomePage() {
    pageView->setUrl(homePage);
}

void BrowserWindow::detectedPrintScreen() {
    if(!openedWarning) {
        openedWarning = true;
        this_thread::sleep_for(chrono::milliseconds(200));
        emptyingClipboard();

        auto mBox = QMessageBox("Mam Cie!",
                                "Wykonałeś zrzut ekranu!\nTwój komputer zostanie wyłączony w przeciągu minuty",
                                QMessageBox::Critical, QMessageBox::Ok, QMessageBox::NoButton,
                                QMessageBox::NoButton);
        mBox.setWindowFlag(Qt::WindowStaysOnTopHint);
        mBox.exec();

        if(warningPBar->value() == 0) {
            warningPBar->setVisible(true);
        }

        warningPBar->setValue(warningPBar->value() + 1);

        openedWarning = false;
    }
}
