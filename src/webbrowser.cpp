#include "webbrowser.hpp"

WebBrowser::WebBrowser(QWidget *parent)
        : QMainWindow(parent)
{
    onLoadHistory();
    buildComponents();
    buildMenu();
    applyLayout();
    applyStyle();
    makeConnections();
}

// Connections.
void WebBrowser::makeConnections()
{
    connect(tabViews,&QTabWidget::tabCloseRequested,this, &WebBrowser::onCloseTab);
    connect(tabViews,&QTabWidget::currentChanged,   this, &WebBrowser::onUpdateLineEdit);
    connect(tabViews,&QTabWidget::currentChanged,   this, &WebBrowser::updateTitle);
    connect(tabViews,&QTabWidget::currentChanged,   this, &WebBrowser::updateConnect);
    connect(tabViews,&QTabWidget::currentChanged,   this, &WebBrowser::onUrlChanged);
    connect(localFile,   &QAction::triggered,this, &WebBrowser::loadLocalFile);
    connect(privateTab,  &QAction::triggered,this, &WebBrowser::onNewPrivateTab);
    connect(historyMenu, &QAction::triggered,this, &WebBrowser::onHistory);
    connect(favoriteList,&QAction::triggered,this, &WebBrowser::onFavorites);
    connect(favoritesButton,&QPushButton::clicked,this,&WebBrowser::onAddFavorites);
    connect(searchButton,   &QPushButton::clicked,this,&WebBrowser::onUrl);
    connect(newTabButton,   &QPushButton::clicked,this,&WebBrowser::onNewTab);
    connect(refreshButton,  &QPushButton::clicked,this,&WebBrowser::onRefresh);
    connect(homeButton,     &QPushButton::clicked,this,&WebBrowser::goHome);
    connect(previousButton, &QPushButton::clicked,this,&WebBrowser::onPreviousPage);
    connect(nextButton,     &QPushButton::clicked,this,&WebBrowser::onNextPage);
    connect(downloadButton, &QPushButton::clicked,this,&WebBrowser::downloadCurrentPage);
    connect(currentWebview(), &WebView::loadStarted,this, &WebBrowser::onUrlChanged);
    connect(currentWebview(), &WebView::loadStarted, this, &WebBrowser::onStartLoading);
    connect(currentWebview(), &WebView::loadProgress,this, &WebBrowser::onLoading);
    connect(currentWebview(), &WebView::loadFinished,this, &WebBrowser::onEndLoading);
}

// Building the mainwindow components.
void WebBrowser::buildComponents()
{
    installEventFilter(this);
    myWebView       = new WebView(this);
    loadingBar      = new QProgressBar(this);
    tabViews        = new QTabWidget(this);
    menubar         = new QMenuBar(this);
    menu            = new QMenu("Menu",this);
    urlLineEdit     = new QLineEdit(this);
    searchButton    = new QPushButton(QIcon(":assets/search.ico"),"",this);
    newTabButton    = new QPushButton(QIcon(":assets/new.ico"),"",this);
    homeButton      = new QPushButton(QIcon(":assets/home.ico"),"",this);
    previousButton  = new QPushButton(QIcon(":assets/previous.ico"),"",this);
    nextButton      = new QPushButton(QIcon(":assets/next.ico"),"",this);
    refreshButton   = new QPushButton(QIcon(":assets/refresh.ico"),"",this);
    favoritesButton = new QPushButton(QIcon(":assets/favorites.ico"),"",this);
    downloadButton  = new QPushButton(QIcon(":assets/download.ico"),"",this);
    privateTab      = new QAction(QIcon(":assets/private.ico"),"Private tab",this);
    historyMenu     = new QAction(QIcon(":assets/history.ico"),"History",this);
    localFile       = new QAction(QIcon(":assets/open.ico"),"Open a local file",this);
    favoriteList    = new QAction(QIcon(":assets/list.ico"),"Favorite list",this);
    myWebView->load(HOME_PAGE);
    myWebView->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
    loadingBar->setVisible(false);
    tabViews->setTabsClosable(true);
    tabViews->setTabShape(QTabWidget::Rounded);
    tabViews->setElideMode(Qt::ElideRight);
    tabViews->addTab(myWebView,"New Tab");
}

// Building the menu components.
void WebBrowser::buildMenu()
{
    menubar->addMenu(menu);
    menu->setIcon(QIcon(":assets/menu.ico"));
    menu->addAction(historyMenu);
    menu->addAction(privateTab);
    menu->addAction(localFile);
    menu->addAction(favoriteList);
}

