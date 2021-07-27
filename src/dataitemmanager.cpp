#include "dataitemmanager.hpp"

DataItemManager::DataItemManager()
{
    myDb = QSqlDatabase::addDatabase(DATABASE_TYPE);
    myDb.setDatabaseName(DATABASE_NAME);
    if (myDb.open())
    {
        QSqlQuery queryCreation{myDb};
        queryCreation.exec(
                "CREATE TABLE IF NOT EXISTS History ( Id INTEGER PRIMARY KEY AUTOINCREMENT,Date VARCHAR(30),Title VARCHAR(100),Url TEXT);");
        queryCreation.exec(
                "CREATE TABLE IF NOT EXISTS Favorite ( Id INTEGER PRIMARY KEY AUTOINCREMENT,Date VARCHAR(30),Title VARCHAR(100),Url TEXT);");
    }
}

// Load all the items stored in the history table.
QList<DataItem> DataItemManager::selectAllFromHistory()
{
    QList<DataItem> histLoaded{};
    auto queryAll = myDb.exec("SELECT Date,Title,Url FROM History");
    while (queryAll.next())
    {
        histLoaded.push_back(
                DataItem{queryAll.value(0).toString(), queryAll.value(1).toString(),
                         queryAll.value(2).toString()});
    }
    return histLoaded;
}

/// Load all the favorites stored in the favorites table.
QList<DataItem> DataItemManager::selectAllFavorites()
{
    QList<DataItem> histLoaded{};
    auto queryAll = myDb.exec("SELECT Date,Title,Url FROM Favorites");
    while (queryAll.next())
    {
        histLoaded.push_back(
                DataItem{queryAll.value(0).toString(), queryAll.value(1).toString(),
                         queryAll.value(2).toString()});
    }
    return histLoaded;
}

/// Insert a new url to the history
/// \param toAdd
void DataItemManager::insertItemInHistory(const DataItem &toAdd)
{
    QSqlQuery insertQuery{myDb};
    insertQuery.prepare("INSERT INTO History(Date,Title,Url)"
                        "VALUES (:date, :title, :url)");
    insertQuery.bindValue(":date", toAdd.date);
    insertQuery.bindValue(":title", toAdd.title);
    insertQuery.bindValue(":url", toAdd.page);
    insertQuery.exec();
}

/// Delete a page from the history.
/// \param toDelete
void DataItemManager::deleteItemFromHistory(const DataItem &toDelete)
{
    QSqlQuery deleteQuery{myDb};
    deleteQuery.prepare("DELETE FROM History WHERE (Date= :date AND Title= :title AND Url= :url)");
    deleteQuery.bindValue(":date", toDelete.date);
    deleteQuery.bindValue(":title", toDelete.title);
    deleteQuery.bindValue(":url", toDelete.page);
    deleteQuery.exec();
}

/// Add a page to the favorites
void DataItemManager::addFavorite(const DataItem &newFavorite)
{
    QSqlQuery insertQuery{myDb};
    insertQuery.prepare("INSERT INTO Favorite(Date,Title,Url)"
                        "VALUES (:date, :title, :url)");
    insertQuery.bindValue(":date", newFavorite.date);
    insertQuery.bindValue(":title", newFavorite.title);
    insertQuery.bindValue(":url", newFavorite.page);
    insertQuery.exec();
}

/// Delete a page from the favorites.
/// \param favoriteToDelete
void DataItemManager::deleteFavorite(const DataItem &favoriteToDelete)
{
    QSqlQuery deleteQuery{myDb};
    deleteQuery.prepare("DELETE FROM Favorite WHERE (Date= :date AND Title= :title AND Url= :url)");
    deleteQuery.bindValue(":date", favoriteToDelete.date);
    deleteQuery.bindValue(":title", favoriteToDelete.title);
    deleteQuery.bindValue(":url", favoriteToDelete.page);
    deleteQuery.exec();
}

DataItemManager::~DataItemManager()
{
    myDb.close();
}