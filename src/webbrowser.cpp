#include "webbrowser.hpp"

WebBrowser::WebBrowser(QWidget *parent)
        : QMainWindow(parent)
{
    onLoadHistory();
    buildComponents();
    initTooltips();
    buildMenu();
    applyLayout();
    applyStyle();
    makeConnections();
}

/// Connections
void WebBrowser::makeConnections()
{
    connect(tabViews, &QTabWidget::tabCloseRequested, this, &WebBrowser::onCloseTab);
    connect(tabViews, &QTabWidget::currentChanged, this, &WebBrowser::onUpdateLineEdit);
    connect(tabViews, &QTabWidget::currentChanged, this, &WebBrowser::updateTitle);
    connect(tabViews, &QTabWidget::currentChanged, this, &WebBrowser::updateConnect);
    connect(tabViews, &QTabWidget::currentChanged, this, &WebBrowser::onUrlChanged);
    connect(localFile, &QAction::triggered, this, &WebBrowser::loadLocalFile);
    connect(privateTab, &QAction::triggered, this, &WebBrowser::onNewPrivateTab);
    connect(historyMenu, &QAction::triggered, this, &WebBrowser::onHistory);
    connect(favoriteList, &QAction::triggered, this, &WebBrowser::onFavorites);
    connect(favoritesButton, &QPushButton::clicked, this, &WebBrowser::onAddFavorites);
    connect(searchButton, &QPushButton::clicked, this, &WebBrowser::onUrl);
    connect(newTabButton, &QPushButton::clicked, this, &WebBrowser::onNewTab);
    connect(refreshButton, &QPushButton::clicked, this, &WebBrowser::onReload);
    connect(homeButton, &QPushButton::clicked, this, &WebBrowser::goHome);
    connect(previousButton, &QPushButton::clicked, this, &WebBrowser::onPreviousPage);
    connect(nextButton, &QPushButton::clicked, this, &WebBrowser::onNextPage);
    connect(downloadButton, &QPushButton::clicked, this, &WebBrowser::downloadCurrentPage);
    connect(currentWebView(), &WebView::loadStarted, this, &WebBrowser::onUrlChanged);
    connect(currentWebView(), &WebView::loadStarted, this, &WebBrowser::onStartLoading);
    connect(currentWebView(), &WebView::loadProgress, this, &WebBrowser::onLoading);
    connect(currentWebView(), &WebView::loadFinished, this, &WebBrowser::onEndLoading);
    connect(currentWebView(), &WebView::iconChanged, this, &WebBrowser::updateIcon);
}

/// Building the main window components.
void WebBrowser::buildComponents()
{
    installEventFilter(this);
    myWebView       = new WebView(this);
    loadingBar      = new QProgressBar(this);
    tabViews        = new QTabWidget(this);
    menubar         = new QMenuBar(this);
    menu            = new QMenu("Menu", this);
    urlLineEdit     = new QLineEdit(this);
    searchButton    = new QPushButton(QIcon(QStringLiteral(":assets/search.ico")), "", this);
    newTabButton    = new QPushButton(QIcon(QStringLiteral(":assets/new.ico")), "", this);
    homeButton      = new QPushButton(QIcon(QStringLiteral(":assets/home.ico")), "", this);
    previousButton  = new QPushButton(QIcon(QStringLiteral(":assets/previous.ico")), "", this);
    nextButton      = new QPushButton(QIcon(QStringLiteral(":assets/next.ico")), "", this);
    refreshButton   = new QPushButton(QIcon(QStringLiteral(":assets/refresh.ico")), "", this);
    favoritesButton = new QPushButton(QIcon(QStringLiteral(":assets/favorites.ico")), "", this);
    downloadButton  = new QPushButton(QIcon(QStringLiteral(":assets/download.ico")), "", this);
    privateTab      = new QAction(QIcon(QStringLiteral(":assets/private.ico")), "Private tab", this);
    historyMenu     = new QAction(QIcon(QStringLiteral(":assets/history.ico")), "History", this);
    localFile       = new QAction(QIcon(QStringLiteral(":assets/open.ico")), "Open a local file", this);
    favoriteList    = new QAction(QIcon(QStringLiteral(":assets/list.ico")), "Favorite list", this);
    myWebView->load(HOME_PAGE);
    myWebView->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
    loadingBar->setVisible(false);
    tabViews->setTabsClosable(true);
    tabViews->setTabShape(QTabWidget::Rounded);
    tabViews->setElideMode(Qt::ElideRight);
    tabViews->addTab(myWebView, "New Tab");
    tabViews->setStyleSheet("QTabWidget::pane { border: 0; }");
}

/// Create tooltips for buttons
void WebBrowser::initTooltips()
{
    searchButton->setToolTip(QStringLiteral("Search"));
    newTabButton->setToolTip(QStringLiteral("New tab"));
    previousButton->setToolTip(QStringLiteral("Previous"));
    nextButton->setToolTip(QStringLiteral("Next"));
    refreshButton->setToolTip(QStringLiteral("Refresh"));
    favoritesButton->setToolTip(QStringLiteral("Favorites"));
    downloadButton->setToolTip(QStringLiteral("Download"));
    homeButton->setToolTip(QStringLiteral("Home"));
}