// Define the app layout
void WebBrowser::applyLayout()
{
    auto *lineLayout = new QHBoxLayout();
    lineLayout->addWidget(urlLineEdit,19);
    lineLayout->addWidget(searchButton,1);
    lineLayout->setSpacing(0);
    lineLayout->setContentsMargins(0,0,0,0);
    auto *topLayout  = new QHBoxLayout();
    topLayout->setAlignment(Qt::AlignVCenter);
    topLayout->addWidget(previousButton,1);
    topLayout->addWidget(nextButton,1);
    topLayout->addWidget(refreshButton,1);
    topLayout->addWidget(homeButton,1);
    topLayout->addLayout(lineLayout,24);
    topLayout->addWidget(newTabButton,1);
    topLayout->addWidget(favoritesButton,1);
    topLayout->addWidget(downloadButton,1);
    topLayout->addWidget(menubar,1);
    auto *appLayout = new QVBoxLayout();
    appLayout->setContentsMargins(0,0,0,0);
    appLayout->addLayout(topLayout,1);
    appLayout->addWidget(loadingBar,1);
    appLayout->addWidget(tabViews,28);
    auto central = new QWidget;
    central->setLayout(appLayout);
    setCentralWidget(central);
}

// Set the application style.
void WebBrowser::applyStyle()
{
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,size(),QGuiApplication::primaryScreen()->availableGeometry()));
    setWindowIcon(QIcon(":assets/icon.ico"));
    tabViews->setTabIcon(0,myWebView->icon());
    setStyleSheet(STYLE);
}

// History and Favorites gestion.
void WebBrowser::onLoadHistory()
{
    history = dataManager.selectAllFromHistory();
}

void WebBrowser::onLoadFavorites()
{
    favorites = dataManager.selectAllFavorites();
}

void WebBrowser::addToHistory(const QString &title,const QUrl &urlToAdd)
{
    if((!urlToAdd.toString().isEmpty()) && (!urlToAdd.toString().isNull()))
    {
        const DataItem tempItem {QDateTime::currentDateTime().toString(),title,urlToAdd.toString()};
        dataManager.insertItemInHistory(tempItem);
        history.push_back(tempItem);
    }
}

void WebBrowser::onAddFavorites()
{
    const DataItem tempFav{QDateTime::currentDateTime().toString(),currentWebview()->title(),currentWebview()->url().toString()};
    favorites.push_back(tempFav);
    dataManager.addFavorite(tempFav);
}

void WebBrowser::onHistoryItemDeleted(int pos)
{
    if((pos < history.size()) && (history.size() > 0) && (pos >= 0))
    {
        dataManager.deleteItemFromHistory(history.at(pos));
        history.removeAt(pos);
    }
}

void WebBrowser::onFavoriteDeleted(int pos)
{
    if((pos < favorites.size()) && (!favorites.empty()) && (pos >= 0))
    {
        dataManager.deleteFavorite(favorites.at(pos));
        favorites.removeAt(pos);
    }
}

// Tabs and url manager
void WebBrowser::onNewTab()
{
    auto view = new WebView(this);
    view->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
    view->load(HOME_PAGE);
    tabViews->addTab(view,"New Tab");
    tabViews->setCurrentIndex(tabViews->count() - 1); // Go to the new tab
}

void WebBrowser::updateIcon()
{
    tabViews->setTabIcon(tabViews->currentIndex(),currentWebview()->icon());
}

void WebBrowser::onNewPrivateTab()
{
    auto view = new WebView(this);
    view->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
    view->load(HOME_PAGE);
    tabViews->addTab(view,PRIVATE_TAB);
}

// Private tabs are identified by their name.
bool WebBrowser::isPrivate() const
{
    return (tabViews->tabText(tabViews->currentIndex()) == PRIVATE_TAB);
}

void WebBrowser::onUrlChanged()
{
    urlLineEdit->setText(currentWebview()->url().toString());
    if(!isPrivate())
    {
        updateIcon();
        tabViews->setTabText(tabViews->currentIndex(),currentWebview()->title());
    }
}

void WebBrowser::onUrl()
{
    onOpenUrl(urlLineEdit->text());
}

