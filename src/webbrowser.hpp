#ifndef WEBBROWSER_HPP
#define WEBBROWSER_HPP

#include "dataitem.hpp"
#include "historyview.hpp"
#include "dataitemmanager.hpp"
#include "style.hpp"
#include "webview.hpp"
#include <QFile>
#include <QScreen>
#include <QStyle>
#include <QTextStream>
#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QUrl>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineSettings>
#include <QTabWidget>
#include <QList>
#include <QMessageBox>
#include <QMimeData>
#include <QDateTime>
#include <QKeyEvent>
#include <QProgressBar>
#include <QIcon>

class WebBrowser : public QMainWindow
{
Q_OBJECT

public:
    WebBrowser(QWidget *parent = nullptr);
    ~WebBrowser();
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
private:
    const QString PRIVATE_TAB {"Private Tab"};
    const QString HISTORY_FILENAME {"History.txt"};
    const QString HTTP{"http://"};
    const QString HTTPS{"https://"};
    const QUrl HOME_PAGE{"https://www.google.bj"};

    WebView *currentWebview() const;
    void onLoadHistory();
    void onLoadFavorites();
    void addToHistory(const QString &title,const QUrl &urlToAdd);
    bool isPrivate() const;
    void buildComponents();
    void buildMenu();
    void applyStyle();
    void applyLayout();
    void makeConnections();

    QTabWidget *tabViews;
    QWebEngineView *myWebView;
    QLineEdit *urlLineEdit;
    QUrl currentUrl;

    QList<DataItem> history;
    QList<DataItem> favorites;

    QMenuBar *menubar;
    QMenu *menu;
    QAction *historyMenu;
    QAction *privateTab;
    QAction *localFile;
    QAction *favoriteList;

    QPushButton *favoritesButton;
    QPushButton *searchButton;
    QPushButton *newTabButton;
    QPushButton *previousButton;
    QPushButton *nextButton;
    QPushButton *refreshButton;
    QPushButton *homeButton;
    QPushButton *downloadButton;

    QProgressBar *loadingBar;
    DataItemManager dataManager{};
private slots:
    void onOpenUrl(const QUrl &urlToOpen);
    void onUrl();
    void onRefresh();
    void onNewTab();
    void onUpdateLineEdit();
    void onNewPrivateTab();
    void onCloseTab(const int &index);
    void onHistory();
    void onFavorites();
    void onQuit();
    void onHistoryItemDeleted(int pos);
    void onFavoriteDeleted(int pos);
    void onUrlChanged();
    void onPreviousPage();
    void onNextPage();
    void goHome();
    void onStartLoading();
    void onLoading(int percentage);
    void onEndLoading();
    void onAddFavorites();
    void updateIcon();
    void updateTitle();
    void loadLocalFile();
    void downloadCurrentPage();
    void updateConnect();
};
#endif // WEBBROWSER_HPP