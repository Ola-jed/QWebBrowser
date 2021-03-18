#ifndef WEBVIEW_HPP
#define WEBVIEW_HPP

#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QObject>

class WebView : public QWebEngineView
{
Q_OBJECT
public:
    explicit WebView(QWidget *parent = nullptr);
protected:
    QWebEngineView* createWindow(QWebEnginePage::WebWindowType type) override;
};

#endif // WEBVIEW_HPP