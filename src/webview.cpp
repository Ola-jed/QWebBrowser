#include "webview.hpp"

WebView::WebView(QWidget *parent) : QWebEngineView(parent)
{
    settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
    settings()->setAttribute(QWebEngineSettings::AllowRunningInsecureContent, true);
    settings()->setAttribute(QWebEngineSettings::SpatialNavigationEnabled, true);
    settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
}

QWebEngineView *WebView::createWindow(QWebEnginePage::WebWindowType type)
{
    Q_UNUSED(type)
    return this;
}