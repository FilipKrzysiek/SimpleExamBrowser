//
// Created by Filip on 16.09.2024.
//

#ifndef BROWSER_H
#define BROWSER_H

#include <QMessageBox>
#include <qtimer.h>
#include <QUrl>
#include <QWidget>
#include <thread>


QT_BEGIN_NAMESPACE
namespace Ui { class Browser; }
QT_END_NAMESPACE

class Browser : public QWidget {
Q_OBJECT

public:
    explicit Browser(QWidget *parent = nullptr);
    ~Browser() override;

    void setupWindowsGeometry();

    void loadUrl(const QString &url);

    void setBaseUrl(const QUrl &base_url);

    void setBaseUrl(const QString &base_url);

    void openBaseUrlPage();

    void detectedForbiddenAction(enum DetectedAction action);

protected:
    void closeEvent(QCloseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;


private:
    Ui::Browser *ui;
    QTimer updateTime;
    QTimer closeActionTimer;
    QUrl baseUrl;
    QVector<QWidget*> otherScreensWidget;
    std::thread warningThread;
    QMessageBox warningMessageBox = QMessageBox(this);
    QMessageBox* closeMessageBox;
    bool isAlternateDispMode = false;

    int warningLevel = 0;

    int closeActionCounter = 0;

    void setupUi();

    void setupIcons();

    void setupConnections();

    void setupWarningWindow();

    void showWarningWIndow();

private slots:
    void updateDataTime();

    void closeApplication();

    void createBlackScreen(const QRect &geom);

signals:
    void disableGuarder();
};


#endif //BROWSER_H
