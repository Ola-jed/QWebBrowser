#include "webbrowser.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("QWebBrowser");
    QCoreApplication::setOrganizationName("QWebBrowser");
    WebBrowser w;
    w.show();
    return QApplication::exec();
}