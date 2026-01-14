//
// Created by Filip on 14.01.2026.
//

#ifndef SIMPLEEXAMBROWSER_WEBVIEW_H
#define SIMPLEEXAMBROWSER_WEBVIEW_H
#include <QWebEngineView>


class WebView : public QWebEngineView {
public:
    explicit WebView(QWidget *parent = nullptr);

protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override;
};

inline WebView::WebView(QWidget *parent) : QWebEngineView(parent) {
}




#endif //SIMPLEEXAMBROWSER_WEBVIEW_H