void WebBrowser::onOpenUrl(const QUrl &urlToOpen)
{
    const auto url {((urlToOpen.toString().left(7) == HTTP)||(urlToOpen.toString().left(8) == HTTPS)) ? urlToOpen.toString() : HTTPS+urlToOpen.toString()};
    if(QUrl{url}.isValid())
    {
        urlLineEdit->setText(url);
        currentWebview()->load(QUrl{url});
        if(!isPrivate())
        {
            tabViews->setTabText(tabViews->currentIndex(),currentWebview()->title());
            setWindowTitle(currentWebview()->title());
        }
    }
}

void WebBrowser::onRefresh()
{
    currentWebview()->reload();
}

// Open the home page
void WebBrowser::goHome()
{
    onOpenUrl(HOME_PAGE);
}

void WebBrowser::onUpdateLineEdit()
{
    urlLineEdit->setText(currentWebview()->url().toString());
}

void WebBrowser::onCloseTab(const int &index)
{
    if(tabViews->count() > 1)
    {
        auto wid = tabViews->widget(index);
        tabViews->removeTab(index);
        delete wid;
        wid = nullptr;
    }
    else
    {
        onQuit();
    }
}

void WebBrowser::onHistory()
{
    auto historyView = new HistoryView(this,history);
    connect(historyView,&HistoryView::historyItemDeleted,this,&WebBrowser::onHistoryItemDeleted);
    connect(historyView,&HistoryView::goToUrl,this,&WebBrowser::onOpenUrl);
    historyView->show();
}

void WebBrowser::onFavorites()
{
    auto historyView = new HistoryView(this,favorites,"Favorites");
    connect(historyView,&HistoryView::historyItemDeleted,this,&WebBrowser::onHistoryItemDeleted);
    connect(historyView,&HistoryView::goToUrl,this,&WebBrowser::onOpenUrl);
    historyView->show();
}

// Page management
void WebBrowser::onPreviousPage()
{
    currentWebview()->back();
}

void WebBrowser::onNextPage()
{
    currentWebview()->forward();
}

void WebBrowser::updateTitle()
{
    setWindowTitle(currentWebview()->title());
}

void WebBrowser::updateConnect()
{
    connect(currentWebview(), &WebView::loadStarted,  this, &WebBrowser::onStartLoading);
    connect(currentWebview(), &WebView::loadProgress, this, &WebBrowser::onLoading);
    connect(currentWebview(), &WebView::loadFinished, this, &WebBrowser::onEndLoading);
    connect(currentWebview(), &WebView::loadFinished, this, &WebBrowser::onUrlChanged);
}

// Loading bar.
void WebBrowser::onStartLoading()
{
    loadingBar->setVisible(true);
    if(!isPrivate())
    {
        addToHistory(currentWebview()->title(),currentWebview()->url());
    }
}

void WebBrowser::onLoading(int percentage)
{
    loadingBar->setValue(percentage);
}

void WebBrowser::onEndLoading()
{
    loadingBar->setVisible(false);
    loadingBar->setValue(0);
}

// Load a local file in a qwebview
void WebBrowser::loadLocalFile()
{
    auto const fileName {QFileDialog::getOpenFileName(this,"Open a local page","/home","*.htm *.shtml *.xhtml  *.html")};
    auto view = new WebView(this);
    view->load(QUrl::fromLocalFile(fileName));
    tabViews->addTab(view,fileName);
}

// Download the curent page
void WebBrowser::downloadCurrentPage()
{
    const auto pageName = QFileDialog::getSaveFileName(this,"Download page");
    if((pageName.isEmpty()) || (pageName.isNull())) return;
    currentWebview()->page()->save(pageName);
}

// The enter key event on the urlLineEdit.
bool WebBrowser::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        auto *keyEvent = dynamic_cast<QKeyEvent *>(event);
        if(((keyEvent->key() == Qt::Key_Return)) && (urlLineEdit->cursorPosition() != 0))
        {
            onUrl();
        }
    }
    return QObject::eventFilter(obj, event);
}

// Drag event to open local web pages.
void WebBrowser::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();
    }
}

// Drop event to open local web pages.
void WebBrowser::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    // Check for our needed mime type, here a file or a list of files
    if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();
        // Extract the local paths of the files.
        const QString filename {urlList[0].toString().right(urlList[0].toString().length() - 7)};
        onOpenUrl(QUrl::fromLocalFile(filename));
    }
}

void WebBrowser::onQuit()
{
    qApp->quit();
}

WebView* WebBrowser::currentWebview() const
{
    return qobject_cast<WebView*>(tabViews->currentWidget());
}

WebBrowser::~WebBrowser()
{}
