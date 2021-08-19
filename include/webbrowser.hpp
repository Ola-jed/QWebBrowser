#ifndef WEBBROWSER_HPP
#define WEBBROWSER_HPP

#include "webview.hpp"
#include "dataitem.hpp"
#include "historyview.hpp"
#include "dataitemmanager.hpp"
#include <QUrl>
#include <QFile>
#include <QScreen>
#include <QStyle>
#include <QTextStream>
#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QWebEnginePage>
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
    explicit WebBrowser(QWidget *parent = nullptr);
    ~WebBrowser() = default;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    const QString PRIVATE_TAB{"Private Tab"};
    const QString HTTP{"http://"};
    const QString HTTPS{"https://"};
    const QUrl    HOME_PAGE{"https://www.google.com"};

    WebView *currentWebView() const;
    void onLoadHistory();
    void onLoadFavorites();
    void addToHistory(const QString &title, const QUrl &urlToAdd);
    bool isPrivate() const;
    void buildComponents();
    void initTooltips();
    void buildMenu();
    void applyStyle();
    void applyLayout();
    void makeConnections();

    QTabWidget     *tabViews;
    WebView *myWebView;
    QLineEdit      *urlLineEdit;
    QUrl           currentUrl;
    QList<DataItem> history;
    QList<DataItem> favorites;
    QMenuBar *menubar;
    QMenu    *menu;
    QAction  *historyMenu;
    QAction  *privateTab;
    QAction  *localFile;
    QAction  *favoriteList;

    QPushButton *favoritesButton;
    QPushButton *searchButton;
    QPushButton *newTabButton;
    QPushButton *previousButton;
    QPushButton *nextButton;
    QPushButton *refreshButton;
    QPushButton *homeButton;
    QPushButton *downloadButton;

    QProgressBar    *loadingBar;
    DataItemManager dataManager{};

private slots:

    void onUrl();
    void onReload();
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
    void updateIcon(const QIcon &icon);
    void updateTitle();
    void loadLocalFile();
    void downloadCurrentPage();
    void updateConnect();

public slots:
    void onNewTab();
    void onOpenUrl(const QUrl &urlToOpen);
};

#endif // WEBBROWSER_HPP