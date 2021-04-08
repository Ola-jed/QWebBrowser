#include "webbrowser.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("QWebBrowser");
    QCoreApplication::setOrganizationName("QWebBrowser");
    QCoreApplication::setApplicationVersion("1.0");
    QApplication a(argc, argv);
    WebBrowser w;
    // If the program receives arguments (urls), we open the urls given
    if(argc > 1)
    {
        for(auto i = 1;i < argc;i++)
        {
            w.onNewTab();
            w.onOpenUrl(QUrl(argv[i]));
        }
    }
    w.show();
    return QApplication::exec();
}