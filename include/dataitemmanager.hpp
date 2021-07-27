#ifndef DATAITEMMANAGER_H
#define DATAITEMMANAGER_H

#include <QList>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlResult>
#include <QSqlDriver>
#include <QSqlDatabase>
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
