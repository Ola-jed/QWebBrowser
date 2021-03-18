#ifndef DATAITEMMANAGER_H
#define DATAITEMMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlDriver>
#include <QVariant>
#include <QSqlError>
#include <QList>
#include "dataitem.hpp"

class DataItemManager
{
public:
    DataItemManager();
    ~DataItemManager();
    QList<DataItem> selectAllFromHistory();
    void insertItemInHistory(const DataItem &toAdd);
    void deleteItemFromHistory(const DataItem &toDelete);

    QList<DataItem> selectAllFavorites();
    void addFavorite(const DataItem &newFavorite);
    void deleteFavorite(const DataItem &favoriteToDelete);
private:
    const QString DATABASE_TYPE{"QSQLITE"};
    const QString DATABASE_NAME{"QWebBrowser.db"};
    QSqlDatabase myDb;
};

#endif // DATAITEMMANAGER_H