/// Building the menu components.
void WebBrowser::buildMenu()
{
    menubar->addMenu(menu);
    menu->setIcon(QIcon(":assets/menu.ico"));
    menu->addAction(historyMenu);
    menu->addAction(privateTab);
    menu->addAction(localFile);
    menu->addAction(favoriteList);
}

/// Define the app layout
void WebBrowser::applyLayout()
{
    auto lineLayout = new QHBoxLayout();
    lineLayout->addWidget(urlLineEdit, 19);
    lineLayout->addWidget(searchButton, 1);
    lineLayout->setSpacing(0);
    lineLayout->setContentsMargins(0, 0, 0, 0);
    auto topLayout = new QHBoxLayout();
    topLayout->setAlignment(Qt::AlignVCenter);
    topLayout->addWidget(previousButton, 1);
    topLayout->addWidget(nextButton, 1);
    topLayout->addWidget(refreshButton, 1);
    topLayout->addWidget(homeButton, 1);
    topLayout->addLayout(lineLayout, 24);
    topLayout->addWidget(newTabButton, 1);
    topLayout->addWidget(favoritesButton, 1);
    topLayout->addWidget(downloadButton, 1);
    topLayout->addWidget(menubar, 1);
    auto appLayout = new QVBoxLayout();
    appLayout->setContentsMargins(0, 5, 0, 0);
    appLayout->addLayout(topLayout, 1);
    appLayout->addWidget(loadingBar, 1);
    appLayout->addWidget(tabViews, 28);
    auto central = new QWidget;
    central->setLayout(appLayout);
    setCentralWidget(central);
}

/// Set the application style.
void WebBrowser::applyStyle()
{
    resize(700, 500);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(),
                                    QGuiApplication::primaryScreen()->availableGeometry()));
    setWindowIcon(QIcon(":assets/icon.ico"));
    tabViews->setTabIcon(0, myWebView->icon());
}

/// History and Favorites management.
void WebBrowser::onLoadHistory()
{
    history = dataManager.selectAllFromHistory();
}

/// Load all stored favorites
void WebBrowser::onLoadFavorites()
{
    favorites = dataManager.selectAllFavorites();
}

/// Add a page to the history
/// \param title
/// \param urlToAdd
void WebBrowser::addToHistory(const QString &title, const QUrl &urlToAdd)
{
    if (!urlToAdd.toString().trimmed().isEmpty())
    {
        const DataItem tempItem{QDateTime::currentDateTime().toString(), title,
                                urlToAdd.toString()};
        dataManager.insertItemInHistory(tempItem);
        history.push_back(tempItem);
    }
}

/// Adding a page to the favorites
void WebBrowser::onAddFavorites()
{
    const DataItem tempFav{QDateTime::currentDateTime().toString(),
                           currentWebView()->title(), currentWebView()->url().toString()};
    favorites.push_back(tempFav);
    dataManager.addFavorite(tempFav);
}

/// Deletion of an item in the history
/// \param pos
void WebBrowser::onHistoryItemDeleted(int pos)
{
    if ((pos < history.size()) && (!history.empty()) && (pos >= 0))
    {
        dataManager.deleteItemFromHistory(history.at(pos));
        history.removeAt(pos);
    }
}

/// Deletion of an item in the favorites list
/// \param pos
void WebBrowser::onFavoriteDeleted(int pos)
{
    if ((pos < favorites.size()) && (!favorites.empty()) && (pos >= 0))
    {
        dataManager.deleteFavorite(favorites.at(pos));
        favorites.removeAt(pos);
    }
}

/// Open a new tab and switch to this new tab
void WebBrowser::onNewTab()
{
    auto view = new WebView(this);
    view->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
    view->load(HOME_PAGE);
    tabViews->addTab(view, "New Tab");
    tabViews->setCurrentIndex(tabViews->count() - 1); // Go to the new tab
}

/// Update the favicon
/// \param icon
void WebBrowser::updateIcon(const QIcon &icon)
{
    tabViews->setTabIcon(tabViews->currentIndex(), icon);
}

/// Opening a new private tab
void WebBrowser::onNewPrivateTab()
{
    auto view = new WebView(this);
    view->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
    view->load(HOME_PAGE);
    tabViews->addTab(view, PRIVATE_TAB);
}

/// Private tabs are identified by their name.
bool WebBrowser::isPrivate() const
{
    return (tabViews->tabText(tabViews->currentIndex()) == PRIVATE_TAB);
}

/// Url changed
void WebBrowser::onUrlChanged()
{
    urlLineEdit->setText(currentWebView()->url().toString());
    if (!isPrivate())
    {
        tabViews->setTabText(tabViews->currentIndex(), currentWebView()->title());
    }
}

/// Start opening a new url with the content in the line edit
void WebBrowser::onUrl()
{
    onOpenUrl(urlLineEdit->text());
}

/// Open the url
/// \param urlToOpen
void WebBrowser::onOpenUrl(const QUrl &urlToOpen)
{
    const QUrl url{
            ((urlToOpen.toString().left(7) == HTTP) || (urlToOpen.toString().left(8) == HTTPS))
            ? urlToOpen.toString()
            : HTTPS + urlToOpen.toString()
    };
    if (!url.isValid())
    {
        urlLineEdit->setText(url.toString());
        currentWebView()->load(url);
        if (!isPrivate())
        {
            tabViews->setTabText(tabViews->currentIndex(), currentWebView()->title());
            setWindowTitle(currentWebView()->title());
        }
    }
}

/// Reload the current page
void WebBrowser::onReload()
{
    currentWebView()->reload();
}

/// Open the home page
void WebBrowser::goHome()
{
    onOpenUrl(HOME_PAGE);
}

/// Update the line edit content
void WebBrowser::onUpdateLineEdit()
{
    urlLineEdit->setText(currentWebView()->url().toString());
}

/// Close the given tab
/// \param index
void WebBrowser::onCloseTab(const int &index)
{
    if (tabViews->count() > 1)
    {
        auto wid = tabViews->widget(index);
        tabViews->removeTab(index);
        delete wid;
    }
    else
    {
        onQuit();
    }
}

/// Show the history view
void WebBrowser::onHistory()
{
    auto historyView = new HistoryView(this, history);
    connect(historyView, &HistoryView::historyItemDeleted, this, &WebBrowser::onHistoryItemDeleted);
    connect(historyView, &HistoryView::goToUrl, this, &WebBrowser::onOpenUrl);
    historyView->show();
}

/// Show the favorites list
void WebBrowser::onFavorites()
{
    auto historyView = new HistoryView(this, favorites, "Favorites");
    connect(historyView, &HistoryView::historyItemDeleted, this, &WebBrowser::onHistoryItemDeleted);
    connect(historyView, &HistoryView::goToUrl, this, &WebBrowser::onOpenUrl);
    historyView->show();
}

/// Page management
void WebBrowser::onPreviousPage()
{
    currentWebView()->back();
}

/// Go to the next page
void WebBrowser::onNextPage()
{
    currentWebView()->forward();
}

/// Update the page title
void WebBrowser::updateTitle()
{
    setWindowTitle(currentWebView()->title());
}

/// Make the connections when tab is changed
void WebBrowser::updateConnect()
{
    connect(currentWebView(), &WebView::loadStarted, this, &WebBrowser::onStartLoading);
    connect(currentWebView(), &WebView::loadProgress, this, &WebBrowser::onLoading);
    connect(currentWebView(), &WebView::loadFinished, this, &WebBrowser::onEndLoading);
    connect(currentWebView(), &WebView::loadFinished, this, &WebBrowser::onUrlChanged);
    connect(currentWebView(), &WebView::iconChanged, this, &WebBrowser::updateIcon);
}

/// Loading bar show.
void WebBrowser::onStartLoading()
{
    loadingBar->setVisible(true);
    if (!isPrivate())
    {
        addToHistory(currentWebView()->title(), currentWebView()->url());
    }
}

/// Loading bar update
/// \param percentage
void WebBrowser::onLoading(int percentage)
{
    loadingBar->setValue(percentage);
}

/// Hide loading bar
void WebBrowser::onEndLoading()
{
    loadingBar->setVisible(false);
    loadingBar->setValue(0);
}

/// Load a local file in a qwebview
void WebBrowser::loadLocalFile()
{
    auto const fileName{QFileDialog::getOpenFileName(this, "Open a local page", "/home",
                                                     "*.htm *.shtml *.xhtml  *.html")};
    auto       view = new WebView(this);
    view->load(QUrl::fromLocalFile(fileName));
    tabViews->addTab(view, fileName);
}

/// Download the current page
void WebBrowser::downloadCurrentPage()
{
    const auto pageName{QFileDialog::getSaveFileName(this, "Download page")};
    if ((pageName.trimmed().isEmpty()) || (pageName.isNull()))
    {
        return;
    }
    currentWebView()->page()->save(pageName);
}

/// The enter key event on the urlLineEdit.
/// \param obj
/// \param event
bool WebBrowser::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        auto *keyEvent = dynamic_cast<QKeyEvent *>(event);
        if ((keyEvent->key() == Qt::Key_Return) && (urlLineEdit->cursorPosition() != 0))
        {
            onUrl();
        }
    }
    return QObject::eventFilter(obj, event);
}

/// Drag event to open local web pages.
/// Check for our needed mime type, here a file or a list of files
/// \param e
void WebBrowser::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();
    }
}

/// Drop event to open local web pages.
/// Extract the local path from the file.
/// \param event
void WebBrowser::dropEvent(QDropEvent *event)
{
    const auto urlList = event->mimeData()->urls();
    const auto filename{urlList[0].toLocalFile()};
    onOpenUrl(QUrl::fromLocalFile(filename));
}

/// Exit the app
void WebBrowser::onQuit()
{
    qApp->quit();
}

/// Get the current web view
WebView *WebBrowser::currentWebView() const
{
    return qobject_cast<WebView *>(tabViews->currentWidget());
}